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
#include <QtGlobal>
#include <QtOpenGL>
#include <cmath>
#include "bezierpatch.h"
#include "curvature.h"

BezierPatch::BezierPatch(unsigned int degu, unsigned int degv) :
        Patch(),
        m_factorials(qMax(degu, degv) + 1),
        m_controls(0),
        m_NcontrolFaces(0),
        m_controlFaces(0),
        m_Npoints(0),
        m_points(0),
        m_curvatures(0),
        m_Nfaces(0),
        m_faces(0),
        m_minCurvature(0.0),
        m_maxCurvature(0.0)
{
    m_degu = degu;
    m_degv = degv;
    m_index = 0;
    m_Ncontrols = (degu + 1) * (degv + 1);
    m_controls = new ControlPoint[m_Ncontrols];

    m_cageLists = glGenLists(1);
    m_surfLists = glGenLists(1);
}

BezierPatch::~BezierPatch()
{
    delete []m_controls;
    delete []m_controlFaces;
    delete []m_points;
    delete []m_faces;
    delete []m_curvatures;

    glDeleteLists(m_cageLists, 1);
    glDeleteLists(m_surfLists, 1);
}

void BezierPatch::addControlPoint(float x, float y, float z, float w)
{
    m_controls[m_index].setAll(x, y, z, w);
    m_index++;
}

void BezierPatch::evaluateBoundingBox(BoundingBox & bb)
{
    for(unsigned int i = 0; i < m_Ncontrols; ++i) {
        bb.check(m_controls[i].pos);
    }
}

void BezierPatch::drawObject() const
{
    glCallList(m_surfLists);
}

void BezierPatch::drawCage() const
{
    glCallList(m_cageLists);
}

void BezierPatch::generateMesh()
{
    // TODO: Check if we realy need this
    if(m_points) {
        glDeleteLists(m_cageLists, 1);
        glDeleteLists(m_surfLists, 1);
    }

    genPoints();
    genFaces();
    genNormals();
    genCage();
    calcCurvature();
}

void BezierPatch::generateLists()
{
    glNewList(m_cageLists, GL_COMPILE);
    glBegin(GL_QUADS);
    for(unsigned int i = 0; i < m_NcontrolFaces; ++i) {
        glVertex4fv((GLfloat*)&m_controls[m_controlFaces[i].a]);
        glVertex4fv((GLfloat*)&m_controls[m_controlFaces[i].b]);
        glVertex4fv((GLfloat*)&m_controls[m_controlFaces[i].c]);
        glVertex4fv((GLfloat*)&m_controls[m_controlFaces[i].d]);
    }
    glEnd();
    glEndList();

    glNewList(m_surfLists, GL_COMPILE);
    glBegin(GL_QUADS);
    for(unsigned int i = 0; i < m_Nfaces; ++i) {
        glNormal3fv((GLfloat*)&m_points[m_faces[i].a].nor);
        glColor3fv((GLfloat*)&m_points[m_faces[i].a].col);
        glVertex3fv((GLfloat*)&m_points[m_faces[i].a].pos);

        glNormal3fv((GLfloat*)&m_points[m_faces[i].b].nor);
        glColor3fv((GLfloat*)&m_points[m_faces[i].b].col);
        glVertex3fv((GLfloat*)&m_points[m_faces[i].b].pos);

        glNormal3fv((GLfloat*)&m_points[m_faces[i].c].nor);
        glColor3fv((GLfloat*)&m_points[m_faces[i].c].col);
        glVertex3fv((GLfloat*)&m_points[m_faces[i].c].pos);

        glNormal3fv((GLfloat*)&m_points[m_faces[i].d].nor);
        glColor3fv((GLfloat*)&m_points[m_faces[i].d].col);
        glVertex3fv((GLfloat*)&m_points[m_faces[i].d].pos);
    }
    glEnd();
    glEndList();
}

