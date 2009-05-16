/*!

	\file TestPackage.cpp
	
	\date Friday Novermber 7, 2008
	
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	
	\brief The source file for the TestPackage class

*/

#ifndef TEST_PACKAGE_CPP_INCLUDED
#define TEST_PACKAGE_CPP_INCLUDED

#include "TestPackage.h"
#include <implementation/ArgumentParser.h>
#include <implementation/macros.h>
#include <implementation/debug.h>

#ifdef REPORT_BASE
#undef REPORT_BASE
#endif

#define REPORT_BASE 0

namespace test
{

	////////////////////////////////////////////////////////////////////////////
	// TestPackage::Packable
	TestPackage::Packable::Packable()
	{
		_begin = 0;
		_end = 0;
	}
	
	TestPackage::Packable::Packable( const Packable& packable )
	{
		if( packable.size() > 0 )
		{
			_begin = new uint8_t[ packable.size() ];
			_end = _begin + packable.size();
		}
		else
		{
			_begin = 0;
			_end = 0;
		}
	}

	TestPackage::Packable::~Packable()
	{
		if( _begin != _end )
		{
			delete[] _begin;
		}
	}

	const TestPackage::Packable& 
		TestPackage::Packable::operator=( const Packable& packable )
	{
		assert( &packable != this );
		resize( 0 );

		if( packable.size() > 0 )
		{
			_begin = new uint8_t[ packable.size() ];
			_end = _begin + packable.size();
			memcpy( _begin, packable._begin, packable.size() );
		}
		else
		{
			_begin = 0;
			_end = 0;
		}
		
		return *this;
	}
	
	void TestPackage::Packable::resize( unsigned int bytes )
	{
		if( size() != bytes )
		{
			if( bytes > 0 )
			{
				report( "Packable: Resizing packable " << this << " from " 
					<< size() << " to " << bytes );
				
				unsigned int copySize = MIN( bytes, size() );
				uint8_t* newBegin = new uint8_t[ bytes ];
				
				if( copySize > 0 )
				{
					memcpy( newBegin, _begin, copySize );
					delete[] _begin;
				}
				
				_begin = newBegin;
				_end = _begin + bytes;
			}
			else
			{
				delete[] _begin;
				_begin = 0;
				_end = 0;
			}
		}
	}
	
	unsigned int TestPackage::Packable::bytes() const
	{
		return size() + sizeof( unsigned int );
	}

	unsigned int TestPackage::Packable::size() const
	{
		return _end - _begin;
	}	
	
	uint8_t& TestPackage::Packable::operator[]( unsigned int index )
	{
		assert( index < size() );
		return _begin[ index ];
	}
	
	void TestPackage::Packable::write( uint8_t* data, unsigned int size )
	{
		assert( size <= this->size() );
		memcpy( _begin, data, size );
	}

	bool TestPackage::Packable::operator==( const Packable& packable ) const
	{
		if( packable.size() != size() )
		{
			return false;
		}
		
		return memcmp( packable._begin, _begin, size() ) == 0;
	}
	
	void TestPackage::Packable::pack( void* _package )
	{
		uint8_t* package = static_cast< uint8_t* >( _package );
		TestPackage::Packable* pointer = 
			static_cast< TestPackage::Packable* >( _package );
		uint8_t* data = static_cast< uint8_t* >( package + 
			sizeof( unsigned int ) );
		
		unsigned int size = this->size();
		
		memcpy( pointer, &size, sizeof( unsigned int ) );
		memcpy( data, _begin, size );
	}
	
	void TestPackage::Packable::unpack( void* _package )
	{
		uint8_t* package = static_cast< uint8_t* >( _package );	
		TestPackage::Packable* pointer = 
			static_cast< TestPackage::Packable* >( _package );
		uint8_t* data = static_cast< uint8_t* >( package + 
			sizeof( unsigned int ) );
		
		report( "Unpacking from " << pointer );

		unsigned int size;
		
		memcpy( &size, pointer, sizeof( unsigned int ) );
		resize( size );
		memcpy( _begin, data, size );
	}
	
