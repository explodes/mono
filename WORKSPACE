local_repository(
    name = "com_google_absl",
    path = "third_party/abseil-cpp",
)

local_repository(
    name = "gtest",
    path = "third_party/gtest",
)

local_repository(
    name = "googlebenchmark",
    path = "third_party/googlebenchmark",
)

local_repository(
    name = "io_bazel_rules_go",
    path = "third_party/rules_go",
)

local_repository(
    name = "bazel_skylib",
    path = "third_party/bazel_skylib",
)

load("@io_bazel_rules_go//go:def.bzl", "go_rules_dependencies", "go_register_toolchains")

go_rules_dependencies()

go_register_toolchains()

local_repository(
    name = "org_pubref_rules_protobuf",
    path = "third_party/rules_protobuf",
)

load("@org_pubref_rules_protobuf//cpp:rules.bzl", "cpp_proto_repositories")

cpp_proto_repositories()

load("@org_pubref_rules_protobuf//go:rules.bzl", "go_proto_repositories")

go_proto_repositories()
