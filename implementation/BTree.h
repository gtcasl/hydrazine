/*!
	\file BTree.h
	\date Wednesday May 13, 2009
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	\brief The header file for the BTree class
*/

#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED

#include <implementation/debug.h>
#include <implementation/macros.h>

#ifdef REPORT_BASE
#undef REPORT_BASE
#endif

#define REPORT_BASE 1
#define REPORT_NODE 1
#define REPORT_TREE 1
#define REPORT_LEAF 1

#include <stack>
#include <cstring>
#include <utility>

namespace hydrazine
{

	/*!
		\brief A Btree data structure providing the STL map interface.
	*/
	template< typename Key, typename Value, typename Compare = std::less<Key>, 
		typename _Allocator = std::allocator< std::pair< const Key, 
		Value > >, size_t PageSize = 1024 >
	class BTree
	{
		public:
			class Iterator;
			class ConstIterator;

		public:
			typedef Key key_type;
			typedef Value mapped_type;
			typedef std::pair< key_type, mapped_type > value_type;
		
		public:
			typedef typename _Allocator::template rebind< value_type >::other
				Allocator;

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
			typedef std::pair< iterator, bool > insertion;
				
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
			
		private:
			Allocator _allocator;
			value_compare _compare;
			key_compare _keyCompare;
		
		private:
			class Body;
			class Node;
			class Leaf;
		
		private:
			static const size_type MaxNodes = MAX( 8, 
				PageSize / ( sizeof( key_type ) + sizeof( mapped_type ) ) );
			static const size_type MaxLeafs = MAX( 8, 
				PageSize / ( sizeof( key_type ) + sizeof( Node* ) ) );
			static const size_type MinNodes = MaxNodes / 2;
			static const size_type MinLeafs = MaxLeafs / 2;
	
		private:
			typedef typename _Allocator::template rebind< Body >::other
				BodyAllocator;
			typedef typename _Allocator::template rebind< Leaf >::other
				LeafAllocator;

		private:
			BodyAllocator _bodyAllocator;
			LeafAllocator _leafAllocator;
		
		private:
			/*!
				\brief A base class for an internal node.
			*/
			class Node
			{
				public:
					size_type level;
					size_type size;
					
				public:
					inline void construct( size_type l )
					{
						 level = l;
						 size = 0;
					}
					
					inline bool leaf() const
					{
						return level == 0;
					}
			};

			/*!
				\brief Body Node
			*/
			class Body : public Node
			{
				public:
					key_type keys[ MaxNodes ];
					Node* children[ MaxNodes + 1 ];
										
				public:
					inline void construct( const size_type level )
					{
						Node::construct( level );
					}
					
					inline bool full() const
					{
						return size == MaxNodes;
					}
					
					inline bool deficient() const
					{
						return size < MinNodes;
					}
					
					inline bool borderline() const
					{
						return size <= MinNodes;
					}
			};
			
			/*!
				\brief A leaf node
			*/
			class Leaf : public Node
			{
				public:
					Leaf* previous;
					Leaf* next;
					value_type data[ MaxLeafs ];
				
				public:
					inline void construct()
					{
						Node::construct( 0 );
						previous = 0;
						next = 0;
					}
					
					inline bool full() const
					{
						return size == MaxLeafs;
					}
					
					inline bool deficient() const
					{
						return size < MinLeafs;
					}
					
					inline bool borderline() const
					{
						return size <= MaxLeafs;
					}
					
					inline std::pair< iterator, bool > 
						insert( const value_type& value, 
						value_compare& compare )
					{
						assert( !full() );
						value_type* position = std::lower_bound( data, 
							data + size, value, compare );
						if( position != data + size )
						{
							if( !compare( value, *position ) )
							{
								report( " Insert failed." );
								return std::make_pair( 
									iterator( 0, 0 ), false );
							}
						}
						
						report( " Insert succeeded." );
						std::copy_backward( position, data + size, 
							data + size + 1 );
						*position = value;
						++size;
						return std::make_pair( 
							iterator( this, position - data ), true );
					}
			};

