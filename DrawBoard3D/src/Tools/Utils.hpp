#pragma once
#include "Essentials.hpp"

namespace db3d {
    constexpr f32 PI = 3.14159265358979323846f;
    constexpr f32 Degree2Radian(f32 degree) { return degree * PI / 180.0f; }
    constexpr f32 Radian2Degree(f32 radian) { return radian * 180.0f / PI; }

    constexpr u32 RGBA2UINT32(const glm::vec4& rgba) 
    {
        return
            (static_cast<uint32_t>(rgba.r * 255)) |
            (static_cast<uint32_t>(rgba.g * 255) << 8) |
            (static_cast<uint32_t>(rgba.b * 255) << 16) |
            (static_cast<uint32_t>(1 * 255) << 24);
    }

    template<typename Res = i32, typename T>
        requires std::is_arithmetic_v<T> && std::is_integral_v<Res>
    constexpr Res Round(T x)
    {
        return Res(x + 0.5f);
    }

    template <typename Res = i32, typename... Ts>
        requires std::is_integral_v<Res>
    constexpr auto Round(Ts... xs)
    {
        return std::make_tuple(Round<Res>(xs)...);
    }

    template<typename T1, typename T2, typename T3>
        requires std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_arithmetic_v<T3>
    constexpr u64 To1DIndex(T1 x, T2 y, T3 width)
    {
        return Round<u64>(x) + u64(width) * Round<u64>(y);
    }

    template<typename T>
    concept IsPoint2D = requires(T point, decltype(point[0]) x, decltype(point[1]) y) {
        { decltype(point[0])(point[0]) } -> std::convertible_to<f32>;
        { decltype(point[1])(point[1]) } -> std::convertible_to<f32>;
        { T{ x, y } } -> std::same_as<T>;
    };

    template<typename T>
    concept IsPoint3D = requires(T point, decltype(point[0]) x, decltype(point[1]) y, decltype(point[2]) z) {
        { decltype(point[0])(point[0]) } -> std::convertible_to<f32>;
        { decltype(point[1])(point[1]) } -> std::convertible_to<f32>;
        { decltype(point[2])(point[2]) } -> std::convertible_to<f32>;
        { T{ x, y, z } } -> std::same_as<T>;
    };


    template<typename Point, typename T>
        requires IsPoint2D<Point> && std::is_arithmetic_v<T>
    constexpr u64 To1DIndex(const Point& point, T width)
    {
        return To1DIndex(point[0], point[1], width);
    }

    constexpr u32 PCG_Hash(u32 x)
    {
        u32 state = x * 747796405u + 2891336453u;
        u32 word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
        return (word >> 22u) ^ word;
    }

    constexpr f32 RandomFloat32(u32& seed)
    {
        seed = PCG_Hash(seed);
        return f32(seed) / f32(std::numeric_limits<u32>::max());
    }

}