void BezierPatch::genPoints()
{
    delete[] m_points;
    m_Npoints = m_density * m_density;
    m_points = new Vertex[m_Npoints];

    GLfloat u, v;
    for(unsigned int i = 0; i < m_density; ++i) {
        u = i / GLfloat(m_density - 1);
        for(unsigned int j = 0; j < m_density; ++j) {
            v = j / GLfloat(m_density - 1);
            genPoint(u, v, m_points[i * m_density + j]);
        }
    }
}


void BezierPatch::genPoint(GLfloat u, GLfloat v, Vertex & p)
{
    unsigned int len = m_degv + 1;

    GLfloat prefix, Bi, Bj;
    for(unsigned int i = 0; i <= m_degu; ++i) {
        Bi = B(u, m_degu, i);
        for(unsigned int j = 0; j <= m_degv; ++j) {
            Bj = B(v, m_degv, j);
            prefix = Bi * Bj / m_controls[i * len + j].w;
            p.pos += prefix * m_controls[i * len + j].pos;
        }
    }
}

GLfloat BezierPatch::B(GLfloat u, unsigned int n, unsigned int i)
{
    return (m_factorials[n] / GLfloat(m_factorials[i] * m_factorials[n - i]))
            * (std::pow(1 - u, n - i) * std::pow(u, i));
}

void BezierPatch::calcCurvature()
{
    unsigned int indexes[9];
    Vector3D v0, v1;
    GLfloat anglesSum, areasSum;

    delete [] m_curvatures;
    m_curvatures = new GLfloat[m_Npoints];

    for(unsigned int i = 1; i < m_density - 1; ++i) {
        for(unsigned int j = 1; j < m_density - 1; ++j) {
            indexes[1] = (i - 1) * m_density + j;
            indexes[4] = i * m_density + j;
            indexes[7] = (i + 1) * m_density + j;
            indexes[0] = indexes[1] - 1;
            indexes[2] = indexes[1] + 1;
            indexes[3] = indexes[4] - 1;
            indexes[5] = indexes[4] + 1;
            indexes[6] = indexes[7] - 1;
            indexes[8] = indexes[7] + 1;

            anglesSum = 0.0;
            areasSum = 0.0;
            curvatureValues(indexes[4], indexes[0], indexes[1], anglesSum, areasSum);
            curvatureValues(indexes[4], indexes[1], indexes[2], anglesSum, areasSum);
            curvatureValues(indexes[4], indexes[2], indexes[5], anglesSum, areasSum);
            curvatureValues(indexes[4], indexes[5], indexes[8], anglesSum, areasSum);
            curvatureValues(indexes[4], indexes[8], indexes[7], anglesSum, areasSum);
            curvatureValues(indexes[4], indexes[7], indexes[6], anglesSum, areasSum);
            curvatureValues(indexes[4], indexes[6], indexes[3], anglesSum, areasSum);
            curvatureValues(indexes[4], indexes[3], indexes[0], anglesSum, areasSum);

            m_curvatures[indexes[4]] = (3 * (2 * M_PI - anglesSum)) / (4 * areasSum);
        }

    }

    // Adding values to edges
    for(unsigned int i = 1; i < m_density - 1; ++i) {
        // Left Collumn
        m_curvatures[i] = m_curvatures[i + m_density];

        // Right collumn
        indexes[0] = (m_density - 1) * m_density + i;
        indexes[1] = indexes[0] - m_density;
        m_curvatures[indexes[0]] = m_curvatures[indexes[1]];

        // Bottom row
        indexes[0] = i * m_density;
        indexes[1] = indexes[0] + 1;
        m_curvatures[indexes[0]] = m_curvatures[indexes[1]];

        // Top Row
        indexes[0] = i * m_density + m_density - 1;
        indexes[1] = indexes[0] - 1;
        m_curvatures[indexes[0]] = m_curvatures[indexes[1]];
    }

    //Adding values to corners
    m_curvatures[0] = m_curvatures[1];
    m_curvatures[m_density - 1] = m_curvatures[m_density - 2];
    m_curvatures[(m_density - 1) * m_density] = m_curvatures[(m_density - 1) * m_density + 1];
    m_curvatures[m_Npoints - 1] = m_curvatures[m_Npoints - 2];


    GLfloat min = m_curvatures[0];
    GLfloat max = m_curvatures[0];
    QColor color;
    for(unsigned int i = 1; i < m_Npoints; ++i){
        if(max < m_curvatures[i])
            max = m_curvatures[i];
        if(min > m_curvatures[i])
            min = m_curvatures[i];
    }

    m_minCurvature = min;
    m_maxCurvature = max;

//    qDebug() << "  " << m_minCurvature << " .. " << m_maxCurvature;
}

