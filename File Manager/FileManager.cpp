#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <io.h>
#include <iostream>
#include <ctime>
#include <cstdio>
#include <direct.h>
#include <fstream>

#include "FileManager.h"


// ����� �� ����� �����
void FileManager::ShowFolders(){
	tm * timeinfo;

	strcpy(pathfind, CurrentPath);
	strcat(pathfind, "\\*.*");

	// ������ ������
	int result = _findfirst(pathfind, &fileinfo);

	int flag = result;
	if (result == -1) {std::cout << "\n����� ���������� ���\t"; }
	while (flag != -1){
		if (strcmp(fileinfo.name, ".") && strcmp(fileinfo.name, "..")){

			// ��������� ���������� ��� ���
			if (fileinfo.attrib&_A_SUBDIR) {
				timeinfo = _localtime64(&fileinfo.time_write);

				std::cout.width(2);
				std::cout << timeinfo->tm_mday << '.';

				std::cout.width(2);
				std::cout << timeinfo->tm_mon + 1 << '.' << timeinfo->tm_year + 1900 << ' ';
				std::cout.width(2);
				std::cout << timeinfo->tm_hour << ':';
				std::cout.width(2);
				std::cout << timeinfo->tm_min << "    <DIR>    " << fileinfo.name << std::endl;
			}
		}
		// ���������� �����
		flag = _findnext(result, &fileinfo);
	}
	// ������� �������, ���������� ��� �����
	_findclose(result);
}

void FileManager::ShowFile(){
		tm * timeinfo;

		_finddata_t fileinfo;

		char pathfind[MAX_PATH];
		strcpy(pathfind, CurrentPath);
		strcat(pathfind, "\\*.*");

		// ������ ������
		int result = _findfirst(pathfind, &fileinfo);

		int flag = result;

		/*if (result == -1) {std::cout << "\n����� ���������� ���\t"; }*/
		while (flag != -1){
			if (strcmp(fileinfo.name, ".") && strcmp(fileinfo.name, "..")){

				// ��������� ���������� ��� ���
				if (!(fileinfo.attrib&_A_SUBDIR)) {
					timeinfo = _localtime64(&fileinfo.time_write);

					std::cout.width(2);
					std::cout << timeinfo->tm_mday << '.';
					std::cout.width(2);
					std::cout << timeinfo->tm_mon + 1 << '.' << timeinfo->tm_year + 1900 << ' ';
					std::cout.width(2);
					std::cout << timeinfo->tm_hour << ':';
					std::cout.width(2);
					std::cout << timeinfo->tm_min << "             " << fileinfo.name;
					std::cout << "\t\t" << fileinfo.size << " bytes" << std::endl;
				}
			}
		// ���������� �����
		flag = _findnext(result, &fileinfo);
		}
	// ������� �������, ���������� ��� �����
	_findclose(result);
}

void FileManager::setPath(const char* curDir){
	strcpy_s(CurrentPath, curDir);

	*(strrchr(CurrentPath, '\\') + 1) = '\0';
}

void FileManager::Command(){
	const int size = 10;
	bool flag = true;
	// � ������ ���������� ����� �������, ��������� �������������
	char action[size];

	std::cout << '\n' << "������� ������� help" << std::endl;

	do{
		std::cout << CurrentPath << '>';

		std::cin >> action;

		if (!_strcmpi(action, "HELP")){
			Help();
		}
		else if (!_strcmpi(action, "ROOT")){
			CurrentPath[3] = '\0';
			ShowFileFolders();
		}
		// �������� �� ������� ������������ �����
		else if (!_strcmpi(action, "EXIT")){
			flag = false;
		}
		else if (!_strnicmp(action, "CD ", 2)){
			CommandCD();
		}
		else if (!_strcmpi(action, "DIR")){
			ShowFileFolders();
		}
		else if (!_strnicmp(action, "DEL ", 3)){
			CommandDEL();
		}
		else if (!_strnicmp(action, "MKDIR ", 5)){
			CommandMKDIR();
		}
		else if (!_strnicmp(action, "RENAME ", 6)){
			CommandRENAME();
		}
		else if (!_strnicmp(action, "FILE ", 4)){
			CommandFILE();
		}
		else if (!_strnicmp(action, "MOVE ", 4)){
			CommandMOVE();
		}
		else if (!_strnicmp(action, "COPY ", 4)){
			CommandCOPY();
		}
		else {
			std::cout << "����� ������� ��� " << action << std::endl;
		}
	} while (flag);
}

