#ifndef __ZEDARITHMETIC_HPP__
#define __ZEDARITHMETIC_HPP__

#include <DataTypes.hpp>
//#include <Assert.hpp>
#include <cmath>

const ZED_FLOAT32		ZED_Epsilon = 1.0e-6f;
const ZED_FLOAT32		ZED_HalfEpsilon = 1.0e-3f;

// Handy union for fast float calculation of square root (Game Programming Gems
// 6 - "Floating Point Tricks")
typedef union _FOI
{
	ZED_FLOAT32 Float;
	ZED_INT32 Int;
} ZED_FLOATORINT;

namespace ZED
{
	namespace Arithmetic
	{
		ZED_INLINE ZED_BOOL zedIsZero( ZED_FLOAT32 p_Value )
		{
			// REMINDER!
			// Some SIMD would be nice
			// !REMINDER
			return ( fabsf( p_Value ) < ZED_Epsilon );
		}

		ZED_INLINE ZED_FLOAT32 zedAbsolute( ZED_FLOAT32 p_Value )
		{
			// !REMINDER
			// SIMD would be killer in here
			// !REMINDER
			return ( fabsf( p_Value ) );
		}
		
		ZED_INLINE ZED_FLOAT32 zedSquareRoot( ZED_FLOAT32 p_Value )
		{
			#if ZED_APPROXIMATE
			zedAssert( p_Value > 0.0f );
			ZED_INTORFLOAT WorkingValue;
			WorkingValue.Float = p_Value;
			WorkingValue.Int -= 0x3F800000;	// Subtract 127 from biased exponent
			WorkingValue.Int >> 1;			// Preserve sign
			WorkingValue.Int += 0x3F800000; // Re-bias new exponent
			WorkingValue.Int &= 0x7FFFFFFF; // Remove sign bit
			
			return WorkingValue.Float;
			#else
			return sqrtf( p_Value );
			#endif
		}
		
		ZED_INLINE ZED_FLOAT32 zedInvSquareRoot( ZED_FLOAT32 p_Value )
		{
			#if ZED_APPROXIMATE
			ZED_FLOAT32 Half = 0.5f * p_Value;
			ZED_INTORFLOAT WorkingValue;
			WorkingValue.Float = p_Value;
			// Subtract guess from magic number
			WorkingValue.Int = 0x5F3759DF - ( WorkingValue.Int >> 1 );
			// Newton step, increases accuracy
			WorkingValue.Float =
				WorkingValue.Float * ( 1.5f - Half * 
					( WorkingValue.Float * WorkingValue.Float ) );
					
			return WorkingValue.Float;
			
			#else
			return 1.0f/zedSquareRoot( p_Value );
			#endif
		}

		ZED_INLINE ZED_BOOL zedEqual( ZED_FLOAT32 p_Var1, ZED_FLOAT32 p_Var2 )
		{
			return( zedIsZero( p_Var1-p_Var2 ) );
		}
	}
}

#endif

