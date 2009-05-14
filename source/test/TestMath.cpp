/*!

	\file TestMath.cpp
	
	\author Gregory Diamos
	
	\date July 8, 2008

	\brief THe source for a test program for the functions in 
			math.h

*/

#ifndef TEST_MATH_CPP_INCLUDED
#define TEST_MATH_CPP_INCLUDED

#include "TestMath.h"

namespace test
{
	
	bool TestMath::testIsPowerOfTwo( std::string& stat )
	{
	
		std::stringstream stream;
	
		bool pass;
		
		pass = true;
		
		// test for correctness
		unsigned int powerOfTwoValue;
		
		unsigned int doubles;
		
		doubles = sizeof( unsigned int ) * 8 - 1;
		
		powerOfTwoValue = 1;
		
		for( unsigned int i = 0; i < doubles; i++ )
		{
		
			if( common::isPowerOfTwo( powerOfTwoValue ) )
			{
			
				stream << "Correctly detected " << powerOfTwoValue << " as a power of two.\n";
			
			}
			else
			{
			
				stream << "Incorrectly did not detect " << powerOfTwoValue << " as a power of two.\n";
				
				pass = false;
				
			}
		
			powerOfTwoValue *= 2;
		
		}
		
		for( unsigned int i = 0; i < correctnessIterations; i++ )
		{
		
			unsigned int value;
			
			value = random();
		
			if( common::isPowerOfTwo( value ) )
			{
			
				if( naiveIsPowerOfTwo( value ) )
				{
				
					stream << "Both implementations agreed that " << value << " was a power of two.\n";
				
				}
				else
				{
				
					stream << "Experimental thought that " << value << " was a power of two, but reference did not.\n";
				
					pass = false;
				
				}
			
			}
			else
			{
			
			
				if( naiveIsPowerOfTwo( value ) )
				{
				
					stream << "Experimental thought that " << value << " was not a power of two, but reference did.\n";
					
					pass = false;
				
				}
				else
				{
				
					stream << "Both implementations agreed that " << value << " was not a power of two.\n";
				
				}
			
			}
		
		}
		
		common::Timer Timer;
		
		bool swap;
		
		swap = 0;
		
		Timer.start();
		
		for( unsigned int i = 0; i < iterations; i++ )
		{
		
			swap ^= common::isPowerOfTwo( i );
		
		}
		
		Timer.stop();
		
		stream << "Experimental isPowerOfTwo time was " << Timer.seconds() << " seconds, ( " << Timer.cycles() << " ) cycles. ( " << swap << " )\n";
		
		Timer.start();
		
		for( unsigned int i = 0; i < iterations; i++ )
		{
		
			swap ^= naiveIsPowerOfTwo( i );
		
		}
		
		Timer.stop();
		
		stream << "Reference isPowerOfTwo time was " << Timer.seconds() << " seconds, ( " << Timer.cycles() << " ) cycles. ( " << swap << " )\n";
		
		// same thing for int

		int powerOfTwoIntValue;
		
		powerOfTwoIntValue = 1;
		
		for( unsigned int i = 0; i < doubles; i++ )
		{
		
			if( common::isPowerOfTwo( powerOfTwoIntValue ) )
			{
			
				stream << "Correctly detected " << powerOfTwoIntValue << " as a power of two.\n";
			
			}
			else
			{
			
				stream << "Incorrectly did not detect " << powerOfTwoIntValue << " as a power of two.\n";
				
				pass = false;
				
			}
		
			powerOfTwoIntValue *= 2;
		
		}
		
		for( unsigned int i = 0; i < correctnessIterations; i++ )
		{
		
			int value;
			
			value = random();
		
			if( common::isPowerOfTwo( value ) )
			{
			
				if( naiveIsPowerOfTwo( value ) )
				{
				
					stream << "Both implementations agreed that " << value << " was a power of two.\n";
				
				}
				else
				{
				
					stream << "Experimental thought that " << value << " was a power of two, but reference did not.\n";
				
					pass = false;
				
				}
			
			}
			else
			{
			
			
				if( naiveIsPowerOfTwo( value ) )
				{
				
					stream << "Experimental thought that " << value << " was not a power of two, but reference did.\n";
					
					pass = false;
				
				}
				else
				{
				
					stream << "Both implementations agreed that " << value << " was not a power of two.\n";
				
				}
			
			}
		
		}
		
		int swapInt;
		
		swapInt = 0;
		
		Timer.start();
		
		for( int i = 0; i < (int) iterations; i++ )
		{
		
			swapInt ^= common::isPowerOfTwo( i );
		
		}
		
		Timer.stop();
		
		stream << "Experimental isPowerOfTwo time was " << Timer.seconds() << " seconds, ( " << Timer.cycles() << " ) cycles. ( " << swapInt << " )\n";
		
		Timer.start();
		
		for( int i = 0; i < (int) iterations; i++ )
		{
		
			swapInt ^= naiveIsPowerOfTwo( i );
		
		}
		
		Timer.stop();
		
		stream << "Reference isPowerOfTwo time was " << Timer.seconds() << " seconds, ( " << Timer.cycles() << " ) cycles. ( " << swapInt << " )\n";
		
		stat += stream.str();
		
		return pass;
	
	}

