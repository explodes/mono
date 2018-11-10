# Build rules for the RNG API

cc_library(
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

cc_library(
    name = "endpoints",
    srcs = ["endpoints.cc"],
    hdrs = ["endpoints.h"],
    deps = [
        ":storage",
        "//hoist:logging",
        "//third_party/pistache",
        "//webs/scaffolding:endpoints",
    ],
)

cc_binary(
    name = "kv",
    srcs = ["kv.cc"],
    deps = [
        ":app",
        "//hoist:init",
        "//hoist:logging",
    ],
)

cc_library(
    name = "storage",
    srcs = ["storage.cc"],
    hdrs = ["storage.h"],
    deps = [
        "//hoist:status",
        "//hoist:statusor",
        "//third_party/sqlite_modern_cpp",
        "//webs/scaffolding:component",
    ],
)

cc_test(
    name = "storage_bench",
    srcs = ["storage_bench.cc"],
    tags = [
        "benchmark",
        "exclusive",
        "manual",
    ],
    deps = [
        ":storage",
        "//third_party/benchmark",
    ],
)

cc_test(
    name = "storage_test",
    size = "small",
    srcs = ["storage_test.cc"],
    deps = [
        ":storage",
        "//third_party/googletest:gtest",
    ],
)
