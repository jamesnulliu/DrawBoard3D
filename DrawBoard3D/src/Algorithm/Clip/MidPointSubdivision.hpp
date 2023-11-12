#pragma once
#include <array>

class MidPointSubdivision
{
public:
    using i32 = std::int32_t;
    using f32 = float;
    using Point = std::array<f32, 2>;

public:
    /**
     * @brief Clip a line segment with respect to a rectangle.
     *
     * @param lineStart - Start point of the line segment: {x1, y1}.
     * @param lineEnd - End point of the line segment: {x2, y2}.
     * @param bottomLeft - Bottom left point of the rectangle: {x_min, y_min}.
     * @param topRight - Top right point of the rectangle: {x_max, y_max}.
    */
    static void LineClip(Point& lineStart, Point& lineEnd, const Point& bottomLeft, const Point& topRight)
    {
        i32 code0 = Encode(lineStart, bottomLeft, topRight);
        i32 code1 = Encode(lineEnd, bottomLeft, topRight);
        i32 code = 0, x = 0, y = 0;
        f32 epsilon = 1e-1;

        if (code0 == 0 && code1 == 0) {
            return;
        }

        if ((code0 & code1) != 0) {
            lineStart[0] = lineStart[0] = -1;
            lineEnd[0] = lineEnd[1] = -1;
            return;
        }


        // If p0 is outside the rectangle, find the nearest point to the rectangle starting from p0
        if (code0 != 0) {
            Point p0{ lineStart }, p1{ lineEnd };
            while (Distance(p0, p1) >= epsilon) {
                Point middle = { (p0[0] + p1[0]) / 2.0F, (p0[1] + p1[1]) / 2.0F };
                code = Encode(middle, bottomLeft, topRight);
                if ((code & code0) == 0) {
                    p1 = middle;
                } else {
                    p0 = middle;
                    code0 = code;
                }
            } // Now p0 is the nearest point to the rectangle
            lineStart = p0;
        }

        // If p1 is outside the rectangle, find the nearest point to the rectangle starting from p1
        if (code1 != 0) {
            Point p0{ lineStart }, p1{ lineEnd };
            while (Distance(p0, p1) >= epsilon) {
                Point middle = { (p0[0] + p1[0]) / 2, (p0[1] + p1[1]) / 2 };
                code = Encode(middle, bottomLeft, topRight);
                if ((code & code1) == 0) {
                    p0 = middle;
                } else {
                    p1 = middle;
                    code1 = code;
                }
            } // Now p1 is the nearest point to the rectangle
            lineEnd = p1;
        }
    }

private:
    static const i32 LEFT = 1;
    static const i32 RIGHT = 2;
    static const i32 BOTTOM = 4;
    static const i32 TOP = 8;

    /**
     * @brief Encode a point with respect to a rectangle.
     *
     * @param point - Point to be encoded: {x, y}.
     * @param bottom_left - Bottom left point of the rectangle: {x_min, y_min}.
     * @param top_right - Top right point of the rectangle: {x_max, y_max}.
     * @return Encoded code.
    */
    static int Encode(const Point& point, const Point& bottomLeft, const Point& topRight)
    {
        auto& [x, y] = point;
        auto& [x_min, y_min] = bottomLeft;
        auto& [x_max, y_max] = topRight;
        i32 code = 0;
        if (x < x_min) code |= LEFT;
        else if (x > x_max) code |= RIGHT;
        if (y < y_min) code |= BOTTOM;
        else if (y > y_max) code |= TOP;
        return code;
    }

    static f32 Distance(const Point& point0, const Point poin1)
    {
        return ::sqrt(
            (point0[0] - poin1[0]) * (point0[0] - poin1[0]) + (point0[1] - poin1[1]) * (point0[1] - poin1[1])
        );
    }
};
