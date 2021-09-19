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
#include "boundingbox.h"

BoundingBox::BoundingBox() : m_firstChecked(false)
{ }

float BoundingBox::radius() const {
    float x = lengthX();
    float y = lengthY();
    float z = lengthZ();

    if(x > y && x > z)
        return x;
    if(y > z)
        return y;
    return z;
}

void BoundingBox::check(const Vector3D & v)
{
    if(!m_firstChecked) {
        min.x = max.x = v.x;
        min.y = max.y = v.y;
        min.z = max.z = v.z;

        m_firstChecked = true;
        return;
    }

    if(max.x < v.x)
        max.x = v.x;
    else if(min.x > v.x)
        min.x = v.x;

    if(max.y < v.y)
        max.y = v.y;
    else if(min.y > v.y)
        min.y = v.y;

    if(max.z < v.z)
        max.z = v.z;
    else if(min.z > v.z)
        min.z = v.z;
}
