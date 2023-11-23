#include "ControlReaders.h"

// Пейр нужен для вывода полей сертификата, взял из примера
std::pair<X500Dn::RdnId::Type, std::string> rdnIds[] = {
    std::make_pair(X500Dn::RdnId::commonName, "commonName"),
    std::make_pair(X500Dn::RdnId::surname, "surname"),
    std::make_pair(X500Dn::RdnId::givenName, "givenName"),
    std::make_pair(X500Dn::RdnId::title, "title"),
    std::make_pair(X500Dn::RdnId::pseudonym, "pseudonym"),
    std::make_pair(X500Dn::RdnId::emailAddress, "emailAddress"),
    std::make_pair(X500Dn::RdnId::countryName, "countryName"),
    std::make_pair(X500Dn::RdnId::localityName, "localityName"),
    std::make_pair(X500Dn::RdnId::stateOrProvinceName, "stateOrProvinceName"),
    std::make_pair(X500Dn::RdnId::organization, "organization"),
    std::make_pair(X500Dn::RdnId::organizationalUnit, "organizationalUnit"),
    std::make_pair(X500Dn::RdnId::street, "street"),
    std::make_pair(X500Dn::RdnId::ogrn, "ogrn"),
    std::make_pair(X500Dn::RdnId::ogrnip, "ogrnip"),
    std::make_pair(X500Dn::RdnId::snils, "snils"),
    std::make_pair(X500Dn::RdnId::inn, "inn")
};

// получаем серийные номера всех устройств
int getSerialNumberAllDevices(std::vector<std::string>& result)
{
    // Конструктор и деструктор от рутокенов
    rutoken::pkicore::initialize(".");
    SCOPE_EXIT()
    {
        rutoken::pkicore::deinitialize();
    };

    // Получаем список подключенных устройств
    std::vector<Pkcs11Device> devices = Pkcs11Device::enumerate();

    // Выводим устройства
    if (devices.size() == 0)
    {
        return 1;
    }

    for (Pkcs11Device& d : devices)
    {
        result.push_back(d.getSerialNumber());
    }

    return 0;
}

// читаем устройство
int readDevice(int index, std::string& resultCert)
{
    // Конструктор и деструктор от рутокенов
    rutoken::pkicore::initialize(".");
    SCOPE_EXIT()
    {
        rutoken::pkicore::deinitialize();
    };

    // Получаем список подключенных устройств
    std::vector<Pkcs11Device> devices = Pkcs11Device::enumerate();

    // Выводим устройства
    if (devices.size() == 0)
    {
        return 1;
    }

    Pkcs11Device& d = devices[index];

    // Выводим серийный номер устройства
    resultCert += "Устройство с серийным номером: " + d.getSerialNumber() + "\r\n\r\n";

    // Кому принадлежит сертификат на устройстве
    std::vector<Pkcs11Cert> certs = d.enumerateCerts();
    for (unsigned int i = 0; i < certs.size(); ++i)
    {
        resultCert += "Сертификат номер " + std::to_string(i + 1) + "\r\n";
        Pkcs11Cert& cert = certs[i];
        X500Dn subject = cert.getSubject();

        std::string str;
        stringToANSI(subject.getRdn(rdnIds[0].first), str);

        resultCert += "Владелец сертификата: " + str + "\r\n\r\n";
    }

    return 0;
}

// получаем устройства
int getListDevice(std::vector<std::string>& vecReader, SCARDCONTEXT hContext)
{
    LPCSTR  pReader; // список ридеров
    LONG    result; // результат
    LPCSTR  pmszReaders = NULL; // имена групп ридеров, NULL - все
    DWORD   cch = SCARD_AUTOALLOCATE; // длина буффера

    // Получаем список ридеров
    result = SCardListReadersA(hContext,
        pmszReaders,
        (LPSTR)&pmszReaders,
        &cch);
    if (SCARD_E_NO_READERS_AVAILABLE == result)
    {
        return 1;
    }
    if (SCARD_S_SUCCESS != result)
    {
        return result;
    }

    pReader = pmszReaders;

    if (hContext == NULL) // вне контекста показываем все ридеры
    {
        while ('\0' != *pReader)
        {
            std::string reader(pReader); // для добавления в вектор
            vecReader.push_back(reader);
            pReader = pReader + strlen((char*)pReader) + 1;
        }
    }
    else
    {
        while ('\0' != *pReader)
        {
            std::string reader(pReader); // для добавления в вектор
            if (reader.find("Rutoken") != std::string::npos)
            {
                vecReader.push_back(reader);
            }
            pReader = pReader + strlen((char*)pReader) + 1;
        }
    }

    // Освобождение памяти
    result = SCardFreeMemory(hContext, pmszReaders);
    if (SCARD_S_SUCCESS != result)
    {
        return result;
    }
    return 0;
}

