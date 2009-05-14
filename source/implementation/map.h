/*!

	\file map.h
	
	\date Sunday February 8, 2009
	
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	
	\brief The source file for the map class

*/

#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <deque>
#include <algorithm>
#include <iterator>

namespace common
{

	/*!
	
		\brief A reimplementation of the STL map interface aimed at
			performance using a sorted deque as the underlying container.
	
	*/
	template< typename _Key, typename _Tp, typename _Compare = std::less<_Key>, 
		typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
	class map
	{
	
		public:
		
			typedef _Key key_type;
			typedef _Tp mapped_type;
			typedef std::pair< const _Key, _Tp > value_type;
			typedef _Compare key_compare;
			typedef _Alloc allocator_type;
			typedef unsigned int size_type;
	
		public:

			class value_compare : public std::binary_function< value_type, 
				value_type, bool >
			{

				friend class map< _Key, _Tp, _Compare, _Alloc >;

				protected:
					_Compare comp;
					value_compare(_Compare __c);
					
				public:
					bool operator()( const value_type& __x, 
						const value_type& __y ) const;
					bool operator()( const key_type& __x, 
						const value_type& __y ) const;
					bool operator()( const value_type& __x, 
						const key_type& __y ) const;
			};

		private:
		
			typedef std::pair< _Key, _Tp > InternalType;
			typedef std::deque< InternalType, allocator_type > Deque;

		public:

			typedef typename Deque::iterator iterator;
			typedef typename Deque::const_iterator const_iterator;

			typedef typename Deque::reverse_iterator reverse_iterator;
			typedef typename Deque::const_reverse_iterator 
				const_reverse_iterator;
	
		private:
		
			bool _sorted;
			Deque _deque;
			value_compare _compare;
		
		private:
		
			void _sort();
		
		public:
		
			typedef std::pair< iterator, iterator > IteratorPair;
			typedef std::pair< iterator, bool > IteratorBool;
	
		public:
		
			map();
			map( const map& m );
			map( iterator start, iterator end );
			map( iterator start, iterator end, const key_compare& cmp );
			map( const key_compare& cmp );
			~map();

			mapped_type& operator[]( const key_type& key );
			map operator=(const map& c2);
			bool operator==(const map& c1);
			bool operator!=(const map& c1);
			bool operator<(const map& c1);
			bool operator>(const map& c1);
			bool operator<=(const map& c1);
			bool operator>=(const map& c1);
	
