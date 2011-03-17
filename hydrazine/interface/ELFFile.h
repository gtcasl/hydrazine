/* 	\file   ELFFile.h
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	\date   Thursday March 17, 2011
	\brief  The header file for the ELFFile class.
*/

#ifndef ELF_FILE_H_INCLUDED
#define ELF_FILE_H_INCLUDED

// Hydrazine Includes
#include <hydrazine/interface/ELF.h>

namespace hydrazine
{

/*! \brief A class for parsing and interpretting an ELF file */
class ELFFile
{
public:
	/*! \brief Represents the ELF file header */
	class Header
	{
	public:
		/*! \brief Create a new header from a header data struct */
		Header(void* data = 0, long long unsigned int byte = 0,
			const elf::Elf64_Ehdr& header = elf::Elf64_Ehdr());
	
	public:
		/*! \brief Get access to the raw header data */
		const elf::Elf64_Ehdr& header() const;
		
		/*! \brief Does the magic word agree that this is an ELF binary? */
		bool checkMagic() const;
	
	public:
		/*! \brief Easier access to the underlying data (section count) */
		unsigned int sectionHeaders() const;
		
	private:
		/*! \brief The raw data */
		elf::Elf64_Ehdr _header;
	};
	
	/*! \brief A header for a symbol table entry */
	class SymbolHeader
	{
	public:
		SectionHeader(void* data = 0, long long unsigned int byte = 0,
			const elf::Elf64_Sym& header = elf::Elf64_Sym());
	
	public:
		/*! \brief Get access to the raw header data */
		const elf::Elf64_Sym& header() const;
		
	private:
		/*! \brief The raw header data */
		elf::Elf64_Sym _header;
	};
	
	/*! \brief Represents an ELF section header */
	class SectionHeader
	{
	public:
		SectionHeader(void* data = 0, long long unsigned int byte = 0,
			const elf::Elf64_Shdr& header = elf::Elf64_Shdr());
	
	public:
		/*! \brief Get access to the raw header data */
		const elf::Elf64_Shdr& header() const;
		
	private:
		/*! \brief The raw header data */
		elf::Elf64_Shdr _header;
	};
	
	/*! \brief Represents an ELF section header */
	class ProgramHeader
	{
	public:
		ProgramHeader(void* data = 0, long long unsigned int byte = 0,
			const elf::Elf64_Phdr& header = elf::Elf64_Phdr());
	
	public:
		/*! \brief Get access to the raw header data */
		const elf::Elf64_Phdr& header() const;
		
	private:
		/*! \brief The raw header data */
		elf::Elf64_Phdr _header;
	
	};

public:
	/*! \brief Create a new ELF, bind it to a mmapped or in-memory file */
	ELFFile(void* fileData = 0);

public:
	/*! \brief Get a reference to the header */
	const Header& header() const;

public:
	/*! \brief Get the number of sections in the ELF file */
	unsigned int sections() const;
	/*! \brief Get a specific section header */
	SectionHeader& sectionHeader(unsigned int header);

public:
	/*! \brief Get the number of program headers in the ELF file */
	unsigned int programs() const;	
	/*! \brief Get a specific program header */
	ProgramHeader& programHeader(unsigned int header);

public:
	/*! \brief Get access to a string within the string table (in range) */
	const char* getStringAtOffset(long long unsigned int offset) const;

private:
	/*! \brief A vector of section headers */
	typedef std::vector<SectionHeader> SectionHeaderVector;
	/*! \brief A vector of program headers */
	typedef std::vector<ProgramHeader> ProgramHeaderVector;
	/*! \brief A vector of program headers */
	typedef std::vector<SymbolHeader> SymbolHeaderVector;

private:
	/*! \brief A reference to the file data for lazy access */
	void* _elfdata;
	/*! \brief The ELF file header */
	Header _header;
	/*! \brief The set of sections in the file */
	SectionHeaderVector _sections;
	/*! \brief The set of programs in the file */
	ProgramHeaderVector _programs;
};

}

#endif

