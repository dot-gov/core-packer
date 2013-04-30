#include <Windows.h>
#include <iostream>
#include <string>
#include <algorithm>
#include "tclap/CmdLine.h"

extern BOOL WINAPI DllEntryPoint(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep)
{
	std::cout << "caught exception. " << std::endl;
	std::cout << "\t[EIP] = " << std::hex << ep->ContextRecord->Eip << std::endl;
	std::cout << "\t[ESP] = " << std::hex << ep->ContextRecord->Esp << std::endl;
	std::cout << "\t[EBP] = " << std::hex << ep->ContextRecord->Ebp << std::endl;
	std::cout << "\t[EFLAGS] = " << std::hex << ep->ContextRecord->EFlags << std::endl;

	return EXCEPTION_EXECUTE_HANDLER;
}


int main(int argc, char *argv[])
{
#ifdef _BUILD32
#define _PACKAGENAME "packer32"
#else
#define _PACKAGENAME "packer64"
#endif

	TCLAP::CmdLine cmdline(_PACKAGENAME, ' ', "0.5 - XPACK FIGHT!");
	
	TCLAP::ValueArg<std::string> inputFile("i", "infile", "Input file", true, "homer", "filename");
	TCLAP::ValueArg<std::string> outputFile("o", "outfile", "Output file", false, "homer", "filename");
	TCLAP::ValueArg<std::string> verbose("v", "verbose", "Verbose output", false, "", "");
	TCLAP::ValueArg<std::string> test("t", "testmode", "Enable test mode", false, "", "");

	cmdline.add(inputFile);
	cmdline.add(outputFile);
	cmdline.add(verbose);
	cmdline.add(test);

	//cmdline.parse(argc, argv);

#ifdef _BUILD32
	
#ifdef _TESTCASE

	extern int main32_test(int argc, char *argv[]);
	return main32_test(argc, argv);
#else
	extern int main32(int, char*argv[]);
	extern int unpack32(int, char*argv[]);

	__try
	{
		if (argv[1][0] == '-' && argv[1][1] == 'u')
			return unpack32(argc, argv);
		else
			return main32(argc, argv);
		return main32(argc, argv);
	}
	__except(filter(GetExceptionCode(), GetExceptionInformation())) 
	{
		return -1;
	}
#endif

#else
	extern int main64(int argc, char *argv[]);
	return main64(argc, argv);
#endif


}
