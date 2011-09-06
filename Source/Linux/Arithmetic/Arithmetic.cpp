#include <Arithmetic.hpp>

namespace ZED
{
	namespace Arithmetic
	{

		void DetermineSIMDType( )
		{
			__asm__ __volatile(
				"MOV	%EAX,	\n\t" 	// Setting EAX to zero will have CPUID return the vendor's
										// name
				"CPUID\n\t"				// Call CPUID
				)
		}
	}
}

