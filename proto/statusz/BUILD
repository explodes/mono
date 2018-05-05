# Proto library for system health status reports

load("//tools:buildrules.bzl", "proto_library")

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "statusz",
    protos = [
        "statusz.proto",
    ],
)

proto_library(
    name = "statusz_service",
    proto_deps = [
        ":statusz",
        "//proto/common:empty",
    ],
    protos = [
        "statusz_service.proto",
    ],
    with_grpc = True,
)
