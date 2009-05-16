/*!
	\file TestSerializableArchive.h
	\author Gregory Diamos
	\date Sunday July 20, 2008	
	\brief Header file for the TestSerializableArchive class. 
	\bored Comments Written while sitting in a starbucks with some really noisy 
		people behind me.  Green tea lattes can't compete with chai tea 
		lattes...	
*/

#ifndef TEST_SERIALIZABLE_ARCHIVE_H_INCLUDED
#define TEST_SERIALIZABLE_ARCHIVE_H_INCLUDED

#include <implementation/SerializableArchive.h>
#include <implementation/ArgumentParser.h>
#include <interface/Test.h>

#ifdef HAVE_BOOST_SERIALIZATION

#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/split_member.hpp>

#endif

namespace test
{
	/*!
		\brief Make sure that the implementation of a Serializable archive works
			as intended.  
	
		Make sure that the performance is also reasonable.  Compare
		to the boost serialization library.  This should test and benchmark 
		saving to a buffer then immediately restoring, saving to and 
		restoring from disk, and saving to an archive, sending the archive 
		over mpi, and restoring at the other end.
	*/
	class TestSerializableArchive : public Test
	{
		private:
			class SimpleSerializable : public hydrazine::Serializable
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
					class SimpleSerializableAllocator : 
						public hydrazine::Serializable::Allocator
					{
						public:
							hydrazine::Serializable* allocate() const;
					};
					
					SimpleSerializable();
					~SimpleSerializable();

					unsigned int size() const;
					void resize( unsigned int size );
					void* data();					
					hydrazine::Serializable::Id id() const;
					void serialize( hydrazine::SerializationBuffer& b ) const;
					void deserialize( hydrazine::SerializationBuffer& b );
					hydrazine::Serializable::Allocator* allocator() const;
					bool operator!=( const SimpleSerializable& object ) const;			
			};
			
			bool testSaveLoad();
			bool testDisk();
#ifdef HAVE_MPI
			bool testMpi();
#endif
			bool doTest();
		
		public:
			TestSerializableArchive();

			unsigned int iterations;
			unsigned int size;
			unsigned int rank;
			unsigned int ranks;
	
	};

#ifdef HAVE_BOOST_SERIALIZATION

	template< class Archive >
	void TestSerializableArchive::SimpleSerializable::save( Archive& ar, 
		const unsigned int version ) const
	{
		ar & _size;
		
		for( unsigned int i = 0; i < _size; i++ )
		{
			ar & _data[i];
		}	
	}
	
	template< class Archive >
	void TestSerializableArchive::SimpleSerializable::load( Archive& ar, 
		const unsigned int version )
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

