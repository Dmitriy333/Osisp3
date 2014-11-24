// OSISP3.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <conio.h>
#include <iostream>
static int n = 0;
int _tmain(int argc, _TCHAR* argv[])
{
	CHAR firstStr[255] = "abcdefghijklmnopqrstuvwxyz";
	CHAR bufferStr[255];
	HANDLE hMutex = CreateMutex(NULL, FALSE, L"Global\\MainMutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)//дескриптор последней ошибки
		std::cout << "Program has been started." << std::endl;
	
	HANDLE hMemorySpace = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 255, L"SharedMemory");//проекция файла в памяти
	
	void* sharedMemory =  MapViewOfFile(hMemorySpace, FILE_WRITE_ACCESS, 0, 0, 255);

	if (GetLastError() == ERROR_ALREADY_EXISTS)
		hMemorySpace = OpenFileMapping(FILE_WRITE_ACCESS, FALSE, L"SharedMemory");
	else{
		std::cout<< firstStr<<std::endl;
		CopyMemory(sharedMemory, firstStr, 255);
	}

	getchar();

	while(true){
		std::cout << "Awaiting..." << std::endl;
		WaitForSingleObject(hMutex, INFINITE);
		std::cout << "Enter critical section" << std::endl;
		memcpy(bufferStr, sharedMemory, 255);
		int i = GetCurrentProcessId();
		n++;
		std::cout << "ID:" << i << " " << bufferStr << std::endl;
		std::cout << "try to write into shared memory" << std::endl;
		for (int j = 0; j < strlen(bufferStr); j++)
		{
			bufferStr[j]++;
		}
		memcpy(sharedMemory, bufferStr, 255);
		Sleep(3000);
		std::cout << "Leave critical section" << std::endl;
		ReleaseMutex(hMutex);
	}

	std::cout << "the end" << std::endl;
	UnmapViewOfFile(sharedMemory);
	CloseHandle(hMemorySpace);
	CloseHandle(hMutex);
	system("PAUSE");
	return 0;
}