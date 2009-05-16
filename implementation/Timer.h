/*!	\file Timer.h
*
*	\brief Header file for the Timer class
*
*	\author Gregory Diamos
*
*	\date : 9/27/2007
*
*/

#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <implementation/LowLevelTimer.h>
#include <interface/Stringable.h>

/*!
	\brief a namespace for common classes and functions
*/
namespace hydrazine
{


	class Timer : public LowLevelTimer, public Stringable
	{
		public:	
			std::string toString() const;
	};

}

#endif

