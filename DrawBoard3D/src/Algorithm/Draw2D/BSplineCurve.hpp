#pragma once
#include "Draw2D.hpp"
#include "../Random.hpp"

class BSplineCurve : public Draw2D
{
public:
    explicit BSplineCurve() = default;


    virtual void SetParam(const std::vector<std::pair<std::string, void*>>& params) override final
    {
        for (auto&[name, val] : params)
        {
            if (name == "control_points") {
                m_ctrlPoints = *static_cast<std::vector<Point>*>(val);
            } else if (name == "step") {
                m_step = *static_cast<f32*>(val);
            } else if (name == "order") {
                m_order = *static_cast<u64*>(val);
            } else if (name == "put_point_func") {
                m_putPointFunc = *static_cast<std::function<void(const Point&)>*>(val);
            } else {
                throw std::invalid_argument(std::format("Unknown parameter name: {}", name));
            }
        }
    }

    virtual void Draw() const override final
    {
        for (float u = 0; u <= 1.0; u += m_step) {
            float denominator = 0.0F;
            Point p = { 0.0F, 0.0F };
            for (u64 i = 0; i < m_ctrlPoints.size(); ++i) {
                float weight = m_weights[i] * BasicFunction(i, m_order, u, m_knots);
                denominator += weight;
                p[0] += m_ctrlPoints[i][0] * weight;
                p[1] += m_ctrlPoints[i][1] * weight;
            }
            if (denominator != 0.0F) {
                p[0] /= denominator;
                p[1] /= denominator;
            }
            m_putPointFunc(p);
        }
    }

private:
    void GenerateKnots()
    {
        u64 num = m_ctrlPoints.size() + m_order;
        if (m_knots.size() != num) {
            m_knots.clear();
            // Generate a new random knot vector
            m_knots.resize(num, 0.0F);
            for (u64 i = num - 1; i >= num - m_order; --i) {
                m_knots.at(i) = 1;
            }
            for (u64 i = m_order; i < num - m_order; ++i) {
                // Make sure `m_knots[m_order:num-m_order-1]` is strictly increasing
                float r = m_randGen(std::numeric_limits<float>::epsilon() * 2.0F, 1.0F - m_knots[i - 1]) * 0.5f;
                m_knots[i] = r + m_knots[i - 1];
            }
        }
    }

    void GenerateWeights()
    {
        u64 num = m_ctrlPoints.size();
        if (m_weights.size() != num) {
            m_weights.clear();
            m_weights.resize(num);
            for (auto& w : m_weights)
            {
                w = m_randGen(15, 25);
            }
            float sum = std::accumulate(m_weights.begin(), m_weights.end(), 0.0F);
            for (auto& w : m_weights)
            {
                w /= sum;
            }
        }
    }

    static float BasicFunction(u64 i, int order, float t, std::vector<float>& knots)
    {
        if (order == 1) {
            if (t >= knots[i] && t <= knots[i + 1ULL]) {
                return 1.0F;
            }
            return 0.0F;
        }
        // Or k > 1, then the BSpline is a piecewise polynomial function
        else {
            float deltaA = knots[i + order - 1ULL] - knots[i];
            float deltaB = knots[i + order] - knots[i + 1ULL];
            float a = (t - knots[i]) / (deltaA == 0.0F ? 1.0F : deltaA);
            float b = (knots[i + order] - t) / (deltaB == 0.0F ? 1.0F : deltaB);
            return a * BasicFunction(i, order - 1, t, knots) + b * BasicFunction(i + 1ULL, order - 1, t, knots);
        }

    }

private:
    // Parameters for BSplineCurve
    std::vector<Point> m_ctrlPoints{};
    u64 m_order = 3ULL;
    f32 m_step = 1e-4F;
    std::function<void(const Point&)> m_putPointFunc;
    std::vector<f32> m_weights{};
    mutable std::vector<f32> m_knots{};
    Rand_Uniform<f32> m_randGen{};
};
