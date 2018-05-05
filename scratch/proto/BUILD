# Sample protobuf library featuring proto dependencies,
# grpc, with a server and client using rpc and streaming
# protocols.

load("//hoist:buildrules.bzl", "hoisted_binary")
load("//tools:buildrules.bzl", "proto_library")

hoisted_binary(
    name = "client",
    srcs = ["client.cc"],
    deps = [
        ":sample_service_cc_pb",
        "//webs/statusz:client",
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
        "//webs/statusz:service",
    ],
)