void FileManager::CommandCD() {				//	������� CD
	std::cin.get();
	std::cin.getline(ToPath, MAX_PATH);

	// �������� �� ������ ���� � ����������
	if (ToPath[1] == ':') {
		strcpy(CurrentPath, ToPath);
		if (ToPath[strlen(ToPath) - 1] != '\\'){
			strcat(CurrentPath, "\\");
		}
		ShowFileFolders();
	}
	else if (*ToPath == '.' && ToPath[1] == '.') {
		*strrchr(CurrentPath, '\\') = '\0';
		*(strrchr(CurrentPath, '\\') + 1) = '\0';
		ShowFileFolders();
	}
	else if (*ToPath == '\\') {
		CurrentPath[3] = '\0';
		ShowFileFolders();
	}
}

char* FileManager::ReadPpath() {								//	��������� ����
	std::cin.get();
	std::cin.getline(FromPath, MAX_PATH);
	if (!(FromPath[1] == ':' && FromPath[2] == '\\')){
		strcpy(ToPath, FromPath);
		strcpy(FromPath, CurrentPath);
		strcat(FromPath, ToPath);
	}
	return FromPath;
}

void FileManager::CommandCOPY() {								//	������� ����������� ����� (�����) �� ����� ����� � ������
	ReadPpath();
	std::cout << "������� ���� ��� ����������� ";
	std::cin.getline(ToPath, MAX_PATH);

	if (ValidationFilePath(FromPath)){
		if (Copy_File(FromPath, ToPath))
			std::cout << "���� ����������� ";
		else
			std::cout << "������. ���������� ����������� ���� ";

		std::cout << (strrchr(FromPath, '\\') + 1) << std::endl;
	}
	else
		CopyFileFolders(FromPath, ToPath);
}

void FileManager::CopyFileFolders(const char* From, const char* To){			//	����������� ����� (�����) �� ����� ����� � ������

	_finddata_t fileinfo;

	char FromPach[MAX_PATH];
	strcpy(FromPach, From);
	int sizeFrom = strlen(FromPach);


	char ToPach[MAX_PATH];
	strcpy(ToPach, To);

	int index = sizeFrom - 3;
	while (From[index] != '\\')
		index--;

	strcat(ToPach, &From[index]);
	int sizeTo = strlen(ToPach);

	CreateDir(ToPach);

	char pathfind[MAX_PATH];
	strcpy(pathfind, From);
	strcat(pathfind, "\\*.*");

	// ������ ������
	int result = _findfirst(pathfind, &fileinfo);

	int flag = result;
	if (result == -1) {
		std::cout << "\n����� ���������� ���\t";
	}
	while (flag != -1){
		if (strcmp(fileinfo.name, ".") && strcmp(fileinfo.name, "..")){

			// ��������� ���������� ��� ���
			if (fileinfo.attrib&_A_SUBDIR) {
				strcat(FromPach, "\\");
				CopyFileFolders(strcat(FromPach, fileinfo.name), ToPach);
				FromPach[sizeFrom] = '\0';
				ToPach[sizeTo] = '\0';
			}
			else{
				strcat(FromPach, "\\");
				strcat(ToPach, "\\");
				Copy_File(strcat(FromPach, fileinfo.name), ToPach);
				FromPach[sizeFrom] = '\0';
				ToPach[sizeTo] = '\0';
			}
		}
		// ���������� �����
		flag = _findnext(result, &fileinfo);
	}
	// ������� �������, ���������� ��� �����
	_findclose(result);
}

