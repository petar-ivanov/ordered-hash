#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

#include <cassert>
#include <ctime>
#include <typeinfo>

#include "ordered_hash.h"
using namespace std;

const int S_LEN       = 15;
const int MAX_INT     = int(1e9);

typedef string                      my_key_t;
typedef int                         my_data_t;
typedef pair<my_key_t, my_data_t>   my_value_t;

typedef ordered_hash<my_key_t, my_data_t>   ordered_hash_t;
typedef map<my_key_t, my_data_t>            map_t;

template<typename T>
T gen() {
  assert(false);
  return T();
}

template<>
string gen<string>() {
  string s;

  for(int i=0; i<S_LEN; i++)
    s.push_back('A' + rand()%26 + (rand()%2 ? 0 : ('a' - 'A')));

  return s;
}

template<>
int gen<int>() {
  return rand() % MAX_INT;
}

void gen_test(int n, vector<my_value_t> &sample, int seed) {
  srand(seed);

  for(int i=0; i<n; i++) {
    auto key  = gen<typename my_value_t::first_type>();
    auto data = gen<typename my_value_t::second_type>();
    sample.push_back( my_value_t(key, data) );
  }
}

inline double delay(clock_t start_time) {
  return 1.0 * (clock() - start_time) / CLOCKS_PER_SEC;
}

template<typename T>
string get_type() {
  if (typeid(T).name() == typeid(ordered_hash_t).name())
    return "ordered_hash";
  
  if (typeid(T).name() == typeid(map_t).name())
    return "map";

  return "other";
}

template<typename T>
void insert_all(T &my, const vector<my_value_t> &sample, int seed) {
  srand(seed);

  int n = sample.size();
  assert((int)my.size() == 0);

  clock_t start_time = clock();
  for(auto t : sample)
    my.insert(t);
  double dt = delay(start_time);

  assert((int)my.size() == n);
  printf("%12s %10d ins, %5.2lf s, %8.0lf ins/s\n",
      get_type<T>().c_str(), n, dt, n/dt);
}

template<typename T>
void erase_all(T &my, vector<my_value_t> sample, int seed) {
  srand(seed);

  int n = sample.size();
  random_shuffle(sample.begin(), sample.end());
  assert((int)my.size() == n);

  clock_t start_time = clock();
  for(auto t : sample)
    my.erase(t.first);
  double dt = delay(start_time);

  assert((int)my.size() == 0);
  printf("%12s %10d era, %5.2lf s, %8.0lf era/s\n", 
      get_type<T>().c_str(), n, dt, n/dt);
}

template<typename T>
void benchmark(int n, int seed) {
  T myH;

  vector<my_value_t> sample;
  
  gen_test(n, sample, seed);
  insert_all(myH, sample, seed);
  erase_all(myH, sample, seed);
  
  //myH.insert(my_value_t("hello", 5));
  //myH.insert(my_value_t("hi", 3));
  //myH.insert(my_value_t("alabala", 2));
  //myH.insert(my_value_t("world", 1));

  //for(auto p : myH)
  //  printf("%s -> %s\n", p.first.c_str(), p.second.c_str());
}

void compare(int n, int seed) {
  benchmark< ordered_hash_t > (n, seed);
  benchmark< map_t > (n, seed);
}

int main() {
//  compare(200000, 42 + 0);
  compare(1000000, 42 + 1);
  
  return 0;
}