	bool TestMath::testModPowerOfTwo( std::string& stat )
	{
	
		std::stringstream stream;
	
		bool pass;
		
		pass = true;
		
		// test for correctness
		unsigned int powerOfTwoValue;
		
		unsigned int doubles;
		
		doubles = sizeof( unsigned int ) * 8 - 1;
		
		for( unsigned int i = 0; i < correctnessIterations; i++ )
		{
		
			unsigned int value;
			
			value = abs( random() );
		
			powerOfTwoValue = 1;
		
			for( unsigned int i = 0; i < doubles; i++ )
			{
			
				unsigned int experimental;
				
				unsigned int reference;
				
				experimental = common::modPowerOfTwo( value, powerOfTwoValue );
			
				reference = naiveModPowerOfTwo( value, powerOfTwoValue );
				
				if( experimental == reference )
				{
				
					stream << "Both implementations agree that "<< value << " % " << powerOfTwoValue << " = " << experimental << ".\n";
				
				}
				else
				{
				
					stream << "Experimental thinks that " << value << " % " << powerOfTwoValue << " = " << experimental << ", but reference = " << reference << ".\n";
				
					pass = false;
				
				}
			
				powerOfTwoValue *= 2;
			
			}
		
		}
		
		common::Timer Timer;
		
		unsigned int swap;
		
		swap = 0;
		
		Timer.start();
		
		for( unsigned int i = 0; i < iterations; i++ )
		{
		
			swap ^= common::modPowerOfTwo( i, (unsigned int)2 );
		
		}
		
		Timer.stop();
		
		stream << "Experimental modPowerOfTwo time was " << Timer.seconds() << " seconds, ( " << Timer.cycles() << " ) cycles. ( " << swap << " )\n";
		
		Timer.start();
		
		for( unsigned int i = 0; i < iterations; i++ )
		{
		
			swap ^= naiveModPowerOfTwo<unsigned int>( i, 2 );
		
		}
		
		Timer.stop();
		
		stream << "Reference modPowerOfTwo time was " << Timer.seconds() << " seconds, ( " << Timer.cycles() << " ) cycles. ( " << swap << " )\n";
		
		// same thing for int

		int powerOfTwoIntValue;
		
		powerOfTwoIntValue = 1;
		
		doubles = sizeof( unsigned int ) * 8 - 1;
		
		for( unsigned int i = 0; i < correctnessIterations; i++ )
		{
		
			int value;
			
			value = abs( random() );
		
			powerOfTwoIntValue = 1;
		
			for( unsigned int i = 0; i < doubles; i++ )
			{
			
				int experimental;
				
				int reference;
				
				experimental = common::modPowerOfTwo( value, powerOfTwoIntValue );
			
				reference = naiveModPowerOfTwo( value, powerOfTwoIntValue );
				
				if( experimental == reference )
				{
				
					stream << "Both implementations agree that "<< value << " % " << powerOfTwoIntValue << " = " << experimental << ".\n";
				
				}
				else
				{
				
					stream << "Experimental thinks that " << value << " % " << powerOfTwoIntValue << " = " << experimental << ", but reference = " << reference << ".\n";
				
					pass = false;
				
				}
			
				powerOfTwoIntValue *= 2;
			
			}
		
		}
				
		int swapInt;
		
		swapInt = 0;
		
		Timer.start();
		
		for( int i = 0; i < (int) iterations; i++ )
		{
		
			swapInt ^= common::modPowerOfTwo( i, (int)2 );
		
		}
		
		Timer.stop();
		
		stream << "Experimental modPowerOfTwo time was " << Timer.seconds() << " seconds, ( " << Timer.cycles() << " ) cycles. ( " << swapInt << " )\n";
		
		Timer.start();
		
		for( int i = 0; i < (int)iterations; i++ )
		{
		
			swapInt ^= naiveModPowerOfTwo( i, 2 );
		
		}
		
		Timer.stop();
		
		stream << "Reference modPowerOfTwo time was " << Timer.seconds() << " seconds, ( " << Timer.cycles() << " ) cycles. ( " << swapInt << " )\n";
				
		stat += stream.str();
		
		return pass;
	
	
	}

