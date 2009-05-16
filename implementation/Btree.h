/*!
	\file Btree.h
	\date Wednesday May 13, 2009
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	\brief The header file for the Btree class
*/

#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED

#include <util>
#include <implementation/MmapAllocator.h>

namespace hydrazine
{

	/*!
		\brief A Btree data structure providing the STL map interface.
	*/
	template< typename Key, typename Value, typename Compare = std::less<Key>, 
		typename _Allocator = common::MmapAllocator< std::pair< const Key, 
		Value > >, size_t PageSize = 1024 >
	class BTree
	{
		public:
			class Iterator;
			class ConstIterator;

		public:
			typedef Key key_type;
			typedef Value mapped_type;
			typedef std::pair< const key_type, mapped_type > value_type;
		
		public:
			typedef typename _Allocator::template rebind< value_type >::other
				Allocator

		public:			
			typedef Compare key_compare;
			typedef Allocator allocator_type;
			typedef typename Allocator::reference reference;
			typedef typename Allocator::const_reference const_reference;
			typedef Iterator iterator;
			typedef ConstIterator const_iterator;
			typedef typename Allocator::size_type size_type;
			typedef typename Allocator::difference_type difference_type;
			typedef typename Allocator::pointer pointer;
			typedef typename Allocator::const_pointer const_pointer;
			typedef std::reverse_iterator< iterator > reverse_iterator;
			typedef std::reverse_iterator< const_iterator > 
				const_reverse_iterator;
		
		public:
			class value_compare : 
				public std::binary_function< value_type, value_type, bool >
			{
				friend class BTree;
				
				protected:
					Compare compare;
				
				protected:
					value_compare( const Compare& c ) : compare( c ) {}
				
				public:
					bool operator()( const_reference x, const_reference y )
					{
						return comp( x.first, y.first );
					}
			};
			
		private:
			Allocator _allocator;
			value_compare _compare;
			key_compare _keyCompare;
		
		private:
			class Leaf;
			class Node;
			
			/*!
				\brief A combined lead/node pointer
			*/
			union LeafNodePointer
			{
				Leaf* leaf;
				Node* node;
			};

			typedef LeafNodePointer NodeMappedType;
			typedef std::pair< const key_type, NodeMappedType > NodeValueType;
		
			/*!
				\brief An internal node
			*/
			class Node
			{
				public:
					typedef NodeAllocator::pointer Pointer;
					typedef common::Iterator< Pointer > Iterator;
				
				private:
					Allocator _allocator;
					BTree* _tree;
					Iterator _begin;
					Iterator _end;
					Node* _parent;
					size_t _index;
					bool _body;
				
				public:
					Node( Btree* tree ) : _tree( tree ), 
						_begin( _tree._allocator.allocate( PageSize ) ), 
						_end( _begin ), _parent( 0 ), _index( 0 ), 
						_body( false )
					{

					}
					
					Node( const Node& n ) : _tree( n._tree ), 
						_allocator( n._allocator ), 
						_begin( _allocator.allocate( PageSize ) ), 
						_parent( n._parent ), _index( n._index ), _body( false )
					{
						size_type size = n._end - n_begin;
						_end = _begin + size;
						memcpy( _begin, n._begin, size * sizeof( ValueType ) );
					}
					
					~Node()
					{
						_allocator.deallocate( _begin, PageSize );
					}
					
					Node& operator=( const Node& n )
					{
						assert( _tree == n._tree );
						assert( &n != this );
						size_type size = n._end - n_begin;
						_end = _begin + size;
						_index = n._index;
						memcpy( _begin, n._begin, size * sizeof( ValueType ) );						
						return *this;
					}
					
				private:
					
					Iterator lower_bound( const key_type& key )
					{
						return std::lower_bound( _begin, _end, 
							_tree._compare );
					}
					
					std::pair< iterator, bool > insert( const ValueType& val );
					
					void erase( iterator i );
			};
			
			/*!
				\brief An internal leaf
			*/
			class Leaf
			{
				public:
					typedef common::Iterator< pointer > iterator;
					typedef common::Iterator< const_pointer > const_iterator;

				private:
					value_type _data[ PageSize ];
					iterator _begin;
					iterator _end;
					Node* _parent;
					size_t _index;

				public:
					Leaf( ) : _begin( _data ), 
						_end( _begin ), _parent( 0 ), _index( 0 )
					{
					}
					
