#ifndef __ZEDARITHMETIC_HPP__
#define __ZEDARITHMETIC_HPP__

#include <System/DataTypes.hpp>
#include <cmath>

const ZED_FLOAT32		ZED_Epsilon = 1.0e-6f;
const ZED_FLOAT32		ZED_HalfEpsilon = 1.0e-3f;
const ZED_FLOAT32		ZED_Pi = 3.1415926535897932384626433832795f;
const ZED_FLOAT32		ZED_IEEE754Infinite32 =
	static_cast< ZED_FLOAT32 >( 0x7f800000 );

// Handy union for fast float calculation of square root (Game Programming Gems
// 6 - "Floating Point Tricks")
typedef union _FOI
{
	ZED_FLOAT32 Float;
	ZED_SINT32 Int;
} ZED_FLOATORINT;

namespace ZED
{
	/**
		\namespace ZED::Arithmetic
		\brief Contains optimised classes for performing mathematical operations
		which 3D applications are so very dependent upon.
	*/
	namespace Arithmetic
	{
		// SIMD type to use
		// ---
		// x86
		// ---
		// 0 = No SIMD
		// 1 = MMX
		// 2 = MMX EX
		// 3 = 3DNOW!
		// 4 = 3DNOW!+/[ENHANCED/EXTENDED] 3D NOW!
		// 5 = AMD GEODE 3DNOW! PROFESSIONAL
		// 6 = SSE
		// 7 = SSE2
		// 8 = SSE3
		// 9 = SSSE3
		// 10 = SSE4.1
		// 11 = SSE4.2
		// 12 = SSE4a
		// ---
		// ARM
		// ---
		// 0 = No SIMD
		// 1 = VFPv3
		// 2 = NEON
		static ZED_UINT8 SIMDType;

		// Determine SIMD type
		void DetermineSIMDType( );

		ZED_INLINE ZED_BOOL IsZero( ZED_FLOAT32 p_Value )
		{
			// REMINDER!
			// Some SIMD would be nice
			// !REMINDER
			return ( fabsf( p_Value ) < ZED_Epsilon );
		}

		ZED_INLINE ZED_FLOAT32 Absolute( ZED_FLOAT32 p_Value )
		{
			// !REMINDER
			// SIMD would be killer in here
			// !REMINDER
			return ( fabsf( p_Value ) );
		}
		
		ZED_INLINE ZED_FLOAT32 SquareRoot( ZED_FLOAT32 p_Value )
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
		
		ZED_INLINE ZED_FLOAT32 InvSquareRoot( ZED_FLOAT32 p_Value )
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
			return 1.0f/SquareRoot( p_Value );
			#endif
		}

		ZED_INLINE ZED_BOOL Equal( ZED_FLOAT32 p_Var1, ZED_FLOAT32 p_Var2 )
		{
			return( IsZero( p_Var1-p_Var2 ) );
		}

		ZED_INLINE void SinCos( ZED_FLOAT32 p_Angle, ZED_FLOAT32 &p_Sine,
			ZED_FLOAT32 &p_Cosine )
		{
			// SIMD would be great everywhere!
			p_Sine = sinf( p_Angle );
			p_Cosine = cosf( p_Angle );
		}
	}
}

#endif

