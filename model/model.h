#pragma once
#include "../point/point.h"

namespace NGeometry3d {
    struct BoundingBox {
        Point ld;
        Point ru;
        BoundingBox(const Point& ld, const Point& ru);
        BoundingBox(const BoundingBox& b);
        Point center() const;
        FLOAT_TYPE size(int i) const;
    };
    struct Info {
        Point sun;
    };
    struct Triple {
        int first;
        int second;
        int third;
        Triple();
        Triple(int f, int s, int t);
        Triple(const Triple& t);
    };
    class Model {
        vector<Point> vertexes;
        vector<Point> normals;
        vector<Triple> triangles;
    public:
        Model();
        Model(const char* filename); // read from __filename__.obj
        Model(const Model& m);
        void to_box(const BoundingBox& box);
        void translate(const Point& p);
        void rotate(const Point& center, FLOAT_TYPE angle, int num); // num must be in { 0, 1, 2 }
        void scale(const Point&, FLOAT_TYPE);
        void show() const;
        size_t count_triangles() const;
        size_t count_vertexes() const;
        size_t count_normals() const;
        Point get_vertex(int i) const;
        Point get_normal(int i) const;
        Triple get_triangle(int i) const;
        BoundingBox get_box() const;
    };
    NMatrix::Matrix matrix_scale(const Point&, FLOAT_TYPE);
    NMatrix::Matrix matrix_translate(const Point& p);
    NMatrix::Matrix matrix_rotate(FLOAT_TYPE angle, int num);
};