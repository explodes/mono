# Build rules for a C++ scratchpad.

cc_binary(
    name = "scratch",
    srcs = ["scratch.cc"],
)

cc_test(
    name = "hello-test",
    size = "small",
    srcs = [
        "hello_test.cc",
    ],
    deps = [
        "//third_party/googletest:gtest",
    ],
)

cc_test(
    name = "hello-bench",
    size = "enormous",
    srcs = [
        "hello_bench.cc",
    ],
    tags = [
        "benchmark",
        "exclusive",
        "manual",
    ],
    deps = [
        "//third_party/benchmark",
    ],
)
