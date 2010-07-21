
#ifndef __PORTABLE_FRAMETIMER_H___
#define __PORTABLE_FRAMETIMER_H___

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

/*!
 *	\brief	This function initialises the frame timer and should be called once 
 *			In your initialisation code.
 */
EXTERN void  InitFrameTimer();

/*!
 *	\brief	This function updates the frametimer.
 */
EXTERN void  SortFrameTimer();

/*!
 *	\brief	This functon will return access to the time taken for the last frame.
 *			This is also bound into LUA if you do not specify -DNO_LUA in the compile 
 *			options
 */
EXTERN float FrameTime();

#endif


