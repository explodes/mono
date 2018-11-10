# Build rules for various toys and small projects

cc_binary(
    name = "balldrop",
    srcs = ["balldrop.cc"],
    deps = [
        "//hoist:init",
        "//hoist:input",
        "//hoist:statusor",
    ],
)

cc_binary(
    name = "blackjack",
    srcs = ["blackjack.cc"],
    deps = [
        "//hoist:init",
        "//hoist:input",
        "//hoist:math",
        "//hoist:statusor",
    ],
)

cc_binary(
    name = "hilo",
    srcs = ["hilo.cc"],
    deps = [
        "//hoist:init",
        "//hoist:input",
        "//hoist:math",
        "//hoist:statusor",
    ],
)

cc_binary(
    name = "loggs",
    srcs = ["loggs.cc"],
    deps = [
        "//hoist:init",
        "//hoist:logging",
    ],
)

cc_binary(
    name = "mud",
    srcs = ["mud.cc"],
    deps = [
        "//hoist:init",
    ],
)

cc_binary(
    name = "multithreaded",
    srcs = ["multithreaded.cc"],
    linkopts = ["-pthread"],
    deps = [
        "//hoist:init",
        "//hoist/sync:waitgroup",
    ],
)

cc_binary(
    name = "profile",
    srcs = ["profile.cc"],
    deps = [
        "//hoist:init",
        "//hoist:input",
        "//hoist:statusor",
    ],
)

cc_binary(
    name = "race",
    srcs = ["race.cc"],
    linkopts = ["-pthread"],
    deps = [
        "//hoist:init",
    ],
)

cc_binary(
    name = "socketed",
    srcs = ["socketed.cc"],
    copts = [
        "-fexceptions",
    ],
    deps = [
        "//hoist:init",
    ],
)

cc_binary(
    name = "sqlitetest",
    srcs = ["sqlitetest.cc"],
    copts = [
        "-fexceptions",
    ],
    deps = [
        "//hoist:init",
        "//third_party/sqlite_modern_cpp",
    ],
)
