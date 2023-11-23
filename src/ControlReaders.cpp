#include "ControlReaders.h"

// ���� ����� ��� ������ ����� �����������, ���� �� �������
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

// �������� �������� ������ ���� ���������
int getSerialNumberAllDevices(std::vector<std::string>& result)
{
    // ����������� � ���������� �� ���������
    rutoken::pkicore::initialize(".");
    SCOPE_EXIT()
    {
        rutoken::pkicore::deinitialize();
    };

    // �������� ������ ������������ ���������
    std::vector<Pkcs11Device> devices = Pkcs11Device::enumerate();

    // ������� ����������
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

// ������ ����������
int readDevice(int index, std::string& resultCert)
{
    // ����������� � ���������� �� ���������
    rutoken::pkicore::initialize(".");
    SCOPE_EXIT()
    {
        rutoken::pkicore::deinitialize();
    };

    // �������� ������ ������������ ���������
    std::vector<Pkcs11Device> devices = Pkcs11Device::enumerate();

    // ������� ����������
    if (devices.size() == 0)
    {
        return 1;
    }

    Pkcs11Device& d = devices[index];

    // ������� �������� ����� ����������
    resultCert += "���������� � �������� �������: " + d.getSerialNumber() + "\r\n\r\n";

    // ���� ����������� ���������� �� ����������
    std::vector<Pkcs11Cert> certs = d.enumerateCerts();
    for (unsigned int i = 0; i < certs.size(); ++i)
    {
        resultCert += "���������� ����� " + std::to_string(i + 1) + "\r\n";
        Pkcs11Cert& cert = certs[i];
        X500Dn subject = cert.getSubject();

        std::string str;
        stringToANSI(subject.getRdn(rdnIds[0].first), str);

        resultCert += "�������� �����������: " + str + "\r\n\r\n";
    }

    return 0;
}

// �������� ����������
int getListDevice(std::vector<std::string>& vecReader, SCARDCONTEXT hContext)
{
    LPCSTR  pReader; // ������ �������
    LONG    result; // ���������
    LPCSTR  pmszReaders = NULL; // ����� ����� �������, NULL - ���
    DWORD   cch = SCARD_AUTOALLOCATE; // ����� �������

    // �������� ������ �������
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

    if (hContext == NULL) // ��� ��������� ���������� ��� ������
    {
        while ('\0' != *pReader)
        {
            std::string reader(pReader); // ��� ���������� � ������
            vecReader.push_back(reader);
            pReader = pReader + strlen((char*)pReader) + 1;
        }
    }
    else
    {
        while ('\0' != *pReader)
        {
            std::string reader(pReader); // ��� ���������� � ������
            if (reader.find("Rutoken") != std::string::npos)
            {
                vecReader.push_back(reader);
            }
            pReader = pReader + strlen((char*)pReader) + 1;
        }
    }

    // ������������ ������
    result = SCardFreeMemory(hContext, pmszReaders);
    if (SCARD_S_SUCCESS != result)
    {
        return result;
    }
    return 0;
}

// �������� ��������
int getContextCard(SCARDCONTEXT& hContext)
{
    LONG result; // ���������

    result = SCardReleaseContext(hContext);
    if (SCARD_S_SUCCESS != result)
    {
        if (ERROR_INVALID_HANDLE != result)
        {
            return result;
        }
    }

    // �������� ��������
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

// ��������� ��������� �������
int getAttrReader(std::vector<std::string>& vecAttrReader,
    SCARDCONTEXT hContext,
    std::vector<std::string> vecReader)
{
    SCARDHANDLE  hCardHandle; // ���������� ������
    DWORD        dwAP;        // ���� ���������
    LONG result; // ���������
    LPBYTE pbAttr = NULL; // ��������
    LPBYTE pbAttrTemp = NULL; // ����� ��� ������ � ������

    for (unsigned int i = 0; i < vecReader.size(); ++i)
    {
        // ������������ � ������
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

        DWORD cByte = SCARD_AUTOALLOCATE; // ����� �������

        // �������� �������� ������
        result = SCardGetAttrib(hCardHandle,
            SCARD_ATTR_DEVICE_SYSTEM_NAME,
            (LPBYTE)&pbAttr,
            &cByte);
        if (SCARD_S_SUCCESS != result)
        {
            return result;
        }

        // ����� �������� � ������
        pbAttrTemp = pbAttr;
        std::string readerAttr((char*)pbAttrTemp);
        vecAttrReader.push_back(readerAttr);

        // ��������� �� ������
        result = SCardDisconnect(hCardHandle, SCARD_LEAVE_CARD);
        if (SCARD_S_SUCCESS != result)
        {
            return result;
        }

        // ������������ ������
        result = SCardFreeMemory(hContext, pbAttr);
        if (SCARD_S_SUCCESS != result)
        {
            return result;
        }
    }
    return 0;
}

// �������� ������
int deleteReader(std::string oldName, SCARDCONTEXT hContext)
{
    LONG result = -1; // ���������

    // ������� ������ ���
    result = SCardForgetReader(hContext, oldName.c_str());
    if (SCARD_S_SUCCESS != result)
    {
        return result;
    }
    return 0;
}

// ���������� ������
int addNameReader(std::string newName, SCARDCONTEXT hContext, std::string pbAttr)
{
    LONG result = -1; // ���������
    
    // ��������� ����� ��� ������
    result = SCardIntroduceReader(hContext, newName.c_str(), pbAttr.c_str());
    if (SCARD_S_SUCCESS != result)
    {
        return result;
    }
    return 0;
}

// ��������� � ANSI
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