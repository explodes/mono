cc_library(
    name = "arch",
    srcs = [],
    hdrs = ["arch.h"],
    deps = [
    ],
)

cc_library(
    name = "bolt",
    srcs = ["bolt.cc"],
    hdrs = ["bolt.h"],
    deps = [
        ":bytes",
        ":constants",
        ":errors",
        ":page",
        ":status",
        ":statusor",
    ],
)

cc_test(
    name = "bucket_test",
    srcs = ["bucket_test.cc"],
    deps = [
        ":bolt",
        "//third_party/googletest:gtest",
    ],
)

cc_library(
    name = "bytes",
    srcs = [],
    hdrs = ["bytes.h"],
    deps = [
    ],
)

cc_library(
    name = "constants",
    srcs = [],
    hdrs = ["constants.h"],
    deps = [
        "//db/bolt:arch",
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

cc_test(
    name = "database_test",
    srcs = ["database_test.cc"],
    deps = [
        ":bolt",
        "//third_party/googletest:gtest",
    ],
)

cc_library(
    name = "errors",
    srcs = [],
    hdrs = ["errors.h"],
    deps = [
        ":status",
    ],
)

cc_library(
    name = "freelist",
    srcs = ["freelist.cc"],
    hdrs = ["freelist.h"],
    deps = [
    ],
)

cc_test(
    name = "freelist_test",
    srcs = ["freelist_test.cc"],
    deps = [
        ":freelist",
        "//third_party/googletest:gtest",
    ],
)

cc_library(
    name = "node",
    srcs = ["node.cc"],
    hdrs = ["node.h"],
    deps = [
    ],
)

cc_test(
    name = "node_test",
    srcs = ["node_test.cc"],
    deps = [
        ":node",
        "//third_party/googletest:gtest",
    ],
)

cc_library(
    name = "page",
    srcs = ["page.cc"],
    hdrs = ["page.h"],
    deps = [
    ],
)

cc_test(
    name = "page_test",
    srcs = ["page_test.cc"],
    deps = [
        ":page",
        "//third_party/googletest:gtest",
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
    name = "status",
    srcs = ["status.cc"],
    hdrs = ["status.h"],
)

cc_test(
    name = "status_test",
    size = "small",
    srcs = ["status_test.cc"],
    deps = [
        ":status",
        "//third_party/googletest:gtest",
    ],
)

cc_library(
    name = "statusor",
    srcs = ["statusor.cc"],
    hdrs = ["statusor.h"],
    deps = [":status"],
)

cc_test(
    name = "statusor_test",
    size = "small",
    srcs = ["statusor_test.cc"],
    deps = [
        ":status",
        ":statusor",
        "//third_party/googletest:gtest",
    ],
)

cc_test(
    name = "tx_test",
    srcs = ["tx_test.cc"],
    deps = [
        ":bolt",
        "//third_party/googletest:gtest",
    ],
)
