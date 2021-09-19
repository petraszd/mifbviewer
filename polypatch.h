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
#ifndef POLYPATCH_H
#define POLYPATCH_H

#include "patch.h"
#include "vertex.h"
#include "triangle.h"

class PolyPatch : public Patch
{
private:
    unsigned int m_Npoints;
    unsigned int m_Nfaces;

    unsigned int m_pointIndex;
    unsigned int m_faceIndex;

    Vertex* m_points;
    Triangle* m_faces;

    GLuint m_list;

public:
    PolyPatch(unsigned int vertexCount, unsigned int facesCount);
    ~PolyPatch();

    void addVertex(GLfloat x, GLfloat y, GLfloat z);
    void addFace(GLuint a, GLuint b, GLuint c);

    virtual void drawCage() const;
    virtual void drawObject() const;

    virtual void evaluateBoundingBox(BoundingBox & bb);
    virtual void generateMesh();
    virtual void generateLists();
private:
    void genNormals();
};

#endif // POLYPATCH_H
