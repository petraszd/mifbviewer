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
#ifndef SURFACE_H
#define SURFACE_H

#include <QHash>
#include <QList>
#include <QString>
#include "drawable.h"
#include "patch.h"
#include "boundingbox.h"

class Surface : public Drawable
{
private:
    QHash<QString, QColor> m_colors;
    QHash<QString, QList<Patch*> > m_patches;
public:
    Surface();
    Surface(const QString &filename);
    Surface(const char * filename);
    ~Surface();

    void fromFile(const QString &filename);

    void drawObject() const;
    void drawCage() const;

    const BoundingBox getBoundingBox() const;
    void setDensity(const QString & group, unsigned int density);

    void renormalize();

    const QList<QString> groups() {
        return m_patches.keys();
    }

    void setColor(const QString & group, const QColor & color) {
        m_colors[group] = color;
    }
private:
    void clearPatches();
    void generate();
};

#endif // SURFACE_H
