/*******************************************************************
* Copyright (C) Aktiv Co. 2003-2021                                *
* rtpkcs11t.h                                                      *
* Файл, включающий все символы для работы с библиотекой PKCS#11,   *
* а также расширения для Rutoken.                                  *
********************************************************************/

#ifndef __RTPKCS11T_H__
#define __RTPKCS11T_H__

#include "pkcs11tc26_12.h"
#include "pkcs11tc26_18.h"

#define CK_VENDOR_PKCS11_RU_TEAM_TK26   NSSCK_VENDOR_PKCS11_RU_TEAM

/* GOST KEY TYPES */
#define CKK_GOSTR3410                   0x00000030
#define CKK_GOSTR3411                   0x00000031
#define CKK_GOST28147                   0x00000032

/* GOST OBJECT ATTRIBUTES */
#define CKA_GOSTR3410_PARAMS            0x00000250
#define CKA_GOSTR3411_PARAMS            0x00000251
#define CKA_GOST28147_PARAMS            0x00000252

/* Extended attributes for GOST private keys */
/* Ключ только для операций с журналом - тип CK_BBOOL */
#define CKA_VENDOR_KEY_JOURNAL          (CKA_VENDOR_DEFINED | 0x2002)
/* Deprecated */
#define CKA_VENDOR_KEY_PIN_ENTER        (CKA_VENDOR_DEFINED | 0x2000)
#define CKA_VENDOR_KEY_CONFIRM_OP       (CKA_VENDOR_DEFINED | 0x2001)

/* Extended attributes for private keys for Rutoken Touch */
/* Операция подписи требует подтверждения нажатием кнопки - тип CK_BBOOL */
#define CKA_VENDOR_CONFIRM_BY_TOUCH     (CKA_VENDOR_DEFINED | 0x2003)

#define CKA_VENDOR_SECURE_MESSAGING_AVAILABLE       (CKA_VENDOR_DEFINED | 0x3000)
#define CKA_VENDOR_CURRENT_SECURE_MESSAGING_MODE    (CKA_VENDOR_DEFINED | 0x3001)
#define CKA_VENDOR_SUPPORTED_SECURE_MESSAGING_MODES (CKA_VENDOR_DEFINED | 0x3002)

#define CKA_VENDOR_CURRENT_TOKEN_INTERFACE          (CKA_VENDOR_DEFINED | 0x3003)
#define CKA_VENDOR_SUPPORTED_TOKEN_INTERFACE        (CKA_VENDOR_DEFINED | 0x3004)

#define CKA_VENDOR_EXTERNAL_AUTHENTICATION          (CKA_VENDOR_DEFINED | 0x3005)
#define CKA_VENDOR_BIOMETRIC_AUTHENTICATION         (CKA_VENDOR_DEFINED | 0x3006)

#define CKA_VENDOR_SUPPORT_CUSTOM_PIN               (CKA_VENDOR_DEFINED | 0x3007)
#define CKA_VENDOR_CUSTOM_ADMIN_PIN                 (CKA_VENDOR_DEFINED | 0x3008)
#define CKA_VENDOR_CUSTOM_USER_PIN                  (CKA_VENDOR_DEFINED | 0x3009)

#define CKA_VENDOR_SUPPORT_INTERNAL_TRUSTED_CERTS   (CKA_VENDOR_DEFINED | 0x300A)

#define CKA_VENDOR_SUPPORT_FKC2                     (CKA_VENDOR_DEFINED | 0x300B)

/* Атрибуты КТИ */
/* Массив байт, содержащий контрольную сумму объекта */
#define CKA_VENDOR_CHECKSUM                         (CKA_VENDOR_DEFINED | 0x3100)

/* Массив байт, содержащий HMAC от передаваемых данных */
#define CKA_VENDOR_HMAC_VALUE                       (CKA_VENDOR_DEFINED | 0x3101)

/* Атрибут сертификата - доверенный сертификат, занесенный с помощью ключа эмитента */
#define CKA_VENDOR_INTERNAL_TRUSTED_CERT            (CKA_VENDOR_DEFINED | 0x3102)

/* Атрибут вектора инициализации используемого при занесении данных с помощью ключа эмитента */
#define CKA_VENDOR_IV                               (CKA_VENDOR_DEFINED | 0x3103)