		public:
		
			class Iterator
			{
				friend class BTree;
				friend class Node;
				friend class Leaf;

				private:
					Leaf* _leaf;
					size_type _current;

				private:
					inline Iterator( Leaf* l, size_type c ) : 
						_leaf( l ), _current( c )
					{}
					
				public:
					inline Iterator( const Iterator& i ) : _leaf( i._leaf ), 
						_current( i._current ) {}
					inline Iterator& operator=( const Iterator& i )
					{
						_leaf = i._leaf;
						_current = i._current;
					}
					
					inline reference operator*() const
					{
						assert( _current < _leaf->size );
						return _leaf->data[ _current ];
					}
					
					inline pointer operator->() const
					{
						assert( _current < _leaf->size );
						return &_leaf->data[ _current ];
					}
					
					inline Iterator& operator++()
					{
						if( _current + 1 < _leaf->size )
						{
							++_current;
						}
						else if( 0 != _leaf->next )
						{
							_current = 0;
							_leaf = _leaf->next;
						}
						else
						{
							_current = _leaf->size;
						}
						return *this;
					}
					
					inline Iterator operator++( int )
					{
						Iterator next( *this );
						++next;
						return std::move( next );	
					}
					
					inline Iterator& operator--()
					{
						if( _current != 0 )
						{
							--_current;
						}
						else if( _leaf->previous != 0 )
						{
							_leaf = _leaf->previous;
							_current = _leaf->size - 1;		
						}
						else
						{
							_current = 0;
						}
						return *this;
					}
					
					inline Iterator operator--( int )
					{
						Iterator previous( *this );
						--previous;
						return std::move( previous );	
					}
					
				public:
					bool operator==( const ConstIterator& i )
					{
						return _leaf == i._leaf && _current == i._current;
					}

					bool operator!=( const ConstIterator& i )
					{
						return _leaf != i._leaf || _current != i._current;
					}
				
			};

		public:
		
			class ConstIterator
			{
				friend class BTree;
				friend class Node;
				friend class Leaf;

				private:
					Leaf* _leaf;
					size_type _current;

				private:
					inline ConstIterator( Leaf* l, size_type c ) : 
						_leaf( l ), _current( c )
					{}
				
				public:
					inline ConstIterator() {}
					inline ConstIterator( const Iterator& i ) : 
						_leaf( i._leaf), _current( i._current ) {}
					inline ConstIterator( const ConstIterator& i ) : 
						_leaf( i._leaf), _current( i._current ) {}
					inline ConstIterator& operator=( const ConstIterator& i )
					{
						_leaf = i._leaf;
						_current = i._current;
						return *this;
					}
					
					inline reference operator*() const
					{
						assert( _current < _leaf->size );
						return _leaf->data[ _current ];
					}
					
					inline pointer operator->() const
					{
						assert( _current < _leaf->size );
						return &_leaf->data[ _current ];
					}
					
					inline ConstIterator& operator++()
					{
						if( _current + 1 < _leaf->size )
						{
							++_current;
						}
						else if( 0 != _leaf->next )
						{
							_current = 0;
							_leaf = _leaf->next;
						}
						else
						{
							_current = _leaf->size;
						}
						return *this;
					}
					
					inline ConstIterator operator++( int )
					{
						Iterator next( *this );
						++next;
						return std::move( next );	
					}
					
					inline ConstIterator& operator--()
					{
						if( _current != 0 )
						{
							--_current;
						}
						else if( _leaf->previous != 0 )
						{
							_leaf = _leaf->previous;
							_current = _leaf->size - 1;		
						}
						else
						{
							_current = 0;
						}
						return *this;
					}
					
					inline ConstIterator operator--( int )
					{
						Iterator previous( *this );
						--previous;
						return std::move( previous );	
					}
					
					const pointer& base() const
					{
						return _current;
					}
				
				public:
					bool operator==( const ConstIterator& i )
					{
						return _leaf == i._leaf && _current == i._current;
					}

