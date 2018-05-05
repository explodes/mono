#include <utility>
#include "benchmark/benchmark.h"

template <typename T>
void swapCopy(T &a, T &b) {
  T tmp{a};
  a = b;
  b = tmp;
}

template <typename T>
void swapMove(T &a, T &b) {
  T tmp{std::move(a)};
  a = std::move(b);
  b = std::move(tmp);
}

static void BM_SwapCopy_str(benchmark::State &state) {
  std::string a = "hello, world";
  std::string b = "its me, you";
  for (auto _ : state) {
    swapCopy(a, b);
  }
}
BENCHMARK(BM_SwapCopy_str);

static void BM_SwapMove_str(benchmark::State &state) {
  std::string a = "hello, world";
  std::string b = "its me, you";
  for (auto _ : state) {
    swapMove(a, b);
  }
}
BENCHMARK(BM_SwapMove_str);

static void BM_SwapSwap_str(benchmark::State &state) {
  std::string a = "hello, world";
  std::string b = "its me, you";
  for (auto _ : state) {
    std::swap(a, b);
  }
}
BENCHMARK(BM_SwapSwap_str);

static void BM_SwapCopy_int(benchmark::State &state) {
  int a = 1, b = 2;
  for (auto _ : state) {
    swapCopy(a, b);
  }
}
BENCHMARK(BM_SwapCopy_int);

static void BM_SwapMove_int(benchmark::State &state) {
  int a = 1, b = 2;
  for (auto _ : state) {
    swapMove(a, b);
  }
}
BENCHMARK(BM_SwapMove_int);

static void BM_SwapSwap_int(benchmark::State &state) {
  int a = 1, b = 2;
  for (auto _ : state) {
    std::swap(a, b);
  }
}
BENCHMARK(BM_SwapSwap_int);

BENCHMARK_MAIN();