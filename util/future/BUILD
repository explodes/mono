cc_library(
    name = "executor",
    srcs = [],
    hdrs = ["executor.h"],
    deps = [
        ":future",
        ":queue",
        "//hoist:status",
        "//hoist:status_macros",
        "//hoist:statusor",
    ],
)

cc_library(
    name = "future",
    srcs = [],
    hdrs = ["future.h"],
    deps = [
        ":queue",
        "//hoist:status",
        "//hoist:status_macros",
        "//hoist:statusor",
    ],
)

cc_library(
    name = "queue",
    srcs = [],
    hdrs = ["queue.h"],
    deps = [
        "//hoist:logging",
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
        ":executor",
        ":future",
        "//hoist:init",
        "//hoist:logging",
        "//hoist:statusor",
    ],
)
