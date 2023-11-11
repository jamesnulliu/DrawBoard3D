#pragma once
#include "Draw2D.hpp"

class BezierCurve : public Draw2D
{
public:
    explicit BezierCurve() = default;

    virtual void SetParam(const std::vector<std::pair<std::string, void*>>& params) override final
    {
        for (auto&[name, val] : params) {
            if (name == "control_points") {
                m_ctrlPoints = *static_cast<std::vector<Point>*>(val);
            } else if (name == "step") {
                m_step = *static_cast<float*>(val);
            } else if (name == "put_point_func") {
                m_putPointFunc = *static_cast<std::function<void(const Point&)>*>(val);
            } else {
                throw std::invalid_argument(std::format("Unknown parameter name: {}", name));
            }
        }
    }

    virtual void Draw() const override final
    {
        std::vector<float> ts;
        for (float t = 0; t <= 1; t += m_step) {
            ts.push_back(t);
        }
        if (m_ctrlPoints.size() < 2) { return; }
        for (auto t : ts) {
            for (u64 i = 0; i < m_ctrlPoints.size(); ++i) {
                m_putPointFunc(CalculateBezierPoint(m_ctrlPoints, t));
            }
        }
    }

    Point CalculateBezierPoint(const std::vector<Point>& points, float u) const
    {
        u64 n = points.size();
        std::vector<Point> bezierPoint(n - 1);
        for (u64 i = 0; i < n - 1; i++) {
            bezierPoint[i][0] = (1 - u) * points[i][0] + u * points[i + 1][0];
            bezierPoint[i][1] = (1 - u) * points[i][1] + u * points[i + 1][1];
        }
        if (n == 2) {
            return bezierPoint[0];
        } else {
            return CalculateBezierPoint(bezierPoint, u);
        }
    }

private:
    std::vector<Point> m_ctrlPoints = {};
    f32 m_step = 0.001;
    std::function<void(const Point&)> m_putPointFunc = [](const Point&) {};
};
