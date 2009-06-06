/*!
*	\file SerializableArchive.cpp
*
*	\author Gregory Diamos
*
*	\date 7/16/2008
*	
*	\brief 	The source file for the SerializableArchive class
*
*/

#ifndef SERIALIZABLE_ARCHIVE_CPP_INCLUDED
#define SERIALIZABLE_ARCHIVE_CPP_INCLUDED

#include "SerializableArchive.h"

namespace hydrazine
{

	SerializableArchive::~SerializableArchive()
	{
	
		for( constIterator ci = registeredClasses.begin(); 
			ci != registeredClasses.end(); ci++ )
		{
		
			delete ci->second;
		
		}
	
	}
	
	void SerializableArchive::registerClass( const Serializable* s )
	{
	
		if( registeredClasses.count( s->id() ) == 0 )
		{
		
			registeredClasses.insert( std::make_pair( s->id(), s->allocator() ) );
		
		}
		
	}
	
	void SerializableArchive::removeRegistration( const Serializable* s )
	{
	
		removeRegistration( s->id() );
	
	}
	
	void SerializableArchive::removeRegistration( Serializable::Id id )
	{
	
		iterator fi = registeredClasses.find( id );
		
		assert( fi != registeredClasses.end() );
		
		delete fi->second;
		
		registeredClasses.erase( fi );
	
	}
	
	void SerializableArchive::save( const Serializable* s )
	{
	
		iterator fi = registeredClasses.find( s->id() );
	
		if( fi == registeredClasses.end() )
		{
		
			registerClass( s );
		
		}
		
		Serializable::Id id;
		
		id = s->id();
		
		_buffer.write( &id, sizeof( Serializable::Id ) );
		
		s->serialize( _buffer );
	
	}
	
	Serializable* SerializableArchive::load()
	{
	
		assert( !_buffer.empty() );
		
		Serializable::Id id;
		
		_buffer.read( &id, sizeof( Serializable::Id ) );
		
		iterator fi = registeredClasses.find( id );
		
		assert( fi != registeredClasses.end() );
		
		Serializable* loadedObject;
		
		loadedObject = fi->second->allocate();
		
		loadedObject->deserialize( _buffer );
		
		return loadedObject;
			
	}
	
	void SerializableArchive::load( Serializable* s )
	{
	
		assert( !_buffer.empty() );
		
		Serializable::Id id;
		
		_buffer.read( &id, sizeof( Serializable::Id ) );
		
		assert( id == s->id() );
		
		s->deserialize( _buffer );
	
	}

	void SerializableArchive::clear()
	{
	
		_buffer.clear();
	
	}
	
	void SerializableArchive::clobber()
	{
	
		_buffer.clear();
		
		for( constIterator ci = registeredClasses.begin(); ci != registeredClasses.end(); ci++ )
		{
		
			delete ci->second;
		
		}
		
		registeredClasses.clear();
	
	}

	void SerializableArchive::saveToFile( const std::string& fileName ) const
	{
	
		std::ofstream file( fileName.c_str(), std::ofstream::binary );
		
		assert( file.is_open() );
		
		file.write( _buffer.ptr(), _buffer.size() );
	
	}
	
	void SerializableArchive::loadFromFile( const std::string& fileName )
	{
	
		_buffer.clear();
		
		std::ifstream file( fileName.c_str(), std::ofstream::binary );
		
		assert( file.is_open() );
		
		unsigned int size;
		
		// get size of file
		file.seekg( 0, std::ifstream::end );
		
		size = file.tellg();
		
		file.seekg( 0 );
		
		_buffer.resize( size );
		
		_buffer.initializeWritePointer( size );
		
		file.read( _buffer.ptr(), size );
	
	}
	
	SerializationBuffer& SerializableArchive::buffer()
	{
	
		return _buffer;
	
	}	

}

#endif

