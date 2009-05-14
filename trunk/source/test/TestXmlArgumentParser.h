/*!

	\file TestXmlArgumentParser.h
	
	\date Tuesday September 16, 2008
	
	\author Gregory Diamos <gregory.diamos@gatech.edu>

	\brief The header file for the TestXmlArgumentParser class.

*/

#ifndef TEST_XML_ARGUMENT_PARSER_H_INCLUDED
#define TEST_XML_ARGUMENT_PARSER_H_INCLUDED

#include <common/interface/Test.h>
#include <common/implementation/XmlTree.h>

namespace test
{

	/*!
	
		\brief Generate a random XML tree, keep the path down to each node.
	
	*/
	class TestXmlArgumentParser : public Test
	{
	
		private:
		
			class Path
			{
			
				public:
				
					std::list< std::string > path;
					std::string value;
			
			};
	
		private:
		
			void initializeTree( common::XmlTree& tree, 
				std::list< Path >& paths ) const;
			bool parseTree( 
				common::XmlTree& parsed, 
				std::list< Path >& paths,
				std::string& status  ) const;
		
			bool doTest( std::string& status );
		
		public:
		
			TestXmlArgumentParser();
			
			bool verbose;
			unsigned int seed;
			bool shouldSeed;
			unsigned int nodes;
			unsigned int maxLevels;
			std::string tempFile;
			bool dontDelete;
	
	};

}

int main( int argc, char** argv );

#endif

