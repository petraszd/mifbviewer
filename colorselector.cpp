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
#include "colorselector.h"

ColorSelector::ColorSelector(const QColor & color, QWidget * parent)
    : QLabel(parent), m_colorDialog(color, this), m_color(color)
{
    setPalette(QPalette(color));
    setAutoFillBackground(true);

    connect(&m_colorDialog, SIGNAL(colorSelected (const QColor&)),
            this, SLOT(setColor(const QColor&)));
    connect(&m_colorDialog, SIGNAL(rejected()), this, SLOT(restoreColor()));
}

void ColorSelector::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        m_colorDialog.open();
}
