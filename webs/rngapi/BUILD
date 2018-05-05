# Build rules for the RNG API

load("//hoist:buildrules.bzl", "hoisted_binary", "hoisted_library")
load("//tools:buildrules.bzl", "cc_gtest")

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
        "//third_party/nlohmann_json",
        "//third_party/pistache",
        "//webs/scaffolding:endpoints",
    ],
)

cc_gtest(
    name = "rngapi_test",
    srcs = ["rngapi_test.cc"],
    deps = [
        ":app",
        ":endpoints",
        ":storage",
        "//webs/scaffolding:component",
    ],
)

hoisted_binary(
    name = "rngservice",
    srcs = ["rngservice.cc"],
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
