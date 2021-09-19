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
#include <cmath>
#include "curvature.h"

GLfloat curvature::gaussian(const Vector3D & v00, const Vector3D & v01, const Vector3D & v02,
                            const Vector3D & v10, const Vector3D & v20, const Vector3D & v11,
                            unsigned int degu, unsigned int degv)
{
    Vector3D derivs[3][3];
    unsigned int degu1 = degu - 1;
    unsigned int degv1 = degv - 1;

    derivs[0][1] = degu * (v01 - v00);
    if(degu1 == 0)
        derivs[0][2].toZero();
    else
        derivs[0][2] = degu * degu1 * (v02 - 2 * v01 + v00);
    derivs[1][0] = degv * (v10 - v00);
    if(degv1 == 0)
        derivs[2][0].toZero();
    else
        derivs[2][0] = degv * degv1 * (v20 - 2 * v10 + v00);

    derivs[1][1] = degu * degv * (v11 - v01 - v10 + v00);

    return _gaussian(v00, derivs);
}

GLfloat curvature::_gaussian(const Vector3D & v00, Vector3D derivs[3][3])
{
    double x,y,z,d;
    double xu,yu,zu,du, xv,yv,zv,dv;
    double xuu,yuu,zuu,duu, xvv,yvv,zvv,dvv;
    double xuv,yuv,zuv,duv;
    double kes, m1, m2, m3;
    double L, M, N;
    double E, G, F;
    double K;

    x = v00.x; y = v00.y;
    z = v00.z; d = 1.0;

    xu = derivs[1][0].x; yu = derivs[1][0].y;
    zu = derivs[1][0].z; du = 1.0;

    xuu = derivs[2][0].x; yuu = derivs[2][0].y;
    zuu = derivs[2][0].z; duu = 1.0;

    xv = derivs[0][1].x; yv = derivs[0][1].y;
    zv = derivs[0][1].z; dv = 1.0;

    xvv = derivs[0][2].x; yvv = derivs[0][2].y;
    zvv = derivs[0][2].z; dvv = 1.0;

    xuv = derivs[1][1].x; yuv = derivs[1][1].y;
    zuv = derivs[1][1].z; duv = 1.0;

    L=det4(xuu,yuu,zuu,duu,
           xu,yu,zu,du,
           xv,yv,zv,dv,
           x,y,z,d);
    N=det4(xvv,yvv,zvv,dvv,
           xu,yu,zu,du,
           xv,yv,zv,dv,
           x,y,z,d);
    M=det4(xuv,yuv,zuv,duv,
           xu,yu,zu,du,
           xv,yv,zv,dv,
           x,y,z,d);

    E = (xu*d-x*du)*(xu*d-x*du) + (yu*d-y*du)*(yu*d-y*du) +
        (zu*d-z*du)*(zu*d-z*du);
    G = (xv*d-x*dv)*(xv*d-x*dv) + (yv*d-y*dv)*(yv*d-y*dv) +
        (zv*d-z*dv)*(zv*d-z*dv);
    F = (xu*d-x*du)*(xv*d-x*dv) + (yu*d-y*du)*(yv*d-y*dv) +
        (zu*d-z*du)*(zv*d-z*dv);

    m1=det3(y,z,d,yu,zu,du,yv,zv,dv);
    m2=det3(x,z,d,xu,zu,du,xv,zv,dv);
    m3=det3(x,y,d,xu,yu,du,xv,yv,dv);
    kes=m1*m1+m2*m2+m3*m3;

    K = d*d*d*d*(L*N-M*M)/(kes*kes);  // Gaussian curvature

    return K;
}

GLfloat curvature::det4( GLfloat x11, GLfloat x12, GLfloat x13, GLfloat x14,
                         GLfloat x21, GLfloat x22, GLfloat x23, GLfloat x24,
                         GLfloat x31, GLfloat x32, GLfloat x33, GLfloat x34,
                         GLfloat x41, GLfloat x42, GLfloat x43, GLfloat x44)
{
    GLfloat t0 = x11*x22*x33*x44-x11*x22*x34*x43-
                 x11*x32*x23*x44+x11*x32*x24*x43+x11*x42*x23*x34-
                 x11*x42*x24*x33-x21*x12*x33*x44+x21*x12*x34*x43+
                 x21*x32*x13*x44-x21*x32*x14*x43-x21*x42*x13*x34+
                 x21*x42*x14*x33+x31*x12*x23*x44-x31*x12*x24*x43-
                 x31*x22*x13*x44+x31*x22*x14*x43+x31*x42*x13*x24-
                 x31*x42*x14*x23-x41*x12*x23*x34+x41*x12*x24*x33+
                 x41*x22*x13*x34-x41*x22*x14*x33-x41*x32*x13*x24+
                 x41*x32*x14*x23;
    return t0;
}

GLfloat curvature::det3( GLfloat x11, GLfloat x12, GLfloat x13,
                         GLfloat x21, GLfloat x22, GLfloat x23,
                         GLfloat x31, GLfloat x32, GLfloat x33)
{
    GLfloat t0 = x11*x22*x33-x11*x23*x32-x12*x21*x33+x12*x23*x31+
                 x13*x21*x32-x13*x22*x31;
    return t0;
}

//{
//
//double krv(VEC v00, VEC derivs[3][3], REAL* crv_result)
//
//
//
//        if(0) {//fabs(kes) < tol*tol) {  // temp for Jorg & Kestas & me class A
//                K = H = Max = Min = 0;
//        }
//        else {
//
//                H = d*(L*G-2*M*F+N*E) / sqrt(kes*kes*kes) /2;  // Mean curvature
////         printf("[krv] %f, %f \n",K, H);
//                disc = H*H - K;
//                if (disc < 0) {
//                        if (disc < -tol)
//                                printf("[krv] disc %f H %f \n",disc, H);
//                        Max = Min = H;
//                }
//                else {
//                        disc = sqrt(disc);
//                        Max = H + disc;
//                        Min = H - disc;
//                }
//        }
//
//        if(0) {  // scale the result by log?
//        K = scalebylog(K);
//        H = scalebylog(H);
//        Max = scalebylog(Max);
//        Min = scalebylog(Min);
//        }
//
//    crv_result[0] = K; // Gaussian curvature
//    crv_result[1] = H; // Mean curvature
//    crv_result[2] = Max; // max curvature
//    crv_result[3] = Min; // min curvature
//
//        // a special
//        special_curvature =  ratio_a*K + ratio_b*H*H;
////	printf("special_curvature : %f\n", special_curvature);
//
//    if( freshObject )
//                min_crv_value[4] = max_crv_value[4] = special_curvature;
//        else {
//                if(special_curvature<min_crv_value[4]) min_crv_value[4] = special_curvature;
//                if(special_curvature>max_crv_value[4]) max_crv_value[4] = special_curvature;
//        }
//
//        // set the maximum or minimum value of all four curvatures
//        minmax(crv_result, GAUSS_CRV, 4);
//
//
//    return K;
//}
