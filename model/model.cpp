#include "model.h"

#include <string>
#include <fstream>
#include <cmath>
#include <iostream>

namespace NGeometry3d {
    #define dmax(a, b) (((a) < (b)) ? (b) : (a))
	#define dmin(a, b) (((a) > (b)) ? (b) : (a))

	using std::string;

	Point::Point() : x{}, y{}, z{} { }
	Point::Point(FLOAT_TYPE x, FLOAT_TYPE y, FLOAT_TYPE z) : x(x), y(y), z(z) { }
	Point::Point(const Point& p) : x(p.x), y(p.y), z(p.z) { }

	Point Point::operator+(const Point& p) const {
		return Point(x + p.x, y + p.y, z + p.z);
	}
	FLOAT_TYPE& Point::operator[](int i) {
		return *(vector<FLOAT_TYPE*>{&x, &y, &z} [i] );
	}
	FLOAT_TYPE Point::operator[]( int i) const {
		return (vector<FLOAT_TYPE>{x, y, z}[i] );
	}
	Point Point::operator*(FLOAT_TYPE k) const {
		return Point(x * k, y * k, z * k);
	}
	Point Point::norm() const {
		FLOAT_TYPE len = std::sqrt(x * x + y * y + z * z);
		return (*this) * (1.0 / len);
	}

	BoundingBox::BoundingBox(const Point& l, const Point& r) : ld(l), ru(r) { }
    BoundingBox::BoundingBox(const BoundingBox& b) : ld(b.ld), ru(b.ru) { }
    Point BoundingBox::center() const {
        return (ld + ru) * 0.5;
    }
	Point BoundingBox::operator[](int i) const {
		return vector<Point>({ ld, ru })[i];
	}
    FLOAT_TYPE BoundingBox::size(int i) const {
        return ru[i] - ld[i];
    }
    Triple::Triple() : first{}, second{}, third{} { }
    Triple::Triple(int f, int s, int t) : first(f), second(s), third(t) { }
    Triple::Triple(const Triple& t) : first(t.first), second(t.second), third(t.third) { }

	vector<string> split(const string& s, char del) {
		vector<string> ans;
		string cur;
		for (auto el : s) {
			if (el == del) {
				if (!cur.empty())
					ans.push_back(cur);
				cur.clear();
				continue;
			}
			cur.push_back(el);
		}
		if (!cur.empty())
			ans.push_back(cur);
		return ans;
	}

	double to_double(const string& s) {
		return atof(s.c_str());
	}

	int to_int(const string& s) {
		return atoi(s.c_str());
	}

    Model::Model() : vertexes{}, normals{}, triangles{} { }
    Model::Model(const char* filename) {
		FILE* file;
		fopen_s(&file, filename, "r");
        enum { MAX_LEN_STRING = 100 };
        char str[MAX_LEN_STRING + 1];
        while (fgets(str, MAX_LEN_STRING, file)) {
            static int cnt = 0;
            auto splt = split(string(str), ' ');
            if (splt.empty() || splt[0] == "#") { continue; }
            if (splt[0] == "v") {
                FLOAT_TYPE x = to_double(splt[1]);
                FLOAT_TYPE y = to_double(splt[2]);
                FLOAT_TYPE z = to_double(splt[3]);
                vertexes.push_back(Point(x, y, z));
            } else if (splt[0] == "vn") {
                FLOAT_TYPE x = to_double(splt[1]);
                FLOAT_TYPE y = to_double(splt[2]);
                FLOAT_TYPE z = to_double(splt[3]);
                normals.push_back(Point(x, y, z).norm());
            } else if (splt[0] == "f") {
                triangles.push_back(Triple(
                    to_int(split(splt[1], '/')[0]) - 1,
                    to_int(split(splt[2], '/')[0]) - 1,
                    to_int(split(splt[3], '/')[0]) - 1
                ));
            }
        }
        if (normals.size() != vertexes.size()) {
            throw ("count of normals not equal count of vertexes");
        }
	}
    Model::Model(const Model& m) : vertexes(m.vertexes), normals(m.normals), triangles(m.triangles) { }
    void Model::to_box(const BoundingBox& box) {
        /*BoundingBox b = get_box();
        FLOAT_TYPE s[] = {box.size(0) / b.size(0), box.size(1) / b.size(1), box.size(2) / b.size(2)};
        this->scale(b.center(), dmin(s[0], dmin(s[1], s[2])));
        this->translate(box.center() - b.center());
		*/
    }
    void Model::translate(const Point& p) {
        /*NMatrix::Matrix tr = matrix_translate(p);
        for (size_t i = 0; i < vertexes.size(); ++i) {
            vertexes[i] = tr * NMatrix::Matrix(vertexes[i]);
        }
		*/
    }
    void Model::rotate(const Point& p, FLOAT_TYPE angle, int num) {
        /*this->translate(p * -1);
        NMatrix::Matrix m = matrix_rotate(angle, num);
        for (size_t i = 0; i < vertexes.size(); ++i) {
            vertexes[i] = m * NMatrix::Matrix(vertexes[i]);
            normals[i] = m * NMatrix::Matrix(normals[i]);
        }
        this->translate(p);
		*/
    }
    void Model::scale(const Point& p, FLOAT_TYPE k) {
        /*NMatrix::Matrix tr = matrix_scale(p, k);
        for (size_t i = 0; i < vertexes.size(); ++i) {
            vertexes[i] = tr * NMatrix::Matrix(vertexes[i]);
        }
		*/
    }
    void Model::show() const {
    }
    BoundingBox Model::get_box() const {
        Point ld = vertexes[0], ru = vertexes[0];
        for (auto vert : vertexes) {
            ld[0] = dmin(ld[0], vert[0]);
            ld[1] = dmin(ld[1], vert[1]);
            ld[2] = dmin(ld[2], vert[2]);
        }
        for (auto vert : vertexes) {
            ru[0] = dmax(ru[0], vert[0]);
            ru[1] = dmax(ru[1], vert[1]);
            ru[2] = dmax(ru[2], vert[2]);
        }
        return BoundingBox(ld, ru);
    }
    size_t Model::count_triangles() const {
        return triangles.size();
    }
    size_t Model::count_vertexes() const {
        return vertexes.size();
    }
    size_t Model::count_normals() const {
        return normals.size();
    }
    Point Model::get_vertex(int i) const {
        return vertexes[i];
    }
    Point Model::get_normal(int i) const {
        return normals[i];
    }
    Triple Model::get_triangle(int i) const {
        return triangles[i];
    }
};