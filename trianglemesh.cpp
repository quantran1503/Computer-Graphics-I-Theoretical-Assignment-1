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
    // TODO: 1) read all vertices and triangles from the file
    // You can ignore all other information in the file

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

    // TODO: 3) draw triangles with immediate mode
}
