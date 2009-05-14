/*!

	\file TestSerializableArchive.h
	
	\author Gregory Diamos
	
	\date Sunday July 20, 2008
	
	\boredComments Written while sitting in a starbucks with some really noisy people
	behind me.  Green tea lattes can't compete with chai tea lattes...
	
	\brief Header file for the TestSerializableArchive class. 
	

*/

#ifndef TEST_SERIALIZABLE_ARCHIVE_H_INCLUDED
#define TEST_SERIALIZABLE_ARCHIVE_H_INCLUDED

#include <common/implementation/SerializableArchive.h>
#include <common/implementation/ArgumentParser.h>
#include <common/interface/Test.h>

#ifdef HAVE_BOOST_SERIALIZATION

#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/split_member.hpp>

#endif

#ifdef HAVE_MPI

#ifdef SEEK_SET
#undef SEEK_SET
#endif

#ifdef SEEK_CUR
#undef SEEK_CUR
#endif

#ifdef SEEK_END
#undef SEEK_END
#endif

#include <mpi.h>

#endif

namespace test
{
	/*!
	
		\brief Make sure that the implementation of a Serializable archive works
		as intended.  
	
		Make sure that the performance is also reasonable.  Compare
		to the boost serialization library.  This should test and benchmark saving to
		a buffer then immediately restoring, saving to and restoring from disk,
		and saving to an archive, sending the archive over mpi, and restoring at the
		other end.
	
	*/
	class TestSerializableArchive : public Test
	{
	
		private:
	
			class SimpleSerializable : public common::Serializable
			{
			
				private:

#ifdef HAVE_BOOST_SERIALIZATION

					friend class boost::serialization::access;
					
					template< class Archive >
					void save( Archive& ar, const unsigned int version ) const;
					
					template< class Archive >
					void load( Archive& ar, const unsigned int version );

					BOOST_SERIALIZATION_SPLIT_MEMBER()

#endif
						
					char* _data;
					
					unsigned int _size;
					
				public:
				
					class SimpleSerializableAllocator : public common::Serializable::Allocator
					{
					
						public:
						
							common::Serializable* allocate() const;
					
					};
					
					SimpleSerializable();
					
					~SimpleSerializable();
					
					unsigned int size() const;
					
					void resize( unsigned int size );
					
					void* data();
					
					common::Serializable::Id id() const;
					
					void serialize( common::SerializationBuffer& b ) const;
					
					void deserialize( common::SerializationBuffer& b );
					
					common::Serializable::Allocator* allocator() const;
					
					bool operator!=( const SimpleSerializable& object ) const;
			
			};
			
			bool testSaveLoad( std::string& status ) const;
			
			bool testDisk( std::string& status ) const;

#ifdef HAVE_MPI
			
			bool testMpi( std::string& status ) const;

#endif
			
			bool doTest( std::string& status );
		
		public:
		
			TestSerializableArchive();
		
			unsigned int iterations;
			
			unsigned int size;
			
			unsigned int rank;
			
			unsigned int ranks;
			
			bool verbose;
	
	
	};

#ifdef HAVE_BOOST_SERIALIZATION

	template< class Archive >
	void TestSerializableArchive::SimpleSerializable::save( Archive& ar, const unsigned int version ) const
	{
	
		ar & _size;
		
		for( unsigned int i = 0; i < _size; i++ )
		{
		
			ar & _data[i];
		
		}				
	
	}
	
	template< class Archive >
	void TestSerializableArchive::SimpleSerializable::load( Archive& ar, const unsigned int version )
	{
	
		unsigned int size;
	
		ar & size;
		
		resize( size );
		
		for( unsigned int i = 0; i < _size; i++ )
		{
		
			ar & _data[i];
		
		}				
	
	}
				
#endif

}

int main( int argc, char** argv );

#endif

