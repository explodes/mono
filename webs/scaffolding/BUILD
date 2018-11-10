# Build rules for the RNG API

cc_library(
    name = "component",
    srcs = ["component.cc"],
    hdrs = ["component.h"],
    visibility = ["//visibility:public"],
    deps = [
    ],
)

cc_library(
    name = "endpoints",
    srcs = ["endpoints.cc"],
    hdrs = ["endpoints.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":component",
        "//third_party/pistache",
    ],
)
