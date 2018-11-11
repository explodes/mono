# spacefight server and client implementations

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "color",
    hdrs = ["color.h"],
)

cc_library(
    name = "debug",
    srcs = ["debug.cc"],
    hdrs = ["debug.h"],
    deps = [
        "//hoist:logging",
        "@com_google_grpc//:grpc++",
    ],
)

cc_library(
    name = "elements",
    hdrs = ["elements.h"],
    deps = [
        ":maths",
    ],
)

cc_library(
    name = "game",
    srcs = ["game.cc"],
    hdrs = ["game.h"],
    deps = [
        ":color",
        ":debug",
        ":elements",
        ":physics",
        "//hoist:clock",
        "//hoist:likely",
        "//hoist:logging",
        "//hoist:math",
        "//proto/spacefight:spacefight_cc_pb",
    ],
)

cc_library(
    name = "maths",
    hdrs = ["maths.h"],
)

cc_library(
    name = "physics",
    srcs = ["physics.cc"],
    hdrs = [
        "physics.h",
    ],
    deps = [
        "//proto/spacefight:spacefight_cc_pb",
    ],
)

cc_binary(
    name = "server",
    srcs = ["server.cc"],
    deps = [
        ":game",
        ":service",
        "//hoist:init",
        "//hoist:logging",
        "//net/statusz:service",
    ],
)

cc_library(
    name = "service",
    srcs = ["service.cc"],
    hdrs = ["service.h"],
    deps = [
        ":game",
        "//hoist:logging",
        "//hoist:math",
        "//hoist:status",
        "//proto/spacefight:spacefight_cc_pb",
        "//proto/spacefight:spacefight_service_cc_pb",
    ],
)
