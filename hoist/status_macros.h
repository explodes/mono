#ifndef HOIST_STATUS_MACROS_H
#define HOIST_STATUS_MACROS_H

#include "hoist/likely.h"

// Testing only.
#define EXPECT_OK(status) EXPECT_TRUE((status).ok()) << status.ToString();
// Testing only.
#define ASSERT_OK(status) ASSERT_TRUE((status).ok()) << status.ToString();

// Evaluate 'stmt' to a Status object and if it results in an error, return that
// error.  Use 'tmp' as a variable name to avoid shadowing any variables named
// tmp.
#define RETURN_IF_ERROR_IMPL(tmp, stmt) \
  do {                                  \
    auto tmp = (stmt);                  \
    if (UNLIKELY(!tmp.ok())) {          \
      return tmp;                       \
    }                                   \
  } while (false)

// Create a unique variable name to avoid shadowing local variables.
#define RETURN_IF_ERROR_CONCAT(line, stmt) \
  RETURN_IF_ERROR_IMPL(__CONCAT(_status_, line), stmt)

// Macro to allow exception-like handling of error return values.
//
// If the evaluation of stmt results in an error, return that error
// from current function.
//
// Example usage:
// Status bar() { ... }
//
// RETURN_IF_ERROR(status);
// RETURN_IF_ERROR(bar());
#define RETURN_IF_ERROR(stmt) RETURN_IF_ERROR_CONCAT(__LINE__, stmt)

#define ASSIGN_OR_RETURN_IMPL(tmp, lhs, stmt) \
  auto tmp = (stmt);                          \
  RETURN_IF_ERROR(tmp.status());              \
  lhs = std::move(tmp.ValueOrDie());

#define ASSIGN_OR_RETURN_CONCAT(line, lhs, stmt) \
  ASSIGN_OR_RETURN_IMPL(__CONCAT(_status_or_, line), lhs, stmt)

// Macro to allow exception-like handling of error return values.
//
// If the evaluation of stmt results in an error, return that error
// from the current function. Otherwise, assign the result to lhs.
//
// This macro supports both move and copy assignment operators. lhs
// may be either a new local variable or an existing non-const
// variable accessible in the current scope.
//
// Example usage:
// StatusOr<MyType> foo() { ... }
//
// ASSIGN_OR_RETURN(auto myVar, foo());
// ASSIGN_OR_RETURN(myExistingVar, foo());
// ASSIGN_OR_RETURN(myMemberVar, foo());
#define ASSIGN_OR_RETURN(lhs, stmt) ASSIGN_OR_RETURN_CONCAT(__LINE__, lhs, stmt)

#endif  // HOIST_STATUS_MACROS_H