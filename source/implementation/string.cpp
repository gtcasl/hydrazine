/*!

	\file string.cpp
	
	\date Friday February 13, 2009
	
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	
	\brief Function sources for common C string manipulations

*/

#ifndef STRING_CPP_INCLUDED
#define STRING_CPP_INCLUDED

#include <common/implementation/string.h>

namespace common
{

	void strlcpy( char* dest, const char* src, unsigned int length )
	{
		
		const char* end = src + ( length - 1 );
		for( ; src != end; ++src, ++dest )
		{
	
			*dest = *src;
	
			if( *src == '\0' )
			{
		
				return;
		
			}
	
		}
	
		*dest = '\0';
	
	}

}

#endif

