# Build rules for the Hoist library

load("//hoist:buildrules.bzl", "hoisted_library")
load("//tools:buildrules.bzl", "cc_gtest")

hoisted_library(
    name = "base",
    srcs = [],
    hdrs = ["base.h"],
)

hoisted_library(
    name = "buffer",
    srcs = ["buffer.cc"],
    hdrs = ["buffer.h"],
    deps = [
        ":base",
    ],
)

cc_gtest(
    name = "buffer_test",
    srcs = ["buffer_test.cc"],
    deps = [
        ":buffer",
    ],
)

hoisted_library(
    name = "io",
    srcs = [],
    hdrs = ["io.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":base",
        ":buffer",
    ],
)
