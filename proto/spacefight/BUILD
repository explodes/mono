# Proto library for system health status reports

load("//tools:buildrules.bzl", "proto_library")

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "spacefight",
    proto_deps = [
        "//proto/game:physics",
    ],
    protos = [
        "spacefight.proto",
    ],
)

proto_library(
    name = "spacefight_service",
    proto_deps = [
        ":spacefight",
        "//proto/common:empty",
    ],
    protos = [
        "spacefight_service.proto",
    ],
    with_grpc = True,
)
