new_http_archive(
    name = "gtest",
    build_file = "third_party/gtest.BUILD",
    sha256 = "b58cb7547a28b2c718d1e38aee18a3659c9e3ff52440297e965f5edffe34b6d0",
    strip_prefix = "googletest-release-1.7.0",
    url = "https://github.com/google/googletest/archive/release-1.7.0.zip",
)

new_http_archive(
    name = "googlebenchmark",
    build_file = "third_party/googlebenchmark.BUILD",
    sha256 = "7f5f3608c9228fa023151a4b54e91f4ada4b7b49c26facede6c5b8b83ddbedad",
    strip_prefix = "benchmark-1.4.0",
    url = "https://github.com/google/benchmark/archive/v1.4.0.zip",
)

http_archive(
    name = "io_bazel_rules_go",
    sha256 = "1868ff68d6079e31b2f09b828b58d62e57ca8e9636edff699247c9108518570b",
    url = "https://github.com/bazelbuild/rules_go/releases/download/0.11.1/rules_go-0.11.1.tar.gz",
)

load("@io_bazel_rules_go//go:def.bzl", "go_rules_dependencies", "go_register_toolchains")

go_rules_dependencies()

go_register_toolchains()

# http_archive(
#     name = "com_google_protobuf",
#     sha256 = "7404d040865a031e80c2810cd9453eafefb2bbbf5f5f9a282b1d071d8e12c4bf",
#     strip_prefix = "protobuf-3.5.2",
#     url = "https://github.com/google/protobuf/archive/v3.5.2.zip",
# )

git_repository(
    name = "org_pubref_rules_protobuf",
    remote = "https://github.com/pubref/rules_protobuf",
    tag = "v0.8.2",
    #commit = "..." # alternatively, use latest commit on master
)

load("@org_pubref_rules_protobuf//cpp:rules.bzl", "cpp_proto_repositories")

cpp_proto_repositories()

load("@org_pubref_rules_protobuf//go:rules.bzl", "go_proto_repositories")

go_proto_repositories()
