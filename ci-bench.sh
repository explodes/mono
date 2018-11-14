#!/bin/bash

function _abort {
    echo $1
    exit 1
}

mapfile -t benchmark_rules < <(bazel query 'kind(.*_test, attr(tags, manual, ...)) except //third_party/...' --keep_going --output label 2>/dev/null)
for rule in "${benchmark_rules[@]}"; do
  echo "executing test $rule..."
  bazel test $rule || _abort "$rule benchmark failed."
done

echo "Success!"
echo "${#benchmark_rules[@]} benchmarks ran sucessfully."