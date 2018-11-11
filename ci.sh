#!/bin/bash

function _abort {
    echo $1
    exit 1
}

mapfile -t test_rules < <(bazel query 'kind(.*_test, ...) except //third_party/... except attr(tags, manual, ...)' --keep_going --output label 2>/dev/null)
for rule in "${test_rules[@]}"; do
  echo "executing test $rule..."
  bazel test $rule || _abort "$rule test failed."
done

mapfile -t library_rules < <(bazel query 'kind(.*_library, ...) except //third_party/...' --keep_going --output label 2>/dev/null)
for rule in $library_rules; do
  echo "building library $rule..."
  bazel build $rule || _abort "$rule library failed."
done

mapfile -t binary_rules < <(bazel query 'kind(.*_binary, ...) except //third_party/...' --keep_going --output label 2>/dev/null)
for rule in $binary_rules; do
  echo "building binary $rule..."
  bazel build $rule || _abort "$rule binary failed."
done

echo "Success!"
echo "${#test_rules[@]} tests ran sucessfully."
echo "${#library_rules[@]} libraries built sucessfully."
echo "${#binary_rules[@]} binaries built sucessfully."