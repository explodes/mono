licenses(["notice"])

load("//tools:buildrules.bzl", "cc_gtest_group")

cc_library(
    name = "pistache",
    srcs = glob(["src/**/*.cc"]),
    hdrs = glob(["include/**/*.h"]),
    copts = ["-w"],
    includes = [
        "include",
    ],
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
)

cc_gtest_group(
    name = "pistache",
    copts = ["-w"],
    programs = glob(["tests/**/*.cc"]),
    deps = [
        ":pistache",
    ],
)
