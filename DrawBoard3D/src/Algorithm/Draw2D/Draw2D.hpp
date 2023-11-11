#pragma once
#include <functional>
#include <array>
#include <vector>
#include <stdexcept>
#include <format>

// @brief Base class for drawing 2D shapes.
class Draw2D
{
public:
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

    using Point = std::array<f32, 2>;

public:
    virtual ~Draw2D() {}

    virtual void SetParam(const std::vector<std::pair<std::string, void*>>& params) = 0;
    virtual void SetParam(const std::string& name, void* value) { SetParam({ {name, value} }); }

    virtual void Draw() const = 0;

public:
    template<typename Res = i32, typename T>
        requires std::is_arithmetic_v<T> && std::is_integral_v<Res>
    static Res Round(T x)
    {
        return Res(x + 0.5f);
    }

    template <typename Res = i32, typename... Ts>
        requires std::is_integral_v<Res>
    static auto Round(Ts... xs)
    {
        return std::make_tuple(Round<Res>(xs)...);
    }
};
