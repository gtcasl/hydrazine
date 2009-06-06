/*!	\file SerializationBuffer.h
*
*	\brief Header file for the serilaizationBuffer class
*
*	\author Gregory Diamos
*
*
*
*/

#ifndef SERIALIZATION_BUFFER_H_INCLUDED
#define SERIALIZATION_BUFFER_H_INCLUDED

#include <vector>
#include <sstream>
#include <queue>
#include "macros.h"
#include <assert.h>
#include <hydrazine/interface/Clonable.h>

/*!
	\brief a namespace for common classes and functions
*/

namespace hydrazine
{
	/*!
	
		\brief A class for storing objects in a stream of bytes.
		
		This class provides methods for dynamically creating a buffer and writing
		in objects by refernce and size.  This particular implementation tries
		to align everything to page boundaries.
	
	
	*/
	
	class SerializationBuffer : public Clonable
	{
		private:
					
			// a char buffer is used to represent the underlying data
			char* buffer;
			
			// the used size of the char buffer
			unsigned int bufferSize;
			
			// where to read from offset from the base
			char* readPointer;
			
			unsigned int readOffset;
			
			// where to write to offset from the base
			char* writePointer;
			
			unsigned int writeOffset;
			
			const unsigned int pageSize;
			
			// figure out what the size of memory align to pages is
			unsigned int pageAlign(unsigned int size);
			
			
		public:
			//
			SerializationBuffer();
			~SerializationBuffer();
			
			void write( std::string& str );
			void read( std::string& str );
			
			/// Write data into the buffer
			void write(const void* address, unsigned int size);
			
			/// Read data out of the buffer
			void read(void* address, unsigned int size);
			
			// get the pointer associated with the shared array
			char* ptr() const;
			
			// get the size associated with the shared array
			unsigned int size() const;
			
			// empty
			bool empty() const;
			
			/// resize the buffer
			void resize(unsigned int size);
			
			/// set the write pointer
			void initializeWritePointer( unsigned int size );
			
			/// set the pointer and size associated with the shared array
			void ptr(char* buffer, unsigned int size);
			
			/// copy constructor
			SerializationBuffer(const SerializationBuffer& b);
			
			/// assignment contructor
			SerializationBuffer& operator=(const SerializationBuffer& b);
		
			/// clone
			Clonable* clone( bool ) const;
			
			//// clear the buffer
			void clear();
		
	};

	////////////////////////////////////////////////////////////////////////////
	
	
	
	
	
}//ext

#endif
