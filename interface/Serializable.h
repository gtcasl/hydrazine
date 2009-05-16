/*!	\file Serializable.h
*
*	\brief Header file for the Serializable class
*
*/

#ifndef SERIALIZABLE_H_INCLUDED
#define SERIALIZABLE_H_INCLUDED

#include <implementation/SerializationBuffer.h>

/*!
	\brief a namespace for common classes and functions
*/


namespace hydrazine
{
	
	////////////////////////////////////////////////////////////////////////////
	/*!
	
		\brief This class contains a name, along with functions for serializing
				and deserializing from a serializationBuffer.
	
	*/

	class Serializable
	{
	
		public:
		
			/*!
			
				\brief A unique id for this Serializable class.  This should 
				be unique among all serialzable classes being stored in a database.
			
			*/
			typedef unsigned int Id;
			
			
			/*!
			
				\brief A functor that is used to allocate a Serializable class of 
				a specific type.
				
			*/
			class Allocator
			{
			
				public:
				
					virtual Serializable* allocate() const = 0;
			
			};
		
		public:
		
			/*!
			
				\brief Determine the id of this specific class
				
				\return The id of this class
			
			*/
			virtual Id id() const = 0;
		
			/*!
			
				\brief Virtual destructor
			
			*/
			virtual ~Serializable() = 0;
			
			/*!
			
				\brief Deserialize this object from the specified serializationBuffer
				
				\param b The serialization buffer to restore from.
						
			*/
			virtual void deserialize(SerializationBuffer& b) = 0;
			
			/*!
			
				\brief Serialize this object into the specified serializationBuffer
				
				\param b The buffer to store into.
			
			*/
			virtual void serialize(SerializationBuffer& b) const = 0;
			
			/*!
			
				\brief Get a new allocator object for this Serializable class.
				
				Note that this function allocates an object that needs to be deleted
				at some point.
				
				\return The allocator object.
			
			*/
			virtual Allocator* allocator() const = 0;
			
			
	};
	////////////////////////////////////////////////////////////////////////////

}//ext

#endif