					Leaf( const Leaf& n ) : _allocator( n._allocator ), 
						_begin( _allocator.allocate( PageSize ) ), 
						_parent( n._parent ), _index( n._index )
					{
						size_type size = n._end - n_begin;
						_end = _begin + size;
						memcpy( _begin, n._begin, size * sizeof( value_type ) );
					}
					
					~Leaf()
					{
						_allocator.deallocate( _begin, PageSize );
					}
					
					Leaf& operator=( const Leaf& n )
					{
						assert( &this != n );
						size_type size = n._end - n_begin;
						_end = _begin + size;
						_index = n._index;
						memcpy( _begin, n._begin, size * sizeof( value_type ) );						
						return *this;
					}
					
				public:
					Leaf* next() const
					{
						size_t depth = 0;
						size_t index = _index + 1;
						bool rollover = false;
						
						assert( _parent != 0 );
						const Node* node = _parent;

						// run up the tree
						while( node->_begin + index == node->_end )
						{
							if( node->_parent == 0 )
							{
								rollover = true;
								break;
							}
							index = node->_index + 1;
							node = node->_parent;
							++depth;
						}
						
						if( depth > 0 )
						{
							node = ( node->_begin + index )->second.node;
							index = 0;
						}
						
						// run down the tree
						for( unsigned int i = 1; i < depth; ++i )
						{
							node = node->_begin->second.node;
						}
						
						if( rollover )
						{
							return ( node->_end - 1 )->second.leaf;
						}
						else
						{
							return ( node->_begin + index )->second.leaf;
						}
					}

					Leaf* previous() const
					{
						size_t depth = 0;
						size_t index = _index;
						
						assert( _parent != 0 );
						const Node* node = _parent;

						// run up the tree
						while( index == 0 )
						{
							if( node->_parent == 0 )
							{
								index = node->_end - node->begin;
								break;
							}
							index = node->_index;
							node = node->_parent;
							++depth;
						}
						
						--index;
						
						if( depth > 0 )
						{
							node = ( node->_begin + index )->second.node;
						}
						
						// run down the tree
						for( unsigned int i = 1; i < depth; ++i )
						{
							node = ( node->_end - 1 )->second.node;
						}
						
						return ( node->_end - 1 )->second.leaf;
					}

			};
			
		public:
		
			class Iterator
			{
				friend class BTree;
				public:
					typedef value_type value_type;
					typedef distance_type distance_type;
					typedef Leaf::iterator pointer;
					typedef reference reference;

				private:
					Leaf* _leaf;
					pointer _current;
				
				public:
					Iterator() {}
					
					reference operator*() const
					{
						return *_current;
					}
					
					pointer operator->() const
					{
						return _current;
					}
					
					Iterator& operator++()
					{
						++_current;
						if( _current == _leaf->end() )
						{
							_leaf = _leaf->next();
							_current = _leaf->begin();
						}
					}
					
					Iterator operator++( int )
					{
						Iterator next( *this );
						++next;
						return std::move( next );	
					}
					
					Iterator& operator--()
					{
						while( _current == _leaf->begin() )
						{
							_leaf = _leaf->previous();
							assert( _leaf->begin() != _leaf->end() );
							_current = _leaf->end() - 1;
						}
						--_current;
					}
					
					Iterator operator--( int )
					{
						Iterator previous( *this );
						--previous;
						return std::move( previous );	
					}
					
					const pointer& base() const
					{
						return _current;
					}
				
			};

		public:
		
			class ConstIterator
			{
				friend class BTree;
				public:
					typedef value_type value_type;
					typedef distance_type distance_type;
					typedef Leaf::const_iterator pointer;
					typedef const_reference reference;

				private:
					Leaf* _leaf;
					pointer _current;
				
				public:
					Iterator() {}
					
					reference operator*() const
					{
						return *_current;
					}
					
					pointer operator->() const
					{
						return _current;
					}
					
					Iterator& operator++()
					{
						++_current;
						if( _current == _leaf->end() )
						{
							_leaf = _leaf->next();
							_current = _leaf->begin();
						}
					}
					
					Iterator operator++( int )
					{
						Iterator next( *this );
						++next;
						return std::move( next );	
					}
					
					Iterator& operator--()
					{
						while( _current == _leaf->begin() )
						{
							_leaf = _leaf->previous();
							assert( _leaf->begin() != _leaf->end() );
							_current = _leaf->end() - 1;
						}
						--_current;
					}
					
					Iterator operator--( int )
					{
						Iterator previous( *this );
						--previous;
						return std::move( previous );	
					}
					
