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
#ifndef BVIEWER_H
#define BVIEWER_H

#include <QMouseEvent>
#include <QKeyEvent>
#include <QGLWidget>
#include <QPoint>
#include <QWheelEvent>
#include "surface.h"
#include "vector3d.h"
#include "surfaceoptions.h"
#include <Cg/cg.h>
#include <Cg/cgGL.h>

class BViewer : public QGLWidget
{
    Q_OBJECT
public:
    enum MoveState {ROTATE, MOVE, ZOOM};

private:
    QPoint m_mousePos;
    GLfloat m_yRot;
    GLfloat m_xRot;

    Vector3D m_ObjectPlace;
    Vector3D m_moveAmount;

    GLfloat m_camRadius;
    bool m_canMove;

    bool m_drawObject;
    bool m_drawLines;
    bool m_drawPoints;
    bool m_drawIsophotes;

    bool m_isCg;

    MoveState m_moveState;

    Surface m_surface;
    SurfaceOptions* m_surfaceOptions;

    CGcontext m_cgContext;

    QMessageBox * m_waitDialog;
public:
    BViewer(QWidget *parent = 0);
    ~BViewer();

    void openOptionts();

    void setFileName(const QString & fileName);

    void setMoveState(MoveState state) {
        m_moveState = state;
    }

    void setDrawObject(bool draw) {
        m_drawObject = draw;
        repaint();
    }

    void setDrawCurvature(bool draw) {
        if(draw)
            glEnable(GL_COLOR_MATERIAL);
        else
            glDisable(GL_COLOR_MATERIAL);
        repaint();
    }

    void setDrawLines(bool draw) {
        m_drawLines = draw;
        repaint();
    }

    void renormalize() {
        openWaitDialog();
        m_surface.renormalize();
        closeWaitDialog();
        repaint();
    }

    void setDrawPoints(bool draw) {
        m_drawPoints = draw;
        repaint();
    }

    void setDrawIsophotes(bool draw) {
        m_drawIsophotes = draw;
        repaint();
    }

    bool isCg() const{
        return m_isCg;
    }

protected:
    void paintGL();
    void initializeGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent * event);

private:
    void openWaitDialog();
    void closeWaitDialog();

    void draw();
    void initCg();

public slots:
    void setSurfaceOptions(const QString & group, unsigned int density,
                           const QColor & color);
};

#endif // BVIEWER_H
