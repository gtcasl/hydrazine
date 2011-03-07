/*! \file WindowsCompatibility.h
	\date Monday August 2, 2010
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	\brief The header file for hacked code required to assist windows 
		compilaiton
*/

#ifndef WINDOWS_COMPATIBILITY_CPP_INCLUDED
#define WINDOWS_COMPATIBILITY_CPP_INCLUDED

// Hydrazine includes
#include <hydrazine/interface/WindowsCompatibility.h>

#ifndef WIN32
#include <unistd.h> 
#include <sys/sysinfo.h>
#elif __APPLE__
#include <sys/types.h>
#include <sys/sysctl.h>
#else
#include <windows.h>
#endif

namespace hydrazine
{
	unsigned int getHardwareThreadCount()
	{
	#ifdef WIN32
		SYSTEM_INFO sysinfo;
		GetSystemInfo( &sysinfo );

		return sysinfo.dwNumberOfProcessors;
	#elif __APPLE__
		int mib[4];
		size_t len = sizeof(numCPU); 
		int numCPU = 0;

		/* set the mib for hw.ncpu */
		mib[0] = CTL_HW;
		mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

		/* get the number of CPUs from the system */
		sysctl( mib, 2, &numCPU, &len, NULL, 0 );

		if( numCPU < 1 ) 
		{
			mib[1] = HW_NCPU;
			sysctl( mib, 2, &numCPU, &len, NULL, 0 );

			if( numCPU < 1 )
			{
				numCPU = 1;
			}
		}
		
		return numCPU;
	#else
		return sysconf(_SC_NPROCESSORS_ONLN);
	#endif
	}
}

#endif
