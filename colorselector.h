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
#ifndef COLORSELECTOR_H
#define COLORSELECTOR_H

#include <QLabel>
#include <QColorDialog>
#include <QMouseEvent>

class ColorSelector : public QLabel
{
    Q_OBJECT;
private:
    QColorDialog m_colorDialog;
    QColor m_color;
public:
    ColorSelector(const QColor & color, QWidget * parent = 0);

    const QColor color() { return m_color; }
protected:
    void mouseReleaseEvent(QMouseEvent * event);
public slots:
    void setColor(const QColor & color) {
        m_color = color;
        setPalette(QPalette(m_color));
        repaint();
    }
    void restoreColor(){
        m_colorDialog.setCurrentColor(m_color);
    }
};

#endif // COLORSELECTOR_H
