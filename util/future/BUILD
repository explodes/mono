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
    deps = [
        "//hoist:status",
        "//hoist:statusor",
    ],
)

cc_test(
    name = "queue_test",
    srcs = ["queue_test.cc"],
    deps = [
        ":queue",
        "//hoist:status_macros",
        "//third_party/googletest:gtest",
    ],
)

cc_binary(
    name = "scratch",
    srcs = ["scratch.cc"],
    linkopts = [
        "-lpthread",
    ],
    deps = [
        ":function",
        ":queue",
        "//hoist:logging",
        "//hoist:status_macros",
        "//hoist:statusor",
    ],
)
