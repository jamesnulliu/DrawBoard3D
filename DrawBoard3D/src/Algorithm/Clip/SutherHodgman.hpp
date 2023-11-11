#pragma once
#include <array>
#include <vector>

class SutherHodgman
{
public:
    using i64 = ::int64_t;
    using u64 = ::uint64_t;
    using f32 = float;
    using Point = std::array<f32, 2>;

public:
    /**
     * @brief Clip a polygon with respect to a closed boundary.
     *
     * @param polygonPoints - Points of the polygon.
     * @param boundary - Points of the boundary.
    */
    static void Clip(std::vector<Point>& polygonPoints, const std::vector<Point> boundaryPoints)
    {
        for (u64 i = 0; i < boundaryPoints.size(); ++i) {
            i64 k = (i64(i) + 1LL) % i64(boundaryPoints.size());
            ClipWithLine(polygonPoints, { boundaryPoints[i], boundaryPoints[k] });
        }
    }

private:
    /**
     * @brief Calculate the intersection point of two lines.
     * @param line0 - Start and end point: {{x0, y0}, {x1, y1}}
     * @param line1 - Start and end point: {{x2, y2}, {x3, y3}}
     * @return The intersection point: {x, y}
    */
    static Point Intersect(const std::array<Point, 2>& line0, const std::array<Point, 2>& line1)
    {
        auto& [x0, y0] = line0[0];
        auto& [x1, y1] = line0[1];
        auto& [x2, y2] = line1[0];
        auto& [x3, y3] = line1[1];
        f32 x = (x0 * y1 - y0 * x1) * (x2 - x3) - (x0 - x1) * (x2 * y3 - y2 * x3);
        f32 y = (x0 * y1 - y0 * x1) * (y2 - y3) - (y0 - y1) * (x2 * y3 - y2 * x3);
        f32 w = (x0 - x1) * (y2 - y3) - (y0 - y1) * (x2 - x3);
        return { x / w, y / w };
    }

    /**
     * @brief Clip a polygon with respect to a line.
     *
     * @param polygonPoints - Points of the polygon
     * @param clipLine - Start and end point: {{x0, y0}, {x1, y1}}
    */
    static void ClipWithLine(std::vector<Point>& polygonPoints, const std::array<Point, 2>& clipLine)
    {
        std::vector<Point> clippedPolygonPoints{};
        for (u64 i = 0; i < polygonPoints.size(); ++i) {
            i64 k = (i64(i) + 1LL) % i64(polygonPoints.size());
            Point point_i = polygonPoints[i];
            Point point_k = polygonPoints[k];

            f32 pos_i = (clipLine[1][0] - clipLine[0][0]) * (point_i[1] - clipLine[0][1])
                - (clipLine[1][1] - clipLine[0][1]) * (point_i[0] - clipLine[0][0]);

            f32 pos_k = (clipLine[1][0] - clipLine[0][0]) * (point_k[1] - clipLine[0][1])
                - (clipLine[1][1] - clipLine[0][1]) * (point_k[0] - clipLine[0][0]);

            // Case 1: When both points are inside
            if (pos_i < 0 && pos_k < 0) {
                // Only second point is added
                clippedPolygonPoints.push_back(point_k);
            }
            // Case 2: When only the first point is outside
            else if (pos_i >= 0 && pos_k < 0) {
                // Point of intersection with edge and the second point is added
                clippedPolygonPoints.push_back(Intersect({ point_i, point_k }, clipLine));
                clippedPolygonPoints.push_back(point_k);
            }
            // Case 3: When only the second point is outside
            else if (pos_i < 0 && pos_k >= 0) {
                // Only point of intersection with edge is added
                clippedPolygonPoints.push_back(Intersect({ point_i, point_k }, clipLine));
            }
            // Case 4: When both points are outside
            else {
                // No points are added
            }
        }
        polygonPoints = clippedPolygonPoints;
    }
};
