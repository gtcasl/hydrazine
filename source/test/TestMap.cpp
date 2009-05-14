/*!

	\file TestMap.cpp
	
	\date Sunday February 8, 2009
	
	\author Gregory Diamos <gregory.diamos@gatech.edu>

	\brief The source file for the TestMap class.

*/

#ifndef TEST_MAP_CPP_INCLUDED
#define TEST_MAP_CPP_INCLUDED

#include <common/test/TestMap.h>
#include <common/implementation/ArgumentParser.h>
#include <vector>

#include <common/implementation/debug.h>

#ifdef REPORT_BASE
#undef REPORT_BASE
#endif

#define REPORT_BASE 1

namespace test
{

	bool TestMap::doTest( std::string& status )
	{
	
		typedef std::map< unsigned int, unsigned int > StlMap;
		typedef common::map< unsigned int, unsigned int > CommonMap;
		typedef std::pair< StlMap::iterator, bool > StlInsertion;
		typedef std::pair< CommonMap::iterator, bool > CommonInsertion;
		typedef std::vector< std::pair< unsigned int, unsigned int > > Vector;
		
		StlMap stl;
		CommonMap map;
		Vector vector( elements );
		
		for( unsigned int i = 0; i < elements; ++i )
		{
		
			unsigned int key = random();
			unsigned int value = random();
		
			StlInsertion stlInsertion = stl.insert( 
				std::make_pair( key, value ) );
			CommonInsertion commonInsertion = map.insert( 
				std::make_pair( key, value ) );
			
			if( random() & 0x1 )
			{
			
				stlInsertion = stl.insert( 
					std::make_pair( key, value ) );
				commonInsertion = map.insert( 
					std::make_pair( key, value ) );
			
			}
			
			if( stlInsertion.second )
			{
			
				vector[ i ] = std::make_pair( key, value );
			
			}
			else
			{
			
				vector[ i ].first = elements;
			
			}
		
			if( stlInsertion.second != commonInsertion.second )
			{
			
				std::stringstream stream;
				stream << "For key " << key << " stl insertion validity " 
					<< stlInsertion.second << " does not match common " 
					<< commonInsertion.second << "\n";
				status += stream.str();
				return false;
			
			}

			if( stlInsertion.first->first != commonInsertion.first->first )
			{
			
				std::stringstream stream;
				stream << "For key " << key << " stl insertion key " 
					<< stlInsertion.first->first << " does not match common " 
					<< commonInsertion.first->first << "\n";
				status += stream.str();
				return false;
			
			}

			if( stlInsertion.first->second != commonInsertion.first->second )
			{
			
				std::stringstream stream;
				stream << "For key " << key << " stl insertion value " 
					<< stlInsertion.first->second << " does not match common " 
					<< commonInsertion.first->second << "\n";
				status += stream.str();
				return false;
			
			}
		
		}

		for( unsigned int i = 0; i < elements; ++i )
		{
		
			if( random() & 0x1 )
			{
			
				if( vector[i].first != elements )
				{
				
					if( random() & 0x1 )
					{
					
						StlMap::iterator si = stl.find( vector[i].first );
						assert( si != stl.end() );
						
						CommonMap::iterator ci = map.find( vector[i].first );
						
						if( ci == map.end() )
						{
						
							std::stringstream stream;
							stream << "For key " << vector[i].first 
								<< " could not find key in common map.\n";
							status += stream.str();
							return false;
			
						}
						
						if( si->first != ci->first )
						{
			
							std::stringstream stream;
							stream << "For key " << vector[i].first 
								<< " stl lookup key " 
								<< si->first << " does not match common " 
								<< ci->first << "\n";
							status += stream.str();
							return false;
			
						}

						if( si->second != ci->second )
						{
			
							std::stringstream stream;
							stream << "For key " << vector[i].first 
								<< " stl lookup value " 
								<< si->second << " does not match common " 
								<< ci->second << "\n";
							status += stream.str();
							return false;
			
						}
						
					
					}
				
				}

				unsigned int stlCount = stl.erase( vector[i].first );
				unsigned int mapCount = map.erase( vector[i].first );
			
				if( stlCount != mapCount )
				{

					std::stringstream stream;
					stream << "For key " << vector[i].first 
						<< " stl erased count " 
						<< stlCount << " does not match common " 
						<< mapCount << "\n";
					status += stream.str();
					return false;

				}
			
			}
		
		}

		StlMap::iterator si = stl.begin();
		CommonMap::iterator ci = map.begin();
	
		for( ; si != stl.end(); ++si, ++ci )
		{
		
			if( ci == map.end() )
			{
			
				std::stringstream stream;
				stream << "For iteration key " << si->first 
					<< " could not find key in common map.\n";
				status += stream.str();
				return false;

			}
			
			if( si->first != ci->first )
			{

				std::stringstream stream;
				stream << "For iteration stl lookup key " 
					<< si->first << " does not match common " 
					<< ci->first << "\n";
				status += stream.str();
				return false;

			}

			if( si->second != ci->second )
			{

				std::stringstream stream;
				stream << "For iteration key " << si->first 
					<< " stl lookup value " 
					<< si->second << " does not match common " 
					<< ci->second << "\n";
				status += stream.str();
				return false;

			}
		
		}
		
		return true;
	
	}
	
	TestMap::TestMap()
	{
	
		Name = "TestMap";
		
		Description = "A test for the alternate map implementation. Test ";
		Description += "Points: 1) Insert and remove elements into/from the ";
		Description += "stl map and the alternate map, make sure that they ";
		Description += "match via iteration.";
	
	}

}

int main( int argc, char** argv )
{

	common::ArgumentParser parser( argc, argv );
	test::TestMap test;
	
	parser.description( test.description() );
	
	bool help;
	bool verbose;
	
	parser.parse( "-h", help, "Display this help message." );
	parser.parse( "-s", test.seed, 0, 
		"Random seed. 0 implies seed with time." );
	parser.parse( "-v", verbose, 
		"Print out info after the test has finished." );
	
	parser.parse( "-e", test.elements, 10000, 
		"The number of elements to insert in the map." );

	if( help )
	{
	
		std::cout << parser.help();
		
		return 2;
	
	}
	
	test.test();
	
	if( verbose )
	{
	
		std::cout << test.toString();
	
	}
	
	return test.passed();

}

#endif

