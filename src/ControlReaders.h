#pragma once
#include "winscard.h" // для смарткарт
#include <string>
#include <vector>
#include "common.h"

using namespace rutoken::pkicore;

int getSerialNumberAllDevices(std::vector<std::string>& result); // получаем серийные номера всех устройств
int readDevice(int index, std::string& resultCert); // читаем устройство
int getListDevice(std::vector<std::string>& vecReader, SCARDCONTEXT hContext); // получаем устройства
int getContextCard(SCARDCONTEXT& hContext); // Получаем контекст

// Получение атрибутов ридеров
int getAttrReader(std::vector<std::string>& vecAttrReader,
	SCARDCONTEXT hContext,
	std::vector<std::string> vecReader);

int deleteReader(std::string oldName, SCARDCONTEXT hContext); // удаление ридера
int addNameReader(std::string newName, SCARDCONTEXT hContext, std::string pbAttr); // добавление ридера

// Переводим в ANSI
int stringToANSI(std::string src, std::string& strUtf8);