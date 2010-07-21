
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <time.h>
#endif

float g_frametime=0.0001f;

/*
 *	================================================================ Frame timing (WIN32)
 */
#ifdef WIN32
	#include <windows.h>

	float	g_fStartOfFrame_,
			g_fEndOfFrame_  ,
			g_fRateInv_     ;
	__int64 StartClk_       ;

	/*
	 *	Initialise the frame counter
	 */
	void  InitFrameTimer()
	{
		__int64 Rate;
		g_fRateInv_ = 1.0f/(float)CLOCKS_PER_SEC;

		if( !QueryPerformanceFrequency( ( LARGE_INTEGER* )&Rate) )
			return;

		if(!Rate)
			return;

		g_fRateInv_=1.0f/(float)Rate;
		if(!QueryPerformanceCounter( ( LARGE_INTEGER* )&StartClk_) )
			return;

	}

	/*
	 *	Get the current time
	 */
	float CurrentTime_()
	{
		__int64 EndClk;
		QueryPerformanceCounter(( LARGE_INTEGER* )&EndClk);
		return  ( EndClk - StartClk_ )*g_fRateInv_;
	}



#else	/*============================================= Linux frametiming */

	#include <sys/time.h> /* Linux system clock functions */
	#include <time.h>


	double g_fStartOfFrame_=0.0,g_fEndOfFrame_=0.0;

	double CurrentTime_() /* request frame time from Linux */
	{
		struct timeval tv;
		struct timezone tz;
		gettimeofday(&tv, &tz);
		return (double)tv.tv_sec + (double)tv.tv_usec/(1000*1000);
	}

	void  InitFrameTimer() {
		g_fStartOfFrame_ = CurrentTime_();
	}

#endif

/*
 *	Update frame time
 */
void  SortFrameTimer()
{
	do
	{
		g_fEndOfFrame_= CurrentTime_();
	}
	while(g_fEndOfFrame_==g_fStartOfFrame_);

	g_frametime       =  g_fEndOfFrame_ - g_fStartOfFrame_ ;
	g_fStartOfFrame_  =  g_fEndOfFrame_ ;
}



/*!
 *	\brief	This function provides the frame time to the user. This is exactly the
 *			same value as the dt parameter for the Update function. Primarily this
 *			is provided to pass the value through to LUA if needed!
 */
float FrameTime()
{
	return g_frametime;
}

