/*
 * TypeDef.h
 *
 *  Created on: 2018/11/14
 *      Author: imao_yusuke
 */

#ifndef SRC_COMMON_TYPEDEF_H_
#define SRC_COMMON_TYPEDEF_H_

#include <math.h>

typedef      unsigned char         uint1;
typedef      unsigned short int    uint2;
typedef      unsigned long  int    uint4;
typedef      unsigned long  long   uint8;
typedef      signed char           int1;
typedef      signed short int      int2;
typedef      signed long  int      int4;
typedef      signed long  long     int8;
typedef      float                 real4;
typedef      double                real8;

typedef struct FixedPointVal
{
	enum Element
	{
		DecimalVal = 0,
		IntegerVal,
		NumOfFixePointElements
	};

	int4 value[NumOfFixePointElements];

	FixedPointVal():value(){}
	FixedPointVal(const real8 &val)
	{
	    int8 tmp = (int8)(val * 0x100000000);

	    this->value[DecimalVal] = (int4)(tmp & 0xffffffff);
	    this->value[IntegerVal] = (int4)floor(val);
	}

	FixedPointVal &operator=(const real8 &val)
	{
		int8 tmp = (int8)(val * 0x100000000);

		this->value[DecimalVal] = (int4)(tmp & 0xffffffff);
	    this->value[IntegerVal] = (int4)floor(val);

	    return (*this);
	}

	real8 toDouble() const
	{
		real8 decimal = 0.0;
		real8 tmp = 0.0;
		uint4 bit = 0;
		for(uint4 i=0; i<32; ++i){
			tmp = pow(0.5, (int)(32-i));
			bit = (value[DecimalVal]>>i) & 1;
			decimal += tmp*bit;
		}

		return value[IntegerVal]+decimal;
	}

} fix8;


#endif /* SRC_COMMON_TYPEDEF_H_ */
