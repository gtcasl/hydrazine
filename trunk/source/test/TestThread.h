#ifndef TEST_THREAD_H_INCLUDED
#define TEST_THREAD_H_INCLUDED

#include <common/interface/Thread.h>
#include <common/implementation/ArgumentParser.h>
#include <common/interface/Test.h>
#include <iostream>

namespace test
{

	class RingThread : public common::Thread
	{
	
		protected:
		
			void execute();
			
		public:
		
			Id source;
			
			Id destination;
			
			unsigned int loops;
	
	};
	
	RingThread* startRing( unsigned int threads, unsigned int loops );
	
	class TestThread : public Test
	{
	
		private:
		
			bool testMessage( );
			bool doTest( std::string& status );
			
		public:
		
			TestThread();			
			unsigned int threads;
			unsigned int loops;
			
			bool verbose;
	
	};


}

int main( int argc, char* argv[] );

#endif

