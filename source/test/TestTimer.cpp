/*!
	
	\file TestTimer.cpp
	
	\author Greory Diamos
	
	\date June 7, 2008
	
	\brief The source file for the TestTimer function.
	
*/


#ifndef TEST_TIMER_CPP_INCLUDED
#define TEST_TIMER_CPP_INCLUDED

#include "TestTimer.h"

namespace test
{


	bool TestTimer::doTest( std::string& status )
	{
	
		common::Timer Timer;
		
		std::stringstream stream;
		
		stream << "Timer starting " << Timer.toString() << ".\n";
		
		Timer.start();
		
		bool test;
		
		test = false;
		
		for( unsigned int i = 0; i < iterations; i++ )
		{
		
			test = !test;
		
		}
		
		Timer.stop();

		common::Timer::Second seconds;
		
		common::Timer::Cycle cycles;
		
		seconds = Timer.seconds();
		
		cycles = Timer.cycles();
		
		stream << "Timer ending " << Timer.toString() << ".\n";

		for( unsigned int i = 0; i < iterations; i++ )
		{
		
			test = !test;
				
		}

		bool result;

		if( cycles == Timer.cycles() && seconds == Timer.seconds() ) 
		{
		
			result = true;
		
		}
		else
		{
		
			result = false;
		
		}
	
		if( test )
		{
		
			Timer.stop();
		
		}
		
		status += stream.str();
		
		return result;
	
	}

	TestTimer::TestTimer()
	{
	
		Name = "TestTimer";
		
		Description = "A simple test to spin for a number of iterations and print out the time recorded by a Timer in the spin loop.";
	
	}

}

int main( int argc, char** argv )
{

	common::ArgumentParser parser( argc, argv );
	
	test::TestTimer test;
	
	parser.description( test.description() );
	
	bool help;
	
	parser.parse( "-h", help, "Print this help message." );
	
	parser.parse( "-iterations", test.iterations, DEFAULT_ITERATIONS, "How many iterations to spin around in." );
	parser.parse( "-v", test.verbose, "Print out status message when the test is over." );

	if( help )
	{
	
		std::cout << parser.help();
	
		return false;
	
	}

	test.test();

	if( test.verbose )
	{

		std::cout << test.toString();
	
	}

	return test.passed();

}

#endif
