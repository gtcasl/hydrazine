/*! \file SystemCompatibility.h
	\date Monday August 2, 2010
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	\brief The header file for hacked code required to assist windows 
		compilaiton
*/

#ifndef SYSTEM_COMPATIBILITY_CPP_INCLUDED
#define SYSTEM_COMPATIBILITY_CPP_INCLUDED

// Hydrazine includes
#include <hydrazine/interface/SystemCompatibility.h>

// Standard Library Includes
#include <string>

#ifdef WIN32
	#include <windows.h>
#elif __APPLE__
	#include <sys/types.h>
	#include <sys/sysctl.h>
#elif __GNUC__
	#include <unistd.h> 
	#include <sys/sysinfo.h>
#else 
	#error "Unknown system/compiler (WIN32, APPLE, and GNUC are supported)."
#endif

namespace hydrazine
{
	unsigned int getHardwareThreadCount()
	{
	#ifdef WIN32
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);

		return sysinfo.dwNumberOfProcessors;
	#elif __APPLE__
		int nm[2];
	    size_t len = 4;
	    uint32_t count;

	    nm[0] = CTL_HW;
	    nm[1] = HW_AVAILCPU;
	    sysctl(nm, 2, &count, &len, NULL, 0);

	    if(count < 1)
	    {
	        nm[1] = HW_NCPU;
	        sysctl(nm, 2, &count, &len, NULL, 0);
	        if(count < 1)
	        {
	        	count = 1;
	        }
	    }
	    return count;
	#elif __GNUC__
		return sysconf(_SC_NPROCESSORS_ONLN);
	#endif
	}
	
	std::string getExecutablePath(const std::string& executableName)
	{
		#ifdef  __GNUC__
			return executableName;
		#else
			#error "getExecutablePath not implemented for your compiler."
		#endif
	}

}

#endif

