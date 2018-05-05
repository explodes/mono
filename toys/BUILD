# Build rules for various toys and small projects

load("//hoist:buildrules.bzl", "hoisted_binary")

hoisted_binary(
    name = "balldrop",
    srcs = ["balldrop.cc"],
)

hoisted_binary(
    name = "blackjack",
    srcs = ["blackjack.cc"],
)

hoisted_binary(
    name = "hilo",
    srcs = ["hilo.cc"],
)

hoisted_binary(
    name = "loggs",
    srcs = ["loggs.cc"],
)

cc_binary(
    name = "mud",
    srcs = ["mud.cc"],
)

hoisted_binary(
    name = "multithreaded",
    srcs = ["multithreaded.cc"],
    linkopts = ["-pthread"],
    deps = [
        "//hoist/sync:waitgroup",
    ],
)

hoisted_binary(
    name = "profile",
    srcs = ["profile.cc"],
)

hoisted_binary(
    name = "race",
    srcs = ["race.cc"],
    linkopts = ["-pthread"],
)

hoisted_binary(
    name = "socketed",
    srcs = ["socketed.cc"],
)

cc_binary(
    name = "sqlitetest",
    srcs = ["sqlitetest.cc"],
    deps = [
        "//third_party/sqlite_modern_cpp",
    ],
)