	unsigned int TestPackage::Packable::sum() const
	{
		unsigned int result = 0;
	
		for( uint8_t* i = _begin; i != _end; ++i )
		{
			result += *i;
		}
		
		return result;
	}
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// TestPackage
	bool TestPackage::_testCopy()
	{
		unsigned int objects = MAX( 1, random() % this->objects );
		unsigned int adds = MAX( 1, random() % this->adds );
	
		PackageVector packables;
		hydrazine::Package package;
		
		packables.resize( objects );
		unsigned int totalSize = 0;
		unsigned int count = 0;
		
		for( PackageVector::iterator fi = packables.begin(); 
			fi != packables.end(); ++fi, ++count )
		{
			unsigned int size = MAX( 1, random() % ( averageSize * 2 ) );
		
			fi->resize( size );
			totalSize += fi->bytes();
			
			for( unsigned int i = 0; i < size; ++i )
			{
				(*fi)[ i ] = random();
			}
			
			report( " Creating object " << count 
				<< " with value " << fi->sum() );
		}
		
		adds = MIN( adds, objects );
		package.reserve( totalSize );
		
		for( unsigned int i = 0; i < adds; ++i )
		{
			package.insert( packables[ i ] );
		}
		
		hydrazine::Package copy = package;

		PackageVector result;
		result.resize( objects );
		
		for( unsigned int i = 0; i < adds; ++i )
		{
			result[i].unpack( package.find( &packables[i] ) );
			report( " Unpacked object " << i 
				<< " with value " << result[i].sum() );
		}
		
		for( unsigned int i = 0; i < adds; ++i )
		{
			if( !( result[ i ] == packables[ i ] )  )
			{
				status << "Object " << i << " with value " << result[i].sum() 
					<< " did not match reference " << packables[i].sum() 
					<< " .\n";
				status << "Test Point 1 (AddRemove): Failed.\n";
				return false;
			}
		}
		
		status << "Test Point 1 (AddRemove): Passed.\n";
		return true;
	}

