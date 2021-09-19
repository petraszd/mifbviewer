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
#include <QObject>
#include <QFile>
#include "datareader.h"
#include "bezierpatch.h"
#include "polypatch.h"

DataReader::DataReader(const QString &filename)
{
    setFilename(filename);
}

DataReader::DataReader(const char * filename)
{
    setFilename(QString(filename));
}

DataReader::~DataReader()
{
}

void DataReader::setFilename(const QString &filename)
{
    m_filename = filename;
}

QHash<QString, QList<Patch*> > DataReader::getPatches() const
{
    QHash<QString, QList<Patch*> > patches;

    QFile file(m_filename);
    if(!file.open(QFile::ReadOnly)) {
        qWarning(QObject::tr("Can't open file: %s").toStdString().c_str(),
                 m_filename.toStdString().c_str());
        file.close();
        return patches;
    }

    QTextStream stream(&file);

    int type;
    QString group = "default";
    bool converted;
    QString line;
    for(;;) {
        line = stream.readLine();
        if(stream.atEnd())
            break;
        if(line.trimmed().isEmpty())
            continue;
        type = line.toUInt(&converted);
        if(!converted) {
            group = line;
            continue;
        }
        switch(type) {
            case POLY:
                patches[group] << readPoly(stream);
            break;
            case BEZIER:
                patches[group] << readBezier(stream);
            break;
            case BIDIR_BEZIER:
                patches[group] << readBDirBezier(stream);
            break;
            case RATIONAL_BEZIER:
                patches[group] << readRBezier(stream);
            break;
            default:
                qWarning(QObject::tr("Unsuported patch type").toStdString().c_str());
                file.close();
                return patches;
            break;
        }
    }

    file.close();
    return patches;
}

Patch* DataReader::readBezier(QTextStream &stream) const
{
    uint degu;
    uint degv;

    stream >> degu >> degv;

    uint n = (degu + 1) * (degv + 1);
    float x, y, z;
    BezierPatch *p = new BezierPatch(degu, degv);
    for(uint i = 0; i < n; ++i) {
        stream >> x >> y >> z;
        p->addControlPoint(x, y, z, 1.0);
    }

    return p;
}

Patch* DataReader::readRBezier(QTextStream &stream) const
{
    uint degu;
    uint degv;

    stream >> degu >> degv;

    uint n = (degu + 1) * (degv + 1);
    float x, y, z, w;
    BezierPatch *p = new BezierPatch(degu, degv);
    for(uint i = 0; i < n; ++i) {
        stream >> x >> y >> z >> w;
        p->addControlPoint(x, y, z, w);
    }

    return p;
}

Patch* DataReader::readBDirBezier(QTextStream &stream) const
{
    uint degu;
    uint degv;

    stream >> degu;
    degv = degu;

    uint n = (degu + 1) * (degv + 1);
    float x, y, z;
    BezierPatch *p = new BezierPatch(degu, degv);
    for(uint i = 0; i < n; ++i) {
        stream >> x >> y >> z;
        p->addControlPoint(x, y, z, 1.0);
    }

    return p;
}

/// In file poly object can have both Quads and Triangles,
/// but PolyPath excepts only Triangles
/// This method first reads all points and all faces,
/// splits all quads to triangles and pass all data
/// to PolyPath
Patch* DataReader::readPoly(QTextStream &stream) const
{
    uint vertexCount;
    uint faceCount;

    stream >> vertexCount >> faceCount;

    QVector<Vector3D> vertexes;
    QVector<Triangle> faces;

    float x, y, z;
    for(uint i = 0; i < vertexCount; ++i) {
        stream >> x >> y >> z;
        vertexes << Vector3D(x, y, z);
    }

    uint type;
    uint a, b, c, d;
    for(uint i = 0; i < faceCount; ++i) {
        stream >> type;
        if(type == 3) {
            stream >> a >> b >> c;
            faces << Triangle(a, b, c);
        } else {
            stream >> a >> b >> c >> d;
            faces << Triangle(a, b, c);
            faces << Triangle(c, d, a);
        }
    }

    PolyPatch *p = new PolyPatch(vertexes.count(), faces.count());

    foreach(Vector3D v, vertexes)
        p->addVertex(v.x, v.y, v.z);

    foreach(Triangle t, faces)
        p->addFace(t.a, t.b, t.c);

    return p;
}
