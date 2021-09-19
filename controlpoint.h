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
#ifndef CONTROLPOINT_H
#define CONTROLPOINT_H

#include <QtOpenGL>
#include "vector3d.h"

struct ControlPoint
{
    Vector3D pos;
    GLfloat w;

    ControlPoint(GLfloat x=0.0f, GLfloat y=0.0f,
                 GLfloat z=0.0f, GLfloat w=0.0f) :
    pos(x, y, z), w(w) {}

    void setAll(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
        pos.x = x;
        pos.y = y;
        pos.z = z;
        this->w = w;
    }
};

#endif // CONTROLPOINT_H
