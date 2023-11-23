#pragma once
#include "winscard.h" // ��� ���������
#include <string>
#include <vector>
#include "common.h"

using namespace rutoken::pkicore;

int getSerialNumberAllDevices(std::vector<std::string>& result); // �������� �������� ������ ���� ���������
int readDevice(int index, std::string& resultCert); // ������ ����������
int getListDevice(std::vector<std::string>& vecReader, SCARDCONTEXT hContext); // �������� ����������
int getContextCard(SCARDCONTEXT& hContext); // �������� ��������

// ��������� ��������� �������
int getAttrReader(std::vector<std::string>& vecAttrReader,
	SCARDCONTEXT hContext,
	std::vector<std::string> vecReader);

int deleteReader(std::string oldName, SCARDCONTEXT hContext); // �������� ������
int addNameReader(std::string newName, SCARDCONTEXT hContext, std::string pbAttr); // ���������� ������

// ��������� � ANSI
int stringToANSI(std::string src, std::string& strUtf8);