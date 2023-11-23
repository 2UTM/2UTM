/*************************************************************************
* Rutoken                                                                *
* Copyright (c) 2003-2021, Aktiv-Soft JSC. All rights reserved.          *
* Подробная информация:  http://www.rutoken.ru                           *
*------------------------------------------------------------------------*
* Данный файл содержит объявление констант для работы с Рутокен при      *
* помощи библиотеки PKCS#11 на языке C                                   *
*************************************************************************/

#ifndef Common_H
#define Common_H

/************************************************************************
* Включение файлов:                                                     *
*  - stdio.h - для доступа к библиотеке стандартного ввода/вывода       *
*  - Windows.h - для доступа к функциям Win32API                        *
*  - WinCrypt.h - для доступа к функциям CryptoAPI                      *
*  - process.h - для доступа к функциям управления потоками и процессами*
*  - time.h - для доступа к функциям для работы со временем             *
*  - win2nix.h - для переопределения функций Windows для *nix-платформ  *
*  - wintypes.h - для переопределения типов данных Windows для          *
*    *nix-платформ                                                      *
*  - rtPKCS11.h - для доступа к функциям PKCS#11                        *
************************************************************************/
#ifdef _WIN32
	#include <stdio.h>
	#include <Windows.h>
	#include <WinCrypt.h>
	#include <process.h>
	#include <time.h>
#endif

#include "win2nix.h"
#include "wintypes.h"

#include "rtpkcs11.h"

/************************************************************************
* Макросы                                                               *
************************************************************************/
/* Имя библиотеки PKCS#11 */
#ifdef _WIN32
/* Библиотека для Рутокен S и Рутокен ЭЦП, поддерживает только алгоритмы RSA */
	#define PKCS11_LIBRARY_NAME         "rtPKCS11.dll"
/* Библиотека только для Рутокен ЭЦП, поддерживает алгоритмы ГОСТ и RSA */
	#define PKCS11ECP_LIBRARY_NAME      "rtPKCS11ECP.dll"
#endif
#ifdef __unix__
/* Библиотека только для Рутокен ЭЦП, поддерживает алгоритмы ГОСТ и RSA */
	#define PKCS11_LIBRARY_NAME         "librtpkcs11ecp.so"
	#define PKCS11ECP_LIBRARY_NAME      "librtpkcs11ecp.so"
#endif
#ifdef __APPLE__
/* Библиотека только для Рутокен ЭЦП, поддерживает алгоритмы ГОСТ и RSA */
	#define PKCS11_LIBRARY_NAME         "librtpkcs11ecp.dylib"
	#define PKCS11ECP_LIBRARY_NAME      "librtpkcs11ecp.dylib"
#endif

#ifndef TOKEN_TYPE_RUTOKEN
	#define TOKEN_TYPE_RUTOKEN 0x3
#endif

#ifdef _WIN32
	#define HAVEMSCRYPTOAPI
#endif

/* Вычисление размера массива */
#define arraysize(a)                (sizeof(a) / sizeof(a[0]))

/* Максимальный размер массива с хэндлами */
#define MAX_OBJECTS_ARRAY_SIZE     100

/* Предопределенная константа RSA */
#define RSAENH_MAGIC_RSA1           0x31415352

/* Размер симметричного ключа ГОСТ 28147-89 в байтах */
#define GOST_28147_KEY_SIZE         32

/* Размер открытого ключа ГОСТ Р 34.10-2001 в байтах */
#define GOST_3410_KEY_SIZE          64

/* Размер открытого ключа ГОСТ Р 34.10-2012(256) в байтах */
#define GOST_3410_12_256_KEY_SIZE   64

/* Размер открытого ключа ГОСТ Р 34.10-2012(512) в байтах */
#define GOST_3410_12_512_KEY_SIZE   128

/* Размер синхропосылки в байтах */
#define UKM_LENGTH                  8

/* Размер блока в байтах */
#define GOST28147_89_BLOCK_SIZE     8

/* Максимальное количество попыток ввода PIN-кода для Администратора */
#define MAX_ADMIN_RETRY_COUNT       10

/* Максимальное количество попыток доступа для Пользователя */
#define MAX_USER_RETRY_COUNT        10

/************************************************************************
* Вспомогательные переменные                                            *
************************************************************************/
static CK_BBOOL attributeTrue = CK_TRUE;
static CK_BBOOL attributeFalse = CK_FALSE;

/* Тип сертификата */
static CK_CERTIFICATE_TYPE certificateType = CKC_X_509;

/* Категория сертификата */
static CK_ULONG unspecifiedCertificate = 0;
static CK_ULONG tokenUserCertificate = 1;
static CK_ULONG authorityCertificate = 2;
static CK_ULONG otherEntityCertificate = 3;

/* Длина модуля ключа RSA в битах */
static CK_ULONG rsaModulusBits = 1024;

/* Набор параметров КриптоПро A алгоритма ГОСТ 28147-89 */
static CK_BYTE parametersGost28147[] = { 0x06, 0x07, 0x2a, 0x85, 0x03, 0x02, 0x02, 0x1f, 0x01 };

/* Набор параметров КриптоПро A алгоритма ГОСТ Р 34.10-2001 */
static CK_BYTE parametersGostR3410_2001[] = { 0x06, 0x07, 0x2a, 0x85, 0x03, 0x02, 0x02, 0x23, 0x01 };

/* Набор параметров КриптоПро A алгоритма ГОСТ Р 34.10-2012(256) */
static CK_BYTE parametersGostR3410_2012_256[] = { 0x06, 0x07, 0x2a, 0x85, 0x03, 0x02, 0x02, 0x23, 0x01 };

/* Набор параметров КриптоПро A алгоритма ГОСТ Р 34.10-2012(512) */
static CK_BYTE parametersGostR3410_2012_512[] = { 0x06, 0x09, 0x2a, 0x85, 0x03, 0x07, 0x01, 0x02, 0x01, 0x02, 0x01 };

/* Набор параметров КриптоПро алгоритма ГОСТ Р 34.11-1994 */
static CK_BYTE parametersGostR3411_1994[] = { 0x06, 0x07, 0x2a, 0x85, 0x03, 0x02, 0x02, 0x1e, 0x01 };

/* Набор параметров КриптоПро алгоритма ГОСТ Р 34.11-2012(256) */
static CK_BYTE parametersGostR3411_2012_256[] = { 0x06, 0x08, 0x2a, 0x85, 0x03, 0x07, 0x01, 0x01, 0x02, 0x02 };

