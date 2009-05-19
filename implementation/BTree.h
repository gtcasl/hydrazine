/*!
	\file BTree.h
	\date Wednesday May 13, 2009
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	\brief The header file for the BTree class
*/

#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED

#include <implementation/MmapAllocator.h>
#include <implementation/debug.h>

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
		typename _Allocator = hydrazine::MmapAllocator< std::pair< const Key, 
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
			typedef std::pair< key_type, NodeMappedType > NodeValueType;
			typedef typename _Allocator::template rebind< Node >::other
				NodeAllocator;
			typedef typename _Allocator::template rebind< Leaf >::other
				LeafAllocator;

		public:
			class node_compare : 
				public std::binary_function< NodeValueType, 
				NodeValueType, bool >
			{
				friend class BTree;
				
				protected:
					Compare compare;
				
				protected:
					node_compare( const Compare& c ) : compare( c ) {}
				
				public:
					bool operator()( const NodeValueType& x, 
						const NodeValueType& y )
					{
						return compare( x.first, y.first );
					}

					bool operator()( const key_type& x, 
						const NodeValueType& y )
					{
						return compare( x, y.first );
					}

					bool operator()( const NodeValueType& x, 
						const key_type& y )
					{
						return compare( x.first, y );
					}
					
			};

		private:
			LeafAllocator _leafAllocator;
			NodeAllocator _nodeAllocator;
			node_compare _nodeCompare;
		
		private:
			/*!
				\brief An internal node
			*/
			class Node
			{
				public:
					typedef NodeValueType ValueType;
					typedef ValueType* Pointer;
					typedef const ValueType* ConstPointer;
					
				public:
					BTree* tree;
					ValueType begin[ PageSize + 1 ];
					Pointer end;
					Node* parent;
					size_type index;
					bool body;
				
				public:
					explicit Node( BTree* t ) : tree( t ), 
						end( begin + 1 ), parent( 0 ), index( 0 ), 
						body( false )
					{
						begin->first = 0;
						begin->second.leaf = tree->_leafAllocator.allocate( 1 );
						tree->_leafAllocator.construct( 
							begin->second.leaf, Leaf() );
						begin->second.leaf->parent = this;
						begin->second.leaf->index = 0;
					}

					Node( ) : end( begin )
					{
						
					}
					
					Node( const Node& n ) : tree( n.tree ), 
						parent( n.parent ), index( n.index ), body( false )
					{
						size_type size = n.end - n.begin;
						end = begin + size;

						ConstPointer ni = n.begin;
						if( body )
						{
							for( Pointer fi = begin; fi != end; ++fi, ++ni )
							{
								fi->second.node = 
									tree->_nodeAllocator.allocate( 1 );
								tree->_nodeAllocator.construct( 
									fi->second.node, *ni->second.node );
								fi->second.leaf->parent = this;
							}
						}
						else
						{
							for( Pointer fi = begin; fi != end; ++fi, ++ni )
							{
								fi->second.leaf = 
									tree->_leafAllocator.allocate( 1 );
								tree->_leafAllocator.construct( 
									fi->second.leaf, *ni->second.leaf );
								fi->second.leaf->parent = this;
							}
						}
					}
					
					~Node()
					{
						if( body )
						{
							for( Pointer fi = begin; fi != end; ++fi )
							{
								tree->_nodeAllocator.destroy( fi->second.node );
								tree->_nodeAllocator.deallocate( 
									fi->second.node, 1 );
							}
						}
						else
						{
							for( Pointer fi = begin; fi != end; ++fi )
							{
								tree->_leafAllocator.destroy( fi->second.leaf );
								tree->_leafAllocator.deallocate( 
									fi->second.leaf, 1 );
							}
						}
					}
					
					Node& operator=( const Node& n )
					{
						assert( tree == n.tree );
						assert( &n != this );
						clear();
						
						size_type size = n.end - n.begin;
						end = begin + size;
						index = n.index;
						body = n.body;

						ConstPointer ni = n.begin;
						if( body )
						{
							for( Pointer fi = begin; fi != end; ++fi, ++ni )
							{
								fi->second.node = 
									tree->_nodeAllocator.allocate( 1 );
								tree->_nodeAllocator.construct( 
									fi->second.node, *ni->second.node );
								fi->second.leaf->parent = this;
							}
						}
						else
						{
							for( Pointer fi = begin; fi != end; ++fi, ++ni )
							{
								fi->second.leaf = 
									tree->_leafAllocator.allocate( 1 );
								tree->_leafAllocator.construct( 
									fi->second.leaf, *ni->second.leaf );
								fi->second.leaf->parent = this;
							}
						}

						return *this;
					}
					
				public:
					size_type size() const
					{
						return end - begin;
					}
					
					bool empty() const
					{
						return size() == 0;
					}
					
					iterator insert( iterator& position, 
						const value_type& value )
					{
						assert( parent == 0 );
						return position._leaf->insert( position, value );
					}

					void erase( iterator& position )
					{
						position._leaf->erase( position );
						Leaf* left = position._leaf->left();
						Leaf* right = position._leaf->right();
						if( left->size() + right->size() <= PageSize )
						{
							left->recombine( position );
						}
					}
					
					void erase( iterator& begin, iterator& end )
					{
						while( begin != end )
						{
							erase( begin );
						}
					}

					void clear()
					{
						~Node();
						end = begin;
					}

				public:
					Node* left()
					{
						assert( parent != 0 );
						size_type leftIndex = index & ( -2 );
						return parent->begin[ leftIndex ].second.leaf;
					}
			
					Node* right()
					{
						assert( parent != 0 );
						size_type rightIndex = index | 1;
						rightIndex = MAX( rightIndex, parent->size() - 1 );
						return parent->begin[ rightIndex ].second.leaf;	
					}
					
					void split( )
					{
						reportE( REPORT_NODE, "Splitting node " 
							<< begin[0].first );
						assert( size() == PageSize );
						
						if( parent == 0 )
						{
							reportE( REPORT_NODE, 
								" This node is the root, bumping down." );
							Node* root = 
								tree->_nodeAllocator.allocate( 1 );
							tree->_nodeAllocator.construct( root, Node() );
							root->parent = 0;
							root->body = true;
							root->tree = tree;
							parent = root;
							index = 0;
							root->end->second.node = this;
							root->end->first = begin[0].first;
							tree->_root = root;
							++root->end;
							report( " The new root is " << root );
						}
						
						size_type median = PageSize / 2;
						Node* right = 
							tree->_nodeAllocator.allocate( 1 );
						tree->_nodeAllocator.construct( right, Node() );
				
						reportE( REPORT_NODE, " The median was " << median 
							<< " which is node/leaf " 
							<< begin[median].first );
						reportE( REPORT_NODE, " Created new right node " 
							<< right );
				
						right->end += median;
						memcpy( right->begin, begin + median, 
							( PageSize - median ) * sizeof( NodeValueType ) );
						right->parent = parent;
						right->index = index + 1;
						right->tree = tree;
						right->body = body;
				
						if( body )
						{
							for( Pointer fi = right->begin; 
								fi != right->end; ++fi )
							{
								fi->second.node->parent = right;
								fi->second.node->index = fi - right->begin;
							}
						}
						else
						{
							for( Pointer fi = right->begin; 
								fi != right->end; ++fi )
							{
								fi->second.leaf->parent = right;
								fi->second.leaf->index = fi - right->begin;
							}
						}
				
						end = begin + median;
				
						assert( parent->size() >= index );
						size_type parentResize = parent->size() - index;
						memmove( parent->begin + right->index + 1, 
							parent->begin + index + 1, 
							parentResize * sizeof( NodeValueType ) );
						parent->begin[ right->index ].first = 
							right->begin->first;
						parent->begin[ right->index ].second.node = right;
						++parent->end;
						reportE( REPORT_NODE, "Parent has " 
							<< parent->size() << " nodes.");
				
						for( NodeValueType* fi = parent->begin 
							+ right->index + 1; fi != parent->end; ++fi )
						{
							fi->second.node->index += 1;
						}
				
						if( parent->size() == PageSize )
						{
							parent->split( );
						}
					}

					void recombine( iterator& position )
					{
					
						Leaf* leftLeaf = position->_leaf->left();
						Leaf* rightLeaf = position->_leaf->right();
						assert( leftLeaf != rightLeaf );
						assert( leftLeaf->parent == this );
					
						size_type newSize = leftLeaf->size() 
							+ rightLeaf->size();
					
						assert( newSize < PageSize );

						memcpy( leftLeaf->end, rightLeaf->begin, 
							rightLeaf->size() * sizeof( value_type ) );
						leftLeaf->end = leftLeaf->begin + newSize;
										
						memmove( begin + leftLeaf->index + 1,
							begin + leftLeaf->index + 2, 
							( size() - leftLeaf->index - 2 ) 
							* sizeof( Node::ValueType ) );
						--end;
					
						for( Node::ValueType* fi = begin 
							+ leftLeaf->index + 1; fi != end; ++fi )
						{
							fi->second.leaf->index -= 1;
						}
					
						position._leaf = leftLeaf;
						tree->_leafAllocator.deallocate( rightLeaf, 1 );
				
						if( left()->size() + right()->size() < PageSize )
						{
							parent->recombine( left(), right() );
						}
					}

					void recombine( Node* left, Node* right )
					{
					
						assert( left != right );
						assert( left->parent == this );
					
						size_type newSize = left->size() + right->size();
					
						assert( newSize < PageSize );

						memcpy( left->end, right->begin, 
							right->size() * sizeof( Node::ValueType ) );
						left->end = left->begin + newSize;
										
						memmove( begin + left->index + 1,
							begin + left->index + 2, 
							( size() - left->index - 2 ) 
							* sizeof( Node::ValueType ) );
						--end;
					
						for( Node::ValueType* fi = begin 
							+ left->index + 1; fi != end; ++fi )
						{
							fi->second.node->index -= 1;
						}
					
						tree->_leafAllocator.deallocate( right, 1 );
				
						if( left()->size() + right()->size() < PageSize )
						{
							recombine( left(), right() );
						}
					}
					
				public:
				
					iterator lower_bound( const key_type& key )
					{
						if( empty() )
						{
							assert( parent == 0 );
							return tree->_end;
						}
					
						iterator result;
						Node* node = this;
						while( node->body )
						{
							assert( !node->empty() );
							Pointer bound = std::lower_bound( node->begin, 
								node->end - 1, key, tree->_nodeCompare );
							node = bound->second.node;
						}
						
						assert( !node->empty() );
						Pointer bound = std::lower_bound( node->begin, 
							node->end - 1, key, tree->_nodeCompare );
						Leaf* leaf = bound->second.leaf;
						
						pointer value = std::lower_bound( leaf->begin, 
							leaf->end, key, tree->_compare );
						
						result._leaf = leaf;
						result._current = value;
						return result;
					}
					
			};
			
			/*!
				\brief An internal leaf
			*/
			class Leaf
			{
				public:
					value_type begin[ PageSize + 1 ];
					pointer end;
					Node* parent;
					size_type index;

				public:
					Leaf( ) : end( begin ), parent( 0 ), index( 0 )
					{
					}
					
					Leaf( const Leaf& n ) : parent( n.parent ), 
						index( n.index )
					{
						size_type size = n.end - n.begin;
						end = begin + size;
						memcpy( begin, n.begin, size * sizeof( value_type ) );
					}
					
					~Leaf()
					{
					}
					
					Leaf& operator=( const Leaf& n )
					{
						assert( &this != n );
						size_type size = n.end - n.begin;
						end = begin + size;
						index = n.index;
						memcpy( begin, n.begin, size * sizeof( value_type ) );						
						return *this;
					}
					
				public:
					size_type size() const
					{
						return end - begin;
					}
					
					Leaf* next() const
					{
						size_type depth = 0;
						size_type index = index + 1;
						bool rollover = false;
						
						assert( parent != 0 );
						const Node* node = parent;

						// run up the tree
						while( node->begin + index == node->end )
						{
							if( node->parent == 0 )
							{
								rollover = true;
								break;
							}
							index = node->index + 1;
							node = node->parent;
							++depth;
						}
						
						if( depth > 0 )
						{
							node = ( node->begin + index )->second.node;
							index = 0;
						}
						
						// run down the tree
						for( size_type i = 1; i < depth; ++i )
						{
							node = node->begin->second.node;
						}
						
						if( rollover )
						{
							return ( node->end - 1 )->second.leaf;
						}
						else
						{
							return ( node->begin + index )->second.leaf;
						}
					}

					Leaf* previous() const
					{
						size_type depth = 0;
						size_type index = index;
						
						assert( parent != 0 );
						const Node* node = parent;

						// run up the tree
						while( index == 0 )
						{
							if( node->parent == 0 )
							{
								index = node->end - node->begin;
								break;
							}
							index = node->index;
							node = node->parent;
							++depth;
						}
						
						--index;
						
						if( depth > 0 )
						{
							node = ( node->begin + index )->second.node;
						}
						
						// run down the tree
						for( size_type i = 1; i < depth; ++i )
						{
							node = ( node->end - 1 )->second.node;
						}
						
						return ( node->end - 1 )->second.leaf;
					}
					
					Leaf* left()
					{
						size_type leftIndex = index & ( -2 );
						return parent->begin[ leftIndex ].second.leaf;
					}
			
					Leaf* right()
					{
						size_type rightIndex = index | 1;
						rightIndex = MAX( rightIndex, parent->size() - 1 );
						return parent->begin[ rightIndex ].second.leaf;	
					}
			
					void split( iterator& position )
					{
						reportE( REPORT_LEAF, "Splitting leaf " << this );
						assert( size() == PageSize );
						size_type median = PageSize / 2;
						Leaf* right = 
							parent->tree->_leafAllocator.allocate( 1 );
						parent->tree->_leafAllocator.construct( right, Leaf() );
						
						reportE( REPORT_LEAF, " The median is " << median 
							<< " which is element " << begin[median].first 
							<< "," << begin[median].second  );
						reportE( REPORT_LEAF, " Created new leaf " << right 
							<< " to the right." );
						right->end += median;
						memcpy( right->begin, begin + median, 
							( PageSize - median ) * sizeof( value_type ) );
						right->parent = parent;
						right->index = index + 1;
				
						end = begin + median;
				
						size_type parentResize =  parent->size() - index;
						memmove( parent->begin + right->index + 1, 
							parent->begin + index + 1, 
							parentResize * sizeof( NodeValueType ) );
						parent->begin[ right->index ].first = 
							right->begin->first;
						parent->begin[ right->index ].second.leaf = right;
						++parent->end;
				
						for( NodeValueType* fi = parent->begin 
							+ right->index + 1; fi != parent->end; ++fi )
						{
							fi->second.node->index += 1;
						}
				
						if( position._current >= end )
						{
							reportE( REPORT_LEAF, 
								" Iterator was on the right side." );
							position._current = right->begin 
								+ ( position._current - begin - median );
							position._leaf = right;
						}
				
						if( parent->size() == PageSize )
						{
							parent->split( );
						}
					}
			
					iterator insert( iterator& position, 
						const value_type& value )
					{
						reportE( REPORT_LEAF, "Inserting " << value.first 
							<< "," << value.second << " into leaf " << this 
							<< " with " << size() << " elements" );
						assert( size() < PageSize );
						iterator result = position;
						assert( result._current <= end );
						size_type moved = end - position._current;						
						bool wasEnd = result._current == end;
						
						report( " Displaced " << moved << " tuples." );
						
						memmove( result._current + 1, result._current, 
							moved * sizeof( value_type ) );
						++end;
						*result._current = value;
						
						if( size() == PageSize )
						{
							reportE( REPORT_LEAF, " Triggered a split." );
							split( result );
						}
						if( wasEnd )
						{
							reportE( REPORT_LEAF, " Updating end." );
							parent->tree->_end = result;
						}
						if( parent->tree->_compare( value, 
							*parent->tree->begin() ) )
						{
							reportE( REPORT_LEAF, " Updating begin." );
							parent->tree->_begin = result;							
						}
						return result;
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
					pointer _current;

				private:
					Iterator( Leaf* l, pointer c ): _leaf( l ), _current( c ) {}

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
						return *this;
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
						return *this;
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
					pointer _current;
				
				public:
					ConstIterator() {}
					ConstIterator( const Iterator& i ) : _leaf( i._leaf), 
						_current( i._current ) {}
					ConstIterator& operator=( const Iterator& i )
					{
						_leaf = i._leaf;
						_current = i._current;
						return *this;
					}
					
					reference operator*() const
					{
						return *_current;
					}
					
					pointer operator->() const
					{
						return _current;
					}
					
					ConstIterator& operator++()
					{
						++_current;
						if( _current == _leaf->end() )
						{
							_leaf = _leaf->next();
							_current = _leaf->begin();
						}
						return *this;
					}
					
					ConstIterator operator++( int )
					{
						Iterator next( *this );
						++next;
						return std::move( next );	
					}
					
					ConstIterator& operator--()
					{
						while( _current == _leaf->begin() )
						{
							_leaf = _leaf->previous();
							assert( _leaf->begin() != _leaf->end() );
							_current = _leaf->end() - 1;
						}
						--_current;
						return *this;
					}
					
					ConstIterator operator--( int )
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
			
			/*!
				\brief Internal Data
			*/
		private:
			Node* _root;
			iterator _begin;
			iterator _end;
			size_t _size;

			/*!
				\brief Copy/Construct/Destroy
			*/
		public:
			explicit BTree( const Compare& comp = Compare(), 
				const Allocator& alloc = Allocator() ) : 
				_allocator( alloc ), _compare( comp ), _nodeCompare( comp ), 
				_size( 0 ) 
			{
				_root = _nodeAllocator.allocate( 1 );
				_nodeAllocator.construct( _root, Node( this ) );
				_begin._leaf = _root->begin->second.leaf;
				_begin._current = _begin._leaf->end;
				_end = _begin;
				reportE( REPORT_TREE, "Created new GPU BTree " << this );
			}

			template < typename InputIterator >			
			BTree( InputIterator first, InputIterator last, 
				const Compare& comp = Compare(), 
				const Allocator& alloc = Allocator() ) : 
				_allocator( alloc ), _compare( comp ), _nodeCompare( comp ), 
				_size( 0 )
			{
				_root = _nodeAllocator.allocate( 1 );
				_nodeAllocator.construct( _root, Node( *this ) );
				for( InputIterator fi = first; fi != last; ++fi )
				{
					insert( *fi );
				}
			}
			
			BTree( const BTree& tree ) : 
				_allocator( tree._allocator ), _compare( tree._compare ), 
				_nodeCompare( tree._nodeCompare ), _size( tree._size )
			{
				_root = _nodeAllocator.allocate( 1 );
				_nodeAllocator.construct( _root, Node( *tree._root ) );
			}
			
			~BTree()
			{
				_nodeAllocator.destroy( _root );
				_nodeAllocator.deallocate( _root, 1 );
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
				std::pair< iterator, bool > fi( lower_bound( x.first ), false );
				
				reportE( REPORT_TREE, "Inserting " << x.first << "," 
					<< x.second );
				
				if( fi.first == end() )
				{
					reportE( REPORT_TREE, 
						" Lower bound is the end, inserting at the end." );
					fi.first = _root->insert( fi.first, x );
					fi.second = true;					
				}
				else
				{
					if( _compare( *fi.first, x ) || _compare( x, *fi.first ) )
					{
						fi.first = _root->insert( fi.first, x );
						fi.second = true;
						reportE( REPORT_TREE, 
							" Lower bound is, " << fi.first->first << "," 
							<< fi.first->second << "." );
					}
				}
				
				return fi;	
			}

			iterator insert( iterator position, const value_type& x )
			{
				iterator guess = lower_bound( x );
				return _root->insert( guess, x );
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
				position._node->erase( position );
			}
			
			size_type erase( const key_type& x )
			{
				iterator position = find( x );
				if( x != end() )
				{
					position._node->erase( position );
					return 1;
				}
				return 0;
			}
			
			void erase( iterator first, iterator last )
			{
				_root->erase( first, last );
			}
			
			void swap( BTree& tree )
			{
				std::swap( _size, tree._size );
				std::swap( _begin._leaf, tree._begin._leaf );
				std::swap( _begin._current, tree._begin._current );
				std::swap( _end._leaf, tree._end._leaf );
				std::swap( _end._current, tree._end._current );
				std::swap( _root->_tree, tree._root->_tree );
				std::swap( _root->_begin, tree._root->_begin );
				std::swap( _root->_end, tree._root->_end );
				std::swap( _root->_parent, tree._root->_parent );
				std::swap( _root->_index, tree._root->_index );
				std::swap( _root->_body, tree._root->_body );
			}

			void clear()
			{
				_root->clear();
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
				const_iterator result = lower_bound( x );
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
				return _root->lower_bound( x );
			}
			
			const_iterator lower_bound( const key_type& x ) const
			{
				return _root->lower_bound( x );
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
				typedef std::stack< const Leaf* > LeafStack;
		
				NodeStack stack;
				LeafStack leafs;
		
				stack.push( _root );

				out << "digraph BTree_" << this << " {\n";
				out << "\trankdir=LR;\n";
				out << "\tnode [ shape = record ];\n\n";
				while( !stack.empty() )
				{
					const Node* node = stack.top();
					stack.pop();
					out << "\tnode_" << node << " [ label = \"";
					out << "<head> node_" << node << " | {";
					for( const NodeValueType* fi = node->begin; 
						fi != node->end; ++fi )
					{
						if( fi != node->begin )
						{
							out << "| ";
						}
						out << "<key_" << ( fi - node->begin ) 
							<< "> " << fi->first;
					}
					out << "}\"];\n";

					if( node->body )
					{
						for( const NodeValueType* fi = node->begin; 
							fi != node->end; ++fi )
						{
							out << "\t" << "\"node_" << node << "\":key_" 
								<< ( fi - node->begin ) << " -> " << "\"node_" 
								<< fi->second.node << "\":head\n\n";
							stack.push( fi->second.node );
						}
					}
					else
					{
						for( const NodeValueType* fi = node->begin; 
							fi != node->end; ++fi )
						{
							out << "\t" << "\"node_" << node << "\":key_" 
								<< ( fi - node->begin ) << " -> " << "\"leaf_" 
								<< fi->second.leaf << "\":head\n\n";
							leafs.push( fi->second.leaf );
						}
					}

					while( !leafs.empty() )
					{
						const Leaf* leaf = leafs.top();
						leafs.pop();
						out << "\tleaf_" << leaf << " [ label = \"";
						out << "<head> leaf_" << leaf << " | {";
						for( BTree::const_pointer fi = leaf->begin; 
							fi != leaf->end; ++fi )
						{
							if( fi != leaf->begin )
							{
								out << "| ";
							}
							out << "{" << fi->first << " | " 
								<< fi->second << "} ";
						}
						out << "}\"];\n\n";
					}
				}	
				out << "}" ;
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

