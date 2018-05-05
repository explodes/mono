# Build rules for a C++ scratchpad.

load("//hoist:buildrules.bzl", "hoisted_binary")
load("//tools:buildrules.bzl", "cc_gtest", "cc_gbench")

hoisted_binary(
    name = "scratch",
    srcs = ["scratch.cc"],
)

cc_gtest(
    name = "hello-test",
    srcs = ["hello_test.cc"],
)

cc_gbench(
    name = "hello-bench",
    srcs = ["hello_bench.cc"],
)