/* PIN state */
#define CKA_VENDOR_PIN_POLICY_STATE                     (CKA_VENDOR_DEFINED | 0x3200)
/* Pin policies will be removed during format */
#define CKA_VENDOR_PIN_POLICIES_DELETABLE               (CKA_VENDOR_DEFINED | 0x3201)
/* Get array of CK_ATTRIBUTE_TYPE of supported policies */
#define CKA_VENDOR_SUPPORTED_PIN_POLICIES               (CKA_VENDOR_DEFINED | CKF_ARRAY_ATTRIBUTE | 0x3202)
/* Minimal PIN length */
#define CKA_VENDOR_PIN_POLICY_MIN_LENGTH                (CKA_VENDOR_DEFINED | 0x3203)
/* Number of previous PINs remembered. New PINs cannot be set to those values */
#define CKA_VENDOR_PIN_POLICY_HISTORY_DEPTH             (CKA_VENDOR_DEFINED | 0x3204)
/* Permits operations with default PIN */
#define CKA_VENDOR_PIN_POLICY_ALLOW_DEFAULT_PIN_USAGE   (CKA_VENDOR_DEFINED | 0x3205)
/* PIN contains at least one digit */
#define CKA_VENDOR_PIN_POLICY_DIGIT_REQUIRED            (CKA_VENDOR_DEFINED | 0x3206)
/* PIN contains at least one upper case letter */
#define CKA_VENDOR_PIN_POLICY_UPPERCASE_REQUIRED        (CKA_VENDOR_DEFINED | 0x3207)
/* PIN contains at least one lower case letter */
#define CKA_VENDOR_PIN_POLICY_LOWERCASE_REQUIRED        (CKA_VENDOR_DEFINED | 0x3208)
/* PIN contains at least one special character */
#define CKA_VENDOR_PIN_POLICY_SPEC_CHAR_REQUIRED        (CKA_VENDOR_DEFINED | 0x3209)
/* PIN doesn't consist of one repeated character */
#define CKA_VENDOR_PIN_POLICY_DIFF_CHARS_REQUIRED       (CKA_VENDOR_DEFINED | 0x320a)
#define CKA_VENDOR_USER_TYPE                            (CKA_VENDOR_DEFINED | 0x320b)

#define CKA_NAME_HASH_ALGORITHM         0x0000008C
#define CKA_COPYABLE                    0x00000171

/* GOST MECHANISMS */
#define CKM_GOSTR3410_KEY_PAIR_GEN              0x00001200
#define CKM_GOSTR3410                           0x00001201
#define CKM_GOSTR3410_WITH_GOSTR3411            0x00001202
#define CKM_GOSTR3410_KEY_WRAP                  0x00001203
#define CKM_GOSTR3410_DERIVE                    0x00001204
#define CKM_GOSTR3411                           0x00001210
#define CKM_GOSTR3411_HMAC                      0x00001211
#define CKM_GOST28147_KEY_GEN                   0x00001220
#define CKM_GOST28147_ECB                       0x00001221
#define CKM_GOST28147                           0x00001222
#define CKM_GOST28147_MAC                       0x00001223
#define CKM_GOST28147_KEY_WRAP                  0x00001224
#define CKM_KDF_TREE_GOSTR3411_2012_256         KDF_TREE_GOSTR3411_2012_256

/* GOST DIVERSIFICATION TYPES */
#define CKD_CPDIVERSIFY_KDF             0x00000009
#define CKD_KDF_4357                    CKM_KDF_4357
#define CKD_KDF_GOSTR3411_2012_256      CKM_KDF_GOSTR3411_2012_256

#define CKP_PKCS5_PBKD2_HMAC_GOSTR3411  0x00000002

typedef struct CK_GOSTR3410_KEY_WRAP_PARAMS {
  CK_BYTE_PTR pWrapOID;
  CK_ULONG ulWrapOIDLen;
  CK_BYTE_PTR pUKM;
  CK_ULONG ulUKMLen;
  CK_OBJECT_HANDLE hKey;
} CK_GOSTR3410_KEY_WRAP_PARAMS;

typedef CK_GOSTR3410_KEY_WRAP_PARAMS CK_PTR CK_GOSTR3410_KEY_WRAP_PARAMS_PTR;

typedef struct CK_GOSTR3410_DERIVE_PARAMS {
  CK_EC_KDF_TYPE kdf;
  CK_BYTE_PTR pPublicData;
  CK_ULONG ulPublicDataLen;
  CK_BYTE_PTR pUKM;
  CK_ULONG ulUKMLen;
} CK_GOSTR3410_DERIVE_PARAMS;

typedef CK_GOSTR3410_DERIVE_PARAMS CK_PTR CK_GOSTR3410_DERIVE_PARAMS_PTR;

/* Do not attach signed data to PKCS#7 signature */
#define PKCS7_DETACHED_SIGNATURE 0x01

/* Use hardware hash in operation, can be passed only to PKCS#7 signature operation */
#define USE_HARDWARE_HASH 0x02

/* CK_FUNCTION_LIST_EXTENDED is a structure holding a Cryptoki spec
 * version and pointers of appropriate types to all the
 * Cryptoki extended functions */
typedef struct CK_FUNCTION_LIST_EXTENDED CK_FUNCTION_LIST_EXTENDED;

typedef CK_FUNCTION_LIST_EXTENDED CK_PTR CK_FUNCTION_LIST_EXTENDED_PTR;

typedef CK_FUNCTION_LIST_EXTENDED_PTR CK_PTR CK_FUNCTION_LIST_EXTENDED_PTR_PTR;

