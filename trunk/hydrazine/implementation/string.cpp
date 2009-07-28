/*!

	\file string.cpp
	
	\date Friday February 13, 2009
	
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	
	\brief Function sources for common C string manipulations

*/

#ifndef STRING_CPP_INCLUDED
#define STRING_CPP_INCLUDED

#include <hydrazine/implementation/string.h>

namespace hydrazine
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
	
	std::string format( const std::string& input, 
		const std::string& firstPrefix, const std::string& prefix, 
		unsigned int width )
	{
		std::string word;
		std::string result = firstPrefix;
		unsigned int currentIndex = firstPrefix.size();
	
		for( std::string::const_iterator fi = input.begin(); 
			fi != input.end(); ++fi )
		{
			if( *fi == ' ' || *fi == '\t' || *fi == '\n' 
				|| *fi == '\r' || *fi == '\f' )
			{
				if( currentIndex + word.size() > width )
				{
					currentIndex = prefix.size();
					result += "\n";
					result += prefix;
				}
				
				if( !word.empty() )
				{
					result += word + " ";
					++currentIndex;
					word.clear();
				}
			}
			else
			{
				word.push_back( *fi );
				++currentIndex;
			}
		}

		if( currentIndex + word.size() > width )
		{
			result += "\n";
		}
		
		result += word + "\n";
		return result;
	}

}

#endif

