# statusz server and client implementations

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "service",
    srcs = ["service.cc"],
    hdrs = ["service.h"],
    deps = [
        ":memory",
        "//proto/common:empty_cc_pb",
        "//proto/statusz:statusz_cc_pb",
        "//proto/statusz:statusz_service_cc_pb",
    ],
)

cc_binary(
    name = "check",
    srcs = ["check.cc"],
    deps = [
        ":client",
        "//hoist:init",
        "//hoist:logging",
        "//hoist:status",
    ],
)

cc_library(
    name = "client",
    srcs = ["client.cc"],
    hdrs = ["client.h"],
    deps = [
        "//hoist:logging",
        "//hoist:status",
        "//hoist:statusor",
        "//proto/common:empty_cc_pb",
        "//proto/statusz:statusz_cc_pb",
        "//proto/statusz:statusz_service_cc_pb",
    ],
)

cc_library(
    name = "memory",
    srcs = ["memory.cc"],
    hdrs = ["memory.h"],
    visibility = ["//visibility:__pkg__"],
)