					bool operator!=( const ConstIterator& i )
					{
						return _leaf != i._leaf || _current != i._current;
					}

			};
		
		private:
			class Stats
			{
				public:
					size_type elements;
					size_type leafs;
					size_type bodies;
				
				public:
					inline Stats() : elements( 0 ), leafs( 0 ), bodies( 0 ) {}
					inline size_type nodes() const { return leafs + bodies; }
			};
			
			/*!
				\brief Internal Data
			*/
		private:
			Node* _root;
			Leaf* _begin;
			Leaf* _end;
			Stats _stats;

			/*!
				\brief Copy/Construct/Destroy
			*/
		public:
			explicit BTree( const Compare& comp = Compare(), 
				const Allocator& alloc = Allocator() ) : 
				_allocator( alloc ), _compare( comp ),
				_root( 0 ), _begin( 0 ), _end( 0 ) 
			{
			}

			template < typename InputIterator >			
			BTree( InputIterator first, InputIterator last, 
				const Compare& comp = Compare(), 
				const Allocator& alloc = Allocator() ) : 
				_allocator( alloc ), _compare( comp ),
				_root( 0 ), _begin( 0 ), _end( 0 ) 
			{
				insert( first, last );
			}
			
			BTree( const BTree& tree ) : 
				_allocator( tree._allocator ), _compare( tree._compare ),
				_root( 0 ), _begin( 0 ), _end( 0 ) 
			{
				insert( tree.begin(), tree.end() );
			}
			
			~BTree()
			{
				clear();
			}
			
			BTree& operator=( const BTree& tree )
			{
				clear();
				insert( tree.begin(), tree.end() );
			}
			
			/*!
				\brief Iterators
			*/
		public:
			iterator begin()
			{
				return iterator( _begin, 0 );
			}
			
			const_iterator begin() const
			{
				return const_iterator( _begin, 0 );
			}
			
			iterator end()
			{
				return iterator( _end, _end != 0 ? _end->size : 0 );
			}
			
			const_iterator end() const
			{
				return const_iterator( _end, _end != 0 ? _end->size : 0 );
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
				return _stats.elements;
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
					fi = _root->insert( fi, 
						std::make_pair( key, mapped_type() ) );					
				}
				else
				{
					if( _compare( *fi, key ) || _compare( key, *fi ) )
					{
						fi = _root->insert( fi, 
							std::make_pair( key, mapped_type() ) );
					}
				}
				
				return fi->second;
			}
		
			/*!
				\brief Modifiers
			*/
		public:
			std::pair< iterator, bool > insert( const value_type& x )
			{
				report( "Inserting " << x.first << "," << x.second );
				if( _root != 0 )
				{
					Leaf* leaf = _findInsertLeaf( x.first );
					std::pair< iterator, bool > result 
						= leaf->insert( x, _compare );
					if( result.second && leaf->full() )
					{
						report( " Insert caused a split." );
						_split( result.first );
					}
					return result;
				}
				else
				{
					report( " Creating the root node." );
					_createRoot();
					assert( _root->leaf() );
					Leaf* leaf = static_cast< Leaf* >( _root );
					leaf->data[ 0 ] = x;
					leaf->size = 1;
					return insertion( begin(), true );
				}
			}

			iterator insert( iterator position, const value_type& x )
			{
				iterator guess = lower_bound( x );
				return _insert( guess, x );
			}

			template < typename InputIterator >
			void insert( InputIterator first, InputIterator last )
			{
				for( InputIterator fi = first; fi != last; ++fi )
				{
					insert( *fi );
				}
			}
			
			void erase( iterator position )
			{
				assert( "Erase not implemented." == 0 );
			}
			
			size_type erase( const key_type& x )
			{
				iterator position = find( x );
				if( x != end() )
				{
					erase( position );
					return 1;
				}
				return 0;
			}
			
			void erase( iterator first, iterator last )
			{
				assert( "Erase not implemented." == 0 );
			}
			
