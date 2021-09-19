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
#ifndef SURFACEOPTIONS_H
#define SURFACEOPTIONS_H

#include <QDialog>
#include <QList>
#include <QString>
#include <QHash>
#include <QSpinBox>
#include "colorselector.h"

class SurfaceOptions : public QDialog
{
    Q_OBJECT;
private:
    QList<QString> m_groups;
    QHash<QString, QSpinBox*> m_spinBoxes;
    QHash<QString, ColorSelector*> m_colorSelectors;
public:
    SurfaceOptions(const QList<QString> & groups, QWidget *parent=0);
    virtual ~SurfaceOptions();

signals:
    void groupOptions(const QString& group, unsigned int density, const QColor& color);

public slots:
    void accept();
};

#endif // SURFACEOPTIONS_H
