
#include "benchmark/benchmark.h"
#include "storage.h"

static void BM_Set(benchmark::State &state) {
  Storage storage(":memory:");
  for (auto _ : state) {
    storage.put("test", "ok");
  }
}
BENCHMARK(BM_Set);

static void BM_Get(benchmark::State &state) {
  Storage storage(":memory:");
  for (auto _ : state) {
    storage.get("test");
  }
}
BENCHMARK(BM_Get);

static void BM_GetExists(benchmark::State &state) {
  Storage storage(":memory:");
  storage.put("test", "ok");
  for (auto _ : state) {
    storage.get("test");
  }
}
BENCHMARK(BM_GetExists);

static void BM_SetGet(benchmark::State &state) {
  Storage storage(":memory:");
  for (auto _ : state) {
    storage.put("test", "ok");
    storage.get("test");
  }
}
BENCHMARK(BM_SetGet);

BENCHMARK_MAIN();