/* Набор параметров КриптоПро алгоритма ГОСТ Р 34.11-2012(512) */
static CK_BYTE parametersGostR3411_2012_512[] = { 0x06, 0x08, 0x2a, 0x85, 0x03, 0x07, 0x01, 0x01, 0x02, 0x03 };

/* Параметры для инициализации библиотеки: разрешаем использовать объекты синхронизации операционной системы */
static CK_C_INITIALIZE_ARGS initArgs = { NULL_PTR, NULL_PTR, NULL_PTR, NULL_PTR, CKF_OS_LOCKING_OK, NULL_PTR };

// Набор параметров для EC
static CK_BYTE secp256k1Oid[] = { 0x06, 0x05, 0x2B, 0x81, 0x04, 0x00, 0x0A };

/************************************************************************
* PIN-коды Рутокен                                                      *
************************************************************************/
/* DEMO PIN-код Пользователя Рутокен */
static CK_UTF8CHAR USER_PIN[] = { "12345678" };

#define USER_PIN_LEN (sizeof(USER_PIN)-1)

/* Новый DEMO PIN-код Пользователя Рутокен */
static CK_UTF8CHAR NEW_USER_PIN[] = { "55555555" };

#define NEW_USER_PIN_LEN (sizeof(NEW_USER_PIN)-1)

/* Неправильный DEMO PIN-код Пользователя Рутокен */
static CK_UTF8CHAR WRONG_USER_PIN[] = { "00000000" };

#define WRONG_USER_PIN_LEN (sizeof(WRONG_USER_PIN)-1)

/* DEMO PIN-код Администратора Рутокен */
static CK_UTF8CHAR SO_PIN[] = { "87654321" };

#define SO_PIN_LEN (sizeof(SO_PIN)-1)

/* DEMO локальный PIN-код Рутокен */
static CK_UTF8CHAR LOCAL_PIN[] = { "1234567890" };

#define LOCAL_PIN_LEN (sizeof(LOCAL_PIN)-1)

/************************************************************************
* Описание типов объектов                                               *
************************************************************************/
static CK_OBJECT_CLASS publicKeyObject = CKO_PUBLIC_KEY;
static CK_OBJECT_CLASS privateKeyObject = CKO_PRIVATE_KEY;
static CK_OBJECT_CLASS secretKeyObject = CKO_SECRET_KEY;
static CK_OBJECT_CLASS certificateObject = CKO_CERTIFICATE;
static CK_OBJECT_CLASS dataObject = CKO_DATA;

/************************************************************************
* Описание типов ключей                                                 *
************************************************************************/
static CK_KEY_TYPE keyTypeRsa = CKK_RSA;
static CK_KEY_TYPE keyTypeEcdsa = CKK_EC;
static CK_KEY_TYPE keyTypeGost28147 = CKK_GOST28147;
static CK_KEY_TYPE keyTypeGostR3410_2001 = CKK_GOSTR3410;
static CK_KEY_TYPE keyTypeGostR3410_2012_256 = CKK_GOSTR3410;
static CK_KEY_TYPE keyTypeGostR3410_2012_512 = CKK_GOSTR3410_512;
static CK_KEY_TYPE keyTypeKuznechik = CKK_KUZNECHIK;
static CK_KEY_TYPE keyTypeMagma = CKK_MAGMA;

/************************************************************************
* Описание меток объектов                                               *
************************************************************************/
/* DEMO-метка открытого ключа RSA */
static CK_UTF8CHAR publicKeyLabelRsa[] = {"Sample RSA Public Key (Aktiv Co.)"};

/* DEMO-метка закрытого ключа RSA */
static CK_UTF8CHAR privateKeyLabelRsa[] = {"Sample RSA Private Key (Aktiv Co.)"};

/* DEMO ID пары ключей RSA */
static CK_BYTE keyPairIdRsa[] = {"RSA sample key pair ID (Aktiv Co.)"};

/* DEMO-метка открытого ключа ECDSA */
static CK_UTF8CHAR publicKeyLabelEcdsa[] = {"Sample ECDSA Public Key (Aktiv Co.)"};

/* DEMO-метка закрытого ключа ECDSA */
static CK_UTF8CHAR privateKeyLabelEcdsa[] = {"Sample ECDSA Private Key (Aktiv Co.)"};

/* DEMO ID пары ключей ECDSA */
static CK_BYTE keyPairIdEcdsa[] = { "ECDSA sample key pair ID (Aktiv Co.)" };

/* DEMO-метка  открытого ключа #1 ГОСТ Р 34.10-2001 */
static CK_UTF8CHAR publicKeyLabelGost2001_1[] = {"Sample GOST R 34.10-2001 Public Key 1 (Aktiv Co.)"};

/* DEMO-метка  закрытого ключа #1 ГОСТ Р 34.10-2001 */
static CK_UTF8CHAR privateKeyLabelGost2001_1[] = {"Sample GOST R 34.10-2001 Private Key 1 (Aktiv Co.)"};

/* DEMO ID пары ключей #1 ГОСТ Р 34.10-2001 */
static CK_BYTE keyPairIdGost2001_1[] = {"GOST R 34.10-2001 sample key pair 1 ID (Aktiv Co.)"};

/* DEMO-метка открытого ключа #2 ГОСТ Р 34.10-2001 */
static CK_UTF8CHAR publicKeyLabelGost2001_2[] = {"Sample GOST R 34.10-2001 Public Key 2 (Aktiv Co.)"};

/* DEMO-метка закрытого ключа #2 ГОСТ Р 34.10-2001 */
static CK_UTF8CHAR privateKeyLabelGost2001_2[] = {"Sample GOST R 34.10-2001 Private Key 2 (Aktiv Co.)"};

/* DEMO ID пары ключей #2 ГОСТ Р 34.10-2001 */
static CK_BYTE keyPairIdGost2001_2[] = {"GOST R 34.10-2001 sample key pair 2 ID (Aktiv Co.)"};

/* DEMO-метка  открытого ключа #1 ГОСТ Р 34.10-2012(256) */
static CK_UTF8CHAR publicKeyLabelGost2012_256_1[] = { "Sample GOST R 34.10-2012 (256 bits) Public Key 1 (Aktiv Co.)" };

/* DEMO-метка  закрытого ключа #1 ГОСТ Р 34.10-2012(256) */
static CK_UTF8CHAR privateKeyLabelGost2012_256_1[] = { "Sample GOST R 34.10-2012 (256 bits) Private Key 1 (Aktiv Co.)" };

