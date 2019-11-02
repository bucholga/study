#pragma once
#include <vector>

namespace NGeometry3d {
	using std::vector;
	typedef double FLOAT_TYPE;
	class Point {
		FLOAT_TYPE x, y, z;
	public:
		Point();
		Point(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z);
		Point(const Point& p);
		Point operator+(const Point& p) const;
		FLOAT_TYPE& operator[](int i);
		Point operator*(FLOAT_TYPE k) const;
		FLOAT_TYPE operator[](int i) const;
		Point norm() const;
	};
	struct BoundingBox {
		Point ld;
		Point ru;
		BoundingBox();
		BoundingBox(const Point& ld, const Point& ru);
		BoundingBox(const BoundingBox& b);
		Point operator[](int i) const;
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
};