bool FileManager::Copy_File(const char* FromCopy, const char* WhereToCopy){					//	����������� �����
	bool flag = false;
	const int size = 4096;
	char buffer[size] = {};

	strcpy(ToPath, WhereToCopy);
	strcat(ToPath, "\\");
	strcat(ToPath, (strrchr(FromCopy, '\\') + 1));

	std::ifstream in(FromCopy, std::ios::binary | std::ios::in);
	if (in){

		std::ofstream out(ToPath, std::ios::binary | std::ios::out);
		if (out)
			flag = true;
		while (!in.eof()){
			in.read(buffer, size);
			out.write(buffer, in.gcount());
		}
		out.close();
	}
	in.close();
	return flag;
}

void FileManager::CommandMOVE() {							//	�������	����������� ����� (�����) �� ����� ����� � ������
	MoveFileFolders(ReadPpath());
}

void FileManager::MoveFileFolders(const char* path){				//	����������� ����� (�����) �� ����� ����� � ������
	std::cout << "������� ���� ��� ����������� ";
	std::cin.getline(ToPath, MAX_PATH);

	strcat(ToPath, "\\");
	strcat(ToPath, (strrchr(path, '\\') + 1));

	if (rename(path, ToPath))
		std::cout << "������ ���������� ����������� ";
	else
		std::cout << "���������� ";

	std::cout << (strrchr(path, '\\') + 1) << std::endl;
}

void FileManager::CommandFILE() {							//	������� �������� �����
	FileCreation(ReadPpath());
}

void FileManager::FileCreation(const char* path){	
	std::ofstream out(path);
	if (out)
		std::cout << "������� ���� ";
	else
		std::cout << "������! ���������� ������� ���� ";
	out.close();

	std::cout << (strrchr(path, '\\') + 1) << std::endl;
}

void FileManager::CommandRENAME() {						//	������� �������������� ����� ��� �����
	RenamFileOrFolder(ReadPpath());
}

void FileManager::RenamFileOrFolder(const char* path){

	char newName[50]; //  ����� ���(newName)
	std::cout << "������� ����� ��� ";
	std::cin.getline(newName, 50);

	strcpy(ToPath, path);
	*(strrchr(ToPath, '\\') + 1) = '\0';
	strcat(ToPath, newName);

	// ���������� �������������� � �������� ����������
	if (rename(path, ToPath))
		std::cout << "������! ���������� ������������� ";
	else
		std::cout << "������������� ";

	std::cout << (strrchr(path, '\\') + 1) << std::endl;
}

void FileManager::CommandMKDIR() {								//	������� �������� �����
	if(CreateDir(ReadPpath()))
		std::cout << "������� ����� ";
	else
		std::cout << "������! ���������� ������� ����� ";

	std::cout << (strrchr(FromPath, '\\') + 1) << std::endl;
}

bool FileManager::CreateDir(const char* path){
	//������� ���������� � ��������� ���������
	if (_mkdir(path) == -1)
		return false;
	else
		return true;
}

void FileManager::CommandDEL() {							//	������� ������� ���� ��� �����
	if (ValidationFilePath(ReadPpath())){
		if (DeletFileOrFolder(FromPath))
			std::cout << "������� ";
		else
			std::cout << "���������� ������� ";
	}
	else {
		DeleteNonEmptyDirectory(FromPath);
	}

	std::cout << (strrchr(FromPath, '\\') + 1) << std::endl;
}

