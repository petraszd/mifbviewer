/******************************************************************************
 *   Copyright (C) 2009 by Petras Zdanavičius                                 *
 *                                                                            *
 *   This program is free software: you can redistribute it and/or modify     *
 *   the Free Software Foundation, either version 3 of the License, or        *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   This program is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
 *   GNU Lesser General Public License for more details.                      *
 *                                                                            *
 *   You should have received a copy of the GNU Lesser General Public License *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 ******************************************************************************/
#include <QApplication>
#include <QColor>
#include <QtOpenGL>
#include "bviewer.h"
#include "boundingbox.h"
#include "defaults.h"

BViewer::BViewer(QWidget *parent) :
        QGLWidget(parent),
        m_mousePos(0, 0),
        m_ObjectPlace(0.0, 0.0, 0.0),
        m_moveAmount(0.0, 0.0, 0.0),
        m_drawObject(true),
        m_drawLines(false),
        m_drawPoints(false),
        m_drawIsophotes(false),
        m_isCg(true),
        m_moveState(ROTATE),
        m_surfaceOptions(0),
        m_cgContext(0)
{
    m_waitDialog = new QMessageBox();
    m_waitDialog->setWindowTitle(tr("Please Wait..."));

    m_canMove = false;
    m_xRot = 0.0f;
    m_yRot = 0.0f;
    m_camRadius = 0.0f;
}

BViewer::~BViewer()
{
    delete m_waitDialog;
    delete m_surfaceOptions;
}

void BViewer::openWaitDialog()
{
    m_waitDialog->show();
    m_waitDialog->setFixedWidth(250);
    m_waitDialog->setEnabled(false);
}

void BViewer::closeWaitDialog()
{
    m_waitDialog->close();
}

void BViewer::initializeGL()
{
    qglClearColor(QColor(100, 100, 100));

    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLineWidth(2.0f);
    glPointSize(4.0f);

    GLfloat ligthAmbient[] = {0.0, 0.0, 0.0, 0.0};
    GLfloat lightSpecular[] =  {0.5, 0.5, 0.5, 1.0};
    GLfloat lightPosition[] =  {4.0, 0.0, 8.0, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ligthAmbient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    GLfloat materialFrontColor[] = {
        SURFACE_COLOR.redF(), SURFACE_COLOR.greenF(),
        SURFACE_COLOR.blueF(), 1.0};
    GLfloat materialSpecular[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialFrontColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 99);

    initCg();
}

void BViewer::initCg()
{
    // Put filenames to constants
    m_cgContext = cgCreateContext();
    if(!m_cgContext) {
        m_isCg = false;
        return;
    }

    CGprogram cgVProg = cgCreateProgramFromFile(m_cgContext, CG_SOURCE,
                                                SHADER_FILENAME,
                                                CG_PROFILE_ARBVP1, "main", NULL);
    if(!cgVProg) {
        m_isCg = false;
        return;
    }

    cgGLLoadProgram(cgVProg);
    cgGLBindProgram(cgVProg);
}

void BViewer::resizeGL(int w, int h)
{
    GLfloat ratio = GLfloat(w) / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, ratio, 1, 10000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
            0.0f, 0.0f, m_camRadius,
            0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f);

    repaint();
}

void BViewer::paintGL()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glPushMatrix();

    glTranslatef(m_moveAmount.x, m_moveAmount.y, m_moveAmount.z);

    glRotatef(m_xRot, 1.0, 0.0, 0.0);
    glRotatef(m_yRot, 0.0, 1.0, 0.0);

    glTranslatef(m_ObjectPlace.x, m_ObjectPlace.y, m_ObjectPlace.z);

    draw();

    glPopMatrix();
}

void BViewer::openOptionts()
{
    m_surfaceOptions->open();
}

void BViewer::setSurfaceOptions(const QString & group,
                                unsigned int density,
                                const QColor & color)
{
    openWaitDialog();

    m_surface.setColor(group, color);
    m_surface.setDensity(group, density);

    closeWaitDialog();

    repaint();
}

void BViewer::setFileName(const QString & fileName)
{
    openWaitDialog();
    m_surface.fromFile(fileName);
    closeWaitDialog();

    BoundingBox bb = m_surface.getBoundingBox();

    m_ObjectPlace = bb.center();
    m_moveAmount.toZero();

    m_camRadius = bb.radius() * CAMERA_ZOOM;

    delete m_surfaceOptions;
    m_surfaceOptions = new SurfaceOptions(m_surface.groups(), this);
    connect(m_surfaceOptions, SIGNAL(groupOptions(QString,uint,QColor)),
            this, SLOT(setSurfaceOptions(QString,uint,QColor)));

    resizeGL(width(), height());
    repaint();
}

void BViewer::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        m_canMove = true;
        m_mousePos = event->pos();
    }
}

void BViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        m_canMove = false;
}

void BViewer::mouseMoveEvent(QMouseEvent *event)
{
    if(!m_canMove)
        return;
    float deltaY = event->y() - m_mousePos.y();
    float deltaX = event->x() - m_mousePos.x();
    switch(m_moveState) {
                case MOVE:
        m_moveAmount.x += m_camRadius * (deltaX / width());
        m_moveAmount.y -= m_camRadius * (deltaY / height());
        break;
                case ZOOM:
        m_moveAmount.z -= m_camRadius * (deltaY / height());
        break;
                case ROTATE:
                default:
        m_xRot += deltaY;
        m_yRot += deltaX;
        break;
    }

    m_mousePos = event->pos();
    repaint();
}

void BViewer::wheelEvent(QWheelEvent * event)
{
    m_moveAmount.z -= m_camRadius * (float(-event->delta()) / height());
    repaint();
}

void BViewer::draw()
{
    glDisable(GL_LIGHTING);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    qglColor(LINES_COLOR);
    if(m_drawLines)
        m_surface.drawCage();

    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    qglColor(POINTS_COLOR);
    if(m_drawPoints)
        m_surface.drawCage();

    glEnable(GL_LIGHTING);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if(m_drawIsophotes)
        cgGLEnableProfile(CG_PROFILE_ARBVP1);

    if(m_drawObject)
        m_surface.drawObject();

    if(m_drawIsophotes)
        cgGLDisableProfile(CG_PROFILE_ARBVP1);
}
