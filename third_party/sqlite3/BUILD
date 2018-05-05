licenses(["notice"])

cc_library(
    name = "sqlite3",
    srcs = ["sqlite3.c"],
    hdrs = ["sqlite3.h"],
    includes = [
        ".",
    ],
    linkopts = ["-pthread", "-ldl"],
    visibility = ["//visibility:public"],
)
