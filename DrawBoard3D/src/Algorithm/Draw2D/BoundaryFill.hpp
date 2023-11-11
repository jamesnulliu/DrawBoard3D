#pragma once
#include "Draw2D.hpp"
#include <stack>

class BoundaryFill : public Draw2D
{
public:
    explicit BoundaryFill() {}

    virtual void SetParam(const std::vector<std::pair<std::string, void*>>& params) override final
    {
        for (auto&[name, val] : params) {
            if (name == "image_buffer") {
                m_imageBuffer = static_cast<std::vector<u32>*>(val);
            } else if (name == "image_width") {
                m_imageWidth = *static_cast<u64*>(val);
            } else if (name == "image_height") {
                m_imageHeight = *static_cast<u64*>(val);
            } else if (name == "start_point") {
                m_start = *static_cast<Point*>(val);
            } else if (name == "fill_color") {
                m_fillColor = *static_cast<u32*>(val);
            } else if (name == "this_color") {
                m_thisColor = *static_cast<u32*>(val);
            } else {
                throw std::invalid_argument(std::format("Unknown parameter name: {}", name));
            }
        }
    }

    virtual void Draw() const override final
    {
        if (m_imageBuffer == nullptr || m_imageBuffer->at(Round<u64>(m_start[1]) * m_imageWidth + Round<u64>(m_start[0])) == m_fillColor) {
            return;
        }
        std::stack<Point> stk;
        stk.push(m_start);
        while (!stk.empty()) {
            Point coord = stk.top();
            auto [x, y] = Round<u64>(coord[0], coord[1]);
            stk.pop();
            if (x >= m_imageWidth || y >= m_imageHeight) {
                continue;
            }
            if ((*m_imageBuffer).at(y * m_imageWidth + x) != m_thisColor) {
                continue;
            }
            m_imageBuffer->at(y * m_imageWidth + x) = m_fillColor;
            stk.push({ f32(x + 1), f32(y) });
            stk.push({ f32(x - 1), f32(y) });
            stk.push({ f32(x), f32(y + 1) });
            stk.push({ f32(x), f32(y - 1) });
        }
    }

private:
    std::vector<u32>* m_imageBuffer = nullptr;
    u64 m_imageWidth = 0;
    u64 m_imageHeight = 0;
    Point m_start;
    u32 m_fillColor = 0;
    u32 m_thisColor = 0;
};