			void swap( BTree& tree )
			{
				std::swap( _stats, tree._stats );
				std::swap( _begin, tree._begin );
				std::swap( _end, tree._end );
				std::swap( _root, tree._root );
			}

			void clear()
			{
				if( _root != 0 )
				{
					_clear( _root );
					_free( _root );
					
					_root = 0;
					_begin = 0;
					_end = 0;
					
					_stats = Stats();
				}
			}
		
		private:
			inline void _createRoot()
			{
				assert( _root == 0 );
				Leaf* leaf = _leafAllocator.allocate( 1 );
				leaf->construct();
				_root = leaf;
			}
			
			inline void _split( iterator& position )
			{
				assert( "Split leaf not implemented" == 0 );
			}
			
			inline Leaf* _findInsertLeaf( const key_type& _key )
			{
				Node* node = _root;
				
				for( size_type level = _root->level; level > 0; --level )
				{
					assert( level == node->level );
					Body* body = static_cast< Body* >( node );
					Key* key = std::lower_bound( body->keys, 
						body->keys + body->size, _key, _keyCompare );
					assert( key != body->keys + body->size );
					node = body->children[ key - body->keys ];
				}
				
				assert( node->leaf() );
				Leaf* leaf = static_cast< Leaf* >( node );
				return leaf;				
			}
		
			inline void _clear( Node* n )
			{
				if( !n->leaf() )
				{
					Body* body = static_cast< Body* >( n );
					for( size_type i = 0; i < n->size; ++i )
					{
						_clear( body->children[ i ] );
						_free( body->children[ i ] );
					}
				}
			}
		
			inline Body* _allocateBody( size_type level )
			{
				Body* body = _bodyAllocator.allocate( 1 );
				body->construct( level );
				++_stats.bodies;
				return body;
			}
			
			inline Leaf* _allocateLeaf( )
			{
				Leaf* leaf = _leafAllocator.allocate( 1 );
				leaf->construct();
				++_stats.leafs;
				return leaf;
			}
			
			inline void _free( Node* n )
			{
				if( n->leaf() )
				{
					_leafAllocator.deallocate( static_cast< Leaf* >( n ), 1 );
					--_stats.leafs;
				}
				else
				{
					_bodyAllocator.deallocate( static_cast< Body* >( n ), 1 );				
					--_stats.bodies;
				}
			}
			
			inline void _insert( iterator position, const value_type& val )
			{
				assert( "_insert not implemented." == 0 );
			}
			
			/*!
				\brief Observers
			*/
		public:
			key_compare key_comp() const
			{
				return _keyCompare;
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
				iterator result = lower_bound( x );
				reportE( REPORT_TREE, "Finding key " << x );
				if( result != end() )
				{
					if( !_keyCompare( x, result->first ) )
					{
						reportE( REPORT_TREE, " Found value " 
							<< result->second );
						return result;
					}
				}
				reportE( REPORT_TREE && result == end(), 
					" Could not find value for key." );
				return end();
			}
			
			const_iterator find( const key_type& x ) const
			{
				const_iterator result = lower_bound( x );
				reportE( REPORT_TREE, "Finding key " << x );
				if( result != end() )
				{
					if( !_compareKey( x, result->first ) )
					{
						reportE( REPORT_TREE, " Found value " 
							<< result->second );
						return result;
					}
				}
				reportE( REPORT_TREE && result == end(), 
					" Could not find value for key." );
				return end();
			}
			
			size_type count( const key_type& x ) const
			{
				return find( x ) != end();
			}
			
			iterator lower_bound( const key_type& x )
			{
				if ( _root == 0 ) return end();
				Node* node = _root;
				
				while( !node->leaf() )
				{
					Body* body = static_cast< Body* >( node );
					Key* key = std::lower_bound( body->keys, 
						body->keys + body->size, x, _keyCompare );
					node = body->children[ key - body->keys ];
				}
				
				Leaf* leaf = static_cast< Leaf* >( node );
				value_type* fi = std::lower_bound( leaf->data, 
					leaf->data + leaf->size, x, _compare );
				size_type index = fi - leaf->data;
				if( index == leaf->size )
				{
					return end();
				}
				return iterator( leaf, index );
			}
			
