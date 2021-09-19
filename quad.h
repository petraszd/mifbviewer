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
#ifndef QUAD_H
#define QUAD_H

class Quad
{
public:
    GLuint a;
    GLuint b;
    GLuint c;
    GLuint d;
public:
    Quad() : a(0), b(0), c(0), d(0)
    { }

    Quad(GLuint a, GLuint b, GLuint c, GLuint d) : a(a), b(b), c(c), d(d)
    { }

    Quad(const Quad & q) : a(q.a), b(q.b), c(q.c), d(q.d)
    { }
};

#endif // QUAD_H