	bool TestPackage::_testComplex()
	{
		unsigned int reads = MAX( 1, random() % this->reads );
		unsigned int writes = MAX( 1, random() % this->writes );
		unsigned int objects = MAX( 1, random() % this->objects );
		unsigned int adds = MAX( 1, random() % this->adds );

		PackageVector packables;
		hydrazine::Package package;
		PackableSet set;

		packables.resize( objects );		
		adds = MIN( adds, objects );
		unsigned int totalSize = 0;
		unsigned int count = 0;
		
		for( PackageVector::iterator fi = packables.begin(); 
			fi != packables.end(); ++fi, ++count )
		{
			unsigned int size = MAX( 1, random() % ( averageSize * 2 ) );
		
			fi->resize( size );
			totalSize += fi->bytes();
			
			for( unsigned int i = 0; i < size; ++i )
			{
				(*fi)[ i ] = random();
			}
		}
		
		unsigned int _readCount = 0;
		unsigned int _writeCount = 0;
		unsigned int _addCount = 0;
		
		while
		( 
			_readCount < reads || 
			_writeCount < writes ||
			_addCount < adds ||
			!set.empty()
		)
		{
			unsigned int operation = random() % 6;
		
			report( "Reads " << _readCount << ", writes " << _writeCount 
				<< ", adds " << _addCount << ", set size " << set.size() );
		
			switch( operation )
			{
				// read
				case 0:
				{
					if( _readCount < reads )
					{
						unsigned int object = random() % objects;
						PackableSet::iterator pi = set.find( object );
						
						if( pi != set.end() )
						{
							Packable packable;
							packable.unpack( package.find( 
								&packables[object] ) );
												
							if( !( packable == packables[ object ] ) )
							{
								status << "Object " << object << " with value " 
									<< packable.sum() 
									<< " did not match reference " 
									<< packables[object].sum() 
									<< " .\n";
								status << "Test Point 2 (Complex): Failed.\n";
								return false;
							}
							
							++_readCount;
							break;
						}
					}
				}
				
				// write
				case 1:
				{
					if( _writeCount < writes )
					{
						unsigned int object = random() % objects;
						PackableSet::iterator pi = set.find( object );
						
						if( pi != set.end() )
						{
							Packable packable;
							packable.unpack( package.find( 
								&packables[ object ] ) );

							for( unsigned int i = 0; i < packable.size(); ++i )
							{
								packable[i] = random();
								packables[object][i] = packable[i];
							}
							
							packable.pack( package.find( 
								&packables[object] ) );
						}
						else
						{						
							for( unsigned int i = 0; 
								i < packables[object].size(); ++i )
							{
								packables[object][i] = random();
							}
						}
						
						++_writeCount;
						break;
					}
				}
				
				// add
				case 2:
				{
					if( _addCount < adds )
					{
						unsigned int object = random() % objects;
						PackableSet::iterator pi = 
							set.find( object );
						
						if( pi == set.end() )
						{
							set.insert( object );
							package.insert( packables[ object ] );
							++_addCount;
						}
						break;
					}
				}
				
				// remove
				case 3:
				{
					if( !set.empty() )
					{
						if( !( set.size() == 1 && ( _writeCount < writes 
							|| _readCount < reads ) ) )
						{
							unsigned int object = *set.begin();
							set.erase( set.begin() );
					
							Packable _packable;
							Packable* pointer = reinterpret_cast< Packable* >( 
								package.find( &packables[ object ] ) );

							assert( pointer != 0 );
							_packable.unpack( pointer );

							if( !( _packable == packables[ object ] ) )
							{
								status << "Object " << object << " with value " 
									<< _packable.sum() 
									<< " did not match reference " 
									<< packables[object].sum() 
									<< " .\n";
								status << "Test Point 2 (Complex): Failed.\n";
								return false;
							}
					
							package.remove( &packables[ object ] );							
							break;
						}
					}
				}
				
				// compress
				case 4:
				{
					package.compress();
					break;
				}
				
				// reserve
				default:
				{
					package.reserve( totalSize );
					break;
				}
			}
		}

		status << "Test Point 2 (Complex): Passed.\n";
		return true;
	}

	bool TestPackage::doTest()
	{
	
		report( "Test Point 1 (Add Remove)" );
		if( !_testCopy( ) )
		{
		
			return false;
		
		}
		
		report( "Test Point 2 (Complex)" );
		if( !_testComplex( ) )
		{
		
			return false;
		
		}
		
		return true;
	
	}

	TestPackage::TestPackage()
	{
	
		name = "TestPackage";
		
		description = "A test for the Package class. TestPoints: 1) Add some";
		description += " variables, copy the package, remove the variables, ";
		description += "make sure that they match. 2) Randomly add, remove, ";
		description += "reserve, and compress the package.  Finally remove ";
		description += "all variables and make sure that they match.";
	
	}
	////////////////////////////////////////////////////////////////////////////

}

int main( int argc, char** argv )
{

	hydrazine::ArgumentParser parser( argc, argv );
	test::TestPackage test;
	parser.description( test.testDescription() );

	parser.parse( "-v", test.verbose, false,
		"Print out info after the test." );

	parser.parse( "-r", test.reads, 100, "Number of writes to do." );
	parser.parse( "-w", test.writes, 100, "Number of reads to do." );
	parser.parse( "-b", test.averageSize, 1024, 
		"Average object size in bytes." );
	parser.parse( "-n", test.objects, 20, "Number of objects to create." );
	parser.parse( "-a", test.adds, 40, "How many times to add objects." );
	parser.parse();

	assert( test.reads > 0 );
	assert( test.writes > 0 );
	assert( test.averageSize > 0 );
	assert( test.objects > 0 );
	assert( test.adds > 0 );

	test.test();

	return test.passed();

}

#endif