/* Data structure use in C_EX_InitToken - extended function */
/* for all token reformat (C_InitToken will format only PKCS#11) */
/*
 * ulSizeofThisStructure [in] - init this field by size of this
 *                              structure. For example -
 *         st.ulSizeofThisStructure = sizeof(CK_RUTOKEN_INIT_PARAM)
 *
 * UseRepairMode [in] == 0: format procedure requires authentication
 *                          as administrator
 *                    != 0: format procedure executes without
 *                          authentication as administrator
 *
 * pNewAdminPin [in] - pointer to byte array with new administrator
 *                     PIN
 *
 * ulNewAdminPinLen [in] - length of new administrator PIN in bytes:
 *                minimum bMinAdminPinLength, maximum 32.
 *
 * pNewUserPin [in] - pointer to byte array with new user PIN
 *
 * ulNewUserPinLen [in] - length of new user PIN in bytes:
 *                 minimum bMinUserPinLength, maximum 32.
 *
 * ChangeUserPINPolicy [in] - (flags) policy of change user PIN.
 *        Values:
 *        1) if set TOKEN_FLAGS_ADMIN_CHANGE_USER_PIN (0x1) -
 *           administrator can change user PIN
 *        2) if set TOKEN_FLAGS_USER_CHANGE_USER_PIN (0x2) - user can
 *           change user PIN
 *        3) if set 2 flags: TOKEN_FLAGS_ADMIN_CHANGE_USER_PIN and
 *           TOKEN_FLAGS_USER_CHANGE_USER_PIN (0x3) - administrator
 *           and user can change user PIN
 *        4) in another cases - error
 *
 * ulMinAdminPinLen [in] - minimal size of administrator PIN in bytes
 *                         minimum 1, maximum 31.
 *
 * ulMinUserPinLen [in] - minimal size of user PIN in bytes
 *                        minimum 1, maximum 31.
 *
 * ulMaxAdminRetryCount [in] - minimum 3, maximum 10
 * ulMaxUserRetryCount [in] - minimum 1, maximum 10
 *
 * pTokenLabel [in] - pointer to byte array with new token symbol
 *                    name, if pTokenLabel == NULL - token symbol
 *                    name will not set
 *
 * ulLabelLen [in] - length of new token symbol name
 */
typedef struct CK_RUTOKEN_INIT_PARAM {
  CK_ULONG    ulSizeofThisStructure;
  CK_ULONG    UseRepairMode;
  CK_BYTE_PTR pNewAdminPin;
  CK_ULONG    ulNewAdminPinLen;
  CK_BYTE_PTR pNewUserPin;
  CK_ULONG    ulNewUserPinLen;
  /* Correct values (see description):
   * TOKEN_FLAGS_ADMIN_CHANGE_USER_PIN
   * TOKEN_FLAGS_USER_CHANGE_USER_PIN
   * TOKEN_FLAGS_ADMIN_CHANGE_USER_PIN | TOKEN_FLAGS_USER_CHANGE_USER_PIN
   */
  CK_FLAGS    ChangeUserPINPolicy; /* see below */
  CK_ULONG    ulMinAdminPinLen;
  CK_ULONG    ulMinUserPinLen;
  CK_ULONG    ulMaxAdminRetryCount;
  CK_ULONG    ulMaxUserRetryCount;
  CK_BYTE_PTR pTokenLabel;
  CK_ULONG    ulLabelLen;
  CK_ULONG    ulSmMode;
} CK_RUTOKEN_INIT_PARAM;

typedef CK_RUTOKEN_INIT_PARAM CK_PTR CK_RUTOKEN_INIT_PARAM_PTR;

/* C_EX_SlotManage mode */
#define MODE_RESTORE_FACTORY_DEFAULTS    0x06
#define MODE_GET_PIN_SET_TO_BE_CHANGED   0x07

/* Data structure to be used in C_EX_SlotManage -
 * extended function with MODE_RESTORE_FACTORY_DEFAULTS for
 * token supporting KTI
 *
 * ulSizeofThisStructure [in] - init this field by size of this
 *                              structure. For example -
 *  st.ulSizeofThisStructure = sizeof(CK_RUTOKEN_CHANGE_EMITENT_PASS_PARAM)
 *
 * pAdminPin [in] - pointer to byte array with current admin PIN
 *
 * ulAdminPinLen [in] - length of current admin PIN
 *
 * pInitParam [in] - pointer to structure with initialization params
 *
 * pNewEmitentKey [in] - pointer to byte array with new emitent key (32 bytes)
 *
 * ulNewEmitentKeyLen [in] - length of new emitent key (32)
 *
 * ulNewEmitentKeyRetryCount [in] - emitent key retry count
 */
typedef struct CK_VENDOR_RESTORE_FACTORY_DEFAULTS_PARAMS {
  CK_ULONG                  ulSizeofThisStructure;
  CK_BYTE_PTR               pAdminPin;
  CK_ULONG                  ulAdminPinLen;
  CK_RUTOKEN_INIT_PARAM_PTR pInitParam;
  CK_BYTE_PTR               pNewEmitentKey;
  CK_ULONG                  ulNewEmitentKeyLen;
  CK_ULONG                  ulNewEmitentKeyRetryCount;
} CK_VENDOR_RESTORE_FACTORY_DEFAULTS_PARAMS;