//// TODO: refactoring
//void BezierPatch::calcCurvature()
//{
//    if(!m_points) {
//        return;
//    }
//    delete [] m_curvatures;
//    m_curvatures = new GLfloat[m_Npoints];
//    for(unsigned int i = 0; i < m_Npoints; ++i) {
//        m_curvatures[i] = 0.0;
//    }
//
//    unsigned int _00, _01, _02, _10, _20, _11;
//    // Main
//    for(unsigned int i = 0; i < m_density - 2; ++i) {
//        for(unsigned int j = 0; j < m_density - 2; ++j) {
//            _00 = (i) * m_density + (j);
//            _10 = _00 + 1;
//            _20 = _00 + 2;
//            _01 = _00 + m_density;
//            _11 = _00 + m_density + 1;
//            _02 = _00 + 2 * m_density;
//            m_curvatures[_00] = curvature::gaussian(
//                    m_points[_00].pos, m_points[_01].pos, m_points[_02].pos,
//                    m_points[_10].pos, m_points[_20].pos,
//                    m_points[_11].pos, m_density - 2, m_density - 2);
//        }
//    }
//
//    // Right edge
//    unsigned int index;
//    for(unsigned int i = 0; i < m_density; ++i) {
//        index = (i) * m_density + m_density - 2;
//        m_curvatures[index] = m_curvatures[index - 1];
//        index++;
//        m_curvatures[index] = m_curvatures[index - 2];
//    }
//
//    // Bottom edge
//    for(unsigned int i = 0; i < m_density; ++i) {
//        index = (m_density - 2) * m_density + i;
//        m_curvatures[index] = m_curvatures[index - m_density];
//        index += m_density;
//        m_curvatures[index] = m_curvatures[index - 2 * m_density];
//    }
////    for(unsigned int i = m_density - 2; i < m_density; ++i) {
////        for(unsigned int j = 0; j < m_density - 2; ++j) {
////            _00 = (i) * m_density + (j);
////            _10 = _00 + 1;
////            _20 = _00 + 2;
////            _01 = _00 - m_density;
////            _11 = _00 - m_density + 1;
////            _02 = _00 - 2 * m_density;
////            m_curvatures[_00] = curvature::gaussian(
////                    m_points[_00].pos, m_points[_01].pos, m_points[_02].pos,
////                    m_points[_10].pos, m_points[_20].pos,
////                    m_points[_11].pos, m_density-1, m_density-1);
////        }
////    }
//
//    // Top Left corner
////    for(unsigned int i = 0; i < 2; ++i) {
////        for(unsigned int j = m_density - 2; j < m_density; ++j) {
////            _00 = (i) * m_density + (j);
////            _10 = _00 - 1;
////            _20 = _00 - 2;
////            _01 = _00 - m_density;
////            _11 = _00 - m_density - 1;
////            _02 = _00 - 2 * m_density;
////            m_curvatures[_00] = curvature::gaussian(
////                    m_points[_00].pos, m_points[_01].pos, m_points[_02].pos,
////                    m_points[_10].pos, m_points[_20].pos,
////                    m_points[_11].pos, m_density-1, m_density-1);
////        }
////    }
//
//    GLfloat min = m_curvatures[0];
//    GLfloat max = m_curvatures[0];
//
//    for(unsigned int i = 0; i < m_Npoints; ++i) {
//        if(m_curvatures[i] == 0.0)
//            continue;
//        if(min > m_curvatures[i])
//            min = m_curvatures[i];
//        if(max < m_curvatures[i])
//            max = m_curvatures[i];
//    }
//
//    m_minCurvature = min;
//    m_maxCurvature = max;
//
//    qDebug() << "  " << m_minCurvature << " .. " << m_maxCurvature;
//}

