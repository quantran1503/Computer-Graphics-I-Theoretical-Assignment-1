// ========================================================================= //
// Authors: Daniel Rutz, Daniel Ströter, Roman Getto, Matthias Bein          //
//                                                                           //
// GRIS - Graphisch Interaktive Systeme                                      //
// Technische Universität Darmstadt                                          //
// Fraunhoferstrasse 5                                                       //
// D-64283 Darmstadt, Germany                                                //
//                                                                           //
// Content: Widget for showing OpenGL scene                                  //
// ========================================================================= //

#include <cmath>

#include <QtDebug>
#include <QMatrix4x4>
#include <QOpenGLVersionFunctionsFactory>

#include "openglview.h"

OpenGLView::OpenGLView(QWidget *parent) : QOpenGLWidget(parent)
{
    setDefaults();

    // Load balloon mesh
    balloonMesh.loadOBJ("../Modelle/ballon.obj");
    // balloonMesh.loadLSA("../Modelle/ballon.lsa");

    brachMesh.loadOBJ("../Modelle/brach.obj");
    // brachMesh.loadLSA("../Modelle/brach.lsa");
    fordMesh.loadOBJ("../Modelle/83ford-gt90.obj");
    // fordMesh.loadLSA("../Modelle/83ford-gt90.lsa");
    delphinMesh.loadOBJ("../Modelle/delphin.obj");

    // Load the sphere of the light
    sphereMesh.loadOBJ("../Modelle/sphere.obj");

    meshes = { &balloonMesh, &brachMesh, &delphinMesh, &fordMesh };

    connect(&fpsCounterTimer, &QTimer::timeout, this, &OpenGLView::refreshFpsCounter);
    fpsCounterTimer.setInterval(1000);
    fpsCounterTimer.setSingleShot(false);
    fpsCounterTimer.start();
}

void OpenGLView::initializeGL()
{
    // load OpenGL functions
    f = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_2_1>(QOpenGLContext::currentContext());
    const GLubyte *versionString = f->glGetString(GL_VERSION);
    qDebug("The current OpenGL version is: %s\n", versionString);

    // black screen
    f->glClearColor(0.f, 0.f, 0.f, 1.f);
    // enable depth buffer
    f->glEnable(GL_DEPTH_TEST);
    // set shading model
    f->glShadeModel(GL_SMOOTH);
    // set lighting and material
    GLfloat global_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat shininess = 128.0f;
    f->glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    f->glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    f->glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    f->glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    f->glEnable(GL_LIGHT0);
    // enable use of glColor instead of glMaterial for ambient and diffuse property
    f->glEnable(GL_COLOR_MATERIAL);
    f->glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    // white shiny specular highlights
    GLfloat specularLightMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat shininessMaterial = 128.0f;
    f->glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininessMaterial);
    f->glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLightMaterial);
}

void OpenGLView::resizeGL(int w, int h)
{
    // Calculate new projection matrix
    QMatrix4x4 projectionMatrix;
    const float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
    projectionMatrix.perspective(65.f, aspectRatio, 0.1f, 100.f);

    // Resize viewport
    f->glViewport(0, 0, w, h);

    // Set projection matrix
    f->glMatrixMode(GL_PROJECTION);
    f->glLoadIdentity();
    f->glMultMatrixf(projectionMatrix.constData());
    f->glMatrixMode(GL_MODELVIEW);
    f->glLoadIdentity();

    update();
}

