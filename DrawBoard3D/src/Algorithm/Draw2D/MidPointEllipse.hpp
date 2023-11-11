#pragma once
#include "Draw2D.hpp"

class MidPointEllipse : public Draw2D
{
public:
    explicit MidPointEllipse() {}

    virtual void SetParam(const std::vector<std::pair<std::string, void*>>& params) override final
    {
        for (auto& [name, val] : params) {
            if (name == "a") {
                m_a = *static_cast<i32*>(val);
            } else if (name == "b") {
                m_b = *static_cast<i32*>(val);
            } else if (name == "transform_func") {
                m_trsfFunc = *static_cast<std::function<Point(const Point&)>*>(val);
            } else if (name == "put_point_func") {
                m_putPointFunc = *static_cast<std::function<void(const Point&)>*>(val);
            } else {
                throw std::invalid_argument(std::format("Unknown parameter name: {}", name));
            }
        }
    }

    virtual void Draw() const override final
    {
        f32 d1 = 1.0F * (m_b * m_b - m_a * m_a * m_b) + 0.25F * m_a * m_a;
        i32 x = 0, y = m_b;
        m_putPointFunc(m_trsfFunc({ f32(x), f32(y) }));
        // Upper part:
        while (m_b * m_b * (x + 1) < m_a * m_a * y - 0.5F * m_a * m_a) {
            if (d1 < 0) {
                d1 += m_b * m_b * (2 * x + 3);
                ++x;
            } else {
                d1 += m_b * m_b * (2 * x + 3) + m_a * m_a * (-2 * y + 2);
                ++x, --y;
            }
            m_putPointFunc(m_trsfFunc({ f32(x), f32(y) }));
        }
        // Lower part:
        float d2 = float(::sqrt(m_b * x + 0.5F * m_b) + ::sqrt(m_a * (y - 1)) - ::sqrt(m_a * m_b));
        while (y > 0) {
            if (d2 < 0) {
                d2 += m_b * m_b * (2 * x + 2) + m_a * m_a * (-2 * y + 3);
                ++x, --y;
            } else {
                d2 += m_a * m_a * (-2 * y + 3);
                --y;
            }
            m_putPointFunc(m_trsfFunc({ f32(x), f32(y) }));
        }
    }

private:
    i32 m_a;
    i32 m_b;
    std::function<Point(const Point&)> m_trsfFunc;
    std::function<void(const Point&)> m_putPointFunc = [](const Point&) {};
};