	bool TestMath::testPowerOfTwo( std::string& stats )
	{
	
		std::stringstream stream;
		
		bool pass;
		
		pass = true;
		
		for( unsigned int i = 0; i < correctnessIterations; i++ )
		{
		
			unsigned int value = random();
			
			unsigned int experimental;
			
			unsigned int reference;
			
			experimental = common::powerOfTwo( value );
			
			reference = naivePowerOfTwo( value );
			
			if( experimental == reference )
			{
			
				stream << "Both implementations agree that the next power of two of "<< value << " = " << experimental << ".\n";
			
			}
			else
			{
			
				stream << "Experimental thinks that the next power of two of "<< value << " = " << experimental << ", but reference = " << reference << ".\n";
			
				pass = false;
			
			}
			
		}
		
		common::Timer Timer;
		
		unsigned int swap;
		
		swap = 0;
		
		Timer.start();
		
		for( unsigned int i = 0; i < iterations; i++ )
		{
		
			swap ^= common::powerOfTwo( i );
		
		}
		
		Timer.stop();
	
		stream << "Experimental powerOfTwo time was " << Timer.seconds() << " seconds, ( " << Timer.cycles() << " ) cycles. ( " << swap << " )\n";
		
		Timer.start();
		
		for( unsigned int i = 0; i < iterations; i++ )
		{
		
			swap ^= naivePowerOfTwo( i );
		
		}
		
		Timer.stop();
	
		stream << "Reference powerOfTwo time was " << Timer.seconds() << " seconds, ( " << Timer.cycles() << " ) cycles. ( " << swap << " )\n";
		
		// do for int
		for( unsigned int i = 0; i < correctnessIterations; i++ )
		{
		
			int value = abs( random() );
			
			int experimental;
			
			int reference;
			
			experimental = common::powerOfTwo( value );
			
			reference = naivePowerOfTwo( value );
			
			if( experimental == reference )
			{
			
				stream << "Both implementations agree that the next power of two of "<< value << " = " << experimental << ".\n";
			
			}
			else
			{
			
				stream << "Experimental thinks that the next power of two of "<< value << " = " << experimental << ", but reference = " << reference << ".\n";
			
				pass = false;
			
			}
			
		}
		
		int swapInt;
		
		swapInt = 0;
		
		Timer.start();
		
		for( unsigned int i = 0; i < iterations; i++ )
		{
		
			swapInt ^= common::powerOfTwo( i );
		
		}
		
		Timer.stop();
	
		stream << "Experimental powerOfTwo time was " << Timer.seconds() << " seconds, ( " << Timer.cycles() << " ) cycles. ( " << swapInt << " )\n";
		
		Timer.start();
		
		for( unsigned int i = 0; i < iterations; i++ )
		{
		
			swapInt ^= naivePowerOfTwo( i );
		
		}
		
		Timer.stop();
	
		stream << "Reference powerOfTwo time was " << Timer.seconds() << " seconds, ( " << Timer.cycles() << " ) cycles. ( " << swapInt << " )\n";
		
		stats += stream.str();
		
		return pass;
	
	}

	bool TestMath::doTest( std::string& stat )
	{
	
		bool pass;
		
		pass = testIsPowerOfTwo( stat );
		pass &= testModPowerOfTwo( stat );
		pass &= testPowerOfTwo( stat );
		
		return pass;
	
	}

	TestMath::TestMath()
	{
	
		Name = "TestMath";
		
		Description = "\nA simple test for the functions in math.cpp.  These use\n";
		Description += "bitwise operations to do fast power of 2 operations, this\n";
		Description += "test benchmarks the operations and tests them against\n";
		Description += "equivalent slower implemenations.\n";
	
	}
	

}

int main( int argc, char** argv )
{

	common::ArgumentParser parser( argc, argv );
	
	test::TestMath test;
	
	parser.description( test.description() );
	
	bool help;
	
	parser.parse( "-h", help, "Print this help message." );
	
	parser.parse( "-i", test.iterations, DEFAULT_ITERATIONS, "How many iterations of each instruction to do for timing." );
	parser.parse( "-c", test.correctnessIterations, CORRECTNESS_ITERATIONS, "How many random values to test for correctness." );
	parser.parse( "-s", test.seed, DEFAULT_SEED, "Random seed to use for repeatability." );
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


