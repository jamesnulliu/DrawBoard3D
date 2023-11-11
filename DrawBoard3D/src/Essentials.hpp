#pragma once

// stdc++.h ==============================================================================
#include <vcruntime.h>  // This is a visual c++ project
static_assert(_HAS_CXX20, "C++20 is the lowest standard to run this project.");

// C
#ifndef _GLIBCXX_NO_ASSERT
#include <cassert>
#endif
#include <cctype>
#include <cerrno>
#include <cfloat>

#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <cfenv>
#include <cinttypes>
#include <cstdint>
#include <cwchar>
#include <cwctype>

// C++
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>

// C++11
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

// C++14
#include <shared_mutex>
#include <string_view>

// C++17
#include <any>
#include <charconv>
#include <execution>
#include <filesystem>
#include <optional>
#include <string_view>
#include <variant>

// C++20
#include <concepts>
#include <numbers>
#include <span>
#include <syncstream>
#include <version>
// stdc++.h ==============================================================================


// GLM ===================================================================================
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/intersect.hpp"
// GLM ===================================================================================

namespace db3d {
    using i8 = ::int8_t;
    using i16 = ::int16_t;
    using i32 = ::int32_t;
    using i64 = ::int64_t;
    using u8 = ::uint8_t;
    using u16 = ::uint16_t;
    using u32 = ::uint32_t;
    using u64 = ::uint64_t;
    using f32 = float;
    using f64 = double;
}
