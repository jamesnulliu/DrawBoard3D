#pragma once
#include "Draw2D.hpp"
#include "../Clip/CohenSutherland.hpp"

class Bresenham : public Draw2D
{
public:
    explicit Bresenham() = default;

    virtual void SetParam(const std::vector<std::pair<std::string, void*>>& params) override final
    {
        for (auto& [name, val] : params) {
            if (name == "start_point") {
                m_start = m_trsfFunc(*static_cast<Point*>(val));
            } else if (name == "end_point") {
                m_end = m_trsfFunc(*static_cast<Point*>(val));
            } else if (name == "image_width") {
                m_width = *static_cast<f32*>(val);
            } else if (name == "image_height") {
                m_height = *static_cast<f32*>(val);
            } else if (name == "mode") {
                m_mode = *static_cast<u8*>(val);
            } else if (name == "transform_func") {
                m_trsfFunc = *static_cast<std::function<Point(const Point&)>*>(val);
            } else if (name == "inverse_transform_func") {
                m_inverseTrsfFunc = *static_cast<std::function<Point(const Point&)>*>(val);
            } else if (name == "put_point_func") {
                m_putPointFunc = *static_cast<std::function<void(const Point&)>*>(val);
            } else if (name == "clip_func") {
                m_clipFunc = *static_cast<std::function<void(Point&, Point&, const Point&, const Point&)>*>(val);
            } else {
                throw std::invalid_argument(std::format("Unknown parameter name: {}", name));
            }

        }

        m_clipFunc(m_start, m_end, { 0.0F, 0.0F }, { m_width, m_height }); 
    }

    virtual void Draw() const override final
    {
        // Suppose that the line is from left to right
        auto [x, y, dx, dy] = Round(
            m_start[0], m_start[1], m_end[0] - m_start[0], m_end[1] - m_start[1]
        );

        i32 e = -dx;

        for (i32 i = 0LL; i <= dx; i++)
        {
            if (m_mode == 0) {
                m_putPointFunc(m_inverseTrsfFunc({ f32(x), f32(y) }));
            } else if (m_mode == 1) {
                if (!(i % 30 >= 20 && i % 30 < 30)) {
                    m_putPointFunc(m_inverseTrsfFunc({ f32(x), f32(y) }));
                }
            } else if (m_mode == 2) {
                if (!(i % 10 >= 5 && i % 10 < 10)) {
                    m_putPointFunc(m_inverseTrsfFunc({ f32(x), f32(y) }));
                }
            }
            x = x + 1;
            e = e + 2 * dy;
            if (e >= 0)
            {
                y++, e = e - 2 * dx;
            }
        }
    }

private:
    Point m_start;
    Point m_end;
    u8 m_mode;
    f32 m_width;
    f32 m_height;
    std::function<Point(const Point&)> m_trsfFunc;
    std::function<Point(const Point&)> m_inverseTrsfFunc;
    std::function<void(const Point&)>  m_putPointFunc;
    std::function<void(Point&, Point&, const Point&, const Point&)> m_clipFunc = CohenSutherland::LineClip;
};