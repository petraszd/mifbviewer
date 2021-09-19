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
#ifndef CURVATURE_H
#define CURVATURE_H

#include "bezierpatch.h"

class curvature
{
public:
    static GLfloat gaussian(const Vector3D & v00, const Vector3D & v01, const Vector3D & v02,
                            const Vector3D & v10, const Vector3D & v20, const Vector3D & v11,
                            unsigned int degu, unsigned int degv);

    static GLfloat det4( GLfloat x11, GLfloat x12, GLfloat x13, GLfloat x14,
                         GLfloat x21, GLfloat x22, GLfloat x23, GLfloat x24,
                         GLfloat x31, GLfloat x32, GLfloat x33, GLfloat x34,
                         GLfloat x41, GLfloat x42, GLfloat x43, GLfloat x44);

    static GLfloat det3( GLfloat x11, GLfloat x12, GLfloat x13,
                         GLfloat x21, GLfloat x22, GLfloat x23,
                         GLfloat x31, GLfloat x32, GLfloat x33);
private:
    static GLfloat _gaussian(const Vector3D & v00, Vector3D derivs[3][3]);
};

#endif // CURVATURE_H
