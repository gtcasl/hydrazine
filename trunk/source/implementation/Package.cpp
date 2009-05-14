/*!

	\file Package.cpp
	
	\date Thursday Novermber 6, 2008
	
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	
	\brief The source file for the Package class

*/

#ifndef PACKAGE_CPP_INCLUDED
#define PACKAGE_CPP_INCLUDED

#include "Package.h"
#include <cassert>
#include <common/implementation/debug.h>

#ifdef REPORT_BASE
#undef REPORT_BASE
#endif

#define REPORT_BASE 0

namespace common
{

	Package::Package()
	{
	
		_begin = 0;
		_end = 0;
		_reserved = 0;
	
	}
	
	Package::Package( const Package& package )
	{
		
		_begin = new uint8_t[ package.bytes() ];
		std::memcpy( _begin, package._begin, package.size() );
		
		_reserved = _begin + package.bytes();
		_end = _begin + package.size();

		for( OffsetMap::const_iterator fi = package._offsetMap.begin(); 
			fi != package._offsetMap.end(); ++fi )
		{
		
			Entry entry;
			entry.begin = _begin + ( fi->second.begin - package._begin );
			entry.end = entry.begin + ( fi->second.end - fi->second.begin );
			_offsetMap.insert( std::make_pair( fi->first, entry ) );
		
		}
			
	}
	
	const Package& Package::operator=( const Package& package )
	{
	
		assert( this != &package );
		clear();
		
		_begin = new uint8_t[ package.bytes() ];
		std::memcpy( _begin, package._begin, package.size() );
		
		_reserved = _begin + package.bytes();
		_end = _begin + package.size();

		for( OffsetMap::const_iterator fi = package._offsetMap.begin(); 
			fi != package._offsetMap.end(); ++fi )
		{
		
			Entry entry;
			entry.begin = _begin + ( fi->second.begin - package._begin );
			entry.end = entry.begin + ( fi->second.end - fi->second.begin );
			_offsetMap.insert( std::make_pair( fi->first, entry ) );
		
		}
		
		return *this;
	
	}

	Package::~Package()
	{
	
		if( _begin != _reserved )
		{
		
			delete[] _begin;
		
		}
	
	}
	
	unsigned int Package::size() const
	{
	
		return _end - _begin;
	
	}

	unsigned int Package::bytes() const
	{
	
		return _reserved - _begin;
	
	}

	Package::iterator Package::begin()
	{
	
		return _begin;
	
	}

	Package::iterator Package::end()
	{
	
		return _end;
	
	}
	
	Package::iterator Package::find( Packable* offset )
	{
	
		report( "Searching for packable " << offset );

		OffsetMap::iterator fi = _offsetMap.find( offset );
	
		if( fi == _offsetMap.end() )
		{
		
			report( " Overshot offset." );
			return 0;
		
		}
	
		report( " Got pointer " << reinterpret_cast< unsigned int* >( 
			fi->second.begin ) );
	
		return fi->second.begin;
	
	}

	bool Package::insert( Packable& packable )
	{
	
		report( "Inserting object " << &packable << " into package." );
	
		OffsetMap::iterator fi = _offsetMap.find( &packable );
		
		if( fi == _offsetMap.end() )
		{
		
			Entry entry;

			reserve( size() + packable.bytes() );

			report( " Inserting from " << size() << " to " 
				<< ( size() + packable.bytes() ) << "." );
			
			packable.pack( _end );

			entry.begin = _end;
			entry.end = _end + packable.bytes();
			_end = entry.end;
			
			_offsetMap.insert( std::make_pair( &packable, entry ) );

			assert( _end <= _reserved );
		
			return true;
		
		}
		
		return false;
	
	
	}

	void Package::remove( Packable* offset )
	{

		OffsetMap::iterator fi = _offsetMap.find( offset );
	
		report( " Removing object " << offset );
	
		assert( fi != _offsetMap.end() );
		
		_offsetMap.erase( fi );
			
	
	}

	void Package::clear()
	{
	
		_offsetMap.clear();

		report( "Clearing package." );

		if( _begin != _reserved )
		{
		
			delete[] _begin;

			_begin = 0;
			_end = 0;
			_reserved = 0;
		
		}
	
	}
	
	bool Package::empty( ) const
	{
	
		return _begin == _end;
	
	}
	
	void Package::reserve( unsigned int bytes )
	{
	
		report( "Reserving " << bytes << " bytes, current size is " 
			<< this->bytes() <<  " ." );
	
		if( this->bytes() < bytes )
		{
		
			report( " Growing from " << this->bytes() << " to " << bytes );
			
			unsigned int oldSize = 0;
			iterator newBegin = new uint8_t[ bytes ];
			
			if( _begin != _end )
			{

				for( OffsetMap::iterator fi = _offsetMap.begin(); 
					fi != _offsetMap.end(); ++fi )
				{
				
					unsigned int size = fi->second.end - fi->second.begin;
					
					oldSize += size;
					
					fi->second.begin = newBegin + ( fi->second.begin - _begin );
					fi->second.end = fi->second.begin + size;

					if( size != 0 )
					{
					
						fi->first->pack( fi->second.begin );
				
					}
				
				}
			
				delete[] _begin;
			
			}
			
			_begin = newBegin;
			_end = _begin + oldSize;
			_reserved = _begin + bytes;
		
		}
	
	}
	
	void Package::compress()
	{
	
		report( "Compressing package." );
	
		unsigned int usedSize = 0;
		
		for( OffsetMap::iterator fi = _offsetMap.begin(); 
			fi != _offsetMap.end(); ++fi )
		{
		
			usedSize += ( fi->second.begin - fi->second.end );
		
		}
		
		report( " Used size was " << usedSize << " out of " << bytes()
			<< " allocated bytes." );
		
		if( usedSize < bytes() )
		{
		
			iterator newBegin = new uint8_t[ usedSize ];
			iterator newEnd = newBegin;
			
			for( OffsetMap::iterator fi = _offsetMap.begin(); 
				fi != _offsetMap.end(); ++fi )
			{
				
				unsigned int size = fi->second.end - fi->second.begin;

				assert( fi->first->bytes() == size );
				
				fi->first->pack( newEnd );
				
				fi->second.begin = newEnd;
				newEnd += size;
				fi->second.end = newEnd;
				
			}
			
			delete[] _begin;
			
			_begin = newBegin;
			_end = newEnd;
			_reserved = newEnd;

			assert( this->bytes() == this->size() );

			report( " Compressed to " << this->bytes() );
		
		}
		
	}

}

#endif

