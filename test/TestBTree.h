/*!
	\file TestBTree.h
	\date Monday May 18, 2009
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	\brief The header file for the TestBTree class.
*/

#ifndef TEST_B_TREE_H_INCLUDED
#define TEST_B_TREE_H_INCLUDED

#include <interface/Test.h>

#define PAGE_SIZE 4

namespace test
{

	/*!
		\brief A unit test for the BTree mapping data structure.
		
		Test Points:
		
			1) Randomly insert and remove elements from a std::map and a BTree
				assert that the final versions have the exact same elements
				stored in the same order.
				
			2) Add elements and then clear the BTree.  Assert that there are
				no elements after the clear and that the correct number is 
				reported by size after each insertion.
				
			3) Test each of the comparison operators.
			
			4) Do not run any tests, simply add a sequence to the tree and 
				write it out to graph viz files after each operaton.

	*/
	class TestBTree : public Test
	{
		private:
			bool testRandom();
			bool testClear();
			bool testComparisons();
			void doBenchmark();
			bool doTest();
		
		public:
			bool benchmark;
			unsigned int elements;
			std::string path;
		
		public:
			TestBTree();
		
	};

}

int main( int argc, char** argv );

#endif

