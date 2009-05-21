/*!
	\file TestBTree.cpp
	\date Monday May 18, 2009
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	\brief The source file for the TestBTree class.
*/

#ifndef TEST_B_TREE_CPP_INCLUDED
#define TEST_B_TREE_CPP_INCLUDED

#include <test/TestBTree.h>
#include <implementation/Exception.h>
#include <implementation/ArgumentParser.h>
#include <fstream>

namespace test
{

	void TestBTree::_init( Vector& v )
	{
		for( Vector::iterator fi = v.begin(); fi != v.end(); ++fi )
		{
			*fi = random() % elements;
		}
	}

	bool TestBTree::testRandom()
	{
		typedef std::pair< Map::iterator, bool > MapInsertion;
		typedef std::pair< Tree::iterator, bool > TreeInsertion;

		status << "Running Test Random\n";

		Map map;
		Tree tree;
		Vector vector( elements );
		_init( vector );
		
		for( unsigned int i = 0; i < iterations; ++i )
		{
			switch( random() % 3 )
			{
				case 0:
				{
					size_t index = random() % vector.size();
					MapInsertion mapInsertion = map.insert( 
						std::make_pair( vector[ index ], i ) );
					TreeInsertion treeInsertion = tree.insert( 
						std::make_pair( vector[ index ], i ) );
					if( mapInsertion.second != treeInsertion.second )
					{
						status << "At index " << i << std::boolalpha 
							<< " map insertion " 
							<< mapInsertion.second 
							<< " did not match tree insertion " 
							<< treeInsertion.second << "\n";
						std::stringstream stream;
						stream << path << "/tree_random.dot";
						std::ofstream out( stream.str().c_str() );
						if( !out.is_open() )
						{
							throw hydrazine::Exception( 
								"Could not open file " + stream.str() );
						}
						tree.toGraphViz( out );
						return false;
					}
					
					if( mapInsertion.first->first 
						!= treeInsertion.first->first )
					{
						status << "At index " << i << " map key " 
							<< mapInsertion.first->first 
							<< " did not match tree key " 
							<< treeInsertion.first->first << "\n";
						std::stringstream stream;
						stream << path << "/tree_random.dot";
						std::ofstream out( stream.str().c_str() );
						if( !out.is_open() )
						{
							throw hydrazine::Exception( 
								"Could not open file " + stream.str() );
						}
						tree.toGraphViz( out );
						return false;
					}

					if( mapInsertion.first->second 
						!= treeInsertion.first->second )
					{
						status << "At index " << i << " map value " 
							<< mapInsertion.first->second 
							<< " did not match tree value " 
							<< treeInsertion.first->second << "\n";
						std::stringstream stream;
						stream << path << "/tree_random.dot";
						std::ofstream out( stream.str().c_str() );
						if( !out.is_open() )
						{
							throw hydrazine::Exception( 
								"Could not open file " + stream.str() );
						}
						tree.toGraphViz( out );
						return false;
					}
					
					break;
				}
				
				case 1:
				{
					size_t index = random() % vector.size();
					Map::const_iterator mi = map.find( vector[ index ] );
					Tree::const_iterator ti = tree.find( vector[ index ] );
					bool mapEnd = mi == map.end();
					bool treeEnd = ti == tree.end();
										
					if( mapEnd || treeEnd )
					{
						if( mapEnd != treeEnd )
						{
							status << "At index " << i << std::boolalpha 
								<< " for key " << vector[ index ] << " map end "
								<< mapEnd << " did not match tree end " 
								<< treeEnd << "\n";
							std::stringstream stream;
							stream << path << "/tree_random.dot";
							std::ofstream out( stream.str().c_str() );
							if( !out.is_open() )
							{
								throw hydrazine::Exception( 
									"Could not open file " + stream.str() );
							}
							tree.toGraphViz( out );
							return false;						
						}
					}
					else
					{
						if( mi->first != ti->first )
						{
							status << "At index " << i << " map key " 
								<< mi->first << " did not match tree key " 
								<< ti->first << "\n";
							std::stringstream stream;
							stream << path << "/tree_random.dot";
							std::ofstream out( stream.str().c_str() );
							if( !out.is_open() )
							{
								throw hydrazine::Exception( 
									"Could not open file " + stream.str() );
							}
							tree.toGraphViz( out );
							return false;
						}

						if( mi->second != ti->second )
						{
							status << "At index " << i << " map value " 
								<< mi->second << " did not match tree value " 
								<< ti->second << "\n";
							std::stringstream stream;
							stream << path << "/tree_random.dot";
							std::ofstream out( stream.str().c_str() );
							if( !out.is_open() )
							{
								throw hydrazine::Exception( 
									"Could not open file " + stream.str() );
							}
							tree.toGraphViz( out );
							return false;
						}
					}
				}
				
				case 2:
				{
					size_t index = random() % vector.size();
					Map::iterator mi = map.find( vector[ index ] );
					Tree::iterator ti = tree.find( vector[ index ] );
					bool mapEnd = mi == map.end();
					bool treeEnd = ti == tree.end();
										
					if( mapEnd || treeEnd )
					{
						if( mapEnd != treeEnd )
						{
							status << "At index " << i << std::boolalpha 
								<< " for key " << vector[ index ] << " map end "
								<< mapEnd << " did not match tree end " 
								<< treeEnd << "\n";
							std::stringstream stream;
							stream << path << "/tree_random.dot";
							std::ofstream out( stream.str().c_str() );
							if( !out.is_open() )
							{
								throw hydrazine::Exception( 
									"Could not open file " + stream.str() );
							}
							tree.toGraphViz( out );
							return false;	
						}
					}
					else
					{
						map.erase( mi );
						tree.erase( ti );
					}
				}
			}
		}
		
		status << "Test Random Passed\n";
		return true;
	}

