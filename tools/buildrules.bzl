# Commmonly used rules and rule helpers

load("@org_pubref_rules_protobuf//cpp:rules.bzl", "cc_proto_library")
load("@org_pubref_rules_protobuf//go:rules.bzl", "go_proto_library")

def cc_gtest(**kwargs):
    """Create a test for use with gtest.

    The dependency:
        #include "gtest/gtest.h"
    is made available, for convenience.

    See //scratch/hello_test.cc for an example.
    """
    _add_dep(kwargs, '//third_party/googletest:gtest')
    _set_default(kwargs, 'size', 'small')
    native.cc_test(**kwargs)

def cc_gbench(**kwargs):
    """Create a benchmark for use with googlebenchmark.

    The dependency:
        #include "benchmark/benchmark.h"
    is made available, for convenience.

    See //scratch/hello_bench.cc for an example.
    """
    _add_dep(kwargs, '//third_party/googlebenchmark:main')
    _add_strings(kwargs, 'tags', 'exclusive', 'benchmark', 'manual')
    _set_default(kwargs, 'size', 'enormous')
    native.cc_test(**kwargs)

def cc_test_group(**kwargs):
    """Create multiple tests, one per file.

    Args:
        programs: list of programs to create test rules for.
            Each file will be its own test.
    """
    _test_group(kwargs, native.cc_test)

def cc_gtest_group(**kwargs):
    """Create multiple tests, one per file, with gtest support.

    Args:
        programs: list of programs to create test rules for.
            Each file will be its own test.
    """
    _test_group(kwargs, cc_gtest)

def _test_group(kwargs, test_func):
    """Create multiple tests, one per file.

    Args:
        programs: list of programs to create test rules for.
            Each file will be its own test.
        test_func: function to create individual tests with.
    """
    TEST_LIST_ARG = 'programs'
    name = _require(kwargs, 'name')
    tests = _require(kwargs, TEST_LIST_ARG)

    for test in tests:
        safe = test.replace('.', '_').replace('/', '_')
        new_kwargs = dict(**kwargs)
        new_kwargs.pop(TEST_LIST_ARG)
        new_kwargs['name'] = '{}_{}'.format(name, safe)
        new_kwargs['srcs'] = [test]
        test_func(**new_kwargs)

def proto_library(**kwargs):
    """Create a proto library and language-specific counterparts.

    The proto_library is defined with the given name, and language-specific
    counterparts are suffixed with a _<lang>_pb extension.

    Example:
        proto_library(name="hello_world")
    
    creates:
        cc_proto_library("hello_world_cc_pb")
    
    See //scratch/proto for an example.
    """
    _require(kwargs, 'name')
    _set_default(kwargs, 'verbose', 0)
    _set_default(kwargs, 'with_grpc', False)

    go_package = kwargs.pop('go_package', None)

    cc_params = _proto_params(kwargs, 'cc')
    cc_proto_library(**cc_params)

    go_params = _proto_params(kwargs, 'go')
    if go_package:
        go_params['go_package'] = go_package
    go_proto_library(**go_params)

def _proto_params(params, lang_code):
    """Builds params for a specific language.

    This appends the _lang_pb suffix to the name and to all proto_deps.

    Args:
        params: dict of arguments to duplicate and modify
        lang_code: language code to use for renaming

    Returns:
        The new params
    """
    # Duplicate the parameters with a new name
    out = _with_name_suffix(params, '_' + lang_code + '_pb')

    # Rename all proto_deps by appending _lang_pb to the name
    deps = params.get('proto_deps', None)
    if deps:
      out['proto_deps'] = [dep + '_' + lang_code + '_pb' for dep in deps]

    return out

def _with_name_suffix(params, suffix):
    """Append a suffix to the name of params.

    Args:
        params: dict of arguments to duplicate and modify
        suffix: suffix to append

    Returns:
        The new params
    """
    out = dict(**params)
    out['name'] = out['name'] + suffix
    return out

def _add_dep(params, dep, param_key='deps'):
    """Add a dependency the deps in some parameters.

    Args:
        params: dict of arguments to modify
        dep: build target of the dependency to add
        param_key: name of the dependencies parameter
    """
    deps = depset(params.get(param_key, []))
    deps += depset([dep])
    params[param_key] = deps

def _add_strings(params, param_key, *s):
    """Add strings to a list inside parameters.

    Args:
        params: dict of arguments to modify
        param_key: value to append strings to
        s: strings to add
    """
    copts = params.get(param_key, [])
    copts.extend(s)
    params[param_key] = copts

def _set_default(params, param_key, value):
    """Assign a default value if a parameter is missing.

    Args:
        params: dict of arguments to modify
        param_key: value to set, if missing
        value: value to set param to, if missing
    """
    if param_key not in params:
        params[param_key] = value

def _require(params, param_key):
    """Fail if a parameter is missing.
    
    Args:
        params: dict of arguments to test against
        param_key: name of the parameter to test for

    Returns:
        The value of the parameter
    """
    if param_key not in params:
        fail('parameter {} is missing.'.format(param_key))
    return params[param_key]