typedef CK_VENDOR_RESTORE_FACTORY_DEFAULTS_PARAMS CK_PTR CK_VENDOR_RESTORE_FACTORY_DEFAULTS_PARAMS_PTR;

/* CK_TOKEN_INFO_EXTENDED provides extended information about a
 * token */
typedef struct CK_TOKEN_INFO_EXTENDED {
  /* init this field by size of this structure
   * [in] - size of input structure
   * [out] - return size of filled structure
   */
  CK_ULONG ulSizeofThisStructure;
  /* type of token: */
  /* DEPRECATED: HW feature with the type CKH_VENDOR_TOKEN_INFO
   * should be used to get current connection interface.
   */
  CK_ULONG ulTokenType;       /* see below */
  /* exchange protocol number */
  CK_ULONG ulProtocolNumber;
  /* microcode number */
  CK_ULONG ulMicrocodeNumber;
  /* order number */
  CK_ULONG ulOrderNumber;
  /* information flags */
  CK_FLAGS flags;                 /* see below */
  /* maximum and minimum PIN length */
  CK_ULONG ulMaxAdminPinLen;
  CK_ULONG ulMinAdminPinLen;
  CK_ULONG ulMaxUserPinLen;
  CK_ULONG ulMinUserPinLen;
  /* max count of unsuccessful login attempts */
  CK_ULONG ulMaxAdminRetryCount;
  /* count of unsuccessful attempts left (for administrator PIN)
   * if field equal 0 - that means that PIN is blocked */
  CK_ULONG ulAdminRetryCountLeft;
  /* min counts of unsuccessful login attempts */
  CK_ULONG ulMaxUserRetryCount;
  /* count of unsuccessful attempts left (for user PIN)
   * if field equal 0 - that means that PIN is blocked */
  CK_ULONG ulUserRetryCountLeft;
  /* token serial number in Big Endian format */
  CK_BYTE  serialNumber[8];
  /* size of all memory */
  CK_ULONG ulTotalMemory;         /* in bytes */
  /* size of free memory */
  CK_ULONG ulFreeMemory;          /* in bytes */
  /* atr of the token */
  CK_BYTE  ATR[64];
  /* size of atr */
  CK_ULONG ulATRLen;
  /* class of token */
  CK_ULONG ulTokenClass;          /* see below */
  /* Battery Voltage */
  CK_ULONG ulBatteryVoltage;      /* millivolts */
  /* Body color of the token */
  CK_ULONG ulBodyColor;           /* see below */
  /* Checksum of token firmware */
  CK_ULONG ulFirmwareChecksum;
} CK_TOKEN_INFO_EXTENDED;

typedef CK_TOKEN_INFO_EXTENDED CK_PTR CK_TOKEN_INFO_EXTENDED_PTR;

/* DEPRECATED: Token types (field "ulTokenType") */
#define TOKEN_TYPE_UNKNOWN                      0xFF
#define TOKEN_TYPE_RUTOKEN_ECP                  0x01
#define TOKEN_TYPE_RUTOKEN_LITE                 0x02
#define TOKEN_TYPE_RUTOKEN                      0x03
#define TOKEN_TYPE_RUTOKEN_ECPDUAL_USB          0x09
#define TOKEN_TYPE_RUTOKEN_ECPDUAL_BT           0x69
#define TOKEN_TYPE_RUTOKEN_ECPDUAL_UART         0xA9
#define TOKEN_TYPE_RUTOKEN_ECP_SC               TOKEN_TYPE_RUTOKEN_SC_JC
#define TOKEN_TYPE_RUTOKEN_SCDUAL               0x49
#define TOKEN_TYPE_RUTOKEN_SCDUAL_NFC           0xC9
#define TOKEN_TYPE_RUTOKEN_LITE_SC_JC           0x42
#define TOKEN_TYPE_RUTOKEN_MIKRON               0x05
#define TOKEN_TYPE_RUTOKEN_MIKRON_SC            0x45
#define TOKEN_TYPE_RUTOKEN_MIKRON_SCDUAL        0x4D
#define TOKEN_TYPE_RUTOKEN_MIKRON_SCDUAL_NFC    0xCD
#define TOKEN_TYPE_RUTOKEN_ECP_NFC              0xC1
#define TOKEN_TYPE_RUTOKEN_SC_JC                0x41
#define TOKEN_TYPE_RUTOKEN_PINPAD_FAMILY        0x04
#define TOKEN_TYPE_RUTOKEN_WEB                  0x23
#define TOKEN_TYPE_RUTOKEN_ECP_SD               0x81
#define TOKEN_TYPE_RUTOKEN_LITE_SD              0x82

/* Token flags (field "flags" from CK_TOKEN_INFO_EXTENDED +
 * field "ChangeUserPINPolicy" from CK_RUTOKEN_INIT_PARAM) */
/* TOKEN_FLAGS_ADMIN_CHANGE_USER_PIN - if it is set, that
 * means that administrator (SO) can change user PIN
 */
