/*!

	\file TestActiveTimer.cpp
	
	\date September 25, 2008
	
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	
	\brief The source file for the TestActiveTimer class.

*/

#ifndef TEST_ACTIVE_TIMER_CPP_INCLUDED
#define TEST_ACTIVE_TIMER_CPP_INCLUDED

#include "TestActiveTimer.h"
#include <common/implementation/ArgumentParser.h>
#include <ctime>
#include <vector>

namespace test
{
	
	////////////////////////////////////////////////////////////////////////////
	// TestActiveTimer::Timer
	void TestActiveTimer::Timer::fired()
	{
	
		timer.stop();
	
	}
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// TestActiveTimer
	bool TestActiveTimer::doTest( std::string& status )
	{
	
		typedef std::vector< Timer > Vector;
		Vector timers;
		
		timers.resize( objects );

		unsigned int count = objects - 1;
		
		common::Timer::Second time = ( count * delayStep/1000.0 )  + 
			( minDelay / 1000.0 );
		
		for( Vector::reverse_iterator fi = timers.rbegin(); 
			fi != timers.rend(); ++fi )
		{
		
			fi->timer.start();
			fi->start( time );
			std::stringstream stream;
			stream << "Timer " << count-- << " will fire after " << time 
				<< " seconds.\n";
			status += stream.str();
			time -= delayStep/1000.0;
		
		}
		
		for( Vector::iterator fi = timers.begin(); fi != timers.end(); ++fi )
		{
		
			fi->wait();
		
		}
		
		count = 0;
		
		for( Vector::iterator fi = timers.begin(); fi != timers.end(); ++fi )
		{
		
			if( fi != timers.begin() )
			{
			
				if( fi->timer.seconds() < time )
				{
				
					std::stringstream stream;
					stream << "Timer fired after " << fi->timer.seconds() 
						<< " before previous timer which fired at " 
						<< time << "\n";
					status += stream.str();
					return false;
				
				}
				else
				{
				
					std::stringstream stream;
					stream << "Timer " << count++ << " fired after " 
					<< fi->timer.seconds() 
						<< " seconds.\n";
					status += stream.str();
					
				}
				
			}
			else
			{
			
				std::stringstream stream;
				stream << "Timer " << count++ << " fired after " 
					<< fi->timer.seconds() 
					<< " seconds.\n";
				status += stream.str();
	
			}
			
			time = fi->timer.seconds();
		
		}
		
		status += "Test passed.\n";
		return true;
		
	}
		
	TestActiveTimer::TestActiveTimer()
	{
	
		Name = "TestActiveTimer";
		
		Description = "Create a bunch of timers, record when they start, ";
		Description += "tell them to fire in order.  Make sure that they ";
		Description += "actually do.";
	
	}
	////////////////////////////////////////////////////////////////////////////
			
}

int main( int argc, char** argv )
{
	
	common::ArgumentParser parser( argc, argv );
	
	test::TestActiveTimer test;
	
	parser.description( test.testDescription() );
	
	bool help;
	
	parser.parse( "-h", help, "Print this help message." );
	parser.parse( "-v", test.verbose, "Print info when the test is over." );
	
	parser.parse( "-s", test.seed, 0, "Random seed for repeatability." );
	parser.parse( "-t", test.objects, 10, "Number of timers to fire." );
	parser.parse( "-min", test.minDelay, 50, 
		"Min amount of microseconds to wait." );
	parser.parse( "-step", test.delayStep, 50, 
		"Number of microseconds to wait between objects." );
	
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

