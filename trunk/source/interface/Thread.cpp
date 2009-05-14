/*!
*	\file Thread.cpp
*
*	\author Gregory Diamos
*	\date 4/10/2008
*	
*
*	
*
*	\brief 	The source file for the thread class
*
*					This file provides a class wrapper for pthreads
*/

#ifndef THREAD_CPP_INCLUDED
#define THREAD_CPP_INCLUDED

#include "Thread.h"

#include <common/implementation/debug.h>
#include <common/implementation/Exception.h>

#ifdef REPORT_BASE
#undef REPORT_BASE 
#endif

#define REPORT_BASE 0

////////////////////////////////////////////////////////////////////////////////

namespace common
{

	Thread::Queue::Queue()
	{
	
		pthread_mutex_init( &_mutex, 0 );
		pthread_cond_init( &_condition, 0 );
	
	}
	
	Thread::Queue::~Queue()
	{
	
		assert( _queue.empty() );	
	
		pthread_mutex_destroy( &_mutex );
		pthread_cond_destroy( &_condition );
	
	}
	
	void Thread::Queue::push( const Message& message )
	{
	
		pthread_mutex_lock( &_mutex );
		
		_queue.push_back( message );
		
		pthread_cond_broadcast( &_condition );
		pthread_mutex_unlock( &_mutex );
	
	}
	
	Thread::Message Thread::Queue::pull( Id id )
	{
	
		Message result;
		result.type = Message::Invalid;
		
		pthread_mutex_lock( &_mutex );
		
		while( result.type == Message::Invalid )
		{
		
			for( MessageQueue::iterator message = _queue.begin(); 
				message != _queue.end(); ++message )
			{
			
				if( _compare( message->source, id ) )
				{
				
					assert( message->type != Message::Invalid );
					result = *message;
					_queue.erase( message );
					break;
				
				}
			
			}
			
			if( result.type == Message::Invalid )
			{
			
				pthread_cond_wait( &_condition, &_mutex );
		
			}
			
		}
		
		pthread_mutex_unlock( &_mutex );
		
		return result;
	
	}

	bool Thread::Queue::test( Id& id, bool block )
	{
	
		bool found = false;
	
		pthread_mutex_lock( &_mutex );
		
		do
		{
		
			for( MessageQueue::iterator message = _queue.begin(); 
					message != _queue.end(); ++message )
			{
		
				if( _compare( message->source, id ) )
				{
			
					assert( message->type != Message::Invalid );
					id = message->source;
					found = true;
					break;
			
				}
		
			}
			
			if( block && !found )
			{
			
				pthread_cond_wait( &_condition, &_mutex );
			
			}
		
		}
		while( block && !found );
		
		pthread_mutex_unlock( &_mutex );
		
		return found;
	
	}

	Thread::Group::Group()
	{
	
		pthread_mutex_init( &_mutex, 0 );
	
	}

	Thread::Group::~Group()
	{
	
		assert( empty() );
		pthread_mutex_destroy( &_mutex );
	
	}

	void Thread::Group::add( Thread* thread )
	{
	
		pthread_mutex_lock( &_mutex );
	
		assert( _threads.count( thread->id() ) == 0 );
	
		_threads.insert( std::make_pair( thread->id(), thread ) );
	
		pthread_mutex_unlock( &_mutex );
	
	}

	void Thread::Group::remove( Thread* thread )
	{
	
		pthread_mutex_lock( &_mutex );
	
		assert( _threads.count( thread->id() ) != 0 );
	
		_threads.erase( thread->id() );
	
		pthread_mutex_unlock( &_mutex );
	
	}

	Thread* Thread::Group::find( Id id )
	{
	
		Thread* result = 0;
	
		pthread_mutex_lock( &_mutex );
	
		ThreadMap::iterator thread = _threads.find( id );
	
		if( thread != _threads.end() )
		{
		
			result = thread->second;
		
		}
	
		pthread_mutex_unlock( &_mutex );
	
		return result;
	
	}

	void Thread::Group::push( const Message& message )
	{
	
		assert( message.destination != THREAD_ANY_ID );
	
		if( message.destination == THREAD_CONTROLLER_ID )
		{
		
			assert( _threads.count( message.source ) != 0 );		
			_controllerQueue.push( message );
		
		}
		else
		{
		
			pthread_mutex_lock( &_mutex );

			ThreadMap::iterator thread = _threads.find( message.destination );
			assert( thread != _threads.end() );
		
			thread->second->_threadQueue.push( message );

			pthread_mutex_unlock( &_mutex );
				
		}
			
	}
	
	Thread::Message Thread::Group::pull( Id source )
	{
	
		return _controllerQueue.pull( source );
	
	}

	bool Thread::Group::test( Id& source, bool block )
	{

		return _controllerQueue.test( source, block );	
	
	}