#define TOKEN_FLAGS_ADMIN_CHANGE_USER_PIN         0x00000001

/* TOKEN_FLAGS_USER_CHANGE_USER_PIN - if it is set, that
 * means that user can change user PIN
 */
#define TOKEN_FLAGS_USER_CHANGE_USER_PIN          0x00000002

/* TOKEN_FLAGS_ADMIN_PIN_NOT_DEFAULT - if it is set, that
 * means that current administrator (SO) PIN is not default
 */
#define TOKEN_FLAGS_ADMIN_PIN_NOT_DEFAULT         0x00000004

/* TOKEN_FLAGS_USER_PIN_NOT_DEFAULT - if it is set, that
 * means that current user PIN not default
 */
#define TOKEN_FLAGS_USER_PIN_NOT_DEFAULT          0x00000008

/* TOKEN_FLAGS_SUPPORT_FKN - if it is set, that means that
 * token supports CryptoPro FKN
 */
#define TOKEN_FLAGS_SUPPORT_FKN                   0x00000010

/* TOKEN_FLAGS_SUPPORT_SM - if it is set, that means that
 * token supports communication using Secure Messaging.
 * This flag is deprecated, use
 * TOKEN_FLAGS_SUPPORT_SECURE_MESSAGING instead.
 */
#define TOKEN_FLAGS_SUPPORT_SM                    0x00000040

/* TOKEN_FLAGS_HAS_FLASH_DRIVE - if it is set, that means
 * that token has Flash Drive
 */
#define TOKEN_FLAGS_HAS_FLASH_DRIVE               0x00000080

/* TOKEN_FLAGS_CAN_CHANGE_SM_MODE - if it is set, that means
 * that token can be formatted to support SM. This flag
 * is deprecated, use TOKEN_FLAGS_SUPPORT_SECURE_MESSAGING
 * instead.
 */
#define TOKEN_FLAGS_CAN_CHANGE_SM_MODE            0x00000100

/* TOKEN_FLAGS_SUPPORT_SECURE_MESSAGING - if it is set, that 
 * means that token supports Secure Messaging 
 */
#define TOKEN_FLAGS_SUPPORT_SECURE_MESSAGING      0x00000100

/* TOKEN_FLAGS_HAS_BUTTON - if it is set, that means
 * that token is Rutoken Touch
 */
#define TOKEN_FLAGS_HAS_BUTTON                    0x00000200

/* TOKEN_FLAGS_SUPPORT_JOURNAL - if it is set, that means 
 * that token supports journal
 */
#define TOKEN_FLAGS_SUPPORT_JOURNAL               0x00000400

/* TOKEN_FLAGS_USER_PIN_UTF8 - if it is set, that means
 * that user PIN in UTF-8
 */
#define TOKEN_FLAGS_USER_PIN_UTF8                 0x00000800

/* TOKEN_FLAGS_ADMIN_PIN_UTF8 - if it is set, that means
 * that admin PIN in UTF-8
 */
#define TOKEN_FLAGS_ADMIN_PIN_UTF8                0x00001000

/* TOKEN_FLAGS_FW_CHECKSUM_UNAVAILIBLE - if it is set, that
 * means Token does not support firmware checksum request
 * and 'ulFirmwareChecksum' can not be used
 */
#define TOKEN_FLAGS_FW_CHECKSUM_UNAVAILIBLE       0x40000000

/* TOKEN_FLAGS_FW_CHECKSUM_INVALID - if it is set, that
 * means that 'ulFirmwareChecksum' field contains different
 * from the reference (stored at Token) firmware checksum
 */
#define TOKEN_FLAGS_FW_CHECKSUM_INVALID           0x80000000

/* class of token */
#define TOKEN_CLASS_UNKNOWN             0xFFFFFFFF
#define TOKEN_CLASS_S                   0x00
#define TOKEN_CLASS_ECP                 0x01
#define TOKEN_CLASS_LITE                0x02
#define TOKEN_CLASS_ECP_BT              TOKEN_CLASS_ECPDUAL
/* DEPRECATED */
#define TOKEN_CLASS_WEB                 0x03
#define TOKEN_CLASS_PINPAD              0x04
#define TOKEN_CLASS_ECPDUAL             0x09

/* Body color of the token */
#define TOKEN_BODY_COLOR_UNKNOWN        0
#define TOKEN_BODY_COLOR_WHITE          1
#define TOKEN_BODY_COLOR_BLACK          2

