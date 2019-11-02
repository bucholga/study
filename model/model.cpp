#include "model.h"
#include "../../../bonka/include/all.h"
#include "../../../bonka/error/error.h"
#include "../../../bonka/debug/debug.h"

#include <fstream>
#include <cmath>

namespace NGeometry3d {
    #define dmax(a, b) (((a) < (b)) ? (b) : (a))
    BoundingBox::BoundingBox(const Point& l, const Point& r) : ld(l), ru(r) { }
    BoundingBox::BoundingBox(const BoundingBox& b) : ld(b.ld), ru(b.ru) { }
    Point BoundingBox::center() const {
        return (ld + ru) * 0.5;
    }
    FLOAT_TYPE BoundingBox::size(int i) const {
        return ru[i] - ld[i];
    }
    NMatrix::Matrix matrix_scale(const Point& p, FLOAT_TYPE k) {
        NMatrix::Matrix answer(4);
        answer[0][0] = answer[1][1] = answer[2][2] = k;
        return matrix_translate(p) * answer * matrix_translate(p * -1);
    }
    NMatrix::Matrix matrix_rotate(FLOAT_TYPE angle, int num) {
        if (num < 0 || num >= 3) {
            throw NError::Error("impossible make rotation near this direction");
        }
        FLOAT_TYPE c = std::cos(angle);
        FLOAT_TYPE s = std::sin(angle);
        NMatrix::Matrix answer(4);
        vector<int> coords = {0, 1, 2};
        coords.erase(coords.begin() + num);

        answer[coords[0]][coords[0]] = c;
        answer[coords[1]][coords[1]] = c;

        answer[coords[0]][coords[1]] = -s;
        answer[coords[1]][coords[0]] = s;

        return answer;
    }
    NMatrix::Matrix matrix_translate(const Point& p) {
        return NMatrix::Matrix({
            {1, 0, 0, p[0]},
            {0, 1, 0, p[1]},
            {0, 0, 1, p[2]},
            {0, 0, 0,   1 },
        });
    }
    Triple::Triple() : first{}, second{}, third{} { }
    Triple::Triple(int f, int s, int t) : first(f), second(s), third(t) { }
    Triple::Triple(const Triple& t) : first(t.first), second(t.second), third(t.third) { }

    Model::Model() : vertexes{}, normals{}, triangles{} { }
    Model::Model(const char* filename) {
        FILE* file = fopen(filename, "r");
        enum { MAX_LEN_STRING = 100 };
        char str[MAX_LEN_STRING + 1];
        while (fgets(str, MAX_LEN_STRING, file)) {
            static int cnt = 0;
            auto splt = std::split(string(str), ' ');
            if (splt.empty() || splt[0] == "#") { continue; }
            if (splt[0] == "v") {
                FLOAT_TYPE x = std::to_double(splt[1]);
                FLOAT_TYPE y = std::to_double(splt[2]);
                FLOAT_TYPE z = std::to_double(splt[3]);
                vertexes.push_back(Point(x, y, z));
            } else if (splt[0] == "vn") {
                FLOAT_TYPE x = std::to_double(splt[1]);
                FLOAT_TYPE y = std::to_double(splt[2]);
                FLOAT_TYPE z = std::to_double(splt[3]);
                normals.push_back(Point(x, y, z).norm());
            } else if (splt[0] == "f") {
                triangles.push_back(Triple(
                    std::to_int(std::split(splt[1], '/')[0]) - 1,
                    std::to_int(std::split(splt[2], '/')[0]) - 1,
                    std::to_int(std::split(splt[3], '/')[0]) - 1
                ));
            }
        }
        if (normals.size() != vertexes.size()) {
            throw NError::Error("count of normals not equal count of vertexes");
        }
    }
    Model::Model(const Model& m) : vertexes(m.vertexes), normals(m.normals), triangles(m.triangles) { }
    void Model::to_box(const BoundingBox& box) {
        BoundingBox b = get_box();
        FLOAT_TYPE s[] = {box.size(0) / b.size(0), box.size(1) / b.size(1), box.size(2) / b.size(2)};
        this->scale(b.center(), std::min(s[0], std::min(s[1], s[2])));
        this->translate(box.center() - b.center());
    }
    void Model::translate(const Point& p) {
        NMatrix::Matrix tr = matrix_translate(p);
        for (size_t i = 0; i < vertexes.size(); ++i) {
            vertexes[i] = tr * NMatrix::Matrix(vertexes[i]);
        }
    }
    void Model::rotate(const Point& p, FLOAT_TYPE angle, int num) {
        this->translate(p * -1);
        NMatrix::Matrix m = matrix_rotate(angle, num);
        for (size_t i = 0; i < vertexes.size(); ++i) {
            vertexes[i] = m * NMatrix::Matrix(vertexes[i]);
            normals[i] = m * NMatrix::Matrix(normals[i]);
        }
        this->translate(p);
    }
    void Model::scale(const Point& p, FLOAT_TYPE k) {
        NMatrix::Matrix tr = matrix_scale(p, k);
        for (size_t i = 0; i < vertexes.size(); ++i) {
            vertexes[i] = tr * NMatrix::Matrix(vertexes[i]);
        }
    }
    void Model::show() const {
        debug("vertexes");
        for (size_t i = 0; i < vertexes.size(); ++i)
            debug(vertexes[i][0], vertexes[i][1], vertexes[i][2]);
        debug("normals");
        for (size_t i = 0; i < normals.size(); ++i)
            debug(normals[i][0], normals[i][1], normals[i][2]);
    }
    BoundingBox Model::get_box() const {
        Point ld = vertexes[0], ru = vertexes[0];
        for (auto vert : vertexes) {
            ld[0] = std::min(ld[0], vert[0]);
            ld[1] = std::min(ld[1], vert[1]);
            ld[2] = std::min(ld[2], vert[2]);
        }
        for (auto vert : vertexes) {
            ru[0] = std::max(ru[0], vert[0]);
            ru[1] = std::max(ru[1], vert[1]);
            ru[2] = std::max(ru[2], vert[2]);
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