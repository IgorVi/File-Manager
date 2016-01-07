#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <io.h>
#include <iostream>
#include <ctime>
#include <cstdio>
#include <direct.h>
#include <fstream>

#include "FileManager.h"


// Показ на экран папки
void FileManager::ShowFolders(){
	tm * timeinfo;

	strcpy(pathfind, CurrentPath);
	strcat(pathfind, "\\*.*");

	// Начало Поиска
	int result = _findfirst(pathfind, &fileinfo);

	int flag = result;
	if (result == -1) {std::cout << "\nТакой директории нет\t"; }
	while (flag != -1){
		if (strcmp(fileinfo.name, ".") && strcmp(fileinfo.name, "..")){

			// Проверяем Директория или Нет
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
		// Продолжаем Поиск
		flag = _findnext(result, &fileinfo);
	}
	// Очищаем ресурсы, выделенные под поиск
	_findclose(result);
}

void FileManager::ShowFile(){
		tm * timeinfo;

		_finddata_t fileinfo;

		char pathfind[MAX_PATH];
		strcpy(pathfind, CurrentPath);
		strcat(pathfind, "\\*.*");

		// Начало Поиска
		int result = _findfirst(pathfind, &fileinfo);

		int flag = result;

		/*if (result == -1) {std::cout << "\nТакой директории нет\t"; }*/
		while (flag != -1){
			if (strcmp(fileinfo.name, ".") && strcmp(fileinfo.name, "..")){

				// Проверяем Директория или Нет
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
		// Продолжаем Поиск
		flag = _findnext(result, &fileinfo);
		}
	// Очищаем ресурсы, выделенные под поиск
	_findclose(result);
}

void FileManager::setPath(const char* curDir){
	strcpy_s(CurrentPath, curDir);

	*(strrchr(CurrentPath, '\\') + 1) = '\0';
}

void FileManager::Command(){
	const int size = 10;
	bool flag = true;
	// В данной переменной будет команда, введенная пользователем
	char action[size];

	std::cout << '\n' << "Справка команда help" << std::endl;

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
		// Проверка на желание пользователя выйти
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
			std::cout << "Такой команды нет " << action << std::endl;
		}
	} while (flag);
}