/* Расширенные коды ошибок */
#define CKR_CORRUPTED_MAPFILE           (CKR_VENDOR_DEFINED+1)
#define CKR_WRONG_VERSION_FIELD         (CKR_VENDOR_DEFINED+2)
#define CKR_WRONG_PKCS1_ENCODING        (CKR_VENDOR_DEFINED+3)
#define CKR_RTPKCS11_DATA_CORRUPTED     (CKR_VENDOR_DEFINED+4)
#define CKR_RTPKCS11_RSF_DATA_CORRUPTED (CKR_VENDOR_DEFINED+5)
#define CKR_SM_PASSWORD_INVALID         (CKR_VENDOR_DEFINED+6)
#define CKR_LICENSE_READ_ONLY           (CKR_VENDOR_DEFINED+7)
#define CKR_VENDOR_EMITENT_KEY_BLOCKED  (CKR_VENDOR_DEFINED+8)
#define CKR_CERT_CHAIN_NOT_VERIFIED     (CKR_VENDOR_DEFINED+9)
#define CKR_INAPPROPRIATE_PIN           (CKR_VENDOR_DEFINED+10)
#define CKR_PIN_IN_HISTORY              (CKR_VENDOR_DEFINED+11)

/* Rutoken ECP Flash */
typedef CK_ULONG CK_VOLUME_ID_EXTENDED;
typedef CK_ULONG CK_ACCESS_MODE_EXTENDED;
typedef CK_ULONG CK_OWNER_EXTENDED;

#define CKF_ENCRYPTION                  0x01000000

#define ACCESS_MODE_HIDDEN              0x00
#define ACCESS_MODE_RO                  0x01
#define ACCESS_MODE_RW                  0x03
#define ACCESS_MODE_CD                  0x05

/* Data structure to be used in C_EX_SlotManage -
 * extended function with MODE_GET_IMIT argument
 * for st23 2.0 only
 *
 * bMode [in] - 1 - mask, 2 - mask and patch
 *
 * pbGostSymmetricKey [in] - byte array with symmetric gost key
 *
 * pbImit [out] - byte array. Computed MAC will be written here
 *
 */
typedef struct CK_TOKEN_IMIT_DATA {
  CK_BYTE     bMode;
  CK_BYTE     pbGostSymmetricKey[32];
  CK_BYTE     pbImit[8];
} CK_TOKEN_IMIT_DATA;

typedef CK_TOKEN_IMIT_DATA CK_PTR CK_TOKEN_IMIT_DATA_PTR;

/* C_EX_SlotManage mode */
#define MODE_GET_IMIT        0x04

/* Modes of computing of MAC */
#define IMIT_MODE_MASK       0x01
#define IMIT_MODE_MASK_PATCH 0x02

/* Data structure to be used in C_EX_SlotManage -
 * extended function with MODE_GET_LOCAL_PIN_INFO argument
 *
 * ulPinID [in] - id of local PIN
 *
 * ulMinSize, ulMaxSize [out] - minimum and maximum PIN length
 *
 * ulMaxRetryCount [out] - max count of unsuccessful login attempts
 *
 * ulCurrentRetryCount [out] - count of unsuccessful attempts left
 *                   if field equals 0 - that means that PIN is blocked
 *
 * flags [out] - information flags
 */
typedef struct CK_LOCAL_PIN_INFO {
  CK_ULONG ulPinID;
  CK_ULONG ulMinSize;
  CK_ULONG ulMaxSize;
  CK_ULONG ulMaxRetryCount;
  CK_ULONG ulCurrentRetryCount;
  CK_FLAGS flags;
} CK_LOCAL_PIN_INFO;

typedef CK_LOCAL_PIN_INFO CK_PTR CK_LOCAL_PIN_INFO_PTR;

/* C_EX_SlotManage mode */
#define MODE_GET_LOCAL_PIN_INFO        0x05

/* Local PIN flags (field "flags" from CK_LOCAL_PIN_INFO)
 * LOCAL_PIN_FLAGS_NOT_DEFAULT - if it is set, that
 * means that local PIN is not default
 */
#define LOCAL_PIN_FLAGS_NOT_DEFAULT       0x00000001
/* LOCAL_PIN_FLAGS_FROM_SCREEN - if it is set, that 
 * means that PIN can be entered only from screen
 */
#define LOCAL_PIN_FLAGS_FROM_SCREEN       0x00000002
 /* LOCAL_PIN_FLAGS_IS_UTF8 - if it is set, that
 * means that PIN is in UTF-8
 */
#define LOCAL_PIN_FLAGS_IS_UTF8           0x00000004

typedef struct CK_VOLUME_INFO_EXTENDED
{
  CK_VOLUME_ID_EXTENDED   idVolume;     /* Идентификатор раздела, от 1 до 9 */
  CK_ULONG                ulVolumeSize; /* Размер раздела в Мб, не больше общего объема носителя */
  CK_ACCESS_MODE_EXTENDED accessMode;   /* Флаги доступа */
  CK_OWNER_EXTENDED       volumeOwner;  /* Владелец раздела (кто может менять флаги доступа к разделу) */
  CK_FLAGS                flags;        /* Остальные флаги (пока только шифрование) */
} CK_VOLUME_INFO_EXTENDED;

typedef struct CK_VOLUME_FORMAT_INFO_EXTENDED
{
  CK_ULONG                ulVolumeSize;
  CK_ACCESS_MODE_EXTENDED accessMode;
  CK_OWNER_EXTENDED       volumeOwner;
  CK_FLAGS                flags;
} CK_VOLUME_FORMAT_INFO_EXTENDED;

