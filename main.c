#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <elf.h>
#include "str.h"


#define handle_error(msg) \
   do { perror(msg); exit(EXIT_FAILURE); } while (0)

void checkmagic(Elf64_Ehdr header);

//global state noooo
Elf64_Ehdr elfheader = {};

Elf64_Shdr * sht; //section header table

char* shstrt; //section header str table
char* dynstrt; //dynamic symbol header str table
Elf64_Shdr sheader(const char * sectionname)
{
	Elf64_Shdr n = {};
	for(int i = 0 ; i < elfheader.e_shnum; i++){
		char*name = shstrt + sht[i].sh_name;
		if(0==strcmp(sectionname,name) ){
			return  sht[i];
			break;
		}
	}
	return n;
}
Elf64_Off findsection(const char * sectionname)
{
	for(int i = 0 ; i < elfheader.e_shnum; i++){
		char*name = shstrt + sht[i].sh_name;
		if(0==strcmp(sectionname,name) ){
			return  sht[i].sh_offset;
			break;
		}
	}
	return 0;
}

int main(int argc , char**argv)
{


	if (argc<2){
		errno = EINVAL;
		handle_error("no filename provided!");
	}


	char *base;
	int fd;
	struct stat filestat;
	char * fname = argv[1];

	fd = open(fname, O_RDONLY);
	if (fd == -1)
	   handle_error("open");

	if (fstat(fd, &filestat) == -1)  
	   handle_error("fstat");

	base = mmap(NULL,filestat.st_size,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);

		   ///////////
	


	// load elf header 
	memcpy(&elfheader,base,sizeof(Elf64_Ehdr));

	

	checkmagic(elfheader);


	//we are going to assume its all 2LSB 
	assert(elfheader.e_ident[EI_DATA] == ELFDATA2LSB);
	
	printf("file:\t%s\n",fname);
	printf("\tclass:\t%s\n",
			strEI_CLASS(elfheader.e_ident[EI_CLASS]) );
	printf("\tdata:\t%s\n",
			strEI_DATA(elfheader.e_ident[EI_DATA]) );

	printf("\tsht offset:\t0x%08lx\n",elfheader.e_shoff);
	printf("\tsht entrysz:\t0x%x\n",elfheader.e_shentsize);
	printf("\tsection hdrs:\t%d\n",elfheader.e_shnum);
	puts("");




	// populate globals
	sht = (Elf64_Shdr*)(base + elfheader.e_shoff);

	//section header str table
	assert(SHN_UNDEF != elfheader.e_shstrndx);
	printf("SHSTRNDX %d\n",elfheader.e_shstrndx);
	shstrt= base + sht[elfheader.e_shstrndx].sh_offset;

	//find .dynamic table

	Elf64_Off dynstroff = findsection(".dynstr");
	dynstrt = base + dynstroff;

	Elf64_Off dynoff = findsection(".dynamic");
	Elf64_Dyn *dyn;
	dyn = (Elf64_Dyn*)(base + dynoff);




	int dest = 0;
	for(int i = 0 ; i < elfheader.e_shnum; i++){
		// ignore null entries
		if (sht[i].sh_type == SHT_NULL)
			continue;

		char*name = shstrt + sht[i].sh_name;

		if(    0==strcmp(".gnu.version",name)
			|| 0==strcmp(".gnu.version_r",name) ){
			printf("target found %d %d\n",i,dest);
			// nuke section contents
			memset(base+sht[i].sh_offset,0,sht[i].sh_size);

			// nuke section header
			memset(sht+i,0,sizeof(Elf64_Shdr));
			
		}
	}


	//dleete version stuff from .dynamic

	dyn = (Elf64_Dyn*)(base + dynoff);
	Elf64_Dyn* dyndest = (Elf64_Dyn*)(base + dynoff);

	while(dyn->d_tag != DT_NULL){
		Elf64_Dyn nulldyn = { };
		switch(dyn->d_tag){
			case DT_VERSYM:
			case DT_VERDEF:
			case DT_VERDEFNUM:
			case DT_VERNEED:
			case DT_VERNEEDNUM:
				*dyn=nulldyn;
			default:
				*dyndest=*dyn;
				dyndest++;
		}
		dyn++;
	}



	//dumpresults

	char * newname = malloc (strlen(fname) + 20);
	sprintf(newname,"%s_nover",fname);
	printf("creating file: %s\n",newname);
	int outfd = open(newname, O_RDWR|O_CREAT,S_IRWXU);
	if (-1 == outfd)
		handle_error("out fd");
	

	size_t togo = filestat.st_size;
	size_t chunk = 0x2000;
	while (togo > 0){
		size_t w = write(outfd,base, chunk<togo ? chunk : togo);
		if (w==-1)
			handle_error("writing error");
		base+=w;
		togo-=w;
	}
	close(outfd);

	return 0;



}

// type punning is good
union hack {
  Elf64_Ehdr header;
  uint32_t u;
};

void checkmagic(Elf64_Ehdr header){
	
	union hack x;
	x.header=header;
	
	if(x.u !=0x464c457f ){
		errno = EINVAL;
		handle_error("not an elf file");
	}	
}
