licenses(["notice"])

load("//tools:buildrules.bzl", "cc_test_group")

cc_library(
    name = "sqlite_modern_cpp",
    srcs = [],
    hdrs = glob(["hdr/**/*.h"]),
    includes = [
        "hdr",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//third_party/sqlite3",
    ],
)

cc_test_group(
    name = "sqlite_modern_cpp",
    programs = glob(
        ["tests/*.cc"],
        exclude = [
            "tests/boost_optional.cc",
            "tests/exception_dont_execute_nested.cc",
            "tests/sqlcipher.cc",
            "tests/std_optional.cc",
            "tests/variant.cc",
        ],
    ),
    deps = [
        ":sqlite_modern_cpp",
    ],
)
