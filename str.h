const char* strEI_CLASS(uint32_t  value)
{

	switch( value){
		case ELFCLASSNONE:
			 return "ELFCLASSNONE";
		case ELFCLASS32:
			 return "ELFCLASS32";
		case ELFCLASS64:
			 return "ELFCLASS64";
	}
	return "STRNOTFOUND";

}
const char* strEI_DATA (uint32_t value)
{
	switch( value){
		case ELFDATANONE:
			 return "ELFDATANONE";
		case ELFDATA2LSB:
			 return "ELFDATA2LSB";
		case ELFDATA2MSB:
			 return "ELFDATA2MSB";
	}
	return "STRNOTFOUND";
}

//#define EI_OSABI	7		/* OS ABI identification */
//#define ELFOSABI_NONE		0	/* UNIX System V ABI */
//#define ELFOSABI_SYSV		0	/* Alias.  */
//#define ELFOSABI_HPUX		1	/* HP-UX */
//#define ELFOSABI_NETBSD		2	/* NetBSD.  */
//#define ELFOSABI_GNU		3	/* Object uses GNU ELF extensions.  */
//#define ELFOSABI_LINUX		ELFOSABI_GNU /* Compatibility alias.  */
//#define ELFOSABI_SOLARIS	6	/* Sun Solaris.  */
//#define ELFOSABI_AIX		7	/* IBM AIX.  */
//#define ELFOSABI_IRIX		8	/* SGI Irix.  */
//#define ELFOSABI_FREEBSD	9	/* FreeBSD.  */
//#define ELFOSABI_TRU64		10	/* Compaq TRU64 UNIX.  */
//#define ELFOSABI_MODESTO	11	/* Novell Modesto.  */
//#define ELFOSABI_OPENBSD	12	/* OpenBSD.  */
//#define ELFOSABI_ARM_AEABI	64	/* ARM EABI */
//#define ELFOSABI_ARM		97	/* ARM */
//#define ELFOSABI_STANDALONE	255	/* Standalone (embedded) application */
