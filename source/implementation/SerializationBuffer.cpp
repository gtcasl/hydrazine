/*!	\file SerializationBuffer.cpp
*
*	\brief Source file for the serilaizationBuffer class
*
*	Author: Gregory Diamos
*
*
*
*/

#ifndef SERIALIZATION_BUFFER_CPP_INCLUDED
#define SERIALIZATION_BUFFER_CPP_INCLUDED

#include "SerializationBuffer.h"
#include <cstring>

namespace common
{

	
	////////////////////////////////////////////////////////////////////////////
	// Serialization buffer

	SerializationBuffer::SerializationBuffer()
	:
	pageSize( getpagesize() )
	{
		
		buffer = 0;
		bufferSize = 0;
		readPointer = 0;
		writePointer = 0;
		readOffset = 0;
		writeOffset = 0;
		
	}
	
	SerializationBuffer::~SerializationBuffer()
	{
		
		if( bufferSize > 0 )
		{
		
			delete[] buffer;
		
		}
		
	}
	
	unsigned int SerializationBuffer::pageAlign(unsigned int size)
	{
		if( size > ( (size) / pageSize ) * pageSize )
		{
		
			return ( ( (size) + pageSize ) / pageSize ) * pageSize;
		
		}
		else
		{
		
			return size;
		
		}
	
	}

	void SerializationBuffer::write( std::string& str )
	{
		
		unsigned int length;
		
		length = str.size();
			
		write( &length, sizeof(unsigned int) );
		
		write( str.data(), length );
		
	}

	void SerializationBuffer::read( std::string& str )
	{
		
		unsigned int length;
		
		read( &length, sizeof( unsigned int ) );
		
		str.resize( length );
		
		read( const_cast< char* >( str.data() ), length );
	
	}

	void SerializationBuffer::write(const void* address, unsigned int size)
	{
		
		if( ( writeOffset + size ) > bufferSize )
		{
		
			resize( ( this->size() + size ) << 1 );
		
		}
		
		memcpy( writePointer, address, size );
		
		writePointer += size;
		writeOffset += size;	
	
	}
	

	// resize the buffer
	void SerializationBuffer::resize( unsigned int _size )
	{
		
		// align the new size to a page boundary
		_size = pageAlign( _size );
		
		// do we need to create a new buffer?
		if( _size != bufferSize )
		{
		
			char* newBuffer;
		
			unsigned int newSize;
			
			newSize = MIN( _size, size() );
		
			if( _size > 0 )
			{
			
				newBuffer = new char[ _size ];
			
				// copy from the base of the read pointer to the base of the write pointer
				memcpy( newBuffer, readPointer, newSize );
			
			}
			else
			{
			
				newBuffer = 0;
			
			}
			
			// reset the offsets
			readPointer = newBuffer;
			readOffset = 0;
			
			writePointer = ( newBuffer + newSize );
			writeOffset = newSize;
			
			// delete the old buffer
			if( bufferSize > 0 )
			{
			
				delete[] buffer;
			
			}
			
			buffer = newBuffer;
			bufferSize = _size;
		
		}
		else
		{
		
			if( size() != _size )
			{
			
				memmove( buffer, readPointer, size() );
				
				readPointer = buffer;
				writePointer -= readOffset;
				writeOffset -= readOffset;
				readOffset = 0;
				bufferSize = _size;
			
			}
		
		}
	
	}
	
	void SerializationBuffer::initializeWritePointer( unsigned int size )
	{
	
		assert( writeOffset == 0 );
		assert( size <= bufferSize );
		
		writePointer += size;
		writeOffset += size;
	
	}
	
	void SerializationBuffer::read(void* address, unsigned int size)
	{
		
		assert( size <= ( bufferSize - readOffset ) );
		
		memcpy( address, readPointer, size );
		
		readPointer += size;
		readOffset += size;
		
		if( empty() )
		{
		
			resize( 0 );
		
		}
		
	}
	

	char* SerializationBuffer::ptr() const
	{
		
		return readPointer;
	
	}
	

	unsigned int SerializationBuffer::size() const
	{
	
		return ( writeOffset - readOffset );
	
	}
	
	bool SerializationBuffer::empty() const
	{
	
		return writeOffset == readOffset;
	
	}
	

	void SerializationBuffer::ptr(char* buffer, unsigned int size)
	{
	
		resize(0);
	
		bufferSize = size;
		readPointer = buffer;
		writePointer = buffer + size;
		writeOffset = size;
		this->buffer = buffer;
	
	}
	
	// copy constructor
	SerializationBuffer::SerializationBuffer(const SerializationBuffer& b)
	:
	pageSize( getpagesize() )
	{
		
		bufferSize = b.bufferSize;
	
		if( bufferSize > 0 )
		{
		
			writeOffset = b.writeOffset;
			readOffset = b.readOffset;
			buffer = new char[ bufferSize ];
			readPointer = buffer + readOffset;
			writePointer = buffer + writeOffset;
			
			if( b.size() > 0 )
			{
			
				memcpy( readPointer, b.readPointer, size() );
			
			}
			
		}
		else
		{
		
			buffer = 0;
			writeOffset = 0;
			readOffset = 0;
			readPointer = 0;
			writePointer = 0;
		
		}		
		
	}

	// assignment  constructor
	SerializationBuffer& SerializationBuffer::operator=(const SerializationBuffer& b)
	{
	
		if(this!=&b)
		{
			
			if( bufferSize != b.bufferSize )
			{
			
				if( bufferSize > 0 )
				{
				
					delete[] buffer;
				
				}
			
				bufferSize = b.bufferSize;
	
				if( bufferSize > 0 )
				{
		
					writeOffset = b.writeOffset;
					readOffset = b.readOffset;
					buffer = new char[ bufferSize ];
					readPointer = buffer + readOffset;
					writePointer = buffer + writeOffset;
			
					if( b.size() > 0 )
					{
			
						memcpy( readPointer, b.readPointer, size() );
			
					}
			
				}
				else
				{
		
					buffer = 0;
					writeOffset = 0;
					readOffset = 0;
					readPointer = 0;
					writePointer = 0;
		
				}
			
			}
			else
			{
			
				if( bufferSize > 0 )
				{
		
					writeOffset = b.writeOffset;
					readOffset = b.readOffset;
					readPointer = buffer + readOffset;
					writePointer = buffer + writeOffset;
			
					if( b.size() > 0 )
					{
			
						memcpy( readPointer, b.readPointer, size() );
			
					}
			
				}
				else
				{
		
					buffer = 0;
					writeOffset = 0;
					readOffset = 0;
					readPointer = 0;
					writePointer = 0;
		
				}
			
			}
						
		}
		
		return *this;
		
	}

	// clone
	Clonable* SerializationBuffer::clone( bool ) const
	{
		
		return new SerializationBuffer( *this );
		
	}
	
	void SerializationBuffer::clear()
	{
	
		if( bufferSize > 0 )
		{
		
			delete[] buffer;
		
		}
	
		buffer = 0;
		bufferSize = 0;
		readPointer = 0;
		writePointer = 0;
		readOffset = 0;
		writeOffset = 0;
	
	}
	////////////////////////////////////////////////////////////////////////////
	
}


#endif
