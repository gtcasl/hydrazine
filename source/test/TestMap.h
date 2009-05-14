/*!

	\file TestMap.h
	
	\date Sunday February 8, 2009
	
	\author Gregory Diamos <gregory.diamos@gatech.edu>

	\brief The header file for the TestMap class.

*/

#ifndef TEST_MAP_H_INCLUDED
#define TEST_MAP_H_INCLUDED

#include <common/implementation/map.h>
#include <map>
#include <common/interface/Test.h>

namespace test
{

	/*!
	
		\brief A test for the alternate map implementation.
		
		Test Points:
			
			1) Insert and remove elements into/from the stl map and the 
				alternate map, make sure that they match via iteration.
	
	*/
	class TestMap : public Test
	{
	
		private:
		
			bool doTest( std::string& );
	
		public:
		
			TestMap();
	
		public:
		
			unsigned int elements;
	
	};

}

int main( int argc, char** argv );

#endif