void BezierPatch::generateCurvature(GLfloat min, GLfloat max) {
//    min = m_minCurvature;
//    max = m_maxCurvature;
    GLfloat delta = max - min;

    GLfloat intensity;
    QColor color;
    for(unsigned int i = 0; i < m_Npoints; ++i){
        if(m_curvatures[i] == 0.0) {
            m_points[i].col.setAll(0.0, 0.0, 0.0);
            continue;
        }
        intensity = (m_curvatures[i] - min) / delta;
        if(intensity > 1.0)
            intensity = 1.0;

        color.setHsvF(intensity, 1.0, 0.5);
        m_points[i].col.setAll(color.redF(), color.greenF(), color.blueF());
    }
}

void BezierPatch::curvatureValues(unsigned int center,
                                  unsigned int left, unsigned int right,
                                  GLfloat & angles, GLfloat & areas)
{
    Vector3D v0 = m_points[center].pos - m_points[left].pos;
    Vector3D v1 = m_points[center].pos - m_points[right].pos;

    areas += v0.cross(v1).length() / 2.0;

    v0.normalize();
    v1.normalize();

    angles += std::acos(v0.dot(v1));
}


void BezierPatch::genFaces()
{
    delete []m_faces;

    GLuint len = m_density - 1;

    m_Nfaces = len * len;
    m_faces = new Quad[m_Nfaces];

    unsigned int index = 0;
    for(unsigned int i = 0; i < len; ++i){
        for(unsigned int j = 0; j < len; ++j){
            index = i * len + j;
            m_faces[index].a = i * (m_density) + j;
            m_faces[index].b = i * (m_density) + j + 1;
            m_faces[index].c = (i + 1) * (m_density) + j + 1;
            m_faces[index].d = (i + 1) * (m_density) + j;
        }
    }
}

void BezierPatch::genNormals()
{
    Vector3D n;
    for(unsigned int i = 0; i < m_Nfaces; ++i) {
        n = (m_points[ m_faces[i].b ].pos - m_points[ m_faces[i].c ].pos)
            .cross(m_points[ m_faces[i].c ].pos - m_points[ m_faces[i].a ].pos);
        n.normalize();

        m_points[ m_faces[i].a ].nor += n;
        m_points[ m_faces[i].b ].nor += n;
        m_points[ m_faces[i].c ].nor += n;
        m_points[ m_faces[i].d ].nor += n;
    }

    for(unsigned int i = 0; i < m_Npoints; ++i){
        m_points[i].nor.normalize();
        m_points[i].nor *= m_normalDirection;
    }
}

void BezierPatch::genCage()
{
    m_NcontrolFaces = m_degu * m_degv;
    m_controlFaces = new Quad[m_NcontrolFaces];

    unsigned int size = m_degv + 1;
    unsigned int index;

    for(unsigned i = 0; i < m_degu; ++i) {
        for(unsigned j = 0; j < m_degv; ++j) {
            index = i * m_degv + j;
            m_controlFaces[index].a = i * size + j;
            m_controlFaces[index].b = i * size + j + 1;
            m_controlFaces[index].c = (i + 1) * size + j + 1;
            m_controlFaces[index].d = (i + 1) * size + j;
        }
    }
}
