/*!

	\file TestXmlParser.h
	
	\date Monday September 15, 2008
	
	\author Gregory Diamos <gregory.diamos@gatech.edu>

	\brief The header file for the TestXmlParser class.

*/

#ifndef TEST_XML_PARSER_H_INCLUDED
#define TEST_XML_PARSER_H_INCLUDED

#include <common/interface/Test.h>
#include <common/implementation/XmlTree.h>

namespace test
{

	/*!
	
		\brief Generate a random XML tree, write it out to a temp file,
		then parse it and compare to make sure that the trees match.
	
	*/
	class TestXmlParser : public Test
	{
	
		private:
		
			void initializeTree( common::XmlTree& tree ) const;
			void copyTree( 
				common::XmlTree& reference, 
				common::XmlTree& parsed ) const;
			bool compareTrees( 
				common::XmlTree& reference, 
				common::XmlTree& parsed,
				std::string& status  ) const;
		
			bool doTest( std::string& status );
		
		public:
		
			TestXmlParser();
			
			bool verbose;
			unsigned int seed;
			unsigned int nodes;
			unsigned int maxLevels;
			unsigned int maxNodesPerLevel;	
			std::string tempFile;
			bool dontDelete;
	
	};

}

int main( int argc, char** argv );

#endif

