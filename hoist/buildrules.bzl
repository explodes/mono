def _add_hoist_dep(params):
    deps = depset(params.get('deps', []))
    deps += depset(['//hoist'])
    params['deps'] = deps

def hoisted_library(**kwargs):
    _add_hoist_dep(kwargs)
    native.cc_library(**kwargs)

def hoisted_binary(**kwargs):
    _add_hoist_dep(kwargs)
    native.cc_binary(**kwargs)
