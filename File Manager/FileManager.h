#pragma once
#include<iostream>
#include<vector>
#include <io.h>

class FileManager
{
public:
	void setPath(const char*);					//	set ����
	void Command();								//	�������
private:
	char CurrentPath[MAX_PATH];					//	����
	char FromPath[MAX_PATH];					//	���� ��
	char ToPath[MAX_PATH];						//	���� �
	_finddata_t fileinfo;
	char pathfind[MAX_PATH];

	void ShowFile();							//	����� ������ �����
	void ShowFolders();							//	����� ����� ����������
	void ShowFileFolders();						//	����� ����������� �����

	void CommandDEL();							//	������� ������� ���� ��� �����
	bool DeletFileOrFolder(const char*);		//	�������� ����� ��� �����
	void DeleteNonEmptyDirectory(const char*);	//	�������� �������� ����������

	void CommandMKDIR();						//	������� �������� �����
	bool CreateDir(const char*);				//	�������� �����

	void CommandRENAME();						//	������� �������������� ����� ��� �����
	void RenamFileOrFolder(const char*);		//	�������������� ����� ��� �����

	void CommandFILE();							//	������� �������� �����
	void FileCreation(const char*);				//	�������� �����

	void CommandCOPY();								//	������� ����������� ����� (�����) �� ����� ����� � ������
	void CopyFileFolders(const char*, const char*);	//	����������� ����� (�����) �� ����� ����� � ������
	bool Copy_File(const char*, const char*);		//	����������� �����
	

	void CommandMOVE();								//	�������	����������� ����� (�����) �� ����� ����� � ������
	void MoveFileFolders(const char*);				//	����������� ����� (�����) �� ����� ����� � ������

	void CommandCD();							//	������� CD
	void Help();
	bool ValidationFilePath(const char*);			//	�������� ���� � �����
	char* ReadPpath();								//	��������� ����
	bool EmptyDirectory(const char*);				//	������ ����������
};

