cc_library(
    name = "btree",
    srcs = [],
    hdrs = ["btree.h"],
    deps = [
        ":key",
    ],
)

cc_test(
    name = "btree_test",
    srcs = ["btree_test.cc"],
    deps = [
        ":btree",
        "//third_party/googletest:gtest",
    ],
)

cc_library(
    name = "key",
    srcs = [],
    hdrs = ["key.h"],
    deps = [
    ],
)

cc_test(
    name = "key_test",
    srcs = ["key_test.cc"],
    deps = [
        ":key",
        "//third_party/googletest:gtest",
    ],
)

cc_library(
    name = "page",
    srcs = ["page.cc"],
    hdrs = ["page.h"],
    deps = [
        "//hoist:likely",
        "//hoist:status",
        "//hoist:statusor",
    ],
)

cc_test(
    name = "page_test",
    srcs = ["page_test.cc"],
    deps = [
        ":page",
        "//hoist:status",
        "//hoist:statusor",
        "//third_party/googletest:gtest",
    ],
)