void FileManager::DeleteNonEmptyDirectory(const char* path) {				//	�������� �������� ����������

	_finddata_t fileinfo;

	char FromPach[MAX_PATH];
	strcpy(FromPach, path);
	int sizeFrom = strlen(FromPach);

	char pathfind[MAX_PATH];
	strcpy(pathfind, FromPach);
	strcat(pathfind, "\\*.*");

	// ������ ������
	int result = _findfirst(pathfind, &fileinfo);

	int flag = result;
	if (result == -1) {
		std::cout << "\n����� ���������� ���\t";
	}
	while (flag != -1){
		if (strcmp(fileinfo.name, ".") && strcmp(fileinfo.name, "..")){

			// ��������� ���������� ��� ���
			if (fileinfo.attrib&_A_SUBDIR) {	//	�����
				strcat(FromPach, "\\");
				if (EmptyDirectory(strcat(FromPach, fileinfo.name))){
					DeleteNonEmptyDirectory(FromPach);
					*(strrchr(FromPach, '\\') + 1) = '\0';
				}
				else {
					DeletFileOrFolder(FromPach);
					*(strrchr(FromPach, '\\') + 1) = '\0';
				}
			}
			else{
				strcat(FromPach, "\\");
				DeletFileOrFolder(strcat(FromPach, fileinfo.name));
				*(strrchr(FromPach, '\\') + 1) = '\0';
			}
		}
		// ���������� �����
		flag = _findnext(result, &fileinfo);
	}
	// ������� �������, ���������� ��� �����
	_findclose(result);

	DeletFileOrFolder(path);
}

bool FileManager::DeletFileOrFolder(const char* path){
	bool flag = true;
	if (remove(path) != 0)
		if (_rmdir(path) == -1)
			flag = false;

	return flag;
}

void FileManager::Help(){
	std::cout << std::endl;
	std::cout << "CD ����         ����� ����� \n\t\t(��������, cd �:\\Windows\\System32)\n" << std::endl;
	std::cout << "CD ..           ������� �� ������������ �����\n\t\t(��������, ��� ���� �:\\Windows\\System32 \n\t\t������������ ������ ��������  �:\\Windows)\n" << std::endl;
	std::cout << "CD \\            ������� � �������� ���� ��� ������ �����\n\t\t(��������, ��� ���� �:\\Windows\\System\n\t\t�������� ������ �������� �:)\n" << std::endl;
	std::cout << "ROOT            ������� � �������� ���� ��� ������ �����\n" << std::endl;
	std::cout << "DIR             ����� ����������� ������� �����\n" << std::endl;
	std::cout << "MKDIR           MKDIR [����]���\n\t\t�������� �����\n" << std::endl;
	std::cout << "RENAME          RENAME [����]���\n\t\t�������������� ����� ��� �����\n" << std::endl;
	std::cout << "DEL             DEL [����]���\n\t\t�������� ����� ��� �����\n" << std::endl;
	std::cout << "FILE            FILE [����]���\n\t\t�������� �����\n" << std::endl;
	std::cout << "MOVE            MOVE [����]���\n\t\t����������� ����� (�����) �� ����� ����� � ������\n" << std::endl;
	std::cout << "COPY            COPY [����]���\n\t\t����������� ����� (�����) �� ����� ����� � ������\n" << std::endl;
	std::cout << "EXIT            ����� �� ��������" << std::endl;
	std::cout << std::endl;
}

void FileManager::ShowFileFolders(){						//	����� ����������� �����
	std::cout << "\n���������� ����� " << CurrentPath << std::endl << std::endl;
	ShowFolders();
	ShowFile();
}

bool FileManager::ValidationFilePath(const char* path){			//	�������� ���� � �����
	bool flag = true;

	std::ifstream in(path, std::ios::binary | std::ios::in);
	if (!in)
		flag = false;

	return flag;
}

bool FileManager::EmptyDirectory(const char* path) {				//	������ ����������
	strcpy(pathfind, path);
	strcat(pathfind, "\\*.*");

	// ������ ������
	int result = _findfirst(pathfind, &fileinfo);
	bool IsEmpty = false;
	int flag = result;

	while (flag != -1){
		if (strcmp(fileinfo.name, ".") && strcmp(fileinfo.name, ".."))	IsEmpty = true;

		flag = _findnext(result, &fileinfo);
	}
	// ������� �������, ���������� ��� �����
	_findclose(result);

	return IsEmpty;
}