					const pointer& base() const
					{
						return _current;
					}
				
			};
			
			/*!
				\brief Internal Data
			*/
		private:
			Node _root;
			iterator _begin;
			iterator _end;
			size_t _size;

			/*!
				\brief Copy/Construct/Destroy
			*/
		public:
			explicit BTree( const Compare& comp = Compare(), 
				const Allocator& alloc = Allocator() ) : 
				_allocator( alloc ), _compare( comp ), _size( 0 ) {}

			template < typename InputIterator >			
			BTree( InputIterator first, InputIterator last, 
				const Compare& comp = Compare(), 
				const Allocator& alloc = Allocator() ) : 
				_allocator( alloc ), _compare( comp ), _size( 0 )
			{
				for( InputIterator fi = first; fi != last; ++fi )
				{
					insert( *fi );
				}
			}
			
			BTree( const BTree& tree ) : 
				_allocator( tree._allocator ), _compare( tree._compare )
			{
				if( tree._size > 0 )
				{
					_size = tree._size;
					_root = _tree._root;
				}
			}
			
			~BTree()
			{
			
			}
			
			BTree& operator=( const BTree& tree )
			{
				assert( &tree != this );
				_size = tree._size;
				_root = tree._root;
				return *this;
			}
			
			/*!
				\brief Iterators
			*/
		public:
			iterator begin()
			{
				return _begin;
			}
			
			const_iterator begin() const
			{
				return _begin;
			}
			
			iterator end()
			{
				return _end;
			}
			
			const_iterator end() const
			{
				return _end;
			}
			
			reverse_iterator rbegin()
			{
				return reverse_iterator( end() );
			}
			
			const_reverse_iterator rbegin() const
			{
				return const_reverse_iterator( end() );
			}
			
			reverse_iterator rend()
			{
				return reverse_iterator( begin() );
			}
			
			const_reverse_iterator rend() const
			{
				return const_reverse_iterator( begin() );
			}
			
			/*!
				\brief Capacity
			*/
		public:
			bool empty() const
			{
				return size() == 0;
			}
			
			size_type size() const
			{
				return _size;
			}
			
			size_type max_size() const
			{
				return _allocator.max_size();
			}
			