	bool TestBTree::testClear()
	{
		return false;
	}
	
	bool TestBTree::testComparisons()
	{
		return false;
	}
	
	void TestBTree::doBenchmark()
	{
		Tree tree;
		Vector vector( elements );
		_init( vector );
				
		for( Vector::iterator fi = vector.begin(); fi != vector.end(); ++fi )
		{
			std::pair< Tree::iterator, bool > insertion 
				= tree.insert( std::make_pair( *fi, fi - vector.begin() ) );
			if( insertion.second )
			{
				std::stringstream stream;
				stream << path << "/tree_" << (fi - vector.begin()) << ".dot";
				std::ofstream out( stream.str().c_str() );
				if( !out.is_open() )
				{
					throw hydrazine::Exception( "Could not open file " 
						+ stream.str() );
				}
				tree.toGraphViz( out );
			}
		}
		
		for( Vector::iterator fi = vector.begin(); fi != vector.end(); ++fi )
		{
			Tree::iterator ti = tree.find( *fi );
			if( ti != tree.end() )
			{
				tree.erase( ti );
				std::stringstream stream;
				stream << path << "/tree_" << (fi - vector.begin() + elements) 
					<< ".dot";
				std::ofstream out( stream.str().c_str() );
				if( !out.is_open() )
				{
					throw hydrazine::Exception( "Could not open file " 
						+ stream.str() );
				}
				tree.toGraphViz( out );
			}
		}	
	}

	bool TestBTree::doTest()
	{
		if( benchmark )
		{
			doBenchmark();
			return true;
		}
		else
		{
			return testRandom() && testClear() && testComparisons();
		}
	}

	TestBTree::TestBTree()
	{
		name = "TestBTree";
		description = "A unit test for the BTree mapping data structure. ";
		description += "Test Points: 1) Randomly insert and remove elements ";
		description += "from a std::map and a BTree	assert that the final ";
		description += "versions have the exact same elements stored in the ";
		description += "same order. 2) Add elements and then clear the BTree. ";
		description += " Assert that there are no elements after the clear and";
		description += " that the correct number is reported by size after ";
		description += "each insertion. 3) Test each of the comparison ";
		description += "operators. 4) Do not run any tests, simply add a ";
		description += "sequence to the tree and write it out to graph viz ";
		description += "files after each operaton.";
	}

}

int main( int argc, char** argv )
{
	hydrazine::ArgumentParser parser( argc, argv );
	test::TestBTree test;
	parser.description( test.testDescription() );

	parser.parse( "-s", "--seed", test.seed, 0, 
		"Seed for random tests, 0 implies seed with time." );
	parser.parse( "-v", "--verbose", test.verbose, false, 
		"Print out info after the test." );
	parser.parse( "-b", "--benchmark", test.benchmark, false,
		"Rather than testing, print out the tree after several changes." );
	parser.parse( "-e", "--elements", test.elements, 10,
		"The number of elements to add to the tree." );
	parser.parse( "-i", "--iterations", test.iterations, 1000,
		"The number of iterations to perform tests." );
	parser.parse( "-p", "--path", test.path, "temp",
		"Relative path to store graphviz representations of the tree." );
	parser.parse();
	
	test.test();
	
	return test.passed();
}

#endif

