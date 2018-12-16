cc_library(
    name = "memfile",
    srcs = ["memfile.cc"],
    hdrs = ["memfile.h"],
    deps = [
        "//hoist:macros",
        "//hoist:status",
        "//hoist:status_macros",
        "//hoist:statusor",
    ],
)

cc_test(
    name = "memfile_test",
    srcs = ["memfile_test.cc"],
    deps = [
        ":memfile",
        "//hoist:status",
        "//hoist:status_macros",
        "//third_party/googletest:gtest",
    ],
)