// Получаем контекст
int getContextCard(SCARDCONTEXT& hContext)
{
    LONG result; // результат

    result = SCardReleaseContext(hContext);
    if (SCARD_S_SUCCESS != result)
    {
        if (ERROR_INVALID_HANDLE != result)
        {
            return result;
        }
    }

    // Получаем контекст
    result = SCardEstablishContext(SCARD_SCOPE_SYSTEM,
        NULL,
        NULL,
        &hContext);
    if (SCARD_S_SUCCESS != result)
    {
        return result;
    }
    return 0;
}

// Получение атрибутов ридеров
int getAttrReader(std::vector<std::string>& vecAttrReader,
    SCARDCONTEXT hContext,
    std::vector<std::string> vecReader)
{
    SCARDHANDLE  hCardHandle; // дискриптор ридера
    DWORD        dwAP;        // флаг протокола
    LONG result; // результат
    LPBYTE pbAttr = NULL; // атрибуты
    LPBYTE pbAttrTemp = NULL; // копия для записи в вектор

    for (unsigned int i = 0; i < vecReader.size(); ++i)
    {
        // Подключаемся к ридеру
        result = SCardConnect(hContext,
            vecReader[i].c_str(),
            SCARD_SHARE_SHARED,
            SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
            &hCardHandle,
            &dwAP);
        if (SCARD_S_SUCCESS != result)
        {
            return result;
        }

        DWORD cByte = SCARD_AUTOALLOCATE; // длина буффера

        // Получаем атрибуты ридера
        result = SCardGetAttrib(hCardHandle,
            SCARD_ATTR_DEVICE_SYSTEM_NAME,
            (LPBYTE)&pbAttr,
            &cByte);
        if (SCARD_S_SUCCESS != result)
        {
            return result;
        }

        // Пишем атрибуты в вектор
        pbAttrTemp = pbAttr;
        std::string readerAttr((char*)pbAttrTemp);
        vecAttrReader.push_back(readerAttr);

        // Дисконект от ридера
        result = SCardDisconnect(hCardHandle, SCARD_LEAVE_CARD);
        if (SCARD_S_SUCCESS != result)
        {
            return result;
        }

        // Освобождение памяти
        result = SCardFreeMemory(hContext, pbAttr);
        if (SCARD_S_SUCCESS != result)
        {
            return result;
        }
    }
    return 0;
}

// удаление ридера
int deleteReader(std::string oldName, SCARDCONTEXT hContext)
{
    LONG result = -1; // результат

    // Удаляем старое имя
    result = SCardForgetReader(hContext, oldName.c_str());
    if (SCARD_S_SUCCESS != result)
    {
        return result;
    }
    return 0;
}

// добавление ридера
int addNameReader(std::string newName, SCARDCONTEXT hContext, std::string pbAttr)
{
    LONG result = -1; // результат
    
    // Добавляем новое имя ридеру
    result = SCardIntroduceReader(hContext, newName.c_str(), pbAttr.c_str());
    if (SCARD_S_SUCCESS != result)
    {
        return result;
    }
    return 0;
}

// Переводим в ANSI
int stringToANSI(std::string src, std::string& strANSI)
{
    int size = MultiByteToWideChar(CP_UTF8, NULL, src.c_str(), src.length(), NULL, NULL);
    if (size == 0)
    {
        return 1;
    }

    std::wstring w_str(size, 0);
    int err = MultiByteToWideChar(CP_UTF8, NULL, src.c_str(), src.length(), &w_str[0], size);
    if (err == 0)
    {
        return 2;
    }

    int strANSI_size = WideCharToMultiByte(CP_ACP, NULL, w_str.c_str(), w_str.length(), NULL, NULL, NULL, NULL);
    if (strANSI_size == 0)
    {
        return 3;
    }

    std::string ANSI_str(strANSI_size, 0);
    err = WideCharToMultiByte(CP_ACP, NULL, w_str.c_str(), ANSI_str.length(), &ANSI_str[0], strANSI_size, NULL, NULL);
    if (err == 0)
    {
        return 4;
    }

    strANSI = ANSI_str;

    return 0;
}