/*!
	
	\file TestSerializationBuffer.h
	
	\author Greory Diamos
	
	\date July 8, 2008
	
	\brief The source file for the TestSerializationBuffer test suite.
	
*/

#ifndef TEST_SERIALIZATION_BUFFER_CPP_INCLUDED
#define TEST_SERIALIZATION_BUFFER_CPP_INCLUDED

#include "TestSerializationBuffer.h"

namespace test
{
	
	bool TestSerializationBuffer::testReadWrite( std::string& stat )
	{
	
		bool pass;
		
		pass = true;
	
		std::stringstream stream;
	
		std::queue< std::vector< char > > values;
		
		mersenneTwister random(seed);
		
		common::SerializationBuffer buffer;
		
		for( unsigned int i = 0; i < iterations; i++ )
		{
		
			unsigned int size;
			
			size = random() % maxSize;
			
			std::vector< char > element;
			
			element.resize( size );
			
			for( std::vector< char >::iterator fi = element.begin(); fi != element.end(); fi++ )
			{
			
				*fi = random();
			
			}
			
			buffer.write( &element[0], size );
		
			values.push( element );
			
			while( ( ( random() & 0x1 ) > 0 ) && !values.empty() )
			{
			
				std::vector< char > readElement;
				
				readElement.resize( values.front().size() );
				
				buffer.read( &readElement[0], readElement.size() );
			
				std::vector< char >::iterator vi;
				
				vi = values.front().begin();
			
				for( std::vector< char >::iterator fi = readElement.begin(); fi != readElement.end(); fi++ )
				{
				
					assert( vi != values.front().end() );
				
					unsigned int val;
					
					unsigned int ref;
					
					ref = *vi;
					val = *fi;
				
					if( val != ref )
					{
					
						stream << "Read write test failed at iteration " << i << ", read value " << val << " did not match reference " << ref << ".\n";
					
						pass = false;
					
					}
				
					++vi;
				
				}
				
				if( !pass )
				{
				
					break;
				
				}
				
				values.pop();
			
			}
			
			if( !pass )
			{
			
				break;
			
			}
		
		}
		
		while( !values.empty() )
		{
		
			std::vector< char > readElement;
			
			readElement.resize( values.front().size() );
			
			buffer.read( &readElement[0], readElement.size() );
		
			std::vector< char >::iterator vi;
			
			vi = values.front().begin();
		
			for( std::vector< char >::iterator fi = readElement.begin(); fi != readElement.end(); fi++ )
			{
			
				assert( vi != values.front().end() );
			
				unsigned int val;
					
				unsigned int ref;
				
				ref = *vi;
				val = *fi;
			
				if( val != ref )
				{
				
					stream << "Read write test failed at end, read value " << val << " did not match reference " << ref << ".\n";
				
					pass = false;
				
				}
			
				++vi;
			
			}
			
			if( !pass )
			{
			
				break;
			
			}
			
			values.pop();
		
		}
				
		stat += stream.str();
		
		return pass;
	
	}
	
	bool TestSerializationBuffer::testExternalBuffer( std::string& stat )
	{
	
		std::stringstream stream;
	
		mersenneTwister random(seed);
	
		common::SerializationBuffer buffer;
	
		bool pass;
		
		pass = true;
		
		char* myBuffer;
		
		unsigned int size;
		
		size = random() % MAX_ELEMENT_BYTES;
		
		std::queue< char > reference;
		
		myBuffer = new char[ size ];
		
		for( unsigned int i = 0; i < size; i++ )
		{
		
			char value;
			
			value = random();
			
			myBuffer[i] = value;
			
			reference.push( value );
		
		}
		
		buffer.ptr( myBuffer, size );
		
		for( unsigned int i = 0; i < size; i++ )
		{
		
			char value;
			
			buffer.read( &value, 1 );
			
			unsigned int val;
			unsigned int ref;
			
			val = value;
			ref = reference.front();
			
			if( value != reference.front() )
			{
			
				pass = false;
				
				stream << "Read value " << val << " does not match reference " << ref << " at index " << i << ".\n";
			
			}
			
			reference.pop();
					
		}
		
		if( !buffer.empty() )
		{
		
			pass = false;
			
			stream << "Buffer should have been empty after reading out individual elements, it had " << buffer.size() <<" elements.\n";
		
		}
		
		stat += stream.str();
		
		return pass;
	
	}
	
