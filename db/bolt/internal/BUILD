package(default_visibility = [
    "//db/bolt:__pkg__",
])

cc_library(
    name = "constants",
    srcs = [],
    hdrs = ["constants.h"],
    deps = [
        "//db/bolt:arch",
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
