/*!

	\file Package.h
	
	\date Thursday Novermber 6, 2008
	
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	
	\brief The header file for the Package class

*/

#ifndef PACKAGE_H_INCLUDED
#define PACKAGE_H_INCLUDED

#include <common/interface/Packable.h>
#include <cstdint>
#include <cstring>
#include <unordered_map>

namespace common
{

	/*!
		
		\brief A class that stores multiple packable objects in a contiguous
			array.  
		
	*/
	class Package
	{

		public:

			typedef std::uint8_t uint8_t;
			typedef uint8_t* iterator;
			typedef const uint8_t* const_iterator;
	
		private:
		
			class Entry
			{
			
				public:
				
					iterator begin;
					iterator end;
			
			};
	
		private:
		
			typedef std::unordered_map< Packable*, Entry > OffsetMap;
			
		private:
		
			OffsetMap _offsetMap;
	
			iterator _begin;
			iterator _end;
			iterator _reserved;
	
		public:
		
			/*!
			
				\brief Constructor
			
			*/
			Package();
			
			/*!
			
				\brief Copy Constructor
			
				\param package The package being copied from
			
			*/
			Package( const Package& package );
			
			/*!
			
				\brief Assignment Operator
				
				\param package The Package being assigned from.
				
				\return A reference to this package.
			
			*/
			const Package& operator=( const Package& package );
		
			/*!
			
				\brief Destructor
			
			*/
			~Package();
			
			/*!
			
				\brief The total number of bytes stored in this array.
			
			*/
			unsigned int bytes() const;

			/*!
			
				\brief The total number of used bytes stored in this array.
			
			*/
			unsigned int size() const;

			/*!
			
				\brief Get a pointer to the base of the array.
			
				\return An iterator to the base of the array.
				
			*/
			iterator begin();
			
			/*!
			
				\brief Get a pointer to the end of the array.
			
				\return An iterator to the end of the array.
				
			*/
			iterator end();
			
			/*!
			
				\brief Get a pointer to a specific packable object in the array.
			
				\param pointer A pointer to the original object
				
				\return An iterator to the start of the object's data, or end if 
					the object does not exist.
			
			*/
			iterator find( Packable* pointer );

			/*!
			
				\brief Insert a Packable object into the array.
				
				INSERT INVALIDATES ITERATORS.
				
				Inserting the same packable object multiple times does nothing.
				
				\param packable The packable object being inserted.
			
				\return True if the insert succeeded.
			
			*/
			bool insert( Packable& packable );
			
			/*!
			
				\brief Removes a Packable object from the array.
			
				REMOVE IVALIDATES ITERATORS.
			
				\param pointer The pointer of the object being removed.
			
			*/
			void remove( Packable* offset );

			/*!
			
				\brief Clear the Package.
			
			*/
			void clear();
			
			/*!
			
				\brief Is the package empty?
			
				\return True if the package is empty.
			
			*/
			bool empty() const;
			
			/*!
			
				\brief Reserve some bytes at the end of the array s a resize
					is not needed on the next insert.
					
				\param bytes The total number of bytes that should be held by
					the Package.
			
			*/
			void reserve( unsigned int bytes );
			
			/*!
			
				\brief Compress a packge to reduce unused space.
			
			*/
			void compress();

	};

}

#endif

