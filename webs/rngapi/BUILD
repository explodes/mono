# Build rules for the RNG API

cc_library(
    name = "app",
    srcs = ["app.cc"],
    hdrs = ["app.h"],
    deps = [
        ":endpoints",
        ":storage",
        "//third_party/pistache",
        "//webs/scaffolding:component",
    ],
)

cc_library(
    name = "endpoints",
    srcs = ["endpoints.cc"],
    hdrs = ["endpoints.h"],
    deps = [
        ":storage",
        "//hoist:math",
        "//third_party/nlohmann_json",
        "//third_party/pistache",
        "//webs/scaffolding:endpoints",
    ],
)

cc_library(
    name = "rngservice",
    srcs = ["rngservice.cc"],
    deps = [
        ":app",
        "//hoist:init",
    ],
)

cc_library(
    name = "storage",
    srcs = ["storage.cc"],
    hdrs = ["storage.h"],
    deps = [
        "//hoist:status",
        "//hoist:statusor",
        "//third_party/sqlite_modern_cpp",
        "//webs/scaffolding:component",
    ],
)
