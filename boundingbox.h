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
#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QtGlobal>
#include "vector3d.h"

class BoundingBox
{
private:
    bool m_firstChecked;

public:
    Vector3D min;
    Vector3D max;
public:
    BoundingBox();

    void check(const Vector3D & v);

    float radius() const;

    float lengthX() const {
        return qAbs(max.x - min.x);
    }

    float lengthY() const {
        return qAbs(max.y - min.y);
    }

    float lengthZ() const {
        return qAbs(max.z - min.z);
    }

    float deltaX() const {
        return -max.x + lengthX() / 2.0f;
    }

    float deltaY() const {
        return -max.y + lengthY() / 2.0f;
    }

    float deltaZ() const {
        return -max.z + lengthZ() / 2.0f;
    }

    const Vector3D center() const {
        return Vector3D(deltaX(), deltaY(), deltaZ());
    }
};

#endif // BOUNDINGBOX_H
