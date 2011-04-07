/*! \file SystemCompatibility.h
	\date Monday August 2, 2010
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	\brief The header file for hacked code required to assist windows 
		compilaiton
*/

#ifndef SYSTEM_COMPATIBILITY_H_INCLUDED
#define SYSTEM_COMPATIBILITY_H_INCLUDED

// Standard Library Includes
#include <string>
#include <unordered_map>
#include <unordered_set>

/*****************************************************************************\
	Standard Library Includes 
\*****************************************************************************/
// this is a really horrible hack because unordered_map is declared in std::tr1
// in visual studio
#ifdef WIN32
namespace std
{
	template<typename T1, typename T2>
	class unordered_map : public std::tr1::unordered_map<T1, T2> {};	

	template<typename T1>
	class unordered_set : public std::tr1::unordered_set<T1> {};

	template<typename T1>
	T1 move(const T1& t) { return t; }
}

#else
#endif

namespace hydrazine
{
	/*! \brief Get the number of hardware threads */
	unsigned int getHardwareThreadCount();
	/*! \brief Get the full path to the named executable */
	std::string getExecutablePath(const std::string& executableName);
	/*! \brief The the amount of free physical memory */
	long long unsigned int getFreePhysicalMemory();
}

#endif
