// Copyright (c) 2020 John Pursey
//
// Use of this source code is governed by an MIT-style License that can be found
// in the LICENSE file or at https://opensource.org/licenses/MIT.

#ifndef GB_TEST_TEST_UTIL_H_
#define GB_TEST_TEST_UTIL_H_

#include <string>

namespace gb {

// Generates a pseudo-random ASCII string of the specified size which further
// guarantees that no two adjacent characters in the string are the same (to
// help detect off-by-one errors).
std::string GenerateTestString(int64_t size);

// Generates a pseudo-random ASCII string of the specified size containing only
// lowercase characters. It further guarantees that no two adjacent characters
// in the string are the same (to help detect off-by-one errors).
std::string GenerateAlphaTestString(int64_t size);

}  // namespace gb

#endif  // GB_TEST_TEST_UTIL_H_
