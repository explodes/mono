cc_library(
    name = "function",
    srcs = [],
    hdrs = ["function.h"],
    deps = [],
)

cc_library(
    name = "executor",
    srcs = ["executor.cc"],
    hdrs = ["executor.h"],
    deps = [
        ":function",
        ":future",
    ],
)

cc_test(
    name = "executor_test",
    srcs = ["executor_test.cc"],
    deps = [
        ":executor",
        ":function",
        ":future",
        ":queue",
        "//hoist:logging",
        "//third_party/googletest:gtest",
    ],
)

cc_library(
    name = "future",
    srcs = [],
    hdrs = ["future.h"],
    deps = [
        ":queue",
        "//hoist:logging",
    ],
)

cc_library(
    name = "queue",
    srcs = [],
    hdrs = ["queue.h"],
    deps = [],
)