typedef CK_VOLUME_INFO_EXTENDED CK_PTR CK_VOLUME_INFO_EXTENDED_PTR;
typedef CK_VOLUME_FORMAT_INFO_EXTENDED CK_PTR CK_VOLUME_FORMAT_INFO_EXTENDED_PTR;

/* Rutoken ECP Bluetooth */

/* C_EX_TokenManage mode */

/* CK_VENDOR_PIN_PARAMS is a structure to be used in C_EX_TokenManage
* extended function to change default PIN
*
* userType [in] - user type
*
* pPinValue [in] - new default PIN
*
* ulPinLength [in] - new default PIN length (in range from 6 to 32)
*/

typedef struct CK_VENDOR_PIN_PARAMS {
  CK_USER_TYPE userType;
  CK_UTF8CHAR_PTR pPinValue;
  CK_ULONG ulPinLength;
} CK_VENDOR_PIN_PARAMS;

typedef CK_VENDOR_PIN_PARAMS CK_PTR CK_VENDOR_PIN_PARAMS_PTR;

#define MODE_SET_BLUETOOTH_POWEROFF_TIMEOUT     0x01
#define MODE_SET_CHANNEL_TYPE                   0x02
#define MODE_RESET_CUSTOM_PIN_TO_STANDARD       0x03
#define MODE_RESET_PIN_TO_DEFAULT               0x04
#define MODE_CHANGE_DEFAULT_PIN                 0x05
#define MODE_FORCE_USER_TO_CHANGE_PIN           0x06

#define BLUETOOTH_POWEROFF_TIMEOUT_DEFAULT      0x00
#define BLUETOOTH_POWEROFF_TIMEOUT_MAX          0x46

#define CHANNEL_TYPE_USB                        0x00
#define CHANNEL_TYPE_BLUETOOTH                  0x01

/* C_EX_GenerateActivationPassword */
#define GENERATE_NEXT_PASSWORD                  0x00    /* Генерировать пароль со "следующим номером" */

/* C_EX_GenerateActivationPassword character sets */
#define CAPS_AND_DIGITS                         0x00    /* Заглавные буквы латинского алфавита без O и цифры без 0 */
#define CAPS_ONLY                               0x01    /* Заглавные буквы латинского алфавита */

/* Rutoken S vendor defined constants */

/* Механизм генерации ключей ГОСТ */
#define CKM_GOST_KEY_GEN        (CKM_VENDOR_DEFINED + 1)

/* Механизм шифрования по алгоритму ГОСТ */
#define CKM_GOST                (CKM_VENDOR_DEFINED + 2)

/* Механизм доверенного импорта ключевой информации */
#define CKM_VENDOR_SECURE_IMPORT                (CKM_VENDOR_DEFINED + 3)

/* Механизм согласования ключей VKO-512 */
#define CKM_VENDOR_VKO_GOSTR3410_2012_512       (CKM_VENDOR_DEFINED + 4)

/* Механизм согласования ключей KEG */
#define CKM_VENDOR_GOST_KEG     (CKM_VENDOR_DEFINED + 5)

/* CK_KEY_TYPE объекта CKO_SECRET_KEY*/
#define CKK_GOST                (CKK_VENDOR_DEFINED + 1)

/* Атрибут ключа ГОСТ - его опции */
#define CKA_GOST_KEY_OPTIONS    (CKA_VENDOR_DEFINED + 1)

/* Атрибут ключа ГОСТ - его флаги */
#define CKA_GOST_KEY_FLAGS      (CKA_VENDOR_DEFINED + 2)

/* Возможные опции ключа ГОСТ */
#define CKO_GOST_KEY_PZ         0x00    /* Простая замена */
#define CKO_GOST_KEY_GAMM       0x01    /* Гаммирование (по умолчанию) */
#define CKO_GOST_KEY_GAMMOS     0x02    /* Гаммирование с обратной связью */

/* Возможные флаги ключа ГОСТ */
#define CKF_GOST_KEY_CLOSED     0x00    /* Закрытый ключ (по умолчанию) */
#define CKF_GOST_KEY_OPENLEN    0x02    /* Ключ с открытой длиной */

typedef struct CK_VENDOR_VKO_GOSTR3410_2012_512_PARAMS {
  CK_EC_KDF_TYPE kdf;
  CK_BYTE_PTR pPublicData;
  CK_ULONG ulPublicDataLen;
  CK_BYTE_PTR pUKM;
  CK_ULONG ulUKMLen;
} CK_VENDOR_VKO_GOSTR3410_2012_512_PARAMS;

typedef CK_VENDOR_VKO_GOSTR3410_2012_512_PARAMS CK_PTR CK_VENDOR_VKO_GOSTR3410_2012_512_PARAMS_PTR;

typedef struct CK_VENDOR_GOST_KEG_PARAMS {
  CK_BYTE_PTR pPublicData;
  CK_ULONG ulPublicDataLen;
  CK_BYTE_PTR pUKM;
  CK_ULONG ulUKMLen;
} CK_VENDOR_GOST_KEG_PARAMS;

