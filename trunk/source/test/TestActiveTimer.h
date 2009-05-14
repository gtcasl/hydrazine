/*!

	\file TestActiveTimer.h
	
	\date September 25, 2008
	
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	
	\brief The header file for the TestActiveTimer class.

*/

#ifndef TEST_ACTIVE_TIMER_H_INCLUDED
#define TEST_ACTIVE_TIMER_H_INCLUDED

#include <common/interface/Test.h>
#include <common/interface/ActiveTimer.h>
#include <boost/random/mersenne_twister.hpp>

namespace test
{

	class TestActiveTimer : public Test
	{
	
		private:
				
			class Timer : public common::ActiveTimer
			{
			
				public:
				
					common::Timer timer;
					void fired();
			
			};

		private:

			bool doTest( std::string& status );
		
		public:
			
			TestActiveTimer();
			
			bool verbose;
			
			unsigned int objects;
			common::Timer::Second delayStep;
			common::Timer::Second minDelay;
	
	};

}

int main( int argc, char** argv );

#endif

