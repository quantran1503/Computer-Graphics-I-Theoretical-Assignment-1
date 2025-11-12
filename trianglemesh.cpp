// ========================================================================= //
// Authors: Daniel Rutz, Daniel Ströter, Roman Getto, Matthias Bein          //
//                                                                           //
// GRIS - Graphisch Interaktive Systeme                                      //
// Technische Universität Darmstadt                                          //
// Fraunhoferstrasse 5                                                       //
// D-64283 Darmstadt, Germany                                                //
//                                                                           //
// Content: Simple class for reading and rendering triangle meshes           //
// ========================================================================= //

#include <iostream>
#include <fstream>
#include <cfloat>

#include <QtMath>
#include <QOpenGLContext>
#include <QOpenGLFunctions_2_1>

#include "trianglemesh.h"

void TriangleMesh::calculateNormals(bool weightByAngle)
{
    normals.clear();
    normals.resize(vertices.size());
    // TODO: 4a) calculate normals for each vertex
    if (weightByAngle == false) {
        for (int i = 0; i < triangles.size(); i++) {
            Vec3f v_one = vertices[triangles[i].x()];
            Vec3f v_two = vertices[triangles[i].y()];
            Vec3f v_three = vertices[triangles[i].z()];

            Vec3f e_one = v_two - v_one;
            Vec3f e_two = v_two - v_three;

            Vec3<float> normal = cross(e_one, e_two);

            normals[triangles[i].x()] += normal;
            normals[triangles[i].y()] += normal;
            normals[triangles[i].z()] += normal;
        }

        for (int i = 0; i < normals.size(); i++) {
            normals[i] = normals[i].normalized(); // Or normalize() ?
        }
    }

    else {
        for (int i = 0; i < triangles.size(); i++) {
            Vec3f v_one = vertices[triangles[i].x()];
            Vec3f v_two = vertices[triangles[i].y()];
            Vec3f v_three = vertices[triangles[i].z()];

            Vec3f e_one = v_two - v_one;
            Vec3f e_two = v_two - v_three;
            Vec3f e_three = v_one - v_three;

            Vec3f e_one_norm = e_one.normalized();
            Vec3f e_two_norm = e_two.normalized();  // Or normalize() ?
            Vec3f e_three_norm = e_three.normalized();

            float dot_one = e_one_norm * e_two_norm;
            float dot_two = e_two_norm * e_three_norm;
            float dot_three = e_one_norm * e_three_norm;

            float angle_one = acos(dot_one);
            float angle_two = acos(dot_two);
            float angle_three = acos(dot_three);

            Vec3<float> normal = cross(e_one, e_two);

            normals[triangles[i].x()] += normal * angle_three;
            normals[triangles[i].y()] += normal * angle_one;
            normals[triangles[i].z()] += normal * angle_two;
        }

        for (int i = 0; i < normals.size(); i++) {
            normals[i] = normals[i].normalized(); // Or normalize() ?
        }
    }
    // TODO: 4b) weight normals by angle if weightByAngle is true

    for (auto &normal : normals) {
        // the normalize() function returns a boolean which can be used if you want to check for
        // erroneous normals
        normal.normalize();
    }
}

// ================
// === RAW DATA ===
// ================

vector<TriangleMesh::Vertex> &TriangleMesh::getPoints()
{
    return vertices;
}
vector<TriangleMesh::Triangle> &TriangleMesh::getTriangles()
{
    return triangles;
}

vector<TriangleMesh::Normal> &TriangleMesh::getNormals()
{
    return normals;
}

const vector<TriangleMesh::Vertex> &TriangleMesh::getPoints() const
{
    return vertices;
}

const vector<TriangleMesh::Triangle> &TriangleMesh::getTriangles() const
{
    return triangles;
}

const vector<TriangleMesh::Normal> &TriangleMesh::getNormals() const
{
    return normals;
}

void TriangleMesh::flipNormals()
{
    for (auto &normal : normals) {
        normal *= -1.0;
    }
}

// =================
// === LOAD MESH ===
// =================

void TriangleMesh::loadLSA(const char *filename)
{
    const constexpr auto DEG_TO_RAD = M_PI / 180.;

    std::ifstream in(filename);
    if (!in.is_open()) {
        cout << "loadLSA: can not open " << filename << endl;
        return;
    }

    float baseline = -1.0; // Initial baseline with invalid value. Needs to be updated when reading the file.
    vertices.resize(0);
    triangles.resize(0);
    // read vertices and triangles
    // TODO: 2) read alpha, beta, gamma for each vertex and calculate vertex coordinates
    // TODO: 2) read all triangles from the file
    
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#')
            continue;
    }
    char type;
    float a, b, g;
    if (line[0] == 'b') {
        sscanf(line.c_str(), "b %f", &baseline);
    }
    if (line[0] == 'v') {
        sscanf(line.c_str(), "v %f %f %f", &a, &b, &g);
        //convert deg to rad
        float alpha = a * DEG_TO_RAD;
        float beta = b * DEG_TO_RAD;
        float gamma = g * DEG_TO_RAD;

        //calculate the angle to coordinate
        float x = baseline + cos(beta) * sin(alpha);
        float y = sin(gamma);
        float z = -cos(beta) * cos(alpha);

        vertices.emplace_back(x, y, z);
    }
    if (line[0] == 'f') {
        int i1, i2, i3;
        sscanf(line.c_str(), "f %d %d %d", &i1, &i2, &i3);
        triangles.emplace_back(i1 - 1, i2 - 1, i3 - 1);
    }
    // calculate normals
    calculateNormals();
}


void TriangleMesh::loadOBJ(const char *filename)
{ 
    std::ifstream in(filename);
    if (!in.is_open()) {
        cout << "loadOBJ: can not find " << filename << endl;
        return;
    }

    vertices.resize(0);
    triangles.resize(0);

    // read vertices and triangles
    // 1) read all vertices and triangles from the file
    // You can ignore all other information in the file
    FILE *file = fopen(filename, "r");
    while (true) {
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        if (strcmp(lineHeader, "v") == 0) {
            Vec3f vertex;
            fscanf(file, "%f %f %f\n", &vertex.x(), &vertex.y(), &vertex.z());
            vertices.push_back(vertex);
        } else if (strcmp(lineHeader, "f") == 0) {
            Vec3i triangle;
            fscanf(file, "%d %d %d\n", &triangle.x(), &triangle.y(), &triangle.z());
            triangles.push_back(triangle);
        }
    }
    fclose(file);

    // calculate normals
    calculateNormals();
}

// ==============
// === RENDER ===
// ==============

void TriangleMesh::draw(QOpenGLFunctions_2_1 *f)
{
    if (triangles.empty())
        return;

    // 3) draw triangles with immediate mode
    f->glBegin(GL_TRIANGLES);
    // render objects as white for now 
    f->glColor3f(1.f, 1.f, 1.f);
    foreach (Vertex vertex, vertices)
        f->glVertex3f(vertex.x(), vertex.y(), vertex.z());
    f->glEnd();
}
