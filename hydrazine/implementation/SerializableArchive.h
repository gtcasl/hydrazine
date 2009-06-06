/*!
*	\file SerializableArchive.h
*
*	\author Gregory Diamos
*
*	\date 7/16/2008
*	
*	\brief 	The header file for the SerializableArchive class
*
*/

#ifndef SERIALIZABLE_ARCHIVE_H_INCLUDED
#define SERIALIZABLE_ARCHIVE_H_INCLUDED

#include <hydrazine/implementation/SerializationBuffer.h>
#include <hydrazine/interface/Serializable.h>
#include <fstream>
#include <map>

namespace hydrazine
{

	/*!
	
		\brief This class allows Serializable objects to be stored to and 
		loaded from.  
		
		It should automatically build a table of allocators and
		Serializable ids when objects are saved, however, they cannot
		be loaded without first being registered with the archive.
		
		In that case, the objects need to be registered using the appropriate
		functions before being loaded.
	
		This class also contains functions for storing and loading from a file.
	
	*/
	class SerializableArchive
	{
	
		private:
		
			/*!
			
				\brief The serialization buffer associated with this archive.
			
			*/
			SerializationBuffer _buffer;
		
			/*!
			
				\brief The map of Serializable classes to allocators that have been
				regsitered with this archive.
			
			*/
			std::map< Serializable::Id, Serializable::Allocator* > registeredClasses;
			
			/*!
			
				\brief Shorthand for an iterator
			
			*/
			typedef std::map< Serializable::Id, Serializable::Allocator* >::iterator iterator;
			
			/*!
			
				\brief Shorthand for a const_iterator
			
			*/
			typedef std::map< Serializable::Id, Serializable::Allocator* >::const_iterator constIterator;
			
		public:
		
			/*!
			
				\brief Destructor is needed to delete the allocators that have been 
				registered.
			
			*/
			~SerializableArchive();
			
			/*!
			
				\brief Register a Serializable object with the archive but do not 
				save it.
				
				\param s A pointer to the object being serialized
			
			*/
			void registerClass( const Serializable* s );
			
			/*!
			
				\brief Remove the class from the list of Serializable classes
				registered with the archive.
				
				\param s A pointer to an object of the class being removed.
			
			*/
			void removeRegistration( const Serializable* s );
			
			/*!
			
				\brief Remove the class from the list of Serializable classes 
				registered with the archive by id rather than instance.
				
				\param id The id referring to the class being removed
			
			*/
			void removeRegistration( Serializable::Id id );
			
			/*!
			
				\brief Save a Serializable object in the archive.
				
				The class associated with the object will be added if it is 
				not already in the archive.
				
				\param s A pointer to the object being serialized.
			
			*/
			void save( const Serializable* s );
			
			/*!
			
				\brief Load a Serializable object from the archive.  
				
				These will be returned in the order that they were saved.
			
				\return A pointer to a newly allocated object that was loaded.
			
			*/
			Serializable* load();
			
			/*!
			
				\brief Initialize a Serializable object by loading from the archive.
				
				By doing this you must ensure that the object pointed to by s 
				is the same type as the object that will be loaded next.
			
				\param s A pointer to the object being loaded to.
			
			*/
			void load( Serializable* s );
	
			/*!
			
				\brief Clear all objects stored in the archive.
				
				This will not remove any registered classes.
			
			*/
			void clear();
			
			/*!
			
				\brief Clear all objects and remove all registered classes.
			
			*/
			void clobber();
	
			/*!
			
				\brief Save the archive to a file.
				
				Note that this will not clear any objects stored in the archive.
				
				\param fileName The name of the file that the archive should be 
				written to.
			
			*/
			void saveToFile( const std::string& fileName ) const;
			
			/*!
			
				\brief Load an archive from a file.
				
				Note that this will clear any objects stored in the archive before
				doing the load.
				
				\param fileName The name of the file that the archive should be 
				loaded from.
			
			*/
			void loadFromFile( const std::string& fileName );
			
			/*!
			
				\brief Get direct access to the underlying SerializationBuffer.  
				
				This is a poweruser feature that should be used at your own risk
				in performance code only.
			
			*/
			SerializationBuffer& buffer();
	
	
	};

}

#endif

