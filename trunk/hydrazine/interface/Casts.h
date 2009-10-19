/*!
	\file Casts.h
	\date Monday October 19, 2009
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	\brief The header file for a set of non-standard casts
*/

#ifndef CASTS_H_INCLUDED
#define CASTS_H_INCLUDED

#include <hydrazine/implementation/debug.h>

namespace hydrazine
{
	template< typename Type >
	Type bit_cast( char i )
	{
		return *reinterpret_cast< Type* >( &i );
	}
	
	template< typename Type >
	Type bit_cast( unsigned char i )
	{
		return *reinterpret_cast< Type* >( &i );
	}
	
	template< typename Type >
	Type bit_cast( short i )
	{
		return *reinterpret_cast< Type* >( &i );
	}
	
	template< typename Type >
	Type bit_cast( unsigned short i )
	{
		return *reinterpret_cast< Type* >( &i );
	}
	
	template< typename Type >
	Type bit_cast( int i )
	{
		return *reinterpret_cast< Type* >( &i );
	}
	
	template< typename Type >
	Type bit_cast( unsigned int i )
	{
		return *reinterpret_cast< Type* >( &i );
	}
	
	template< typename Type >
	Type bit_cast( long long int i )
	{
		return *reinterpret_cast< Type* >( &i );
	}
	
	template< typename Type >
	Type bit_cast( unsigned long long int i )
	{
		return *reinterpret_cast< Type* >( &i );
	}
	
	template< typename Type >
	Type bit_cast( float i )
	{
		return *reinterpret_cast< Type* >( &i );
	}
	
	template< typename Type >
	Type bit_cast( double i )
	{
		return *reinterpret_cast< Type* >( &i );
	}

}

#endif