/* DEMO ID пары ключей #1 ГОСТ Р 34.10-2012(256) */
static CK_BYTE keyPairIdGost2012_256_1[] = { "GOST R 34.10-2012 (256 bits) sample key pair 1 ID (Aktiv Co.)" };

/* DEMO-метка открытого ключа #2 ГОСТ Р 34.10-2012(256) */
static CK_UTF8CHAR publicKeyLabelGost2012_256_2[] = { "Sample GOST R 34.10-2012 (256 bits) Public Key 2 (Aktiv Co.)" };

/* DEMO-метка закрытого ключа #2 ГОСТ Р 34.10-2012(256) */
static CK_UTF8CHAR privateKeyLabelGost2012_256_2[] = { "Sample GOST R 34.10-2012 (256 bits) Private Key 2 (Aktiv Co.)" };

/* DEMO ID пары ключей #2 ГОСТ Р 34.10-2012(256) */
static CK_BYTE keyPairIdGost2012_256_2[] = { "GOST R 34.10-2012 (256 bits) sample key pair 2 ID (Aktiv Co.)" };

/* DEMO-метка  открытого ключа #1 ГОСТ Р 34.10-2012(512) */
static CK_UTF8CHAR publicKeyLabelGost2012_512_1[] = { "Sample GOST R 34.10-2012 (512 bits) Public Key 1 (Aktiv Co.)" };

/* DEMO-метка  закрытого ключа #1 ГОСТ Р 34.10-2012(512) */
static CK_UTF8CHAR privateKeyLabelGost2012_512_1[] = { "Sample GOST R 34.10-2012 (512 bits) Private Key 1 (Aktiv Co.)" };

/* DEMO ID пары ключей #1 ГОСТ Р 34.10-2012(512) */
static CK_BYTE keyPairIdGost2012_512_1[] = { "GOST R 34.10-2012 (512 bits) sample key pair 1 ID (Aktiv Co.)" };

/* DEMO-метка открытого ключа #2 ГОСТ Р 34.10-2012(512) */
static CK_UTF8CHAR publicKeyLabelGost2012_512_2[] = { "Sample GOST R 34.10-2012 (512 bits) Public Key 2 (Aktiv Co.)" };

/* DEMO-метка закрытого ключа #2 ГОСТ Р 34.10-2012(512) */
static CK_UTF8CHAR privateKeyLabelGost2012_512_2[] = { "Sample GOST R 34.10-2012 (512 bits) Private Key 2 (Aktiv Co.)" };

/* DEMO ID пары ключей # 2 ГОСТ Р 34.10-2012(512) */
static CK_BYTE keyPairIdGost2012_512_2[] = { "GOST R 34.10-2012 (512 bits) sample key pair 2 ID (Aktiv Co.)" };

/* DEMO-метка симметричного ключа ГОСТ 28147-89 */
static CK_UTF8CHAR secretKeyLabel[] = {"Sample GOST 28147-89 Secret Key (Aktiv Co.)"};

/* DEMO-метка симметричного ключа ГОСТ 34.12-2018 с длиной блока 128 бит (Кузнечик) */
static CK_UTF8CHAR secretKeyKuznechikLabel[] = {"Sample Kuznechik Secret Key (Aktiv Co.)"};

/* DEMO-метка симметричного ключа ГОСТ 34.12-2018 с длиной блока 64 бита (Магма) */
static CK_UTF8CHAR secretKeyMagmaLabel[] = {"Sample Magma Secret Key (Aktiv Co.)"};

/* DEMO ID симметричного ключа ГОСТ 28147-89 */
static CK_BYTE secretKeyId[] = {"GOST 28147-89 Secret Key ID (Aktiv Co.)"};

/* DEMO ID симметричного ключа ГОСТ 34.12-2018 с длиной блока 128 бит (Кузнечик) */
static CK_BYTE secretKeyKuznechikId[] = {"Kuznechik Secret Key ID (Aktiv Co.)"};

/* DEMO ID симметричного ключа ГОСТ 34.12-2018 с длиной блока 64 бита (Магма) */
static CK_BYTE secretKeyMagmaId[] = {"Magma Secret Key ID (Aktiv Co.)"};

/* DEMO-метка выработанного ключа обмена */
static CK_UTF8CHAR derivedKeyLabel[] = {"Derived GOST 28147-89 key"};

/* DEMO-метка для сессионного ключа */
static CK_UTF8CHAR sessionKeyLabel[] = {"GOST 28147-89 key to wrap"};

/* DEMO-метка для демаскированного ключа */
static CK_UTF8CHAR unwrappedKeyLabel[] = {"Unwrapped GOST 28147-89 key"};

/* DEMO-метка для объекта с бинарными данными */
static CK_UTF8CHAR dataObjectLabel[] = "Sample data object label (Aktiv Co.)";

/* DEMO-метка приложения для объекта с бинарными данными */
static CK_UTF8CHAR applicationLabel[] = "Sample application label (Aktiv Co.)";

/************************************************************************
* Описание меток токена                                                 *
************************************************************************/
/* DEMO метка Rutoken ("длинная") */
static CK_CHAR tokenLongLabel[] = {"!!!Sample Rutoken Long-long-long-long-long label!!!"};

/* DEMO метка Rutoken ("обычная") */
static CK_CHAR tokenStdLabel[] = {"!!!Sample Rutoken label!!!"};

/* DEMO метка Rutoken */
static CK_UTF8CHAR tokenLabel[] = { 'M', 'y', ' ', 'R', 'u', 't', 'o', 'k',
                                    'e', 'n', ' ', ' ', ' ', ' ', ' ', ' ',
                                    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
                                    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };

/*************************************************************************
* Механизмы PKCS#11                                                      *
*************************************************************************/
/*  Механизм генерации ключевой пары RSA */
static CK_MECHANISM rsaKeyPairGenMech = {CKM_RSA_PKCS_KEY_PAIR_GEN, NULL_PTR, 0};

/* Механизм шифрования/расшифрования по алгоритму RSA */
static CK_MECHANISM rsaEncDecMech = {CKM_RSA_PKCS, NULL_PTR, 0};

/* Механизм подписи/проверки подписи по алгоритму RSA */
static CK_MECHANISM rsaSigVerMech = {CKM_RSA_PKCS, NULL_PTR, 0};

/*  Механизм подписи/проверки подписи по алгоритму RSA со встроенным хэшированием по алгоритму SHA-256 */
static CK_MECHANISM rsaSigVerMech_WithSha256 = {CKM_SHA256_RSA_PKCS, NULL_PTR, 0};

