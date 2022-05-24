#include "control.h"

Control::Control(QObject *parent) : QObject(parent)
{
}

// Структура нужна для вывода полей сертификата, взял из примера
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

// Копирование папки утм
int Control::copyPath(QString sourceDir, QString destinationDir)
{
    QDir originDirectory(sourceDir);
    if (!originDirectory.exists())
    {
        return 1;
    }
    originDirectory.mkpath(destinationDir);

    foreach (QString directoryName, originDirectory.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        QString destinationPath = destinationDir + "/" + directoryName;
        originDirectory.mkpath(destinationPath);
        copyPath(sourceDir + "/" + directoryName, destinationPath);
    }
    foreach (QString fileName, originDirectory.entryList(QDir::Files))
    {
        QFile::copy(sourceDir + "/" + fileName, destinationDir + "/" + fileName);
    }

    QDir finalDestination(destinationDir);
    finalDestination.refresh();
    if(finalDestination.exists())
    {
        return 0;
    }
    return 1;
}

// Выполнение команды в консоли
int Control::executeCommand(QProcess *process, const QString &command)
{
    process->start(command);
    if(!process->waitForFinished(60000))
    {
        return 1;
    }
//    Sleep(5000);
    return 0;
}

// Переименование папки УТМ
int Control::renamePathUTM(QDir &dir, const QString &newPathUTM)
{
    if (!dir.rename("c:/UTM", newPathUTM))
    {
        return 1;
    }
    return 0;
}

// Замена батников в УТМ
int Control::replaceBatUTM(const QString &copyPathDelete,
                              const QString &copyPathInstall,
                              const QString &copyPathRun,
                              const QString &copyPathStop,
                              const QString &pastePathDelete,
                              const QString &pastePathInstall,
                              const QString &pastePathRun,
                              const QString &pastePathStop)
{
    // Удаляем батники из утм
    QFile::remove(pastePathDelete);
    QFile::remove(pastePathInstall);
    QFile::remove(pastePathRun);
    QFile::remove(pastePathStop);

    // Копируем свои батники в утм
    QFile::copy(copyPathDelete, pastePathDelete);
    QFile::copy(copyPathInstall, pastePathInstall);
    QFile::copy(copyPathRun, pastePathRun);
    QFile::copy(copyPathStop, pastePathStop);

    return 0;
}

// Редактирование порта в конфиге
int Control::replacePortUTM(QByteArray &fileData, const QString &pathConfig, QString &port)
{
    QFile file_read(pathConfig);
    if (!file_read.open(QIODevice::ReadOnly))
    {
        return 1;
    }
    fileData = file_read.readAll();
    QString temp;
    temp = temp.fromLocal8Bit(fileData);
    temp.replace("8080", port);
    file_read.close();

    QFile file_clear(pathConfig);
    if (!file_clear.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return 1;
    }
    file_clear.close();

    QFile file_write(pathConfig);
    if (!file_write.open(QIODevice::WriteOnly))
    {
        return 1;
    }
    file_write.write(temp.toLocal8Bit());
    file_write.close();
    return 0;
}

// Метод проверки доступа к УТМу
QString Control::check_utm(QString url)
{
    QString res_utm;
    //Выполняем запрос к сайту
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(url.toUtf8()));
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    QNetworkReply *response = manager.get(request);
    QEventLoop event;
    connect(response, &QNetworkReply::finished, &event, &QEventLoop::quit);
    event.exec();
    //Получаем ответ
    res_utm = response->readAll();
    return res_utm;
}

// Получение всех серийных номеров
int Control::getSerialNumberDevice(QVector<QString> &vecSerialNumberDevice)
{
    vecSerialNumberDevice.clear();

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

    for (uint i = 0; i < devices.size(); ++i)
    {
        Pkcs11Device& d = devices[i];

        // Выводим серийный номер устройства
        vecSerialNumberDevice.push_back(QString::fromStdString(d.getSerialNumber()));
    }
    return 0;
}