void OpenGLView::paintGL()
{
    // clear and set camera
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glLoadIdentity();

    // translate to centerPos
    f->glTranslatef(centerPos.x(), centerPos.y(), centerPos.z());

    // disable lighting for coordinate system and light sphere
    f->glDisable(GL_LIGHTING);

    // rotate scene, then render cs
    f->glRotatef(angleX, 0.0f, 1.0f, 0.0f);
    f->glRotatef(angleY, 1.0f, 0.0f, 0.0f);
    drawCS();

    if (lightMoves) {
        moveLight();
    }

    // draw sphere for light still without lighting
    drawLight();

    // draw object
    f->glEnable(GL_LIGHTING);
    // render all meshes white for now 
    f->glColor3f(1.f, 1.f, 1.f);

    float x = 0, y = 0;
    int n = 1;
    for (int i = 0; i < n; i++)
    for (TriangleMesh* mesh : meshes) {
        f->glPushMatrix();
        // affine transformations must be outside glBegin immediate mode
        // f->glRotatef(180, 0.0f, 1.0f, 0.0f);
        f->glTranslatef(x, y, 0);
        x += 4.f;
        y += 4.f;
        // f->glScalef(2.0f, 2.0f, 2.0f);
        mesh->draw(f);
        f->glPopMatrix();
    }

    ++frameCounter;
    update();

    // Emit the triangle count to be shown in the UI.
    emit triangleCountChanged(getTriangleCount(n));
}

void OpenGLView::drawCS()
{
    f->glBegin(GL_LINES);
    // red X
    f->glColor3f(1.f, 0.f, 0.f);
    f->glVertex3f(0.f, 0.f, 0.f);
    f->glVertex3f(10.f, 0.f, 0.f);
    // green Y
    f->glColor3f(0.f, 1.f, 0.f);
    f->glVertex3f(0.f, 0.f, 0.f);
    f->glVertex3f(0.f, 10.f, 0.f);
    // blue Z
    f->glColor3f(0.f, 0.f, 1.f);
    f->glVertex3f(0.f, 0.f, 0.f);
    f->glVertex3f(0.f, 0.f, 10.f);
    f->glEnd();
}

void OpenGLView::drawLight()
{
    // set light position in within current coordinate system
    GLfloat lp[] = { lightPos.x(), lightPos.y(), lightPos.z(), 1.0f };
    f->glLightfv(GL_LIGHT0, GL_POSITION, lp);

    // draw yellow sphere for light source
    f->glPushMatrix();

    f->glTranslatef(lp[0], lp[1], lp[2]);
    f->glScalef(0.6f, 0.6f, 0.6f);
    // f->glScalef(2.f, 2.f, 2.f);
    f->glColor3f(1, 1, 0);
    sphereMesh.draw(f);
    f->glPopMatrix();
}

void OpenGLView::moveLight()
{
    lightPos.rotY(lightMotionSpeed * (deltaTimer.restart() / 1000.f));
}

unsigned int OpenGLView::getTriangleCount(int n) const
{
    int count = 0;
    for (TriangleMesh* mesh: meshes)
        count += mesh->getTriangles().size();

    return n * count + sphereMesh.getTriangles().size();
}

void OpenGLView::setDefaults()
{
    // scene Information
    centerPos = Vec3f(1.0f, -2.0f, -25.0f);
    angleX = 0.0f;
    angleY = 0.0f;

    // light information
    lightPos = Vec3f(-10.0f, 0.0f, 0.0f);
    lightMotionSpeed = 80.0f;
}

void OpenGLView::refreshFpsCounter()
{
    emit fpsCountChanged(frameCounter);
    frameCounter = 0;
}

void OpenGLView::recalcNormals(bool weightByAngle)
{
    for (TriangleMesh* mesh : meshes)
        mesh->calculateNormals(weightByAngle);
    update();
}

void OpenGLView::triggerLightMovement(bool shouldMove)
{
    lightMoves = shouldMove;
    if (lightMoves) {
        if (deltaTimer.isValid()) {
            deltaTimer.restart();
        } else {
            deltaTimer.start();
        }
    }
}

void OpenGLView::cameraMoves(float deltaX, float deltaY, float deltaZ)
{
    centerPos[0] += deltaX;
    centerPos[1] += deltaY;
    centerPos[2] += deltaZ;

    update();
}

void OpenGLView::cameraRotates(float deltaX, float deltaY)
{
    angleX = std::fmod(angleX + deltaX, 360.f);
    angleY += deltaY;
}