			const_iterator lower_bound( const key_type& x ) const
			{
				return lower_bound( x );
			}
			
			iterator upper_bound( const key_type& x )
			{
				iterator result = lower_bound( x );
				if( result != end() )
				{
					if( !_compareKey( x, result->first ) )
					{
						++result;
					}
				}
				return result;
			}

			const_iterator upper_bound( const key_type& x ) const
			{
				const_iterator result = lower_bound( x );
				if( result != end() )
				{
					if( !_compareKey( x, result->first ) )
					{
						++result;
					}
				}
				return result;
			}

			std::pair< iterator, iterator > equal_range( const key_type& x )
			{
				std::pair< iterator, iterator > result;
				result.first = lower_bound( x );
				result.second = result.first;
				if( result.second != end() )
				{
					if( !_compareKey( x, result.second->first ) )
					{
						++result.second;
					}
				}
				return result;
			}
			
			std::pair< const_iterator, const_iterator > equal_range( 
				const key_type& x ) const
			{
				std::pair< iterator, iterator > result;
				result.first = lower_bound( x );
				result.second = result.first;
				if( result.second != end() )
				{
					if( !_compareKey( x, result.second->first ) )
					{
						++result.second;
					}
				}
				return result;			
			}

		public:
		
			void toGraphViz( std::ostream& out )
			{
				typedef std::stack< const Node* > NodeStack;
		
				if( _root == 0 )
				{
					return;
				}
				
				NodeStack stack;
		
				stack.push( _root );

				out << "digraph BTree_" << this << " {\n";
				out << "\tnode [ shape = record ];\n\n";
				while( !stack.empty() )
				{
					const Node* node = stack.top();
					stack.pop();
					out << "\tnode_" << node << " [ ";
					if( !node->leaf() )
					{
						out << " color = red, ";					
					} 
					else
					{
						out << " color = black, ";
					}					
					out << "label = \"{";
					if( node->leaf() )
					{
						const Leaf* leaf = static_cast< const Leaf* >( node );
						out << "<head> leaf_" << leaf->data->first 
							<< " (" << leaf->size << ")" << " | {";
						for( const_pointer fi = leaf->data; 
							fi != leaf->data + leaf->size; ++fi )
						{
							if( fi != leaf->data )
							{
								out << "| ";
							}
							out << "{ <key_" << ( fi - leaf->data ) 
								<< "> " << fi->first << " | " 
								<< fi->second << " } ";
						}
						out << "} }\"];\n";
					}
					else
					{
						const Body* body = static_cast< const Body* >( node );
						out << "<head> node_" << *body->keys << " (" 
							<< body->size << ")" << "(l_" << body->level 
							<< ")" << " | {";
						for( const Key* ki = body->keys;
							ki != body->keys + body->size; ++ki )
						{
							if( ki != body->keys )
							{
								out << "| ";
							}
							out << "<key_" << ( ki - body->keys ) 
								<< "> " << *ki << " } ";
						}
						out << "} }\"];\n";

						for( Node* const* ni = &body->children[0];
							ni != body->children + body->size; ++ni )
						{
							out << "node_" << node << "::<key_" 
								<< ( ni - body->children ) << "> -> node_"
								<< *ni << "::<head>\n";
							stack.push( *ni );
						}
						out << "\n";
					}

				}
				out << "}";
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
		return std::lexicographical_compare( x.begin(), x.end(), y.begin(), 
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
		const BTree< Key, T, Compare, Allocator, PageSize >& y)
	{
		return !( x < y );
	}
	
	template < typename Key, typename T, typename Compare, typename Allocator, 
		size_t PageSize >
	bool operator<=(const BTree< Key, T, Compare, Allocator, PageSize >& x,
		const BTree< Key, T, Compare, Allocator, PageSize >& y )
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

