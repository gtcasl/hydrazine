/*!

	\file TestXmlArgumentParser.cpp
	
	\date Wednesday September 17, 2008
	
	\author Gregory Diamos <gregory.diamos@gatech.edu>

	\brief The header file for the TestXmlArgumentParser class.

*/

#ifndef TEST_XML_ARGUMENT_PARSER_CPP_INCLUDED
#define TEST_XML_ARGUMENT_PARSER_CPP_INCLUDED

#include "TestXmlArgumentParser.h"
#include <fstream>
#include <common/implementation/XmlArgumentParser.h>
#include <common/implementation/ArgumentParser.h>
#include <common/implementation/macros.h>

#ifdef REPORT_BASE
#undef REPORT_BASE 
#endif

#define REPORT_BASE 0

namespace test
{

	void TestXmlArgumentParser::initializeTree( common::XmlTree& tree, 
		std::list< TestXmlArgumentParser::Path >& paths ) const
	{

		unsigned int n = 0;
		common::XmlTree::iterator fi;
		unsigned int totalLevels = 0;
		Path path;
				
		while( n < nodes )
		{
		
			unsigned int levels = rand() % maxLevels;
			fi = tree.begin();
			
			for( unsigned int l = 0; l < levels; ++l )
			{
			
				std::stringstream name;
				name << "level_" << totalLevels << "_" << l;
				fi = tree.insert( name.str(), fi, 
					common::XmlTree::XmlTreeNode::Intermediate );
				path.path.push_back( name.str() );
			
				if( rand() & 0x1 )
				{				
				
					std::stringstream nodeName;
					nodeName << rand();
					tree.insert( nodeName.str(), fi, 
						common::XmlTree::XmlTreeNode::Leaf );
					++n;
					path.value = nodeName.str();
					paths.push_back( path );
				
				}				
					
				if( n >= nodes )
				{
				
					break;
				
				}
			
			}
			
			path.path.clear();
			++totalLevels;
		
		}
	
	
	}
		
	bool TestXmlArgumentParser::parseTree( 
		common::XmlTree& reference, 
		std::list< TestXmlArgumentParser::Path >& paths,
		std::string& status  ) const
	{
	
		std::ofstream file( tempFile.c_str() );
		
		assert( file.is_open() );
		reference.toStream( file );
		file.close();
		
		common::XmlArgumentParser parser( tempFile );
		
		if( !dontDelete )
		{
		
			std::remove( tempFile.c_str() );
		
		}		

		for( std::list< TestXmlArgumentParser::Path >::iterator 
			fi = paths.begin(); fi != paths.end(); ++fi )
		{
		
			for( std::list< std::string >::iterator si = fi->path.begin();
				si != fi->path.end(); ++si )
			{
				report( "Searching for node " << *si );
				parser.descend( *si );
			
			}
			
			unsigned int parsedValue;
			parser.parse( parsedValue );
			std::stringstream stream;
			stream << parsedValue;
			
			if( stream.str() != fi->value )
			{
				
				std::stringstream stream1;
				stream1 << "At path ";
				
				for( std::list< std::string >::iterator si = fi->path.begin();
					si != fi->path.end(); ++si )
				{
			
					stream1 << *si << "->";
			
				}
				
				stream1 << fi->value << ", parsed incorrect value " 
					<< stream.str();
				status += stream1.str();
				return false;

			}
			
			parser.reset();
			
		}
		
		status += "Test Passed.";
		return true;
	
	}

	bool TestXmlArgumentParser::doTest( std::string& status )
	{
	
		common::XmlTree tree;
		std::list< Path > paths;
		
		initializeTree( tree, paths );
		return parseTree( tree, paths, status );
	
	}

	TestXmlArgumentParser::TestXmlArgumentParser()
	{
	
		Name = "TestXmlArgumentParser";
		
		Description = "Creates a random XML tree, writes to a temp file,";
		Description += " parses it with the XmlArgumentParser, then makes sure";
		Description += " that all parsed values match their recorded values.";
	
	}

}

int main( int argc, char** argv )
{

	common::ArgumentParser parser( argc, argv );

	test::TestXmlArgumentParser test;
	bool help;
	
	parser.description( test.description() );
	parser.parse( "-h", help, "Display this help message." );
	parser.parse( "-v", test.verbose, "Print out information after the test is over." );
	parser.parse( "-sv", test.seed, 0, "Random seed for repeatability." );
	parser.parse( "-s", test.shouldSeed, "Should the specified seed be used? Otherwise the seed is assumed to be the time." );
	parser.parse( "-n", test.nodes, 100, "The total number of values to specify in the XML file." );
	parser.parse( "-ml", test.maxLevels, 10, "The maximum levels to nest values in the XML tree." );
	
	{
	
		std::stringstream stream;
		if( test.shouldSeed )
		{
			
			srand( test.seed );
		
		}
		else
		{
		
			srand( time(0) );
		
		}
		stream << "tmp_" << rand() << ".txt";
		parser.parse( "-t", test.tempFile, stream.str().c_str(), "The name of a file to use a temp for the XML parser." );
	
	}
	
	parser.parse( "-d", test.dontDelete, "Don't delete the generated temp XML file." );

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

