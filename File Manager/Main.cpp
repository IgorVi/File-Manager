#include <windows.h>
#include <iostream>
#include <conio.h>

#include "FileManager.h"

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	system("color 1F");

	FileManager fm;

	fm.setPath(argv[0]);
	
	fm.Command();
	
	return 0;
}