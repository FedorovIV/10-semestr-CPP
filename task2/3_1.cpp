///////////////////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <algorithm>
#include <iostream>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////

struct Rectangle
{
    int x1 = 0; // левая граница (левый верхний угол)
    int y1 = 0; // верхняя граница (левый верхний угол)
    int x2 = 0; // правая граница (правый нижний угол)
    int y2 = 0; // нижняя граница (правый нижний угол)

    // -------------------------------------------------------------------------
    // Вычисление площади прямоугольника
    int area() const
    {
        int width  = std::max(0, x2 - x1);
        int height = std::max(0, y2 - y1);
        return width * height;
    }

    // -------------------------------------------------------------------------
    // Пересечение двух прямоугольников
    static Rectangle intersect(Rectangle const& a, Rectangle const& b)
    {
        Rectangle r;
        r.x1 = std::max(a.x1, b.x1);
        r.y1 = std::max(a.y1, b.y1);
        r.x2 = std::min(a.x2, b.x2);
        r.y2 = std::min(a.y2, b.y2);

        // если нет пересечения — вернуть пустой прямоугольник
        if (r.x1 >= r.x2 || r.y1 >= r.y2)
            return {0, 0, 0, 0};

        return r;
    }

    // -------------------------------------------------------------------------
    // Наименьший ограничивающий прямоугольник (bounding box)
    static Rectangle bounding_box(std::vector<Rectangle> const& rects)
    {
        if (rects.empty())
            return {0, 0, 0, 0};

        int min_x1 = rects[0].x1;
        int min_y1 = rects[0].y1;
        int max_x2 = rects[0].x2;
        int max_y2 = rects[0].y2;

        for (auto const& r : rects)
        {
            min_x1 = std::min(min_x1, r.x1);
            min_y1 = std::min(min_y1, r.y1);
            max_x2 = std::max(max_x2, r.x2);
            max_y2 = std::max(max_y2, r.y2);
        }

        return {min_x1, min_y1, max_x2, max_y2};
    }

    // -------------------------------------------------------------------------
    // Пересечение нескольких прямоугольников
    static Rectangle intersection_all(std::vector<Rectangle> const& rects)
    {
        if (rects.empty())
            return {0, 0, 0, 0};

        Rectangle result = rects[0];
        for (size_t i = 1; i < rects.size(); ++i)
            result = intersect(result, rects[i]);

        return result;
    }

    // -------------------------------------------------------------------------
    // Удобный вывод прямоугольника
    friend std::ostream& operator<<(std::ostream& os, Rectangle const& r)
    {
        os << '(' << r.x1 << ',' << r.y1 << ") - (" << r.x2 << ',' << r.y2 << ')';
        return os;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    using R = Rectangle;

    // Проверка площади
    R a{0, 0, 10, 10};
    assert(a.area() == 100);

    // Пересечение (частичное)
    R b{5, 5, 15, 15};
    R inter = R::intersect(a, b);
    assert(inter.area() == 25);
    assert(inter.x1 == 5 && inter.y1 == 5 && inter.x2 == 10 && inter.y2 == 10);

    // Пересечение (нет пересечения)
    R c{20, 20, 30, 30};
    R none = R::intersect(a, c);
    assert(none.area() == 0);

    // Bounding box
    std::vector<R> vec = {a, b, c};
    R box = R::bounding_box(vec);
    assert(box.x1 == 0 && box.y1 == 0 && box.x2 == 30 && box.y2 == 30);

    // Пересечение всех
    std::vector<R> group = {a, b};
    R all = R::intersection_all(group);
    assert(all.area() == 25);

    std::cout << "All tests passed!\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////
