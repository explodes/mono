package(default_visibility = [
    "//db/bolt/internal:__pkg__",
])

cc_library(
    name = "arch",
    srcs = [],
    hdrs = ["arch.h"],
    deps = [
    ],
)

cc_library(
    name = "bucket",
    srcs = ["bucket.cc"],
    hdrs = ["bucket.h"],
    deps = [
        ":tx",
        "//db/bolt/internal:page",
    ],
)

cc_test(
    name = "bucket_test",
    srcs = ["bucket_test.cc"],
    deps = [
        ":bucket",
        "//third_party/googletest:gtest",
    ],
)

cc_library(
    name = "cursor",
    srcs = ["cursor.cc"],
    hdrs = ["cursor.h"],
    deps = [
    ],
)

cc_test(
    name = "cursor_test",
    srcs = ["cursor_test.cc"],
    deps = [
        ":cursor",
        "//third_party/googletest:gtest",
    ],
)

cc_library(
    name = "database",
    srcs = ["database.cc"],
    hdrs = ["database.h"],
    deps = [
    ],
)

cc_test(
    name = "database_test",
    srcs = ["database_test.cc"],
    deps = [
        ":database",
        "//third_party/googletest:gtest",
    ],
)

cc_library(
    name = "errors",
    srcs = [],
    hdrs = ["errors.h"],
    deps = [
    ],
)

cc_test(
    name = "quick_test",
    srcs = ["quick_test.cc"],
    deps = [
        "//third_party/googletest:gtest",
    ],
)

cc_test(
    name = "simulation_test",
    srcs = ["simulation_test.cc"],
    deps = [
        "//third_party/googletest:gtest",
    ],
)

cc_library(
    name = "tx",
    srcs = ["tx.cc"],
    hdrs = ["tx.h"],
    deps = [
    ],
)

cc_test(
    name = "tx_test",
    srcs = ["tx_test.cc"],
    deps = [
        ":tx",
        "//third_party/googletest:gtest",
    ],
)