			/*!
				\brief Element access
			*/
		public:
			reference operator[]( const key_type& key )
			{
				iterator fi = lower_bound( key );
				
				if( fi == end() )
				{
					fi = _root.insert( fi, 
						std::make_pair( key, mapped_type() ) );					
				}
				else
				{
					if( _compare( *fi, key ) || _compare( key, *fi )
					{
						fi = _root.insert( fi, 
							std::make_pair( key, mapped_type() ) );
					}
				}
				
				return fi->second;
			}
		
			/*!
				\brief Modifiers
			*/
		public:
			pair< iterator, bool > insert( const value_type& x )
			{
				pair< iterator, bool > fi( lower_bound( x.first ), true );
				
				if( fi.first->first == end() )
				{
					fi.first = _root.insert( fi.first, 
						std::make_pair( x.first, mapped_type() ) );
					fi.second = false;					
				}
				else
				{
					if( _compare( *fi.first, x.first ) 
						|| _compare( x.first, *fi.first )
					{
						fi.first = _root.insert( fi.first, 
							std::make_pair( x.first, mapped_type() ) );
						fi.second = false;
					}
				}
				
				return fi;	
			}

			iterator insert( iterator position, const value_type& x )
			{
				iterator guess = lower_bound( x );
				return _root.insert( guess, x );
			}

			template < typename InputIterator >
			void insert( InputIterator first, InputIterator last )
			{
				for( InputIterator fi = first; fi != last; ++fi )
				{
					_insert( *fi );
				}
			}
			
			void erase( iterator position )
			{
				iterator._node->erase( position );
			}
			
			size_type erase( const key_type& x )
			{
				iterator position = find( x );
				if( x != end() )
				{
					iterator._node->erase( position );
					return 1;
				}
				return 0;
			}
			
			void erase( iterator first, iterator last )
			{
				_root.erase( first, second );
			}
			
			void swap( BTree< Key, T, Compare, Allocator >& tree )
			{
				std::swap( _size, tree._size );
				std::swap( _begin._leaf, tree._begin._leaf );
				std::swap( _begin._current, tree._begin._current );
				std::swap( _end._leaf, tree._end._leaf );
				std::swap( _end._current, tree._end._current );
				std::swap( _root._tree, tree._root._tree );
				std::swap( _root._begin, tree._root._begin );
				std::swap( _root._end, tree._root._end );
				std::swap( _root._parent, tree._root._parent );
				std::swap( _root._index, tree._root._index );
				std::swap( _root._body, tree._root._body );
			}

			void clear()
			{
				_root.clear();
			}
			
			/*!
				\brief Observers
			*/
		public:
			key_compare key_comp() const
			{
				return _compareKey;
			}
			
			value_compare value_comp() const
			{
				return _compare;
			}
			
			/*!
				\brief Map Operations
			*/
		public:
			iterator find( const key_type& x )
			{
				itertator result = lower_bound( x );
				if( result != end() )
				{
					if( !_compareKey( x, result->first ) )
					{
						return result;
					}
				}
				return end();
			}
			
			const_iterator find( const key_type& x ) const
			{
				const_itertator result = lower_bound( x );
				if( result != end() )
				{
					if( !_compareKey( x, result->first ) )
					{
						return result;
					}
				}
				return end();
			}
			
			size_type count( const key_type& x ) const
			{
				return find( x ) != end();
			}
			
			iterator lower_bound( const key_type& x )
			{
				return _root.lower_bound( x );
			}
			
			const_iterator lower_bound( const key_type& x ) const
			{
				return _root.lower_bound( x );
			}
			
			iterator upper_bound( const key_type& x )
			{
				itertator result = lower_bound( x );
				if( result != end() )
				{
					++result;
				}
				return result;
			}

			const_iterator upper_bound( const key_type& x ) const
			{
				const_itertator result = lower_bound( x );
				if( result != end() )
				{
					++result;
				}
				return result;
			}

			pair< iterator, iterator > equal_range( const key_type& x )
			{
				pair< iterator, iterator > result;
				result.first = lower_bound( x );
				result.second = result.first;
				if( result.second != end() )
				{
					++result.second;
				}
				return result;
			}
			
			pair< const_iterator, const_iterator > equal_range( 
				const key_type& x ) const
			{
				pair< iterator, iterator > result;
				result.first = lower_bound( x );
				result.second = result.first;
				if( result.second != end() )
				{
					++result.second;
				}
				return result;			
			}
			
	};
	
	template <typename Key, typename T, typename Compare, typename Allocator, 
		size_t PageSize>
	bool operator==(const BTree< Key, T, Compare, Allocator, PageSize>& x,
		const BTree< Key, T, Compare, Allocator, PageSize >& y)
	{
		if( x.size() != y.size() )
		{
			return false;
		}
		return std::equal( x.begin(), x.end(), y.begin() );
	}

	template < typename Key, typename T, typename Compare, typename Allocator, 
		size_t PageSize >
	bool operator< (const BTree< Key, T, Compare, Allocator, PageSize >& x,
		const BTree< Key, T, Compare, Allocator, PageSize >& y)
	{
		return std::lexogrpahical_compare( x.begin(), x.end(), y.begin(), 
			y.end(), x.value_compare() );
	}

	template < typename Key, typename T, typename Compare, typename Allocator, 
		size_t PageSize >
	bool operator!=(const BTree< Key, T, Compare, Allocator, PageSize >& x,
		const BTree< Key, T, Compare, Allocator, PageSize >& y)
	{
		return !( x == y );
	}
	
	template < typename Key, typename T, typename Compare, typename Allocator, 
		size_t PageSize >
	bool operator> (const BTree< Key, T, Compare, Allocator, PageSize >& x,
		const BTree< Key, T, Compare, Allocator, PageSize >& y)
	{
		return y < x;
	}
	
	template < typename Key, typename T, typename Compare, typename Allocator, 
		size_t PageSize >
	bool operator>=(const BTree< Key, T, Compare, Allocator, PageSize >& x,
		const BTree< Key, T, Compare, Allocator, PageSize >& y);
	{
		return !( x < y )
	}
	
	template < typename Key, typename T, typename Compare, typename Allocator, 
		size_t PageSize >
	bool operator<=(const BTree< Key, T, Compare, Allocator, PageSize >& x,
		const BTree< Key, T, Compare, Allocator, PageSize >& y)
	{
		return !( x > y );
	}
	
	// specialized algorithms:
	template < typename Key, typename T, typename Compare, typename Allocator, 
		size_t PageSize >
	void swap( BTree< Key, T, Compare, Allocator, PageSize >& x,
		BTree< Key, T, Compare, Allocator, PageSize >& y )
	{
		x.swap( y );
	}
}

#endif