	bool Thread::Group::empty() const
	{
	
		return _threads.empty();
		
	}

	unsigned int Thread::Group::size() const
	{
	
		return _threads.size();
		
	}

	Thread::Id Thread::_nextId = THREAD_START_ID;

	void* Thread::_launch( void* argument )
	{
		
		Thread* thread = static_cast< Thread* >( argument );	
		thread->execute();
		pthread_exit( 0 );
		
	}

	bool Thread::_compare( Id one, Id two )
	{
	
		return one == two || one == THREAD_ANY_ID || two == THREAD_ANY_ID;
	
	}
	
	bool Thread::threadTest( Id id )
	{
	
		Id null = 0;
		return _threadQueue.test( id, null );
	
	}

	void Thread::threadSend( MessageData data, Thread::Id id )
	{

		assert( id != THREAD_ANY_ID );
		assert( _id != id );

		Message message;
		message.source = _id;
		message.destination = id;
		message.payload = data;
		message.type = Message::Regular;

		report( "Thread " << _id << " sending message to " 
			<< message.destination << "." );
		assert( _group != 0 );
		_group->push( message );
	
	}
	
	Thread::Thread()
	{
	
		_id = _nextId++;
		_running = false;
		_group = new Group;
		_group->add( this );
		
		pthread_attr_init( &_attribute );
		pthread_attr_setdetachstate( &_attribute, PTHREAD_CREATE_JOINABLE );
	
	}

	Thread::Thread( const Thread& t )
	{
	
		_id = _nextId++;
		_running = false;
		_group = new Group;
		_group->add( this );
		
		pthread_attr_init( &_attribute );
		pthread_attr_setdetachstate( &_attribute, PTHREAD_CREATE_JOINABLE );
	
	}
	
	const Thread& Thread::operator=( const Thread& t )
	{
	
		assert( _running == false );
		assert( _group->size() == 1 );
		assert( t._group->size() == 1 );
		assert( t._running == false );
		return *this;
	
	}

	Thread::~Thread()
	{
	
		if( _running )
		{
		
			join();
		
		}

		assert( !_running );
		
		_group->remove( this );
		
		if( _group->empty() )
		{
		
			delete _group;
		
		}
		
		pthread_attr_destroy( &_attribute );
	
	}

	void Thread::start()
	{
	
		assert( !_running );
		
		_running = true;
		
		report( "Thread " << _id << " starting." );

		int error = pthread_create( &_handle, &_attribute, 
			Thread::_launch, this );
		
		if( error )
		{
		
			throw common::Exception( "Pthread create failed.", error );
		
		}
	
	}

	void Thread::join()
	{
	
		assert( _running );
		
		void* status;
		
		report( "Thread " << _id << " joining." );
		
		int error = pthread_join( _handle, &status );

		_running = false;
	
		if( error )
		{
		
			throw common::Exception( "Pthread join failed.", error );
		
		}
	
	}

	void Thread::associate( Thread* t )
	{
	
		assert( _group->size() == 1 || t->_group->size() == 1 );
		assert( id() != t->id() );
	
		if( _group->size() == 1 )
		{
		
			report( "Thread " << _id << " joining group." );
			_group->remove( this );
			delete _group;
			_group = t->_group;
			_group->add( this );
		
		}
		else
		{
		
			report( "Thread " << t->_id << " joining group." );
			t->_group->remove( this );
			delete t->_group;
			t->_group = _group;
			_group->add( t );
		
		}
	
	}

	void Thread::remove()
	{

		report( "Thread " << _id << " leaving group." );
		assert( !_group->empty() );
		
		if( _group->size() > 1 )
		{
	
			report( " Thread " << _id << " destroying group." );
		
			_group->remove( this );
			_group = new Group;
		
		}
		
	}

	void Thread::send( MessageData data )
	{
	
		report( "Thread " << _id 
			<< " sending message from controller to local thread." );
	
		Message message;
		message.source = THREAD_CONTROLLER_ID;
		message.destination = _id;
		message.payload = data;
		message.type = Message::Regular;
		
		_group->push( message );
	
	}

	bool Thread::test( bool block )
	{
	
		return _group->test( _id, block );
	
	}
	
	std::pair< Thread::Id, bool > Thread::testGroup( bool block, Id source )
	{
	
		std::pair< Id, bool > result;

		result.first = source;
		result.second = _group->test( result.first, block );

		assert( !block || result.second );
		
		return result;
	
	}

	Thread::Id Thread::id() const
	{
	
		return _id;
	
	}
	
	bool Thread::started() const
	{
		
		return _running;
	
	}
	
	Thread* Thread::find( Thread::Id id )
	{

		report( "Looking up thread with id " << id );
	
		if( id == _id )
		{
		
			return this;
		
		}
	
		return _group->find( id );
	
	}
		
}

////////////////////////////////////////////////////////////////////////////////
#endif

