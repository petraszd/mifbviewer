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
#ifndef PATCH_H
#define PATCH_H

#include "drawable.h"
#include "boundingbox.h"

class Patch : public Drawable
{
public:
    static const unsigned int MIN_DENSITY = 5;
    enum NormalDirection {NEGATIVE = -1, POSITIVE = 1};
protected:
    unsigned int m_density;
    NormalDirection m_normalDirection;
public:
    Patch();
    virtual ~Patch() {}

    virtual void evaluateBoundingBox(BoundingBox & bb) = 0;
    virtual void generateMesh() = 0;
    virtual void generateLists() = 0;
    virtual void generateCurvature(GLfloat min, GLfloat max) {
        Q_UNUSED(min);
        Q_UNUSED(max);
    }

    virtual bool hasCurvature() const {
        return false;
    }

    virtual GLfloat minCurvature() const {
        return 0.0;
    }

    virtual GLfloat maxCurvature() const {
        return 0.0;
    }

    void setDensity(unsigned int density) {
        if(m_density == density)
            return;
        if(m_density < MIN_DENSITY)
            return;
        m_density = density;
    }

    void setNormalDitection(NormalDirection direction) {
        if(m_normalDirection == direction)
            return;
        m_normalDirection = direction;
    }

    void renormalize() {
        m_normalDirection = NormalDirection(int(m_normalDirection) * -1);
    }
};

#endif // PATCH_H
