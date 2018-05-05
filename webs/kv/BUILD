# Build rules for the RNG API

load("//hoist:buildrules.bzl", "hoisted_binary", "hoisted_library")
load("//tools:buildrules.bzl", "cc_gtest", "cc_gbench")

hoisted_library(
    name = "app",
    srcs = ["app.cc"],
    hdrs = ["app.h"],
    deps = [
        ":endpoints",
        ":storage",
        "//third_party/pistache",
        "//webs/scaffolding:component",
    ],
)

hoisted_library(
    name = "endpoints",
    srcs = ["endpoints.cc"],
    hdrs = ["endpoints.h"],
    deps = [
        ":storage",
        "//third_party/pistache",
        "//webs/scaffolding:endpoints",
    ],
)

hoisted_binary(
    name = "kv",
    srcs = ["kv.cc"],
    deps = [
        ":app",
    ],
)

hoisted_library(
    name = "storage",
    srcs = ["storage.cc"],
    hdrs = ["storage.h"],
    deps = [
        "//third_party/sqlite_modern_cpp",
        "//webs/scaffolding:component",
    ],
)

cc_gbench(
    name = "storage_bench",
    srcs = ["storage_bench.cc"],
    deps = [
        ":storage",
    ],
)

cc_gtest(
    name = "storage_test",
    srcs = ["storage_test.cc"],
    deps = [
        ":storage",
    ],
)
