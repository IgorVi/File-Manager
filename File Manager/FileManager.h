#pragma once
#include<iostream>
#include<vector>
#include <io.h>

class FileManager
{
public:
	void setPath(const char*);					//	set путь
	void Command();								//	команды
private:
	char CurrentPath[MAX_PATH];					//	путь
	char FromPath[MAX_PATH];					//	путь из
	char ToPath[MAX_PATH];						//	путь в
	_finddata_t fileinfo;
	char pathfind[MAX_PATH];

	void ShowFile();							//	показ файлов папки
	void ShowFolders();							//	показ папок директории
	void ShowFileFolders();						//	показ содержимово папки

	void CommandDEL();							//	команда удалить файл или папку
	bool DeletFileOrFolder(const char*);		//	удаление файла или папки
	void DeleteNonEmptyDirectory(const char*);	//	удаление непустая директории

	void CommandMKDIR();						//	команда создание папки
	bool CreateDir(const char*);				//	создание папки

	void CommandRENAME();						//	команда переименование файла или папки
	void RenamFileOrFolder(const char*);		//	переименование файла или папки

	void CommandFILE();							//	команда создание файла
	void FileCreation(const char*);				//	создание файла

	void CommandCOPY();								//	команда копирование файла (папки) из одной папки в другую
	void CopyFileFolders(const char*, const char*);	//	копирование файла (папки) из одной папки в другую
	bool Copy_File(const char*, const char*);		//	копирование файла
	

	void CommandMOVE();								//	команда	перемещение файла (папки) из одной папки в другую
	void MoveFileFolders(const char*);				//	перемещение файла (папки) из одной папки в другую

	void CommandCD();							//	команды CD
	void Help();
	bool ValidationFilePath(const char*);			//	проверка пути к файлу
	char* ReadPpath();								//	прочитать путь
	bool EmptyDirectory(const char*);				//	пустая директории
};

