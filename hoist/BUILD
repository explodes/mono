# Build rules for the Hoist library
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "clock",
    srcs = ["clock.cc"],
    hdrs = ["clock.h"],
)

cc_library(
    name = "init",
    srcs = ["init.cc"],
    hdrs = ["init.h"],
    deps = [
        ":logging",
    ],
)

cc_library(
    name = "input",
    srcs = ["input.cc"],
    hdrs = ["input.h"],
    deps = [
        ":status",
        ":statusor",
    ],
)

cc_library(
    name = "likely",
    hdrs = ["likely.h"],
    deps = [
        ":portability",
    ],
)

cc_library(
    name = "logging",
    srcs = ["logging.cc"],
    hdrs = ["logging.h"],
)

cc_test(
    name = "logging_test",
    size = "small",
    srcs = ["logging_test.cc"],
    copts = [
        "-DLOG_LEVEL=4",
    ],
    deps = [
        ":logging",
        "//third_party/googletest:gtest",
    ],
)

cc_library(
    name = "macros",
    hdrs = ["macros.h"],
)

cc_library(
    name = "math",
    srcs = ["math.cc"],
    hdrs = ["math.h"],
)

cc_library(
    name = "portability",
    hdrs = ["portability.h"],
)

cc_library(
    name = "status",
    srcs = ["status.cc"],
    hdrs = ["status.h"],
)

cc_library(
    name = "status_macros",
    hdrs = ["status_macros.h"],
    deps = [
        ":likely",
    ],
)

cc_test(
    name = "status_test",
    size = "small",
    srcs = ["status_test.cc"],
    deps = [
        ":status",
        "//third_party/googletest:gtest",
    ],
)

cc_library(
    name = "statusor",
    srcs = ["statusor.cc"],
    hdrs = ["statusor.h"],
    deps = [":status"],
)

cc_test(
    name = "statusor_test",
    size = "small",
    srcs = ["statusor_test.cc"],
    deps = [
        ":status",
        ":statusor",
        "//third_party/googletest:gtest",
    ],
)
