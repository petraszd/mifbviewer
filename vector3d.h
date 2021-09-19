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
#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <QtOpenGL>
#include <cmath>

class Vector3D
{
public:
    GLfloat x;
    GLfloat y;
    GLfloat z;

public:
    Vector3D(GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z = 0.0f) :
            x(x), y(y), z(z)
    { }

    Vector3D(const Vector3D & v) : x(v.x), y(v.y), z(v.z)
    { }

    const Vector3D cross(const Vector3D& v) const {
        return Vector3D(y * v.z - z * v.y,
                        z * v.x - x * v.z,
                        x * v.y - y * v.x);
    }

    GLfloat dot(const Vector3D& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    void normalize() {
        GLfloat len = length();
        x /= len;
        y /= len;
        z /= len;
    }

    const Vector3D normalized() {
        GLfloat len = length();
        return Vector3D(x / len, y / len, z / len);
    }

    void toZero() { x = y = z = 0.0f; }

    void setAll(GLfloat newx, GLfloat newy, GLfloat newz) {
        x = newx;
        y = newy;
        z = newz;
    }

    inline GLfloat length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3D &operator+=(const Vector3D & v) {
        x += v.x; y += v.y; z += v.z; return *this;
    }

    Vector3D &operator-=(const Vector3D & v) {
        x -= v.x; y -= v.y; z -= v.z; return *this;
    }

    Vector3D &operator*=(GLfloat c) {
        x *= c; y *= c; z *= c; return *this;
    }

    Vector3D &operator/=(GLfloat c){
        x /= c; y /= c; z /= c; return *this;
    }

    friend inline const Vector3D operator+(const Vector3D&, const Vector3D&);
    friend inline const Vector3D operator-(const Vector3D&, const Vector3D&);

    friend inline const Vector3D operator*(const Vector3D&, GLfloat);
    friend inline const Vector3D operator*(GLfloat, const Vector3D&);

    friend inline const Vector3D operator/(const Vector3D&, GLfloat);
};

inline const Vector3D operator+(const Vector3D& v1, const Vector3D& v2)
{
    return Vector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline const Vector3D operator-(const Vector3D& v1, const Vector3D& v2)
{
    return Vector3D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline const Vector3D operator*(const Vector3D& v, GLfloat c)
{
    return Vector3D(v.x * c, v.y * c, v.z * c);
}

inline const Vector3D operator*(GLfloat c, const Vector3D& v)
{
    return Vector3D(v.x * c, v.y * c, v.z * c);
}

inline const Vector3D operator/(const Vector3D& v, GLfloat c)
{
    return Vector3D(v.x / c, v.y / c, v.z / c);
}

#endif // VECTOR3D_H
