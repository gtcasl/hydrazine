/*!

	\file Packable.h
	
	\date Thursday Novermber 6, 2008
	
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	
	\brief The header file for the Packable interface

*/

#ifndef PACKABLE_H_INCLUDED
#define PACKABLE_H_INCLUDED

namespace common
{

	/*!
		
		\brief A packable class can be copied into and out of a contiguous 
		buffer of size N.
		
	*/
	class Packable
	{
	
		public:
		
			/*!
			
				\brief Virtual Destructor
			
			*/
			virtual ~Packable() {};
			
			/*!
			
				\brief Determine the amount of bytes required by this object
				when packed contiguously.
			
			*/
			virtual unsigned int bytes() const = 0;

			/*!
			
				\brief Copy this class into the specified contiguous buffer.
				
				\param package The contiguous memory buffer to be packed 
					into.
				
			*/
			virtual void pack( void* package ) = 0;
			
			/*!
			
				\brief Unpack the object from a specified buffer.
			
				\param package The packable being retrieved from.
			
			*/
			virtual void unpack( void* package ) = 0;
	
	};

}

#endif