// Получаем контекст
int Control::getContextCard(SCARDCONTEXT &hContext)
{
    LONG result; // результат

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
int Control::getAttrReader(QVector<QString> &vecAttrReader,
                           SCARDCONTEXT hContext,
                           QVector<QString> vecReader)
{
    SCARDHANDLE  hCardHandle; // дискриптор ридера
    DWORD        dwAP;        // флаг протокола
    LONG result; // результат
    LPBYTE pbAttr = NULL; // атрибуты
    LPBYTE pbAttrTemp = NULL; // копия для записи в вектор
    QString readerAttr; // для добавления в вектор

    for (int i = 0; i < vecReader.size(); ++i)
    {
        wchar_t *readerNameW = new wchar_t[vecReader[i].length() + 1]; // инициализируем массивы wchar
        readerNameW[vecReader[i].toWCharArray(readerNameW)] = '\0';    // приводим QString к wchar

        // Подключаемся к ридеру
        result = SCardConnectW(hContext,
                               readerNameW,
                               SCARD_SHARE_SHARED,
                               SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
                               &hCardHandle,
                               &dwAP);
        if (SCARD_S_SUCCESS != result)
        {
            delete[] readerNameW;
            return result;
        }

        DWORD  cByte = SCARD_AUTOALLOCATE; // длина буффера

        // Получаем атрибуты ридера
        result = SCardGetAttrib(hCardHandle,
                                SCARD_ATTR_DEVICE_SYSTEM_NAME,
                                (LPBYTE)&pbAttr,
                                &cByte);
        if (SCARD_S_SUCCESS != result)
        {

            delete[] readerNameW;
            return result;
        }

        // Пишем атрибуты в вектор
        pbAttrTemp = pbAttr;
        // Приведение wchar к QString и запись в вектор
        readerAttr = "";
        while ('\0' != *pbAttrTemp)
        {
            readerAttr += readerAttr.fromUtf8((char *)pbAttrTemp);
            pbAttrTemp = pbAttrTemp + strlen((char *)pbAttrTemp) + 1;
        }
        vecAttrReader.push_back(readerAttr);

        // Дисконект от ридера
        result = SCardDisconnect(hCardHandle, SCARD_LEAVE_CARD);
        if (SCARD_S_SUCCESS != result)
        {
            delete[] readerNameW;
            return result;
        }

        // Освобождение памяти
        result = SCardFreeMemory(hContext, pbAttr);
        if (SCARD_S_SUCCESS != result)
        {
            delete[] readerNameW;
            return result;
        }
    }
    return 0;
}

// Получение ридеров в контексте, NULL - вне контекста
int Control::listDevice(QVector<QString> &vecReader, SCARDCONTEXT hContext)
{
    LPWSTR  pReader; // список ридеров
    LONG    result; // результат
    LPWSTR  pmszReaders = NULL; // имена групп ридеров, NULL - все
    DWORD   cch = SCARD_AUTOALLOCATE; // длина буффера
    QString reader; // для добавления в вектор

    // Получаем список ридеров
    result = SCardListReaders(hContext,
                              pmszReaders,
                              (LPWSTR)&pmszReaders,
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
    // Приведение wchar к QString и запись в вектор
    if (hContext == NULL)
    {
        while ('\0' != *pReader)
        {
            reader = reader.fromWCharArray(pReader);
            vecReader.push_back(reader);
            pReader = pReader + wcslen((wchar_t *)pReader) + 1;
        }
    }
    else
    {
        while ('\0' != *pReader)
        {
            reader = reader.fromWCharArray(pReader);
            if (!reader.contains("ruToken"))
            {
                vecReader.push_back(reader);
            }
            pReader = pReader + wcslen((wchar_t *)pReader) + 1;
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

// Удаление ридера
int Control::removeNameReader(QString oldName, SCARDCONTEXT hContext)
{
    LONG result;      // результат

    wchar_t *oldNameW = new wchar_t[oldName.length() + 1]; // инициализируем массивы wchar
    oldNameW[oldName.toWCharArray(oldNameW)] = '\0';       // приводим QString к wchar

    // Удаляем старое имя
    result = SCardForgetReaderW(hContext, oldNameW);
    if (SCARD_S_SUCCESS != result)
    {
        delete[] oldNameW;
        return result;
    }
    return 0;
}

// Добавление ридера
int Control::addNameReader(QString newName, SCARDCONTEXT hContext, QString pbAttr)
{
    LONG result;      // результат
    LPBYTE ppbAttr = (LPBYTE)pbAttr.data(); // атрибут ридера

    wchar_t *newNameW = new wchar_t[newName.length() + 1]; // инициализируем массивы wchar
    newNameW[newName.toWCharArray(newNameW)] = '\0';       // приводим QString к wchar

    // Добавляем новое имя ридеру
    result = SCardIntroduceReaderW(hContext,
                                  newNameW,
                                  (LPCWSTR)ppbAttr);
    if (SCARD_S_SUCCESS != result)
    {
        delete[] newNameW;
        return result;
    }

    delete[] newNameW;
    delete ppbAttr;
    return 0;
}

// Передача параметров для чтения сертификатов
void Control::setParametrReadCert(uint numberDevice, uint lenVector)
{
    controlNumberDevice = numberDevice;
    controlLenVector = lenVector;
}

// Прочитать сертификаты с устройств
void Control::readCert()
{
    QString controlResultCert;

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
        emit signalErrorReadCert("", 1);
        return;
    }
    if (devices.size() != controlLenVector)
    {
        emit signalErrorReadCert("", 2);
        return;
    }
    Pkcs11Device& d = devices[controlNumberDevice];

    // Выводим серийный номер устройства
    controlResultCert += "Устройство с серийным номером: " + QString::fromStdString(d.getSerialNumber()) + "\n\n";

    // Кому принадлежит сертификат на устройстве
    std::vector<Pkcs11Cert> certs = d.enumerateCerts();
    for(uint i = 0; i < certs.size(); ++i)
    {
        controlResultCert += "Сертификат номер " + QString::number(i + 1) + "\n";
        Pkcs11Cert& cert = certs[i];
        X500Dn subject = cert.getSubject();
        controlResultCert +="Владелец сертификата: " + QString::fromStdString(subject.getRdn(rdnIds[0].first)) + "\n\n";
    }

    emit signalErrorReadCert(controlResultCert, 0);
}