			iterator begin();
			const_iterator begin() const;
			void clear();
			size_type count(const key_type& key);
			bool empty() const;
			iterator end();
			const_iterator end() const;
			IteratorPair equal_range( const key_type& key );
		    void erase( iterator pos );
			void erase( iterator start, iterator end );
			size_type erase( const key_type& key );
			iterator find( const key_type& key );
			iterator insert( iterator pos, const value_type& pair );
			void insert( iterator start, iterator end );
			IteratorBool insert( const value_type& pair );
			key_compare key_comp() const;
			iterator lower_bound( const key_type& key );
			size_type max_size() const;
			reverse_iterator rbegin();
			const_reverse_iterator rbegin() const;
			reverse_iterator rend();
			const_reverse_iterator rend() const;
			size_type size() const;
			void swap( map& from );
			iterator upper_bound( const key_type& key );
			value_compare value_comp() const;
			
	};

	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	map<_Key, _Tp, _Compare, _Alloc>::value_compare::value_compare( 
		_Compare __c ) : comp( __c )
	{
	
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	bool map<_Key, _Tp, _Compare, _Alloc>::value_compare::operator()( 
		const value_type& __x, const value_type& __y ) const
	{
	
		return comp(__x.first, __y.first);
	
	}

	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	bool map<_Key, _Tp, _Compare, _Alloc>::value_compare::operator()( 
		const key_type& __x, const value_type& __y ) const
	{
	
		return comp(__x, __y.first);
	
	}

	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	bool map<_Key, _Tp, _Compare, _Alloc>::value_compare::operator()( 
		const value_type& __x, const key_type& __y ) const
	{
	
		return comp(__x.first, __y);
	
	}

	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	void map<_Key, _Tp, _Compare, _Alloc>::_sort()
	{

		if( !_sorted )
		{
		
			_sorted = true;
			std::sort( _deque.begin(), _deque.end(), _compare );
		
		}
	
	}

	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	map<_Key, _Tp, _Compare, _Alloc>::map() : _compare( _Compare() )
	{

		_sorted = true;
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	map<_Key, _Tp, _Compare, _Alloc>::map( const map& m )
	{
	
		_deque = m._deque;
		_compare = m._compare;
		_sorted = m._sorted;
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	map<_Key, _Tp, _Compare, _Alloc>::map( iterator start, iterator end )
	{
	
		for( iterator fi = start; fi != end; ++fi )
		{
		
			_deque.push_back( *fi );
			_sorted = false;
		
		}
	
	}

	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	map<_Key, _Tp, _Compare, _Alloc>::map( iterator start, iterator end, 
		const key_compare& cmp ) : _compare( cmp )
	{
	
		for( iterator fi = start; fi != end; ++fi )
		{
		
			_deque.push_back( *fi );
			_sorted = false;
		
		}
	
	}

	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	map<_Key, _Tp, _Compare, _Alloc>::map( 
		const key_compare& cmp ) : _compare( cmp )
	{
	
		_sorted = true;
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	map<_Key, _Tp, _Compare, _Alloc>::~map()
	{
	
	
	}

	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::mapped_type& 
		map<_Key, _Tp, _Compare, _Alloc>::operator[]( const key_type& key )
	{
	
		_sort();
	
		iterator item = std::lower_bound( _deque.begin(), _deque.end(), key, 
			_compare );
			
		if( item == end() || _compare( key, item->first ) )
		{
		
			item = insert( item, value_type( key, mapped_type() ) );
		
		}
		
		return item->second;
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	map<_Key, _Tp, _Compare, _Alloc> 
		map<_Key, _Tp, _Compare, _Alloc>::operator=(const map& c2)
	{
	
		_compare = c2._compare;
		_deque = c2._deque;
		_sorted = c2._sorted;
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	bool map<_Key, _Tp, _Compare, _Alloc>::operator==(const map& c1)
	{
	
		return _deque == c1._deque;
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	bool map<_Key, _Tp, _Compare, _Alloc>::operator!=(const map& c1)
	{
	
		return _deque != c1._deque;
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	bool map<_Key, _Tp, _Compare, _Alloc>::operator<(const map& c1)
	{
	
		return _deque < c1._deque;
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	bool map<_Key, _Tp, _Compare, _Alloc>::operator>(const map& c1)
	{
	
		return _deque > c1._deque;
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	bool map<_Key, _Tp, _Compare, _Alloc>::operator<=(const map& c1)
	{
	
		return _deque <= c1._deque;
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	bool map<_Key, _Tp, _Compare, _Alloc>::operator>=(const map& c1)
	{
	
		return _deque >= c1._deque;
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::iterator 
		map<_Key, _Tp, _Compare, _Alloc>::begin()
	{

		_sort();	
		return _deque.begin();
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::const_iterator 
		map<_Key, _Tp, _Compare, _Alloc>::begin() const
	{
	
		_sort();
		return _deque.begin();
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	void map<_Key, _Tp, _Compare, _Alloc>::clear()
	{
	
		_sorted = true;
		_deque.clear();
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::size_type 
		map<_Key, _Tp, _Compare, _Alloc>::map::count(const key_type& key)
	{
	
		_sort();
	
		IteratorPair pair = std::equal_range( key );
		
		size_type size = std::distance( pair.first, pair.second );
	
		return size;
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	bool map<_Key, _Tp, _Compare, _Alloc>::empty() const
	{
	
		return _deque.empty();
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::iterator 
		map<_Key, _Tp, _Compare, _Alloc>::end()
	{
	
		_sort();
		return _deque.end();
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::const_iterator 
		map<_Key, _Tp, _Compare, _Alloc>::end() const
	{
	
		_sort();
		return _deque.end();
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::IteratorPair 
		map<_Key, _Tp, _Compare, _Alloc>::equal_range( const key_type& key )
	{
	
		_sort();
		return std::equal_range( begin(), end(), key, _compare );
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	void map<_Key, _Tp, _Compare, _Alloc>::erase( iterator pos )
	{
	
		_deque.erase( pos );
	
	}

	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	void map<_Key, _Tp, _Compare, _Alloc>::erase( iterator start, 
		iterator end )
	{
	
		_deque.erase( start, end );
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::size_type 
		map<_Key, _Tp, _Compare, _Alloc>::erase( const key_type& key )
	{
	
		_sort();
		
		IteratorPair range = equal_range( key );
	
		size_type erased = std::distance( range.first, range.second ); 
	
		_deque.erase( range.first, range.second );
		
		return erased;
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::iterator 
		map<_Key, _Tp, _Compare, _Alloc>::find( const key_type& key )
	{
	
		_sort();
		return std::lower_bound( begin(), end(), key, _compare );
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::iterator 
		map<_Key, _Tp, _Compare, _Alloc>::insert( iterator pos, 
		const value_type& pair )
	{
	
		_sorted = false;
		
		_deque.push_back( pair );
		
		return --_deque.end();
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	void map<_Key, _Tp, _Compare, _Alloc>::insert( iterator start, 
		iterator end )
	{
	
		_sorted = false;
	
		for( iterator fi = start; fi != end; ++fi )
		{
		
			_deque.push_back( *fi );
		
		}
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::IteratorBool 
		map<_Key, _Tp, _Compare, _Alloc>::insert( const value_type& pair )
	{
	
		_sorted = false;
		
		_deque.push_back( pair );
		
		return IteratorBool( --_deque.end(), true );
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::key_compare 
		map<_Key, _Tp, _Compare, _Alloc>::key_comp() const
	{
	
		return _compare.comp;
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::iterator 
		map<_Key, _Tp, _Compare, _Alloc>::lower_bound( const key_type& key )
	{
	
		return std::lower_bound( begin(), end(), key, _compare );
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::size_type 
		map<_Key, _Tp, _Compare, _Alloc>::max_size() const
	{
	
		return _deque.max_size();
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::reverse_iterator 
		map<_Key, _Tp, _Compare, _Alloc>::rbegin()
	{
	
		_sort();
		return _deque.rbegin();
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::const_reverse_iterator 
		map<_Key, _Tp, _Compare, _Alloc>::rbegin() const
	{
	
		_sort();
		return _deque.rbegin();
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::reverse_iterator 
		map<_Key, _Tp, _Compare, _Alloc>::rend()
	{
	
		_sort();
		return _deque.rend();
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::const_reverse_iterator 
		map<_Key, _Tp, _Compare, _Alloc>::rend() const
	{
	
		_sort();
		return _deque.rend();
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::size_type 
		map<_Key, _Tp, _Compare, _Alloc>::size() const
	{
	
		return _deque.size();
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	void map<_Key, _Tp, _Compare, _Alloc>::swap( 
		map<_Key, _Tp, _Compare, _Alloc>& from )
	{
	
		std::swap( *this, from );
	
	}
	
	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::iterator 
		map<_Key, _Tp, _Compare, _Alloc>::upper_bound( const key_type& key )
	{
	
		return std::upper_bound( begin(), end(), key, _compare );
	
	}

	template< typename _Key, typename _Tp, typename _Compare, typename _Alloc >
	typename map<_Key, _Tp, _Compare, _Alloc>::value_compare 
		map<_Key, _Tp, _Compare, _Alloc>::value_comp() const
	{
	
		return _compare;
	
	}
		
}

#endif

