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
#ifndef BEZIERPATCH_H
#define BEZIERPATCH_H

#include <QtOpenGL>
#include "patch.h"
#include "vector3d.h"
#include "controlpoint.h"
#include "vertex.h"
#include "quad.h"
#include "factorial.h"

class BezierPatch : public Patch
{
private:
    Factorial m_factorials;

    unsigned int m_degu;
    unsigned int m_degv;

    unsigned int m_index;
    unsigned int m_Ncontrols;
    ControlPoint * m_controls;

    unsigned int m_NcontrolFaces;
    Quad * m_controlFaces;

    unsigned int m_Npoints;
    Vertex * m_points;
    GLfloat * m_curvatures;

    unsigned int m_Nfaces;
    Quad * m_faces;

    GLuint m_cageLists;
    GLuint m_surfLists;

    GLfloat m_minCurvature;
    GLfloat m_maxCurvature;
public:
    BezierPatch(unsigned int degu, unsigned int degv);
    ~BezierPatch();

    virtual void generateMesh();
    virtual void generateLists();

    void addControlPoint(float x, float y, float z, float w);

    virtual void drawCage() const;
    virtual void drawObject() const;

    virtual void evaluateBoundingBox(BoundingBox & bb);

    virtual void generateCurvature(GLfloat min, GLfloat max);

    virtual bool hasCurvature() const {
        return true;
    }

    virtual GLfloat minCurvature() const {
        return m_minCurvature;
    }

    virtual GLfloat maxCurvature() const {
        return m_maxCurvature;
    }

private:
    void genFaces();
    void genNormals();
    void genCage();
    void calcCurvature();

    void curvatureValues(unsigned int center,
                         unsigned int left, unsigned int right,
                         GLfloat & angles, GLfloat & areas);

    void genPoints();
    void genPoint(float u, float v, Vertex & p);
    float B(float u, unsigned int n, unsigned i);
};

#endif // BEZIERPATCH_H
