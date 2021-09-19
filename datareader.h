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
#ifndef DATAREADER_H
#define DATAREADER_H

#include <QString>
#include <QTextStream>
#include <QHash>
#include "patch.h"

class DataReader
{
private:
    enum SURFACE_TYPES{
        POLY = 1,
        BIDIR_BEZIER = 4,
        BEZIER = 5,
        RATIONAL_BEZIER = 8
                      };
    QString m_filename;

public:
    DataReader(const QString &filename);
    DataReader(const char *filename);
    ~DataReader();

    void setFilename(const QString &filename);

    QHash<QString, QList<Patch*> > getPatches() const;

    Patch* readBezier(QTextStream &stream) const;
    Patch* readRBezier(QTextStream &stream) const;
    Patch* readBDirBezier(QTextStream &stream) const;
    Patch* readPoly(QTextStream &stream) const;

};

#endif // DATAREADER_H