	bool TestSerializationBuffer::testStrings( std::string& stat )
	{
	
		std::stringstream stream;
	
		mersenneTwister random(seed);
	
		common::SerializationBuffer buffer;
	
		bool pass;
		
		pass = true;
		
		unsigned int size;
		
		size = random() % MAX_ELEMENT_BYTES;
		
		std::string input;
		std::string output;
		
		for( unsigned int i = 0; i < size; i++ )
		{
		
			char value;
			
			value = random();
			
			stream << (unsigned int)value;
		
		}
		
		input = stream.str();
		
		buffer.write( input );
		
		buffer.read( output );
		
		stream.str("");
		
		if( input != output )
		{
		
			stream << "Input string " << input << " != output string " << output << ".\n";
			
			pass = false;
		
		}
		else
		{
		
			stream << "Input string " << input << " == output string " << output << ".\n";
		
		}
		
		stat += stream.str();
		
		return pass;	
	
	}
	
	void TestSerializationBuffer::performanceTest( std::string& stat )
	{
	
		std::stringstream stream;
		
		common::SerializationBuffer buffer;
		
		char* staticBuffer;
		
		staticBuffer = new char[ maxSize ];		
		
		common::Timer Timer;
		
		mersenneTwister random;
		
		// test writes
		Timer.start();
		
		for( unsigned int i = 0; i < iterations; i++ )
		{
		
			buffer.write( staticBuffer, maxSize );
		
		}
		
		Timer.stop();
		
		stream << "Write of " << iterations << " buffers of " << maxSize << " took " << Timer.seconds() << " seconds ( " << Timer.cycles() << " ).\n";
		
		// test reads
		Timer.start();
		
		for( unsigned int i = 0; i < iterations; i++ )
		{
		
			buffer.read( staticBuffer, maxSize );
		
		}
		
		Timer.stop();
		
		stream << "Read of " << iterations << " buffers of " << maxSize << " took " << Timer.seconds() << " seconds ( " << Timer.cycles() << " ).\n";
		
		Timer.start();
		
		unsigned int reads;
		unsigned int writes;
		
		reads = 0;
		writes = 0;
		
		// test rw
		for( unsigned int i = 0; i < iterations; i++ )
		{
		
			unsigned int randomValue;
		
			bool read;
			
			randomValue = random() % maxSize;
			
			read = randomValue & 0x1;
			
			if( read && ( buffer.size() > randomValue ) )
			{
			
				buffer.read( staticBuffer, randomValue );
				++reads;
			
			}
			else
			{
			
				buffer.write( staticBuffer, randomValue );
				++writes;
			
			}
		
		}
		
		Timer.stop();
		
		stream << "Read of " << reads << " buffers and writes of " << writes << " buffers took " << Timer.seconds() << " seconds ( " << Timer.cycles() << " ).\n";
		
		delete[] staticBuffer;
		
		stat += stream.str();	
	
	}

	bool TestSerializationBuffer::doTest( std::string& stat )
	{
	
		bool pass;
		
		pass = true;
		
		pass &= testReadWrite( stat );
		pass &= testExternalBuffer( stat );
		pass &= testStrings( stat );
		performanceTest( stat );
		
		return pass;
	
	}

	TestSerializationBuffer::TestSerializationBuffer()
	{
	
		Name = "TestSerializationBuffer";
		
		Description = "A test for the serialization buffer.\n\n";
		Description += "Write in some values\n";
		Description += "then read them back and make sure they match.\n\n";
		Description += "Take in an external buffer and read some values out.\n\n";
		Description += "Write in some strings then read them out.";
	
	}
			
}

int main( int argc, char** argv )
{

	common::ArgumentParser parser( argc, argv );
	
	test::TestSerializationBuffer test;
	
	parser.description( test.description() );
	
	bool help;
	
	parser.parse( "-h", help, "Print this help message." );
	
	parser.parse( "-i", test.iterations, ITERATIONS, "How many iterations of random numbers to try serializing." );
	parser.parse( "-m", test.maxSize, MAX_ELEMENT_BYTES, "Max number of bytes to serialize for each iteration." );
	parser.parse( "-s", test.seed, DEFAULT_SEED, "Random seed for repeatability." );
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

