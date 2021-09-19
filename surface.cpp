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
#include "surface.h"
#include "datareader.h"
#include "defaults.h"

Surface::Surface() : Drawable()
{
}

Surface::Surface(const QString &filename) : Drawable()
{
    fromFile(filename);
}

Surface::Surface(const char * filename)
{
    fromFile(QString(filename));
}

Surface::~Surface()
{
    clearPatches();
}

void Surface::clearPatches()
{
    foreach (QList<Patch*> v, m_patches) {
        foreach (Patch * p, v) {
            delete p;
        }
        v.clear();
    }
    m_patches.clear();
}

void Surface::generate()
{
    GLfloat min = 0.0;
    GLfloat max = 0.0;
    bool any = false;
    foreach (QList<Patch*> v, m_patches) {
        foreach (Patch * p, v) {
            p->generateMesh();

            if(!p->hasCurvature())
                continue;
            if(!any) {
                any = true;
                min = p->minCurvature();
                max = p->maxCurvature();
                continue;
            }
            if(min > p->minCurvature())
                min = p->minCurvature();
            if(max < p->maxCurvature())
                max = p->maxCurvature();
        }
    }
    foreach (QList<Patch*> v, m_patches)
        foreach (Patch * p, v)
            p->generateCurvature(min, max);

    foreach (QList<Patch*> v, m_patches)
        foreach (Patch * p, v)
            p->generateLists();
}

void Surface::fromFile(const QString &filename)
{
    DataReader r(filename);
    m_patches = r.getPatches();

    foreach (QString str, m_patches.keys())
        m_colors[str] = SURFACE_COLOR;
    generate();
}

void Surface::drawCage() const
{
    foreach (QList<Patch*> v, m_patches)
        foreach (Patch * p, v)
            p->drawCage();
}

void Surface::drawObject() const
{
    QHashIterator<QString, QList<Patch*> > i(m_patches);
    while (i.hasNext()) {
        i.next();
        GLfloat mat[] = {m_colors[i.key()].redF(),
                         m_colors[i.key()].greenF(),
                         m_colors[i.key()].blueF(), 1.0};
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat);
        foreach (Patch * p, i.value())
            p->drawObject();
    }
}

const BoundingBox Surface::getBoundingBox() const
{
    BoundingBox bb;
    foreach (QList<Patch*> v, m_patches)
        foreach (Patch * p, v)
            p->evaluateBoundingBox(bb);
    return bb;
}

void Surface::setDensity(const QString & group, unsigned int density)
{
    foreach (Patch * p, m_patches[group])
        p->setDensity(density);
    generate();
}


void Surface::renormalize()
{
    foreach (QList<Patch*> v, m_patches)
        foreach (Patch * p, v)
            p->renormalize();
    generate();
}
