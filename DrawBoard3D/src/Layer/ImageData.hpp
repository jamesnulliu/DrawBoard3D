#pragma once
#include <Essentials.hpp>
#include <array>

namespace db3d {
    struct ImageData
    {
        explicit ImageData() : raw(nullptr), range({ 0,0 }) {}
        ~ImageData() { delete[] raw; }

        constexpr const u32& operator[](u64 _index) const { return raw[_index]; }
        constexpr u32& operator[](u64 _index) { return raw[_index]; }

        void resize(u64 _width, u64 _height)
        {
            if (width * height < _width * _height) {
                delete[] raw;
                raw = new u32[_width * _height]{ 0 };
            }
            range = { _width, _height };
        }

        void fill(i32 _data) { memset(raw, _data, width * height * sizeof(u32)); }

        u32* raw;
        union {
            struct {
                u64 width;
                u64 height;
            };
            std::array<u64, 2> range;
        };
    };
}
