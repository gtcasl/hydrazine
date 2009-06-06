/*!
	\file ValueCompare.h
	\date Wednesday May 27, 2009
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	\brief The header file for the ValueCompare class.	
*/
#ifndef VALUE_COMPARE_H_INCLUDED
#define VALUE_COMPARE_H_INCLUDED

namespace hydrazine
{
	/*!
		\brief A class for comparing key/value pairs based on the key only
	*/
	template< typename Key, typename Value, typename Compare, 
		typename Container >
	class ValueCompare : 
		public std::binary_function< std::pair< Key, Value >, 
			std::pair< Key, Value >, bool >
	{
		friend class Container::type;
		public:
			typedef size_t size_type;
			typedef Key key_type;
			typedef Value mapped_type;
			typedef std::pair< const key_type, mapped_type > value_type;
			typedef ptrdiff_t difference_type;
			typedef value_type* pointer;
			typedef value_type& reference;
			typedef const value_type* const_pointer;
			typedef const value_type& const_reference;
			
		protected:
			Compare compare;
	
		protected:
			ValueCompare( const Compare& c ) : compare( c ) {}
	
		public:
			bool operator()( const_reference x, const_reference y )
			{
				return compare( x.first, y.first );
			}

			bool operator()( const key_type& x, const_reference y )
			{
				return compare( x, y.first );
			}

			bool operator()( const_reference x, const key_type& y )
			{
				return compare( x.first, y );
			}
	};

}

#endif

