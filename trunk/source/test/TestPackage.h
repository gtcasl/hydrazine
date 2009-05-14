/*!

	\file TestPackage.h
	
	\date Friday Novermber 7, 2008
	
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	
	\brief The header file for the TestPackage class

*/

#ifndef TEST_PACKAGE_H_INCLUDED
#define TEST_PACKAGE_H_INCLUDED

#include <common/implementation/Package.h>
#include <common/interface/Test.h>
#include <unordered_set>
#include <vector>

namespace test
{

	/*!
	
		\brief A test for the Package class.
		
		TestPoints:
		
			1) Add some variables, copy the package, remove the variables, 
				make sure that they match.
		
			2) Randomly add, remove, reserve, and compress the package.  Finally
				remove all variables and make sure that they match.
	
	*/
	class TestPackage : public Test
	{
	
		private:
				
			class Packable : public common::Packable
			{
			
				private:
				
					uint8_t* _begin;
					uint8_t* _end;
				
				public:
			
					Packable();
					Packable( const Packable& packable );
					~Packable();

					const Packable& operator=( const Packable& packable ); 
					
					void resize( unsigned int bytes );
					
					unsigned int bytes() const;
					unsigned int size() const;

					uint8_t& operator[]( unsigned int index );
					void write( uint8_t* data, unsigned int size );

					bool operator==( const Packable& packable ) const;

					void pack( void* package );
					void unpack( void* package );
					
					unsigned int sum() const;
			
			};

			typedef std::vector< Packable > PackageVector;
			typedef std::unordered_set< unsigned int > PackableSet;
	
		private:
		
			bool _testCopy( std::string& status );
			bool _testComplex( std::string& status );

			bool doTest( std::string& status );
	
		public:
	
			TestPackage();
			
			unsigned int reads;
			unsigned int writes;
			unsigned int averageSize;
			unsigned int objects;
			unsigned int adds;
	
	};


}

int main( int argc, char** argv );

#endif