/*  Механизм генерации ключевой пары ECDSA */
static CK_MECHANISM ecdsaKeyPairGenMech = {CKM_EC_KEY_PAIR_GEN, NULL_PTR, 0};

/* Механизм подписи/проверки подписи по алгоритму ECDSA */
static CK_MECHANISM ecdsaSigVerMech = {CKM_ECDSA, NULL_PTR, 0};

/*  Механизм генерации ключевой пары ГОСТ Р 34.10-2001 */
static CK_MECHANISM gostR3410_2001KeyPairGenMech = {CKM_GOSTR3410_KEY_PAIR_GEN, NULL_PTR, 0};

/*  Механизм генерации ключевой пары ГОСТ Р 34.10-2012(256) */
static CK_MECHANISM gostR3410_2012_256KeyPairGenMech = { CKM_GOSTR3410_KEY_PAIR_GEN, NULL_PTR, 0 };

/*  Механизм генерации ключевой пары ГОСТ Р 34.10-2012(512) */
static CK_MECHANISM gostR3410_2012_512KeyPairGenMech = { CKM_GOSTR3410_512_KEY_PAIR_GEN, NULL_PTR, 0 };

/*  Механизм подписи/проверки подписи по алгоритму ГОСТ Р 34.10-2001 */
static CK_MECHANISM gostR3410_2001SigVerMech = {CKM_GOSTR3410, NULL_PTR, 0};

/*  Механизм подписи/проверки подписи по алгоритму ГОСТ Р 34.10-2012(256) */
static CK_MECHANISM gostR3410_2012_256SigVerMech = { CKM_GOSTR3410, NULL_PTR, 0 };

/*  Механизм подписи/проверки подписи по алгоритму ГОСТ Р 34.10-2012(512) */
static CK_MECHANISM gostR3410_2012_512SigVerMech = { CKM_GOSTR3410_512, NULL_PTR, 0 };

/*  Механизм подписи/проверки подписи по алгоритму ГОСТ Р 34.10-2001 со встроенным хэшированием по алгоритму ГОСТ Р 34.11-94 */
static CK_MECHANISM gostR3410_2001SigVerMech_WithHash = {CKM_GOSTR3410_WITH_GOSTR3411, NULL_PTR, 0};

/*  Механизм подписи/проверки подписи по алгоритму ГОСТ Р 34.10-2012(256) со встроенным хэшированием по алгоритму ГОСТ Р 34.11-2012(256) */
static CK_MECHANISM gostR3410_2012_256SigVerMech_WithHash = {CKM_GOSTR3410_WITH_GOSTR3411_12_256, NULL_PTR, 0};

/*  Механизм подписи/проверки подписи по алгоритму ГОСТ Р 34.10-2012(512) со встроенным хэшированием по алгоритму ГОСТ Р 34.11-2012(512) */
static CK_MECHANISM gostR3410_2012_512SigVerMech_WithHash = {CKM_GOSTR3410_WITH_GOSTR3411_12_512, NULL_PTR, 0};

/*  Механизмы генерации симметричного ключа по алгоритму ГОСТ 34.12 */
static CK_MECHANISM kuznechikKeyGenMech = {CKM_KUZNECHIK_KEY_GEN, NULL_PTR, 0};
static CK_MECHANISM magmaKeyGenMech = {CKM_MAGMA_KEY_GEN, NULL_PTR, 0};