void FileManager::CommandCD() {				//	команды CD
	std::cin.get();
	std::cin.getline(ToPath, MAX_PATH);

	// Проверка на полный путь к Директории
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

char* FileManager::ReadPpath() {								//	прочитать путь
	std::cin.get();
	std::cin.getline(FromPath, MAX_PATH);
	if (!(FromPath[1] == ':' && FromPath[2] == '\\')){
		strcpy(ToPath, FromPath);
		strcpy(FromPath, CurrentPath);
		strcat(FromPath, ToPath);
	}
	return FromPath;
}

void FileManager::CommandCOPY() {								//	команда копирование файла (папки) из одной папки в другую
	ReadPpath();
	std::cout << "Введите путь для копирования ";
	std::cin.getline(ToPath, MAX_PATH);

	if (ValidationFilePath(FromPath)){
		if (Copy_File(FromPath, ToPath))
			std::cout << "Файл скопировано ";
		else
			std::cout << "Ошибка. Невозможно скопировать файл ";

		std::cout << (strrchr(FromPath, '\\') + 1) << std::endl;
	}
	else
		CopyFileFolders(FromPath, ToPath);
}

void FileManager::CopyFileFolders(const char* From, const char* To){			//	копирование файла (папки) из одной папки в другую

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

	// Начало Поиска
	int result = _findfirst(pathfind, &fileinfo);

	int flag = result;
	if (result == -1) {
		std::cout << "\nТакой директории нет\t";
	}
	while (flag != -1){
		if (strcmp(fileinfo.name, ".") && strcmp(fileinfo.name, "..")){

			// Проверяем Директория или Нет
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
		// Продолжаем Поиск
		flag = _findnext(result, &fileinfo);
	}
	// Очищаем ресурсы, выделенные под поиск
	_findclose(result);
}

bool FileManager::Copy_File(const char* FromCopy, const char* WhereToCopy){					//	копирование файла
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

void FileManager::CommandMOVE() {							//	команда	перемещение файла (папки) из одной папки в другую
	MoveFileFolders(ReadPpath());
}

void FileManager::MoveFileFolders(const char* path){				//	перемещение файла (папки) из одной папки в другую
	std::cout << "Введите путь для перемещения ";
	std::cin.getline(ToPath, MAX_PATH);

	strcat(ToPath, "\\");
	strcat(ToPath, (strrchr(path, '\\') + 1));

	if (rename(path, ToPath))
		std::cout << "Ошибка Невозможно переместить ";
	else
		std::cout << "Перемещено ";

	std::cout << (strrchr(path, '\\') + 1) << std::endl;
}

void FileManager::CommandFILE() {							//	команда создание файла
	FileCreation(ReadPpath());
}

void FileManager::FileCreation(const char* path){	
	std::ofstream out(path);
	if (out)
		std::cout << "Создано файл ";
	else
		std::cout << "Ошибка! Невозможно создать файл ";
	out.close();

	std::cout << (strrchr(path, '\\') + 1) << std::endl;
}

void FileManager::CommandRENAME() {						//	команда переименование файла или папки
	RenamFileOrFolder(ReadPpath());
}

void FileManager::RenamFileOrFolder(const char* path){

	char newName[50]; //  новое имя(newName)
	std::cout << "Введите новое имя ";
	std::cin.getline(newName, 50);

	strcpy(ToPath, path);
	*(strrchr(ToPath, '\\') + 1) = '\0';
	strcat(ToPath, newName);

	// Произведем переименование и проверку результата
	if (rename(path, ToPath))
		std::cout << "Ошибка! Невозможно переименовать ";
	else
		std::cout << "Переименовано ";

	std::cout << (strrchr(path, '\\') + 1) << std::endl;
}

void FileManager::CommandMKDIR() {								//	команда создание папки
	if(CreateDir(ReadPpath()))
		std::cout << "Создано папку ";
	else
		std::cout << "Ошибка! Невозможно создать папку ";

	std::cout << (strrchr(FromPath, '\\') + 1) << std::endl;
}

bool FileManager::CreateDir(const char* path){
	//Создаем директорию и проверяем результат
	if (_mkdir(path) == -1)
		return false;
	else
		return true;
}

void FileManager::CommandDEL() {							//	команда удалить файл или папку
	if (ValidationFilePath(ReadPpath())){
		if (DeletFileOrFolder(FromPath))
			std::cout << "Удалено ";
		else
			std::cout << "Невозможно удалить ";
	}
	else {
		DeleteNonEmptyDirectory(FromPath);
	}

	std::cout << (strrchr(FromPath, '\\') + 1) << std::endl;
}

void FileManager::DeleteNonEmptyDirectory(const char* path) {				//	удаление непустая директории

	_finddata_t fileinfo;

	char FromPach[MAX_PATH];
	strcpy(FromPach, path);
	int sizeFrom = strlen(FromPach);

	char pathfind[MAX_PATH];
	strcpy(pathfind, FromPach);
	strcat(pathfind, "\\*.*");

	// Начало Поиска
	int result = _findfirst(pathfind, &fileinfo);

	int flag = result;
	if (result == -1) {
		std::cout << "\nТакой директории нет\t";
	}
	while (flag != -1){
		if (strcmp(fileinfo.name, ".") && strcmp(fileinfo.name, "..")){

			// Проверяем Директория или Нет
			if (fileinfo.attrib&_A_SUBDIR) {	//	папка
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
		// Продолжаем Поиск
		flag = _findnext(result, &fileinfo);
	}
	// Очищаем ресурсы, выделенные под поиск
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
	std::cout << "CD путь         смена папки \n\t\t(например, cd С:\\Windows\\System32)\n" << std::endl;
	std::cout << "CD ..           переход на родительскую папку\n\t\t(например, для пути С:\\Windows\\System32 \n\t\tродительской папкой является  С:\\Windows)\n" << std::endl;
	std::cout << "CD \\            переход в корневой диск для данной папки\n\t\t(например, для пути С:\\Windows\\System\n\t\tкорневой папкой является С:)\n" << std::endl;
	std::cout << "ROOT            переход в корневой диск для данной папки\n" << std::endl;
	std::cout << "DIR             показ содержимого текущей папки\n" << std::endl;
	std::cout << "MKDIR           MKDIR [путь]имя\n\t\tсоздание папки\n" << std::endl;
	std::cout << "RENAME          RENAME [путь]имя\n\t\tпереименование файла или папки\n" << std::endl;
	std::cout << "DEL             DEL [путь]имя\n\t\tудаление файла или папки\n" << std::endl;
	std::cout << "FILE            FILE [путь]имя\n\t\tсоздание файла\n" << std::endl;
	std::cout << "MOVE            MOVE [путь]имя\n\t\tперемещение файла (папки) из одной папки в другую\n" << std::endl;
	std::cout << "COPY            COPY [путь]имя\n\t\tкопирование файла (папки) из одной папки в другую\n" << std::endl;
	std::cout << "EXIT            выход из програмы" << std::endl;
	std::cout << std::endl;
}

void FileManager::ShowFileFolders(){						//	показ содержимово папки
	std::cout << "\nСодержимое папки " << CurrentPath << std::endl << std::endl;
	ShowFolders();
	ShowFile();
}

bool FileManager::ValidationFilePath(const char* path){			//	проверка пути к файлу
	bool flag = true;

	std::ifstream in(path, std::ios::binary | std::ios::in);
	if (!in)
		flag = false;

	return flag;
}

bool FileManager::EmptyDirectory(const char* path) {				//	пустая директории
	strcpy(pathfind, path);
	strcat(pathfind, "\\*.*");

	// Начало Поиска
	int result = _findfirst(pathfind, &fileinfo);
	bool IsEmpty = false;
	int flag = result;

	while (flag != -1){
		if (strcmp(fileinfo.name, ".") && strcmp(fileinfo.name, ".."))	IsEmpty = true;

		flag = _findnext(result, &fileinfo);
	}
	// Очищаем ресурсы, выделенные под поиск
	_findclose(result);

	return IsEmpty;
}