#pragma once
#include "Draw2D.hpp"
#include <algorithm>
#include "../Clip/SutherHodgman.hpp"

struct Edge_ScanLineFill
{
    using u64 = Draw2D::u64;
    using f32 = Draw2D::f32;
    u64 yMax;  // The maximum y coordinate of the edge
    f32 x;     // The x coordinate of lowest edge point
    f32 dx;    // The inverse of the slope of the edge 
};

class EdgeTable_ScanLineFill
{
public:
    using u64 = Draw2D::u64;
    using f32 = Draw2D::f32;
    using Edge = Edge_ScanLineFill;
    using Point = Draw2D::Point;

public:
    explicit EdgeTable_ScanLineFill() = default;
    ~EdgeTable_ScanLineFill() = default;

    void BuildET(const std::vector<Point>& vertice, u64 imageHeight)
    {
        table.clear();
        table.resize(imageHeight, {});
        for (u64 i = 0; i < vertice.size(); ++i) {
            u64 j = (i + 1) % vertice.size();
            yMin = std::min(yMin, u64(vertice[i][1]));
            yMax = std::max(yMax, u64(vertice[i][1]));
            if (vertice[i][1] == vertice[j][1]) {
                continue;
            }
            Edge edge{};
            if (vertice[i][1] < vertice[j][1]) {
                edge.x = vertice[i][0];
                edge.yMax = vertice[j][1];
                edge.dx = (vertice[j][0] - vertice[i][0]) / (vertice[j][1] - vertice[i][1]);
                table[u64(vertice[i][1])].push_back(edge);
            } else {
                edge.x = vertice[j][0];
                edge.yMax = vertice[i][1];
                edge.dx = (vertice[j][0] - vertice[i][0]) / (vertice[j][1] - vertice[i][1]);
                table[u64(vertice[j][1])].push_back(edge);
            }
        }
    }

public:
    u64 yMin = std::numeric_limits<u64>::max();
    u64 yMax = 0ULL;
    std::vector<std::vector<Edge>> table;
};


class ScanLineFill : public Draw2D
{
public:
    using Edge = Edge_ScanLineFill;
    using EdgeTable = EdgeTable_ScanLineFill;

public:
    explicit ScanLineFill() : Draw2D() {}

    virtual void SetParam(const std::vector<std::pair<std::string, void*>>& params) override final
    {
        for (auto& [name, val] : params) {
            if (name == "image_height") {
                m_imageHeight = *static_cast<f32*>(val);
            } else if (name == "image_width") {
                m_imageWidth = *static_cast<f32*>(val);
            } else if (name == "vertices") {
                std::vector<Point> v = *static_cast<std::vector<Point>*>(val);

                std::vector<Point> b(
                    {
                        Point{0.0f,0.0f},
                        Point{0.0f, m_imageHeight - 1},
                        Point{m_imageWidth - 1, m_imageHeight - 1},
                        Point{m_imageWidth, 0.0f}
                    }
                );

                m_clip(v, b);

                m_edgeTable.BuildET(v, m_imageHeight);
            }
        }
    }

    virtual void Draw() const override final
    {
        m_activeEdgeList.clear();
        for (u64 y = m_edgeTable.yMin; y <= m_edgeTable.yMax; ++y) {
            for (auto& edge : m_edgeTable.table[y]) {
                m_activeEdgeList.push_back(edge);
            }
            m_activeEdgeList.erase(
                std::remove_if(
                    m_activeEdgeList.begin(),
                    m_activeEdgeList.end(),
                    [y](const Edge& edge) {
                        return edge.yMax == y;
                    }
                ),
                m_activeEdgeList.end()
            );
            std::ranges::sort(m_activeEdgeList, [](const Edge& lhs, const Edge& rhs) { return lhs.x < rhs.x; });
            for (u64 i = 0; i < m_activeEdgeList.size(); i += 2) {
                for (u64 x = m_activeEdgeList[i].x; i < m_activeEdgeList.size() - 1 && x < m_activeEdgeList[i + 1].x; ++x) {
                    m_putPointFunc({ f32(x), f32(y) });
                }
            }
            for (auto& edge : m_activeEdgeList) {
                edge.x += edge.dx;
            }
        }
    }



private:
    f32 m_imageHeight = 0;
    f32 m_imageWidth = 0;
    mutable std::vector<Edge>         m_activeEdgeList{};
    EdgeTable_ScanLineFill            m_edgeTable{};
    std::function<void(const Point&)> m_putPointFunc;
    std::function<void(std::vector<Point>&, const std::vector<Point>&)> m_clip = SutherHodgman::Clip;
};