/* Механизмы шифрования/расшифрования по алгоритму ГОСТ 34.12 */
static CK_MECHANISM kuznechikEncDecEcbMech = { CKM_KUZNECHIK_ECB, NULL_PTR, 0 };
static CK_BYTE kuznechikEncMechParams[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
static CK_MECHANISM kuznechikEncDecMech = { CKM_KUZNECHIK_CTR_ACPKM, &kuznechikEncMechParams, sizeof(kuznechikEncMechParams) };
static CK_MECHANISM magmaEncDecEcbMech = { CKM_MAGMA_ECB, NULL_PTR, 0 };
static CK_BYTE magmaEncMechParams[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
static CK_MECHANISM magmaEncDecMech = { CKM_MAGMA_CTR_ACPKM, &magmaEncMechParams, sizeof(magmaEncMechParams) };

/*  Механизм выработки имитовставки ГОСТ 34.12 */
static CK_MECHANISM kuznechikMacMech = { CKM_KUZNECHIK_MAC, NULL_PTR, 0 };
static CK_MECHANISM magmaMacMech = { CKM_MAGMA_MAC, NULL_PTR, 0 };

/*  Механизм генерации симметричного ключа по алгоритму ГОСТ 28147-89 */
static CK_MECHANISM gost28147KeyGenMech = {CKM_GOST28147_KEY_GEN, NULL_PTR, 0};

/* Механизм шифрования/расшифрования по алгоритму ГОСТ 28147-89 */
static CK_MECHANISM gost28147EncDecEcbMech = {CKM_GOST28147_ECB, NULL_PTR, 0};
static CK_MECHANISM gost28147EncDecMech = {CKM_GOST28147, NULL_PTR, 0 };

/*  Механизм выработки имитовставки ГОСТ 28147-89 */
static CK_MECHANISM gost28147MacMech = {CKM_GOST28147_MAC, NULL_PTR, 0};

/*  Механизм выработки HMAC по алгоритму ГОСТ Р 34.11-94 */
static CK_MECHANISM gostR3411HmacMech = {CKM_GOSTR3411_HMAC, NULL_PTR, 0};

/*  Механизм выработки HMAC по алгоритму ГОСТ Р 34.11-2012(256) */
static CK_MECHANISM gostR3411_2012_256HmacMech = { CKM_GOSTR3411_12_256_HMAC, NULL_PTR, 0 };

/*  Механизм выработки HMAC по алгоритму ГОСТ Р 34.11-2012(512) */
static CK_MECHANISM gostR3411_2012_512HmacMech = { CKM_GOSTR3411_12_512_HMAC, NULL_PTR, 0 };

/*  Механизм хэширования SHA-256 */
static CK_MECHANISM sha256Mech = {CKM_SHA256, NULL_PTR, 0};

/*  Механизм хэширования ГОСТ Р 34.11-94 */
static CK_MECHANISM gostR3411_1994HashMech = {CKM_GOSTR3411, NULL_PTR, 0};

/*  Механизм хэширования ГОСТ Р 34.11-2012(256) */
static CK_MECHANISM gostR3411_2012_256HashMech = { CKM_GOSTR3411_12_256, NULL_PTR, 0 };

/*  Механизм хэширования ГОСТ Р 34.11-2012(512) */
static CK_MECHANISM gostR3411_2012_512HashMech = { CKM_GOSTR3411_12_512, NULL_PTR, 0 };

/* Механизм для маскирования/демаскирования ключа */
static CK_MECHANISM gost28147WrapMech = {CKM_GOST28147_KEY_WRAP, NULL_PTR, 0};

/* Механизм выработки ключа обмена по алгоритму VKO GOST R 34.10-2001 */
static CK_MECHANISM gostR3410_2001DerivationMech = {CKM_GOSTR3410_DERIVE, NULL_PTR, 0};

/* Механизм выработки ключа обмена по алгоритму VKO GOST R 34.10-2012 */
static CK_MECHANISM gostR3410_12DerivationMech  = { CKM_GOSTR3410_12_DERIVE, NULL_PTR, 0 };

/* Параметры для выработки ключа обмена */
static CK_GOSTR3410_DERIVE_PARAMS deriveParameters = {CKD_CPDIVERSIFY_KDF, NULL_PTR, 0, NULL_PTR, 0};

/*************************************************************************
* Механизмы PKCS#11 с программной реализацией                            *
* Использование данных механизмов предполагает выполнение                *
* криптографической операции на центральном процессоре, что может        *
* положительно повлиять на скорость выполнения операции, но отрицательно *
* на обеспечение безопасности                                            *
*************************************************************************/
/*  Механизм программного хэширования ГОСТ Р 34.11-94
static CK_MECHANISM gostR3411_1994HashMech = {CKM_GOSTR3411, parametersGostR3411_1994, sizeof(parametersGostR3411_1994)}; */

/*  Механизм программного хэширования ГОСТ Р 34.11-2012(256)
static CK_MECHANISM gostR3411_2012_256HashMech = { CKM_GOSTR3411_12_256, parametersGostR3411_2012_256, sizeof(parametersGostR3411_2012_256)}; */

/*  Механизм программного хэширования ГОСТ Р 34.11-2012(512)
static CK_MECHANISM gostR3411_2012_512HashMech = { CKM_GOSTR3411_12_512, parametersGostR3411_2012_512, sizeof(parametersGostR3411_2012_512)}; */

/*  Механизм подписи/проверки подписи по алгоритму ГОСТ Р 34.10-2001 со встроенным программным хэшированием по алгоритму ГОСТ Р 34.11-94
static CK_MECHANISM gostR3410_2001SigVerMech_WithHash = {CKM_GOSTR3410_WITH_GOSTR3411, parametersGostR3411_1994, sizeof(parametersGostR3411_1994)}; */

/*  Механизм подписи/проверки подписи по алгоритму ГОСТ Р 34.10-2012(256) со встроенным программным хэшированием по алгоритму ГОСТ Р 34.11-2012(256)
static CK_MECHANISM gostR3410_2012_256SigVerMech_WithHash = {CKM_GOSTR3410_WITH_GOSTR3411_12_256, parametersGostR3411_2012_256, sizeof(parametersGostR3411_2012_256)}; */

/*  Механизм подписи/проверки подписи по алгоритму ГОСТ Р 34.10-2012(512) со встроенным программным хэшированием по алгоритму ГОСТ Р 34.11-2012(512)
static CK_MECHANISM gostR3410_2012_512SigVerMech_WithHash = {CKM_GOSTR3410_WITH_GOSTR3411_12_512, parametersGostR3411_2012_512, sizeof(parametersGostR3411_2012_512)}; */

/*************************************************************************
* Функция преобразования ошибки PKCS11 к строке                          *
*************************************************************************/
static const char* rvToStr(CK_RV rv)
{
	switch (rv) {
	case CKR_OK: return "CKR_OK";
	case CKR_CANCEL: return "CKR_CANCEL";
	case CKR_HOST_MEMORY: return "CKR_HOST_MEMORY";
	case CKR_SLOT_ID_INVALID: return "CKR_SLOT_ID_INVALID";
	case CKR_GENERAL_ERROR: return "CKR_GENERAL_ERROR";
	case CKR_FUNCTION_FAILED: return "CKR_FUNCTION_FAILED";
	case CKR_ARGUMENTS_BAD: return "CKR_ARGUMENTS_BAD";
	case CKR_NO_EVENT: return "CKR_NO_EVENT";
	case CKR_NEED_TO_CREATE_THREADS: return "CKR_NEED_TO_CREATE_THREADS";
	case CKR_CANT_LOCK: return "CKR_CANT_LOCK";
	case CKR_ATTRIBUTE_READ_ONLY: return "CKR_ATTRIBUTE_READ_ONLY";
	case CKR_ATTRIBUTE_SENSITIVE: return "CKR_ATTRIBUTE_SENSITIVE";
	case CKR_ATTRIBUTE_TYPE_INVALID: return "CKR_ATTRIBUTE_TYPE_INVALID";
	case CKR_ATTRIBUTE_VALUE_INVALID: return "CKR_ATTRIBUTE_VALUE_INVALID";
	case CKR_DATA_INVALID: return "CKR_DATA_INVALID";
	case CKR_DATA_LEN_RANGE: return "CKR_DATA_LEN_RANGE";
	case CKR_DEVICE_ERROR: return "CKR_DEVICE_ERROR";
	case CKR_DEVICE_MEMORY: return "CKR_DEVICE_MEMORY";
	case CKR_DEVICE_REMOVED: return "CKR_DEVICE_REMOVED";
	case CKR_ENCRYPTED_DATA_INVALID: return "CKR_ENCRYPTED_DATA_INVALID";
	case CKR_ENCRYPTED_DATA_LEN_RANGE: return "CKR_ENCRYPTED_DATA_LEN_RANGE";
	case CKR_FUNCTION_CANCELED: return "CKR_FUNCTION_CANCELED";
	case CKR_FUNCTION_NOT_PARALLEL: return "CKR_FUNCTION_NOT_PARALLEL";
	case CKR_FUNCTION_NOT_SUPPORTED: return "CKR_FUNCTION_NOT_SUPPORTED";
	case CKR_KEY_HANDLE_INVALID: return "CKR_KEY_HANDLE_INVALID";
	case CKR_KEY_SIZE_RANGE: return "CKR_KEY_SIZE_RANGE";
	case CKR_KEY_TYPE_INCONSISTENT: return "CKR_KEY_TYPE_INCONSISTENT";
	case CKR_KEY_NOT_NEEDED: return "CKR_KEY_NOT_NEEDED";
	case CKR_KEY_CHANGED: return "CKR_KEY_CHANGED";
	case CKR_KEY_NEEDED: return "CKR_KEY_NEEDED";
	case CKR_KEY_INDIGESTIBLE: return "CKR_KEY_INDIGESTIBLE";
	case CKR_KEY_FUNCTION_NOT_PERMITTED: return "CKR_KEY_FUNCTION_NOT_PERMITTED";
	case CKR_KEY_NOT_WRAPPABLE: return "CKR_KEY_NOT_WRAPPABLE";
	case CKR_KEY_UNEXTRACTABLE: return "CKR_KEY_UNEXTRACTABLE";
	case CKR_MECHANISM_INVALID: return "CKR_MECHANISM_INVALID";
	case CKR_MECHANISM_PARAM_INVALID: return "CKR_MECHANISM_PARAM_INVALID";
	case CKR_OBJECT_HANDLE_INVALID: return "CKR_OBJECT_HANDLE_INVALID";
	case CKR_OPERATION_ACTIVE: return "CKR_OPERATION_ACTIVE";
	case CKR_OPERATION_NOT_INITIALIZED: return "CKR_OPERATION_NOT_INITIALIZED";
	case CKR_PIN_INCORRECT: return "CKR_PIN_INCORRECT";
	case CKR_PIN_INVALID: return "CKR_PIN_INVALID";
	case CKR_PIN_LEN_RANGE: return "CKR_PIN_LEN_RANGE";
	case CKR_PIN_EXPIRED: return "CKR_PIN_EXPIRED";
	case CKR_PIN_LOCKED: return "CKR_PIN_LOCKED";
	case CKR_SESSION_CLOSED: return "CKR_SESSION_CLOSED";
	case CKR_SESSION_COUNT: return "CKR_SESSION_COUNT";
	case CKR_SESSION_HANDLE_INVALID: return "CKR_SESSION_HANDLE_INVALID";
	case CKR_SESSION_PARALLEL_NOT_SUPPORTED: return "CKR_SESSION_PARALLEL_NOT_SUPPORTED";
	case CKR_SESSION_READ_ONLY: return "CKR_SESSION_READ_ONLY";
	case CKR_SESSION_EXISTS: return "CKR_SESSION_EXISTS";
	case CKR_SESSION_READ_ONLY_EXISTS: return "CKR_SESSION_READ_ONLY_EXISTS";
	case CKR_SESSION_READ_WRITE_SO_EXISTS: return "CKR_SESSION_READ_WRITE_SO_EXISTS";
	case CKR_SIGNATURE_INVALID: return "CKR_SIGNATURE_INVALID";
	case CKR_SIGNATURE_LEN_RANGE: return "CKR_SIGNATURE_LEN_RANGE";
	case CKR_TEMPLATE_INCOMPLETE: return "CKR_TEMPLATE_INCOMPLETE";
	case CKR_TEMPLATE_INCONSISTENT: return "CKR_TEMPLATE_INCONSISTENT";
	case CKR_TOKEN_NOT_PRESENT: return "CKR_TOKEN_NOT_PRESENT";
	case CKR_TOKEN_NOT_RECOGNIZED: return "CKR_TOKEN_NOT_RECOGNIZED";
	case CKR_TOKEN_WRITE_PROTECTED: return "CKR_TOKEN_WRITE_PROTECTED";
	case CKR_UNWRAPPING_KEY_HANDLE_INVALID: return "CKR_UNWRAPPING_KEY_HANDLE_INVALID";
	case CKR_UNWRAPPING_KEY_SIZE_RANGE: return "CKR_UNWRAPPING_KEY_SIZE_RANGE";
	case CKR_UNWRAPPING_KEY_TYPE_INCONSISTENT: return "CKR_UNWRAPPING_KEY_TYPE_INCONSISTENT";
	case CKR_USER_ALREADY_LOGGED_IN: return "CKR_USER_ALREADY_LOGGED_IN";
	case CKR_USER_NOT_LOGGED_IN: return "CKR_USER_NOT_LOGGED_IN";
	case CKR_USER_PIN_NOT_INITIALIZED: return "CKR_USER_PIN_NOT_INITIALIZED";
	case CKR_USER_TYPE_INVALID: return "CKR_USER_TYPE_INVALID";
	case CKR_USER_ANOTHER_ALREADY_LOGGED_IN: return "CKR_USER_ANOTHER_ALREADY_LOGGED_IN";
	case CKR_USER_TOO_MANY_TYPES: return "CKR_USER_TOO_MANY_TYPES";
	case CKR_WRAPPED_KEY_INVALID: return "CKR_WRAPPED_KEY_INVALID";
	case CKR_WRAPPED_KEY_LEN_RANGE: return "CKR_WRAPPED_KEY_LEN_RANGE";
	case CKR_WRAPPING_KEY_HANDLE_INVALID: return "CKR_WRAPPING_KEY_HANDLE_INVALID";
	case CKR_WRAPPING_KEY_SIZE_RANGE: return "CKR_WRAPPING_KEY_SIZE_RANGE";
	case CKR_WRAPPING_KEY_TYPE_INCONSISTENT: return "CKR_WRAPPING_KEY_TYPE_INCONSISTENT";
	case CKR_RANDOM_SEED_NOT_SUPPORTED: return "CKR_RANDOM_SEED_NOT_SUPPORTED";
	case CKR_RANDOM_NO_RNG: return "CKR_RANDOM_NO_RNG";
	case CKR_DOMAIN_PARAMS_INVALID: return "CKR_DOMAIN_PARAMS_INVALID";
	case CKR_BUFFER_TOO_SMALL: return "CKR_BUFFER_TOO_SMALL";
	case CKR_SAVED_STATE_INVALID: return "CKR_SAVED_STATE_INVALID";
	case CKR_INFORMATION_SENSITIVE: return "CKR_INFORMATION_SENSITIVE";
	case CKR_STATE_UNSAVEABLE: return "CKR_STATE_UNSAVEABLE";
	case CKR_CRYPTOKI_NOT_INITIALIZED: return "CKR_CRYPTOKI_NOT_INITIALIZED";
	case CKR_CRYPTOKI_ALREADY_INITIALIZED: return "CKR_CRYPTOKI_ALREADY_INITIALIZED";
	case CKR_MUTEX_BAD: return "CKR_MUTEX_BAD";
	case CKR_MUTEX_NOT_LOCKED: return "CKR_MUTEX_NOT_LOCKED";
	case CKR_NEW_PIN_MODE: return "CKR_NEW_PIN_MODE";
	case CKR_NEXT_OTP: return "CKR_NEXT_OTP";
	case CKR_FUNCTION_REJECTED: return "CKR_FUNCTION_REJECTED";
	case CKR_CORRUPTED_MAPFILE: return "CKR_CORRUPTED_MAPFILE";
	case CKR_WRONG_VERSION_FIELD: return "CKR_WRONG_VERSION_FIELD";
	case CKR_WRONG_PKCS1_ENCODING: return "CKR_WRONG_PKCS1_ENCODING";
	case CKR_RTPKCS11_DATA_CORRUPTED: return "CKR_RTPKCS11_DATA_CORRUPTED";
	case CKR_RTPKCS11_RSF_DATA_CORRUPTED: return "CKR_RTPKCS11_RSF_DATA_CORRUPTED";
	case CKR_SM_PASSWORD_INVALID: return "CKR_SM_PASSWORD_INVALID";
	case CKR_LICENSE_READ_ONLY: return "CKR_LICENSE_READ_ONLY";
	default: return "Unknown error";
	}
}
/*************************************************************************
* Макросы проверки ошибки. Если произошла ошибка, то выводится           *
* сообщение и осуществляется переход на заданную метку                   *
*************************************************************************/
#define CHECK_AND_LOG(msg, expression, errMsg, label) \
	do { \
		printf("%s", msg); \
		if (!(expression)) { \
			printf(" -> Failed\n%s\n", errMsg); \
			goto label; \
		} \
		else { \
			printf(" -> OK\n"); \
		} \
	} while (0)

#define CHECK(msg, expression, label) \
	do { \
		printf("%s", msg); \
		if (!(expression)) { \
			printf(" -> Failed\n"); \
			goto label; \
		} \
		else { \
			printf(" -> OK\n"); \
		} \
	} while (0)

/*************************************************************************
* Макросы проверки ошибки при освобождении ресурсов . Если произошла     *
* ошибка, то выводится сообщение и выставляется                          *
* значение переменной errorCode                                          *
*************************************************************************/
#define CHECK_RELEASE_AND_LOG(msg, expression, errMsg, errorCode) \
	do { \
		printf("%s", msg); \
		if (!(expression)) { \
			printf(" -> Failed\n%s\n", errMsg); \
			errorCode = 1; \
		} \
		else { \
			printf(" -> OK\n"); \
		} \
	} while (0)

#define CHECK_RELEASE(msg, expression, errorCode) \
	do { \
		printf("%s", msg); \
		if (!(expression)) { \
			printf(" -> Failed\n"); \
			errorCode = 1; \
		} \
		else { \
			printf(" -> OK\n"); \
		} \
	} while (0)

/*************************************************************************
* Функция поиска объектов по заданному шаблону                           *
*************************************************************************/
static int findObjects(CK_FUNCTION_LIST_PTR functionList, // Указатель на список функций PKCS#11
                       CK_SESSION_HANDLE session,         // Хэндл открытой сессии
                       CK_ATTRIBUTE_PTR attributes,       // Массив с шаблоном для поиска
                       CK_ULONG attrCount,                // Количество атрибутов в массиве поиска
                       CK_OBJECT_HANDLE_PTR* objects,     // Массив для записи найденных объектов
                       CK_ULONG* objectsCount             // Количество найденных объектов
                       )
{
	CK_RV rv;                                           // Код возврата. Могут быть возвращены только ошибки, определенные в PKCS#11
	CK_ULONG newObjectsCount;                           // Количество объектов, найденных при конкретном вызове C_FindObjects
	CK_ULONG bufferSize;                                // Текущий размер буфера с объектами
	CK_OBJECT_HANDLE_PTR buffer;                        // Буфер, получаемый из realloc
	int errorCode = 1;                                  // Флаг ошибки

	/*************************************************************************
	* Инициализировать операцию поиска                                       *
	*************************************************************************/
	rv = functionList->C_FindObjectsInit(session, attributes, attrCount);
	CHECK_AND_LOG("  C_FindObjectsInit", rv == CKR_OK, rvToStr(rv), exit);

	/*************************************************************************
	* Найти все объекты, соответствующие критериям поиска                    *
	*************************************************************************/
	*objects = NULL;
	*objectsCount = 0;

	for (bufferSize = 8;; bufferSize *= 2) {
		buffer = (CK_OBJECT_HANDLE_PTR)realloc(*objects, bufferSize * sizeof(CK_OBJECT_HANDLE));
		CHECK("  Memory allocation for object handles", buffer != NULL, find_final);
		*objects = buffer;

		rv = functionList->C_FindObjects(session, *objects + *objectsCount, bufferSize - *objectsCount, &newObjectsCount);
		CHECK_AND_LOG("  C_FindObjects", rv == CKR_OK, rvToStr(rv), find_final);

		*objectsCount += newObjectsCount;

		if (*objectsCount < bufferSize) {
			break;
		}
	}

	/*************************************************************************
	* Освободить неиспользуемую память                                       *
	*************************************************************************/
	if (*objectsCount != 0) {
		buffer = (CK_OBJECT_HANDLE_PTR)realloc(*objects, *objectsCount * sizeof(CK_OBJECT_HANDLE));
		CHECK("  Memory reallocation for object handles", buffer != NULL, find_final);
		*objects = buffer;
	}

	errorCode = 0;

	/*************************************************************************
	* Деинициализировать операцию поиска                                     *
	*************************************************************************/
find_final:
	rv = functionList->C_FindObjectsFinal(session);
	CHECK_RELEASE_AND_LOG("  C_FindObjectsFinal", rv == CKR_OK, rvToStr(rv), errorCode);

	/*************************************************************************
	* Очистить память, выделенную под объекты                                *
	*************************************************************************/
	if (errorCode || *objectsCount == 0) {
		free(*objects);
		*objects = NULL_PTR;
	}

exit:
	return errorCode;
}

/*************************************************************************
* Функция вывода шестнадцатеричного буфера заданной длины                *
*************************************************************************/
static void printHex(const CK_BYTE* buffer,   // Буфер
                     const CK_ULONG length    // Длина буфера
                     )
{
	unsigned int i;
	const unsigned int width = 16;
	for (i = 0; i < length; ++i) {
		if (i % width == 0) {
			printf("   ");
		}

		printf("%02X ", buffer[i]);

		if ((i + 1) % width == 0 || (i + 1) == length) {
			printf("\n");
		}
	}
}

/*************************************************************************
* Функция вывода UTF-8. Выводит символы 0-127 и кириллицу. Возвращает 0  *
* в случае успешного выполнения, иначе не 0                              *
*************************************************************************/
static int printUTF8String(CK_BYTE* info)
{
#ifdef _WIN32
	CK_ULONG sym = 0;
	UINT cp = GetConsoleOutputCP();
	BOOL set = SetConsoleOutputCP(866); //кодировка cp-866
	if (set == FALSE)
		return 1;
	while (*info) {
		if (*info < 0x80) {
			printf("%c", *info);                         //вывод однобайтовых символов
			++info;
		}
		else if (*info & 0xC0) {                       //вывод двухбайтовых символов
			sym = ((*info & 0x1F) << 6) + (*(info + 1) & 0x3F);
			if (sym >= 0x0410 && sym <= 0x042F) {        //прописные
				printf("%c", sym - 0x0410 + 0x80);
			}
			else if (sym >= 0x0430 && sym <= 0x043F) { //строчные до 'р'
				printf("%c", sym - 0x0430 + 0xA0);
			}
			else if (sym >= 0x0440 && sym <= 0x044F) { //строчные после 'р'
				printf("%c", sym - 0x0440 + 0xE0);
			}
			else if (sym == 0x0401) {                  //Ё
				printf("%c", 0xF0);
			}
			else if (sym == 0x0451) {                  //ё
				printf("%c", 0xF1);
			}
			else {
				printf("?");                                 //все остальные двухбайтные символы
			}
			info += 2;
		}
		else if (*info & 0xE0) {        //трёх- и более байтные символы
			info += 3;
			printf("?");
		}
		else if (*info & 0xF0) {
			info += 4;
			printf("?");
		}
		else if (*info & 0xF8) {
			info += 5;
			printf("?");
		}
		else if (*info & 0xFC) {
			info += 6;
			printf("?");
		}
		else {
			++info;
		}
	}
	set = SetConsoleOutputCP(cp);
	return !set;
#else
	printf("%s", info);
	return 0;
#endif
}

/*************************************************************************
* Функция конвертирования 6-битного кода в печатный символ Base64        *
*************************************************************************/
static char ConvertCodeToSymBase64(CK_BYTE code    // 6-битный код
                                   )
{
	const char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	if (code < 0x40) {
		return alphabet[(int)code];
	} else {
		return '?';
	}
}

/*************************************************************************
* Функция выборки 6 бит из массива байт                                  *
*************************************************************************/
static CK_BYTE GetNext6Bit(CK_BYTE_PTR csr,          // Указатель на начало массива
                           CK_ULONG start,           // Номер бита в массиве, с которого начинается группа из 6 бит
                           CK_ULONG end              // Номер последнего бита массива
                           )
{
	CK_BYTE diff = start % 8;
	csr += start / 8;
	if (end - start > 8) {
		return 0x3F & (*csr << diff | *(csr + 1) >> (8 - diff)) >> 2;
	} else {
		return 0x3F & (*csr << diff >> 2);
	}
}

/*************************************************************************
* Функция конвертирования массива байт в строку Base64                   *
*************************************************************************/
static void ConvertToBase64String(CK_BYTE_PTR data,         // Исходные данные
                                  CK_ULONG size,            // Длина исходного массива
                                  char** result             // Результирующие данные (нуль-терминированная строка)
                                  )
{
	CK_ULONG i = 0;
	char* pt;
	*result = (char*)calloc(((size_t)size + 2) / 3 * 4 + 1, sizeof(char));
	if (*result != NULL) {
		memset(*result, '=', ((size_t)size + 2) / 3 * 4);
		for (pt = *result; i < size * 8; i += 6, ++pt) {
			*pt = ConvertCodeToSymBase64(GetNext6Bit(data, i, size * 8));
		}
	}
}

/*************************************************************************
* Функция преобразования массива байт в PEM формат                       *
*************************************************************************/
static void GetBytesAsPem(CK_BYTE_PTR source,                  // Исходные данные
                          CK_ULONG size,                       // Длина исходного массива
                          const char* header,                  // Начальный тег
                          const char* footer,                  // Конечный тег
                          char** result                        // Результирующий запрос
                          )
{
	size_t length;
	size_t width = 0x40;
	char* buffer;
	size_t i;

	ConvertToBase64String(source, size, &buffer);
	if (buffer == NULL) {
		*result = NULL;
		return;
	}
	length = strlen(buffer);
	*result = (char*)calloc(strlen(header) // Место под начальный тег
		+ length                           // Место под base64 строку
		+ strlen(footer)                   // Место под конечный тег
		+ (length - 1) / width + 1         // Место под переносы строки
		+ 1,                               // Нуль-байт
		sizeof(char));
	if (*result == NULL) {
		free(buffer);
		return;
	}
	//компоновка данных
	#pragma warning(suppress : 4996) // отключаем warning - deprecate
	strcat(*result, header);
	for (i = 0; i < length; i += width) {
		#pragma warning(suppress : 4996) // отключаем warning - deprecate
		strncat(*result, buffer + i, width);
		#pragma warning(suppress : 4996) // отключаем warning - deprecate
		strcat(*result, "\n");
	}
	#pragma warning(suppress : 4996) // отключаем warning - deprecate
	strcat(*result, footer);

	free(buffer);
}

/*************************************************************************
* Функция получения CSR в формате PEM                                    *
*************************************************************************/
static void GetCSRAsPEM(CK_BYTE_PTR source,                  // Исходные данные
                        CK_ULONG size,                       // Длина исходного массива
                        char** result                        // Результирующий запрос
                        )
{
	const char* begin = "-----BEGIN NEW CERTIFICATE REQUEST-----\n"; // Начало запроса
	const char* end = "-----END NEW CERTIFICATE REQUEST-----\n";     // Конец запроса
	
	GetBytesAsPem(source, size, begin, end, result);
}

/*************************************************************************
* Функция получения тела сертификата в формате PEM                       *
*************************************************************************/
static void GetCertAsPem(CK_BYTE_PTR source,    // Исходные данные
                         CK_ULONG size,         // Длина исходного массива
                         char** result          // Указатель на строку с результатом
                         )
{
	const char* begin = "-----BEGIN CERTIFICATE-----\n"; // Начало сертификата
	const char* end = "-----END CERTIFICATE-----\n";     // Конец сертификата
	
	GetBytesAsPem(source, size, begin, end, result);
}

#endif //PKCS11_COMMON_H
