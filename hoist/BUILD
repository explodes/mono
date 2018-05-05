# Build rules for the Hoist library

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "clock",
    srcs = ["clock.cc"],
    hdrs = ["clock.h"],
)

cc_library(
    name = "errors",
    srcs = ["errors.cc"],
    hdrs = ["errors.h"],
)

cc_library(
    name = "hoist",
    srcs = [],
    hdrs = ["hoist.h"],
    deps = [
        ":errors",
        ":init",
        ":input",
        ":logging",
        ":math",
    ],
)

cc_library(
    name = "init",
    srcs = ["init.cc"],
    hdrs = ["init.h"],
)

cc_library(
    name = "input",
    srcs = ["input.cc"],
    hdrs = ["input.h"],
    deps = [
        ":errors",
    ],
)

cc_library(
    name = "logging",
    srcs = [],
    hdrs = ["logging.h"],
)

cc_library(
    name = "math",
    srcs = ["math.cc"],
    hdrs = ["math.h"],
)
