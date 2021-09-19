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
#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <QColor>

const QColor SURFACE_COLOR = QColor(20, 20, 160);
const QColor LINES_COLOR = Qt::black;
const QColor POINTS_COLOR = Qt::white;
const unsigned int DENSITY = 32;
const float CAMERA_ZOOM = 2.0f;
const char SHADER_FILENAME[] = "./shaders/v_isophotes.cg";

#endif // DEFAULTS_H
