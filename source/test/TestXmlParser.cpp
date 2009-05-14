/*!

	\file TestXmlParser.cpp
	
	\date Monday September 15, 2008
	
	\author Gregory Diamos <gregory.diamos@gatech.edu>

	\brief The source file for the TestXmlParser class.

*/

#ifndef TEST_XML_PARSER_CPP_INCLUDED
#define TEST_XML_PARSER_CPP_INCLUDED

#include "TestXmlParser.h"
#include <common/implementation/XmlParser.h>
#include <common/implementation/ArgumentParser.h>

#include <common/implementation/macros.h>

#ifdef REPORT_BASE
#undef REPORT_BASE
#endif

#define REPORT_BASE 0

namespace test
{

	void TestXmlParser::initializeTree( common::XmlTree& tree ) const
	{

		unsigned int n = 0;
		common::XmlTree::iterator fi;
		unsigned int totalLevels = 0;
				
		while( n < nodes )
		{
		
			unsigned int levels = rand() % maxLevels;
			fi = tree.begin();
			
			for( unsigned int l = 0; l < levels; ++l )
			{
			
				unsigned int nodesPerLevel = maxNodesPerLevel;
				std::stringstream name;
				name << "level_" << totalLevels << "_" << l;
				fi = tree.insert( name.str(), fi, 
					common::XmlTree::XmlTreeNode::Intermediate );
			
				for( unsigned int npl = 0; npl < nodesPerLevel; ++npl )
				{
				
					std::stringstream nodeName;
					nodeName << "node_" << n;
					tree.insert( nodeName.str(), fi, 
						common::XmlTree::XmlTreeNode::Leaf );
					++n;
					
					if( n >= nodes )
					{
					
						break;
					
					}
					
				}
				
					
				if( n >= nodes )
				{
				
					break;
				
				}
			
			}
			
			++totalLevels;
		
		}
		
		tree.begin()->identifier = tempFile;
	
	}
	
	void TestXmlParser::copyTree( 
		common::XmlTree& reference, 
		common::XmlTree& parsed ) const
	{
	
		std::ofstream file( tempFile.c_str() );
		
		assert( file.is_open() );
		reference.toStream( file );
		file.close();
		
		common::XmlParser parser( tempFile.c_str() );

		parsed = parser.tree();
		
		if( !dontDelete )
		{
		
			std::remove( tempFile.c_str() );
		
		}
	
	}
	
	
	bool TestXmlParser::compareTrees( 
		common::XmlTree& reference, 
		common::XmlTree& parsed,
		std::string& status  ) const
	{
	
		common::XmlTree::iterator referenceIterator;
		common::XmlTree::iterator parsedIterator = parsed.begin();
				
		if( reference.size() != parsed.size() )
		{
		
			std::stringstream stream;
			stream << "Reference XmlTree size " << reference.size() << 
				" does not equal parsed size " << parsed.size();
			status += stream.str();
			return false;
		
		}
		
		for( referenceIterator = reference.begin(); 
			referenceIterator != reference.end(); 
			++referenceIterator )
		{
		
			
			if( parsedIterator == parsed.end() )
			{
			
				std::stringstream stream;
				stream << "Parsed XML tree hit end early at label " 
					<< parsedIterator->identifier;
				status += stream.str();
				return false;
			
			}
		
			if( parsedIterator->identifier != referenceIterator->identifier )
			{
			
				std::stringstream stream;
				stream << "Reference XmlTree label '" 
					<< referenceIterator->identifier << 
					"' does not equal parsed label '" 
					<< parsedIterator->identifier
					<< "'";
				status += stream.str();
				return false;
			
			}
			
			++parsedIterator;
		
		}
		
		if( parsedIterator != parsed.end() )
		{
		
			std::stringstream stream;
			stream << "Parsed XML tree hit end late at label " 
				<< parsedIterator->identifier;
			status += stream.str();
			return false;
	
		
		}
		
		status += "Test passed.";
		return true;		
	
	}

	bool TestXmlParser::doTest( std::string& status )
	{
	
		common::XmlTree reference;
		common::XmlTree parsed;
		
		srand( seed );
		initializeTree( reference );
		copyTree( reference, parsed );
		
		return compareTrees( reference, parsed, status );
	
	}
	
	TestXmlParser::TestXmlParser()
	{
	
		Name = "TestXmlParser";
		
		Description = "Tests the XmlParser by generating a random XML tree, writing it out to a file, parsing the file, and comparing the two XML trees.";
		
		maxNodesPerLevel = 1;
	
	}

}

int main( int argc, char** argv )
{

	common::ArgumentParser parser( argc, argv );

	test::TestXmlParser test;
	bool help;
	
	parser.description( test.description() );
	parser.parse( "-h", help, "Display this help message." );
	parser.parse( "-v", test.verbose, "Print out information after the test is over." );
	parser.parse( "-s", test.seed, 0, "Random seed for repeatability." );
	parser.parse( "-n", test.nodes, 100, "The total number of values to specify in the XML file." );
	parser.parse( "-ml", test.maxLevels, 10, "The maximum levels to nest values in the XML tree." );
	{
		std::stringstream stream;
		if( test.seed == 0 )
		{
		
			srand( time( 0 ) );
		
		}
		else
		{
		
			srand( test.seed );
		
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

