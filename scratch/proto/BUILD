# Sample protobuf library featuring proto dependencies,
# grpc, with a server and client using rpc and streaming
# protocols.

load("//tools:buildrules.bzl", "proto_library")

cc_binary(
    name = "client",
    srcs = ["client.cc"],
    deps = [
        ":sample_service_cc_pb",
        "//hoist:init",
        "//hoist:statusor",
        "//net/statusz:client",
    ],
)

proto_library(
    name = "sample",
    protos = [
        "sample.proto",
    ],
)

proto_library(
    name = "sample_service",
    proto_deps = [
        ":sample",
    ],
    protos = [
        "sample_service.proto",
    ],
    with_grpc = True,
)

cc_binary(
    name = "server",
    srcs = ["server.cc"],
    deps = [
        ":sample_service_cc_pb",
        "//net/statusz:service",
    ],
)
