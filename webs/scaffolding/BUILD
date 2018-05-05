# Build rules for the RNG API

load("//hoist:buildrules.bzl", "hoisted_binary", "hoisted_library")
load("//tools:buildrules.bzl", "cc_gtest")

hoisted_library(
    name = "component",
    srcs = ["component.cc"],
    hdrs = ["component.h"],
    visibility = ["//visibility:public"],
    deps = [
    ],
)

hoisted_library(
    name = "endpoints",
    srcs = ["endpoints.cc"],
    hdrs = ["endpoints.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":component",
        "//third_party/pistache",
    ],
)
