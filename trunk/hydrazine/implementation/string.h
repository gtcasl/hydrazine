/*! \file string.h
	\date Friday February 13, 2009
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	\brief Function headers for common C string manipulations
*/

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include <string>

namespace hydrazine
{

	/*! \brief Safe string copy
		
		\param destination The target string
		\param source The source string
		\param max The max number of characters to copy
	*/
	void strlcpy( char* destination, const char* source, unsigned int max );
	
	/*! \brief Format a string to fit a specific character width */
	std::string format( const std::string& input, 
		const std::string& firstPrefix = "", const std::string& prefix = "", 
		unsigned int width = 80 );

	/*! \brief Parse a string specifying a binary number, return the number */
	long long unsigned int binaryToUint( const std::string& );

	/*! \brief Convert a string to a label that can be parsed by graphviz */
	std::string toGraphVizParsableLabel( const std::string& );

	/*! \brief Add line numbers to a ver large string */
	std::string addLineNumbers( const std::string& );

}

#endif

