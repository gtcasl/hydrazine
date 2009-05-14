/*!
    
    \file BinaryString.h
    
    \date August 22, 2008
    
    \author Gregory Diamos
    
    \brief The header file for the BinaryString class

*/

#ifndef BINARY_STRING_H_INCLUDED
#define BINARY_STRING_H_INCLUDED

#include <string>
#include <common/interface/Stringable.h>
#include <cstring>

namespace common
{

    /*!
    
        \brief This class is used to convert between a data type and the 
        bit representation of the type.
    
    */
    template< class T, unsigned int wordBytes = 4 >
    class BinaryString : public Stringable
    {
    
        private:
        
            T _data;            
            std::string _string;
    
        public:
    
            BinaryString();            
            void set( const T& data );            
            void set( const std::string& string );            
            const T& data() const;            
            std::string toString() const;    
    
    };
    
    template < class T, unsigned int wordBytes >
    BinaryString< T, wordBytes >::BinaryString()
    {
    
        unsigned int size = sizeof( T );
        
        for( unsigned int bit = 0; bit < size * 8; ++bit )
        {
        
            _string += "U";
        
        }
                
    }
            
    template < class T, unsigned int wordBytes >
    void BinaryString< T, wordBytes >::set( const T& data )
    {
        
        _data = data;        
        _string.clear();        
        char* dataBytes = reinterpret_cast< char* >( &_data );
        unsigned int wordCounter = 0;
        unsigned int size = sizeof( T );
        
        std::vector< char > stringData;
        
        for( unsigned int byte = 0; byte < size; ++byte )
        {
        
        
            for( unsigned int bit = 0; bit < 8; ++bit )
            {
            
                if( ( dataBytes[byte] >> bit ) & 0x1 )
                {
                
                    stringData.push_back( '1' );
                
                }
                else
                {
                
                    stringData.push_back( '0' );
                
                }
            
            }
            
            if( wordCounter >= wordBytes - 1 )
            {
            
                stringData.push_back( ' ' );
                wordCounter = 0;
            
            }
            else
            {
            
                ++wordCounter;
            
            }
        
        }
        
        for
        ( 
            std::vector< char >::reverse_iterator ri = stringData.rbegin(); 
            ri != stringData.rend(); 
            ++ri 
        )
        {
        
            _string += *ri;
        
        }
    
    }
    
    template < class T, unsigned int wordBytes >
    void BinaryString< T, wordBytes >::set( const std::string& string )
    {
    
        _string = string;
        char* dataBytes = reinterpret_cast< char* >( &_data );
        std::vector< char > stringData;
        
        memset( dataBytes, 0, sizeof( T ) );
        unsigned int counter = 0;
                
        for( std::string::iterator fi = _string.begin(); fi != string.end(); ++fi )
        {
        
            if( *fi == '1' || *fi == '0' )
            {
            
                unsigned int bit;
                unsigned int word;
                
                stringData.push_back( *fi );                
                bit = counter & 0x7;
                word = counter >> 3;
            
                if( *fi == '1' )
                {
                
                    dataBytes[word] = dataBytes[word] ^ ( 0x1 << bit );
                
                }
            
            }
            
            ++counter;
        
        }        
    
    }
            
    template < class T, unsigned int wordBytes >
    const T& BinaryString< T, wordBytes >::data() const
    {
    
        return _data;
    
    }
    
    template < class T, unsigned int wordBytes >
    std::string BinaryString< T, wordBytes >::toString() const
    {
    
        return _string;
    
    }

}

#endif

