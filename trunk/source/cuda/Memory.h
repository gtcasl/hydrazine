/*!
	\file Memory.h
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	\date Tuesday May 8, 2009
	\brief The header file for basic Cuda memory management functions
*/

#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include <common/cuda/Cuda.h>
#include <common/implementation/macros.h>

namespace cuda
{


	enum MemcpyType
	{
		MemcpyWarp,
		MemcpyCta,
		MemcpyBase
	};

	template< MemcpyType type >
	__device__ void _memcpy(void* _destination, 
		const void* _source, size_t length)
	{
		int index;
		int stride;
		
		char* destination = (char*) _destination;
		char* source = (char*) _source;
		
		switch( type )
		{
			case MemcpyWarp:
			{
				index =	INTRA_WARP_ID();
				stride = WARP_SIZE();
				break;
			}
			case MemcpyCta:
			{
				index = THREAD_ID();
				stride = CTA_DIMENSION();
				break;
			}
			case MemcpyBase:
			{
				index = GLOBAL_ID();
				stride = TOTAL_THREADS();
				break;
			}
		}
		
		int steps = length/sizeof(int2);
		int doubleStride = stride * 2;
		int i;
	
		// Transfer bulk
		for(i= 0; i< steps - doubleStride; i+=doubleStride )
		{
			int2 tempA = ((int2*)source)[ i + index + stride*0 ];
			int2 tempB = ((int2*)source)[ i + index + stride*1 ];

			((int2*)destination)[ i + index + stride*0 ] = tempA;
			((int2*)destination)[ i + index + stride*1 ] = tempB;
		}

		// Transfer remainder
		for(    ; i< steps; i += stride)
		{
			if( (i + index) < steps )
			{
				((int2*)destination)[ i + index ] = 
					((int2*)source)[ i + index ];
			}
		}
		
		// Transfer last few bytes
		for(i= length - length%sizeof(int2); i< length; i++)
		{
			((char*)destination)[ i ] = ((char*)source)[ i ];
		}
	
	}

	__device__ void memcpyWarp(void* destination, 
		const void* source, size_t length)
	{
		_memcpy<MemcpyWarp>( destination, source, length );
	}

	__device__ void memcpyCta(void* destination, 
		const void* source, size_t length)
	{
		_memcpy<MemcpyCta>( destination, source, length );
	}


	__global__ void memcpy(void* destination, 
		const void* source, size_t length);

}

#endif

