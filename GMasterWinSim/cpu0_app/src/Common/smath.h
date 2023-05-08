/*
 * smath.h
 *
 *  Created on: 2018/11/14
 *      Author: imao_yusuke
 */

#ifndef SRC_COMMON_SMATH_H_
#define SRC_COMMON_SMATH_H_

#include <math.h>
#include <float.h>

#include <stdio.h>
#include <stddef.h>

#ifdef WIN32
#else
#include "xil_printf.h"
#endif

/**
 *
 */
static const float PI = 3.14159265358979323846F;

#define M_2PI  6.28318530717958647692F
#define M_PI2  1.57079632679489661923F
#define S_M_PI 3.14159265358979323846F

#define UNITCNVT_DEG_TO_RAD(a)  ((a)*0.017453292519943295769236907684886) //((a)*3.141592654/180.0)
#define UNITCNVT_RAD_TO_DEG(a)  ((a)*57.295779513082320876798154814105)   //((a)*180.0/3.141592654)


#ifndef M_PI
#define M_PI    PI
#endif

#define f_limit(data,limit) (float)(( (data) >  (limit) ) ?  (limit) : \
                                    ( (data) < -(limit) ) ? -(limit) : \
                                      (data) )

#ifdef __RENESAS__
#define f_abs(data) fabs(data)
#else
#define f_abs(data) (float)( (data) < 0 ? -(data) : (data) )
#endif

#define f_min(a,b) (float)((a) < (b) ? (a) : (b) )

#define f_max(a,b) (float)((a) > (b) ? (a) : (b) )


/**
 *
 */

inline double s_cos(double x)
{
#ifdef __RENESAS__
	double angl = (double)x;
    if(x<0){
        angl *= (-1);
    }

    int angl_i = (int)(angl*0x8000/M_PI);

    float sinv;
    float cosv;

    fsca(angl_i,&sinv,&cosv);

    return (double)(cosv);
#else
    return (double)cos(x);
#endif
}

inline double s_sin(float x)
{
#ifdef __RENESAS__
	double angl = (double)x;
    int angl_i = (int)(angl*0x8000/M_PI);
    float sinv;
    float cosv;

    fsca(angl_i,&sinv,&cosv);

    return (double)(sinv);
#else
    return (double)sin(x);
#endif
}


/**
*  Discription:  Alternate for acos() to consider double calculation deffernce   *
*  inline for MT(fast runtime) use otherwise no reason                           *
*  For Bug 226(The calculation differnce 10e-16 digit) this function was added   *
*/
inline double s_acos(float a){

    //0 <= ret <= pai
    if( a>1.0 && a < 1.000001){
        a=1.0;
    }
    else if(a < -1.0 && a > -1.000001){
        a=-1.0;
    }
    else{
        ;
    }
    return (double)acos(a);
}

/**
*  Discription:  Alternate for asin() to consider double calculation deffernce   *
*  inline for MT(fast runtime) use otherwise no reason                           *
*  For Bug 226(The calculation differnce 10e-16 digit) this function was added   *
*/

inline  double s_asin(float a){
    if(a>1.0 && a < 1.000001){
        a=1.0;
    }
    else if(a < -1.0 && a > -1.000001){
        a=-1.0;
    }
    else{
        ;
    }
    return asin(a);
}

/**
 *
 */
inline double s_tan(float a)
{
	double input = (double)fmod((double)a,360.0);
	double res = 0;
    if( input < 0 ) input += 360.0F;
    if ((input == 0.0F) || (input == 180.0F)){
        res = 0.0F;
    }
    else if ( input == 90.0F ){
        res = FLT_MAX;
    }
    else if (input == 270.0F ){
        res = -FLT_MAX;
    }
    else{
        res = (double)tan (input * M_PI / 180.0);
    }
    return res;
}

//float s_add_f64( float src1, float src2, bool *ok = 0 );

/** atan
 *  return 0 - 2PI
 */
inline float s_atan2(float pos_x, float pos_y)
{

    float ang = (float)atan2(pos_y,pos_x); //return -PI - +PI in radian
    if (ang < 0.0 ){
        ang = ang  + 2.0F * PI;
    }

    return ang;
}

inline double distance2D(long x0, long y0, long x1, long y1)
{
	double dx = x1 - x0;
	double dy = y1 - y0;
	return sqrt(dx * dx + dy * dy);
}

inline double distance3D(long x0, long y0, long z0, long x1, long y1, long z1)
{
	double dx = x1 - x0;
	double dy = y1 - y0;
	double dz = z1 - z0;
	return sqrt(dx * dx + dy * dy + dz * dz);
}

inline void rotate2D(long &xp, long &yp, long x, long y, double theta, long xc=0, long yc=0)
{
	xp = (long)floor((x - xc) * cos(theta) - (y - yc) * sin(theta) + xc + 0.5);
	yp = (long)floor((x - xc) * sin(theta) + (y - yc) * cos(theta) + yc + 0.5);
}

inline void rotateXY(long *dest, const long *src, double angleRad, const long *center=NULL)
{
	if( dest == NULL || src == NULL ) {
		return;
	}

	long cx = 0, cy = 0;
	if( center != NULL ) {
		cx = center[0];
		cx = center[1];
	}
	dest[0] = (long)floor((src[0] - cx) * cos(angleRad) - (src[1] - cy) * sin(angleRad) + cx + 0.5);
	dest[1] = (long)floor((src[0] - cx) * sin(angleRad) + (src[1] - cy) * cos(angleRad) + cy + 0.5);

}

#endif /* SRC_COMMON_SMATH_H_ */
