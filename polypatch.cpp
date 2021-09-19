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
#include "polypatch.h"

PolyPatch::PolyPatch(unsigned int vertexCount, unsigned int facesCount) :
        m_Npoints(vertexCount), m_Nfaces(facesCount),
        m_pointIndex(0), m_faceIndex(0), m_list(0)
{
    m_points = new Vertex[m_Npoints];
    m_faces = new Triangle[m_Nfaces];
}

PolyPatch::~PolyPatch()
{
    if(m_list)
        glDeleteLists(m_list, 1);
    delete[] m_points;
    delete[] m_faces;
}


void PolyPatch::addVertex(GLfloat x, GLfloat y, GLfloat z)
{
    m_points[m_pointIndex].pos.x = x;
    m_points[m_pointIndex].pos.y = y;
    m_points[m_pointIndex].pos.z = z;
    m_pointIndex++;
}

void PolyPatch::addFace(GLuint a, GLuint b, GLuint c)
{
    m_faces[m_faceIndex].a = a;
    m_faces[m_faceIndex].b = b;
    m_faces[m_faceIndex].c = c;
    m_faceIndex++;
}

void PolyPatch::evaluateBoundingBox(BoundingBox & bb)
{
    for(unsigned int i = 0; i < m_Npoints; ++i) {
        bb.check(m_points[i].pos);
    }
}

void PolyPatch::generateMesh()
{
    genNormals();
}

void PolyPatch::generateLists()
{
    if(m_list)
        glDeleteLists(m_list, 1);
    m_list = glGenLists(1);

    glNewList(m_list, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    for(unsigned int i = 0; i < m_Nfaces; ++i) {
        glNormal3fv((GLfloat*)&m_points[m_faces[i].a].nor);
        glVertex3fv((GLfloat*)&m_points[m_faces[i].a].pos);

        glNormal3fv((GLfloat*)&m_points[m_faces[i].b].nor);
        glVertex3fv((GLfloat*)&m_points[m_faces[i].b].pos);

        glNormal3fv((GLfloat*)&m_points[m_faces[i].c].nor);
        glVertex3fv((GLfloat*)&m_points[m_faces[i].c].pos);
    }
    glEnd();
    glEndList();
}

void PolyPatch::drawCage() const
{
    glCallList(m_list);
}

void PolyPatch::drawObject() const
{
    glCallList(m_list);
}

void PolyPatch::genNormals()
{
    Vector3D n;
    for(unsigned int i = 0; i < m_Nfaces; ++i) {
        n = (m_points[ m_faces[i].b ].pos - m_points[ m_faces[i].c ].pos)
            .cross(m_points[ m_faces[i].c ].pos - m_points[ m_faces[i].a ].pos);
        n.normalize();

        m_points[ m_faces[i].a ].nor += n;
        m_points[ m_faces[i].b ].nor += n;
        m_points[ m_faces[i].c ].nor += n;
    }

    for(unsigned int i = 0; i < m_Npoints; ++i){
        m_points[i].nor.normalize();
        m_points[i].nor *= m_normalDirection;
    }
}
