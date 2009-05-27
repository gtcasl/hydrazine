/*!	\file math.h
*
*	\brief Header file for common math functions.
*
*	\author Gregory Diamos
*
*	\date : 9/27/2007
*	\date : 10/24/2007 : added exceptions
*
*/

#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED


#include <math.h>
#include <cassert>
/*!
	\brief a namespace for common classes and functions
*/
namespace hydrazine
{

	/*!
		\brief Check to see if an int is a power of two
	*/
	inline bool isPowerOfTwo (int value);

	/*!
		\brief Check to see if an unsigned int is a power of two
	*/
	inline bool isPowerOfTwo (unsigned int value);

	/*!
		\brief Mod a power of power of two
	*/
	inline unsigned int modPowerOfTwo( unsigned int value1, 
		unsigned int value );

	/*!
		\brief Mod a power of power of two
	*/
	inline int modPowerOfTwo( int value1, int value );
		
	/*!
		\brief Compute the next highest power of two
	*/
	inline unsigned int powerOfTwo( unsigned int value );
	
	
	////////////////////////////////////////////////////////////////////////////
	// Power of two checks
	inline bool isPowerOfTwo( int value )
	{
		return (value & (~value+1)) == value;
	}
	
	inline bool isPowerOfTwo( unsigned int value )
	{
		return (value & (~value+1)) == value;
	}
	
	inline unsigned int modPowerOfTwo( unsigned int value1, unsigned int value )
	{
		assert( value != 0 );
		return value1 & ( value - 1 );
	}

	inline int modPowerOfTwo( int value1, int value )
	{
		assert( value != 0 );
		return value1 & ( value - 1 );
	}
	
	inline unsigned int powerOfTwo( unsigned int value )
	{	
		value--;		
		value |= value >> 1;		
		value |= value >> 2;		
		value |= value >> 4;		
		value |= value >> 8;		
		value |= value >> 16;		
		value++;		
		return value;		
	}
	////////////////////////////////////////////////////////////////////////////

}

#endif