typedef CK_VENDOR_GOST_KEG_PARAMS CK_PTR CK_VENDOR_GOST_KEG_PARAMS_PTR;

/* Data structures needed for CMS verification functions */

typedef struct CK_VENDOR_BUFFER {
  CK_BYTE_PTR pData;
  CK_ULONG ulSize;
} CK_VENDOR_BUFFER;

typedef CK_VENDOR_BUFFER CK_PTR CK_VENDOR_BUFFER_PTR;
typedef CK_VENDOR_BUFFER_PTR CK_PTR CK_VENDOR_BUFFER_PTR_PTR;

typedef CK_ULONG CK_VENDOR_CRL_MODE;

typedef struct CK_VENDOR_X509_STORE {
  CK_VENDOR_BUFFER_PTR pTrustedCertificates;
  CK_ULONG ulTrustedCertificateCount;
  CK_VENDOR_BUFFER_PTR pCertificates;
  CK_ULONG ulCertificateCount;
  CK_VENDOR_BUFFER_PTR pCrls;
  CK_ULONG ulCrlCount;
} CK_VENDOR_X509_STORE;

typedef CK_VENDOR_X509_STORE CK_PTR CK_VENDOR_X509_STORE_PTR;

typedef CK_BYTE_PTR CK_PTR CK_BYTE_PTR_PTR;

/* Flags for customizing certificate verification process, should be passed
 * in flags argument to C_EX_PKCS7VerifyInit
 */

/* Do not search signer certificates in certificates attached to CMS */
#define CKF_VENDOR_DO_NOT_USE_INTERNAL_CMS_CERTS 0x01

/* If this flag is set - chain verification will be successful
 * if at least one CA certificate from chain is in trusted certificates list
 */
#define CKF_VENDOR_ALLOW_PARTIAL_CHAINS 0x02

 /* If this flag is set - certificate verification is not made
  * this flag is not compatible with CKF_VENDOR_DO_NOT_USE_INTERNAL_CMS_CERTS,
  * CKF_VENDOR_ALLOW_PARTIAL_CHAINS and CKF_VENDOR_USE_TRUSTED_CERTS_FROM_TOKEN
  * ckMode should be set to OPTIONAL_CRL_CHECK
  */
#define CKF_VENDOR_CHECK_SIGNATURE_ONLY 0x04

 /* If this flag is set - trusted certificate store will be used
  * to build a certificate chain. ulTrustedCertificateCount
  * in certificate store should be set to 0.
  */
#define CKF_VENDOR_USE_TRUSTED_CERTS_FROM_TOKEN 0x08

/* CRL checking modes, should be passed in ckMode to C_EX_PKCS7VerifyInit */

/* If we have no suitable CRL - it won't be an error */
#define OPTIONAL_CRL_CHECK 0

/* Signer's CA CRL should be passed */
#define LEAF_CRL_CHECK 1

/* CRLs of all CA from the chain should be passed */
#define ALL_CRL_CHECK 2

/* Interfaces */
#define INTERFACE_TYPE_USB      0x00000001
#define INTERFACE_TYPE_BT       0x00000002
#define INTERFACE_TYPE_UART     0x00000004
#define INTERFACE_TYPE_ISO      0x00000008
#define INTERFACE_TYPE_NFC      0x00000020
/* DEPRECATED */
#define INTERFACE_TYPE_SD       0x00000010

/* Secure messaging types */
#define SECURE_MESSAGING_DEFAULT        0x00
#define SECURE_MESSAGING_BUILT_IN       0x01
#define SECURE_MESSAGING_GOST           0x02
#define SECURE_MESSAGING_ENHANCED_GOST  0x03
#define SECURE_MESSAGING_UNSUPPORTED    0xff

/* Values for CKA_VENDOR_PIN_POLICY_STATE */
#define PIN_POLICY_STATE_WELL_DEFINED                       0x00
#define PIN_POLICY_STATE_HAS_UNKNOWN_ATTRIBUTES             0x01
#define PIN_POLICY_STATE_HAS_UNKNOWN_NONDEFAULT_ATTRIBUTES  0x02

/* Biometric types */
#define BIOMETRIC_AUTHENTICATION_NOT_SUPPORTED    0x00000000

/* Hardware feature types */
#define CKH_VENDOR_TOKEN_INFO       (CKH_VENDOR_DEFINED + 0x01)
#define CKH_VENDOR_EMITENT_KEY      (CKH_VENDOR_DEFINED + 0x02)
#define CKH_VENDOR_SECURE_COUNTER   (CKH_VENDOR_DEFINED + 0x03)
#define CKH_VENDOR_NDEF_TAG         (CKH_VENDOR_DEFINED + 0x04)
#define CKH_VENDOR_RNG              (CKH_VENDOR_DEFINED + 0x05)
#define CKH_VENDOR_PIN_POLICY       (CKH_VENDOR_DEFINED + 0x06)

#endif /* __RTPKCS11T_H__ */
