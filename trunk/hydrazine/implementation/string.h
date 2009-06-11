/*!

	\file string.h
	
	\date Friday February 13, 2009
	
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	
	\brief Function headers for common C string manipulations

*/

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

namespace hydrazine
{

	/*!
		\brief Safe string copy
		
		\param destination The target string
		\param source The source string
		\param max The max number of characters to copy
	*/
	void strlcpy( char* destination, const char* source, unsigned int max );

}

#endif

