/***************************************************************
* Copyright (C) Aktiv Co. 2003-2023                            *
* rtpkcs11t.h                                                  *
* This file contains all symbols for PKCS#11 library, and      *
* extensions for Rutoken                                       *
***************************************************************/

#ifndef __RTPKCS11T_H__
#define __RTPKCS11T_H__

#include "pkcs11tc26_12.h"
#include "pkcs11tc26_18.h"

#define CK_VENDOR_PKCS11_RU_TEAM_TK26   NSSCK_VENDOR_PKCS11_RU_TEAM

// Container ID for objects which were created via Aktiv CSP/Rutoken Minidriver
#define  CKA_VENDOR_AKTIV_CSP_CONTAINER_ID          (CKA_VENDOR_DEFINED+9UL)

/* Extended attributes for GOST private keys */
/* Key only for journal operations - CK_BOOL type */
#define CKA_VENDOR_KEY_JOURNAL          (CKA_VENDOR_DEFINED|0x2002UL)
/* DEPRECATED */
#define CKA_VENDOR_KEY_PIN_ENTER        (CKA_VENDOR_DEFINED|0x2000UL)
#define CKA_VENDOR_KEY_CONFIRM_OP       (CKA_VENDOR_DEFINED|0x2001UL)

/* Extended attributes for private keys for Rutoken Touch */
/* Signing should be confirmed by button press - CK_BBOOL type */
#define CKA_VENDOR_CONFIRM_BY_TOUCH     (CKA_VENDOR_DEFINED|0x2003UL)

#define CKA_VENDOR_SECURE_MESSAGING_AVAILABLE       (CKA_VENDOR_DEFINED|0x3000UL)
#define CKA_VENDOR_CURRENT_SECURE_MESSAGING_MODE    (CKA_VENDOR_DEFINED|0x3001UL)
#define CKA_VENDOR_SUPPORTED_SECURE_MESSAGING_MODES (CKA_VENDOR_DEFINED|0x3002UL)

#define CKA_VENDOR_CURRENT_TOKEN_INTERFACE          (CKA_VENDOR_DEFINED|0x3003UL)
#define CKA_VENDOR_SUPPORTED_TOKEN_INTERFACE        (CKA_VENDOR_DEFINED|0x3004UL)

#define CKA_VENDOR_EXTERNAL_AUTHENTICATION          (CKA_VENDOR_DEFINED|0x3005UL)
#define CKA_VENDOR_BIOMETRIC_AUTHENTICATION         (CKA_VENDOR_DEFINED|0x3006UL)

#define CKA_VENDOR_SUPPORT_CUSTOM_PIN               (CKA_VENDOR_DEFINED|0x3007UL)
#define CKA_VENDOR_CUSTOM_ADMIN_PIN                 (CKA_VENDOR_DEFINED|0x3008UL)
#define CKA_VENDOR_CUSTOM_USER_PIN                  (CKA_VENDOR_DEFINED|0x3009UL)

#define CKA_VENDOR_SUPPORT_INTERNAL_TRUSTED_CERTS   (CKA_VENDOR_DEFINED|0x300AUL)

#define CKA_VENDOR_SUPPORT_FKC2                     (CKA_VENDOR_DEFINED|0x300BUL)

#define CKA_VENDOR_SUPPORT_HW_RESULT_FOR_GOST28147_KEY_UNWRAP (CKA_VENDOR_DEFINED|0x300CUL)

#define CKA_VENDOR_SUPPORT_HW_KDF_TREE              (CKA_VENDOR_DEFINED|0x300DUL)

#define CKA_VENDOR_SUPPORT_KIMP15                   (CKA_VENDOR_DEFINED|0x300EUL)
#define CKA_VENDOR_FLASH_DRIVE_AVAILABLE            (CKA_VENDOR_DEFINED|0x300FUL)

/* An UTF8-8 array, that contains symbolic token model name */
#define CKA_VENDOR_MODEL_NAME                       (CKA_VENDOR_DEFINED|0x3010UL)

/* Trusted import attributes */
/* A byte array, that contains checksum of an object */
#define CKA_VENDOR_CHECKSUM                         (CKA_VENDOR_DEFINED|0x3100UL)

/* A byte array, that contains HMAC of transferring data */
#define CKA_VENDOR_HMAC_VALUE                       (CKA_VENDOR_DEFINED|0x3101UL)

/* Certificate attribute - trusted certificate, imported via emitent key */
#define CKA_VENDOR_INTERNAL_TRUSTED_CERT            (CKA_VENDOR_DEFINED|0x3102UL)

/* IV attribute, which is used for data entry via emitent key */
#define CKA_VENDOR_IV                               (CKA_VENDOR_DEFINED|0x3103UL)

/* PIN state */
#define CKA_VENDOR_PIN_POLICY_STATE                     (CKA_VENDOR_DEFINED|0x3200UL)
/* Pin policies will be removed during format */
#define CKA_VENDOR_PIN_POLICIES_DELETABLE               (CKA_VENDOR_DEFINED|0x3201UL)
#define CKA_VENDOR_ARRAY_ATTRIBUTE                      (CKA_VENDOR_DEFINED|CKF_ARRAY_ATTRIBUTE)
/* Get array of CK_ATTRIBUTE_TYPE of supported policies */
#define CKA_VENDOR_SUPPORTED_PIN_POLICIES               (CKA_VENDOR_ARRAY_ATTRIBUTE|0x3202UL)
/* Minimal PIN length */
#define CKA_VENDOR_PIN_POLICY_MIN_LENGTH                (CKA_VENDOR_DEFINED|0x3203UL)
/* Number of previous PINs remembered. New PINs cannot be set to those values */
#define CKA_VENDOR_PIN_POLICY_HISTORY_DEPTH             (CKA_VENDOR_DEFINED|0x3204UL)
/* Permits operations with default PIN */
#define CKA_VENDOR_PIN_POLICY_ALLOW_DEFAULT_PIN_USAGE   (CKA_VENDOR_DEFINED|0x3205UL)
/* PIN contains at least one digit */
#define CKA_VENDOR_PIN_POLICY_DIGIT_REQUIRED            (CKA_VENDOR_DEFINED|0x3206UL)
/* PIN contains at least one upper case letter */
#define CKA_VENDOR_PIN_POLICY_UPPERCASE_REQUIRED        (CKA_VENDOR_DEFINED|0x3207UL)
/* PIN contains at least one lower case letter */
#define CKA_VENDOR_PIN_POLICY_LOWERCASE_REQUIRED        (CKA_VENDOR_DEFINED|0x3208UL)
/* PIN contains at least one special character */
#define CKA_VENDOR_PIN_POLICY_SPEC_CHAR_REQUIRED        (CKA_VENDOR_DEFINED|0x3209UL)
/* PIN doesn't consist of one repeated character */
#define CKA_VENDOR_PIN_POLICY_DIFF_CHARS_REQUIRED       (CKA_VENDOR_DEFINED|0x320AUL)
#define CKA_VENDOR_USER_TYPE                            (CKA_VENDOR_DEFINED|0x320BUL)

/* GOST DIVERSIFICATION TYPES */
#define CKD_KDF_4357                    CKM_KDF_4357
#define CKD_KDF_GOSTR3411_2012_256      CKM_KDF_GOSTR3411_2012_256

/* Do not attach signed data to PKCS#7 signature */
#define PKCS7_DETACHED_SIGNATURE 0x01UL

/* Use hardware hash in operation, can be passed only to PKCS#7 signature operation */
#define USE_HARDWARE_HASH 0x02UL

/* CK_FUNCTION_LIST_EXTENDED is a structure holding a Cryptoki spec
 * version and pointers of appropriate types to all the Cryptoki extended functions */
typedef struct CK_FUNCTION_LIST_EXTENDED CK_FUNCTION_LIST_EXTENDED;

typedef CK_FUNCTION_LIST_EXTENDED CK_PTR CK_FUNCTION_LIST_EXTENDED_PTR;

typedef CK_FUNCTION_LIST_EXTENDED_PTR CK_PTR CK_FUNCTION_LIST_EXTENDED_PTR_PTR;

/* Data structure use in C_EX_InitToken - extended function */
/* for all token reformat (C_InitToken will format only PKCS#11) */
typedef struct CK_RUTOKEN_INIT_PARAM {
  CK_ULONG    ulSizeofThisStructure;    /* [in] size of this structure. For example -
                                           st.ulSizeofThisStructure = sizeof(CK_RUTOKEN_INIT_PARAM) */
  CK_ULONG    UseRepairMode;            /* [in] == 0: format procedure requires authentication as administrator
                                                != 0: format procedure executes without authentication as administrator */
  CK_BYTE_PTR pNewAdminPin;             /* [in] pointer to byte array with new administrator PIN */
  CK_ULONG    ulNewAdminPinLen;         /* [in] length of new administrator PIN in bytes: minimum bMinAdminPinLength, maximum 32 */
  CK_BYTE_PTR pNewUserPin;              /* [in] pointer to byte array with new user PIN */
  CK_ULONG    ulNewUserPinLen;          /* [in] length of new user PIN in bytes: minimum bMinUserPinLength, maximum 32 */
  CK_FLAGS    ChangeUserPINPolicy;      /* [in] (flags) policy of change user PIN.
                                           see below for additional info about values possible values */
  CK_ULONG    ulMinAdminPinLen;         /* [in] minimal size of administrator PIN in bytes: minimum 1, maximum 31 */
  CK_ULONG    ulMinUserPinLen;          /* [in] minimal size of user PIN in bytes: minimum 1, maximum 31 */
  CK_ULONG    ulMaxAdminRetryCount;     /* [in] minimum 3, maximum 10 */
  CK_ULONG    ulMaxUserRetryCount;      /* [in] minimum 1, maximum 10 */
  CK_BYTE_PTR pTokenLabel;              /* [in] pointer to byte array with new token symbol name, 
                                           if pTokenLabel == NULL - token symbol name will not set */
  CK_ULONG    ulLabelLen;               /* [in] length of new token symbol name */
  CK_ULONG    ulSmMode;
} CK_RUTOKEN_INIT_PARAM;

typedef CK_RUTOKEN_INIT_PARAM CK_PTR CK_RUTOKEN_INIT_PARAM_PTR;

/* C_EX_SlotManage mode */
#define MODE_RESTORE_FACTORY_DEFAULTS    0x06UL
#define MODE_GET_PIN_SET_TO_BE_CHANGED   0x07UL

/* Data structure to be used in C_EX_SlotManage - extended function with
 * MODE_RESTORE_FACTORY_DEFAULTS for token supporting trusted import */
typedef struct CK_VENDOR_RESTORE_FACTORY_DEFAULTS_PARAMS {
  CK_ULONG                  ulSizeofThisStructure;      /* [in] init this field by size of this structure. For example -
                                                             st.ulSizeofThisStructure = sizeof(CK_RUTOKEN_CHANGE_EMITENT_PASS_PARAM) */
  CK_BYTE_PTR               pAdminPin;                  /* [in] pointer to byte array with current admin PIN */
  CK_ULONG                  ulAdminPinLen;              /* [in] length of current admin PIN */
  CK_RUTOKEN_INIT_PARAM_PTR pInitParam;                 /* [in] pointer to structure with initialization params*/
  CK_BYTE_PTR               pNewEmitentKey;             /* [in] pointer to byte array with new emitent key (32 bytes) */
  CK_ULONG                  ulNewEmitentKeyLen;         /* [in] length of new emitent key (32) */
  CK_ULONG                  ulNewEmitentKeyRetryCount;  /* [in] emitent key retry count */
} CK_VENDOR_RESTORE_FACTORY_DEFAULTS_PARAMS;

typedef CK_VENDOR_RESTORE_FACTORY_DEFAULTS_PARAMS CK_PTR CK_VENDOR_RESTORE_FACTORY_DEFAULTS_PARAMS_PTR;

/* CK_TOKEN_INFO_EXTENDED provides extended information about a token */
typedef struct CK_TOKEN_INFO_EXTENDED {
  CK_ULONG ulSizeofThisStructure;   /* init this field by size of this structure, [in] - size of input structure [out] - return size of filled structure */
  CK_ULONG ulTokenType;             /* DEPRECATED: type of token; HW feature with the type CKH_VENDOR_TOKEN_INFO should be used to get current connection interface 
  see below for additional info about values possible values */
  CK_ULONG ulProtocolNumber;        /* exchange protocol number */
  CK_ULONG ulMicrocodeNumber;       /* microcode number */
  CK_ULONG ulOrderNumber;           /* order number */
  CK_FLAGS flags;                   /* information flags
                                       see below for additional info about values possible values */
  CK_ULONG ulMaxAdminPinLen;        /* maximum administrator PIN length */
  CK_ULONG ulMinAdminPinLen;        /* minimum administrator PIN length */
  CK_ULONG ulMaxUserPinLen;         /* maximum user PIN length */
  CK_ULONG ulMinUserPinLen;         /* minimum user PIN length */
  CK_ULONG ulMaxAdminRetryCount;    /* max count of unsuccessful login attempts */
  CK_ULONG ulAdminRetryCountLeft;   /* count of unsuccessful attempts left (for administrator PIN), if field equal 0 - that means that PIN is blocked */
  CK_ULONG ulMaxUserRetryCount;     /* min counts of unsuccessful login attempts */
  CK_ULONG ulUserRetryCountLeft;    /* count of unsuccessful attempts left (for user PIN), if field equal 0 - that means that PIN is blocked */
  CK_BYTE  serialNumber[8];         /* token serial number in Big Endian format */
  CK_ULONG ulTotalMemory;           /* size of all memory in bytes */
  CK_ULONG ulFreeMemory;            /* size of free memory in bytes */
  CK_BYTE  ATR[64];                 /* atr of the token */
  CK_ULONG ulATRLen;                /* size of atr */
  CK_ULONG ulTokenClass;            /* class of token
                                       see below for additional info about values possible values */
  CK_ULONG ulBatteryVoltage;        /* battery voltage in millivolts */
  CK_ULONG ulBodyColor;             /* body color of the token
                                       see below for additional info about values possible values */
  CK_ULONG ulFirmwareChecksum;      /* checksum of token firmware */
  CK_ULONG ulBatteryPercentage;     /* token current battery charge percentage */
  CK_ULONG ulBatteryFlags;          /* token battery flags */
} CK_TOKEN_INFO_EXTENDED;

typedef CK_TOKEN_INFO_EXTENDED CK_PTR CK_TOKEN_INFO_EXTENDED_PTR;

/* DEPRECATED: Token types (field "ulTokenType") */
#define TOKEN_TYPE_UNKNOWN                      0xFFUL
#define TOKEN_TYPE_RUTOKEN_ECP                  0x01UL
#define TOKEN_TYPE_RUTOKEN_LITE                 0x02UL
#define TOKEN_TYPE_RUTOKEN                      0x03UL
#define TOKEN_TYPE_RUTOKEN_ECPDUAL_USB          0x09UL
#define TOKEN_TYPE_RUTOKEN_ECPDUAL_BT           0x69UL
#define TOKEN_TYPE_RUTOKEN_ECPDUAL_UART         0xA9UL
#define TOKEN_TYPE_RUTOKEN_ECP_SC               TOKEN_TYPE_RUTOKEN_SC_JC
#define TOKEN_TYPE_RUTOKEN_SCDUAL               0x49UL
#define TOKEN_TYPE_RUTOKEN_SCDUAL_NFC           0xC9UL
#define TOKEN_TYPE_RUTOKEN_LITE_SC_JC           0x42UL
#define TOKEN_TYPE_RUTOKEN_MIKRON               0x05UL
#define TOKEN_TYPE_RUTOKEN_MIKRON_SC            0x45UL
#define TOKEN_TYPE_RUTOKEN_MIKRON_SCDUAL        0x4DUL
#define TOKEN_TYPE_RUTOKEN_MIKRON_SCDUAL_NFC    0xCDUL
#define TOKEN_TYPE_RUTOKEN_ECP_NFC              0xC1UL
#define TOKEN_TYPE_RUTOKEN_SC_JC                0x41UL
#define TOKEN_TYPE_RUTOKEN_PINPAD_FAMILY        0x04UL
#define TOKEN_TYPE_RUTOKEN_WEB                  0x23UL
#define TOKEN_TYPE_RUTOKEN_ECP_SD               0x81UL
#define TOKEN_TYPE_RUTOKEN_LITE_SD              0x82UL

/* Token flags (field "flags" from CK_TOKEN_INFO_EXTENDED +
 * field "ChangeUserPINPolicy" from CK_RUTOKEN_INIT_PARAM) */
/* TOKEN_FLAGS_ADMIN_CHANGE_USER_PIN - if it is set, that
 * means that administrator (SO) can change user PIN */
#define TOKEN_FLAGS_ADMIN_CHANGE_USER_PIN         0x00000001UL

/* TOKEN_FLAGS_USER_CHANGE_USER_PIN - if it is set, that
 * means that user can change user PIN */
#define TOKEN_FLAGS_USER_CHANGE_USER_PIN          0x00000002UL

/* TOKEN_FLAGS_ADMIN_PIN_NOT_DEFAULT - if it is set, that
 * means that current administrator (SO) PIN is not default */
#define TOKEN_FLAGS_ADMIN_PIN_NOT_DEFAULT         0x00000004UL

/* TOKEN_FLAGS_USER_PIN_NOT_DEFAULT - if it is set, that
 * means that current user PIN not default */
#define TOKEN_FLAGS_USER_PIN_NOT_DEFAULT          0x00000008UL

/* TOKEN_FLAGS_SUPPORT_FKN - if it is set, that means that
 * token supports CryptoPro FKN */
#define TOKEN_FLAGS_SUPPORT_FKN                   0x00000010UL
#define TOKEN_FLAGS_SUPPORT_FKC                   TOKEN_FLAGS_SUPPORT_FKN

/* TOKEN_FLAGS_SUPPORT_SM - if it is set, that means that
 * token supports communication using Secure Messaging.
 * This flag is DEPRECATED, use
 * TOKEN_FLAGS_SUPPORT_SECURE_MESSAGING instead. */
#define TOKEN_FLAGS_SUPPORT_SM                    0x00000040UL

/* TOKEN_FLAGS_HAS_FLASH_DRIVE - if it is set, that means
 * that token has Flash Drive */
#define TOKEN_FLAGS_HAS_FLASH_DRIVE               0x00000080UL

/* TOKEN_FLAGS_CAN_CHANGE_SM_MODE - if it is set, that means
 * that token can be formatted to support SM. This flag
 * is DEPRECATED, use TOKEN_FLAGS_SUPPORT_SECURE_MESSAGING
 * instead. */
#define TOKEN_FLAGS_CAN_CHANGE_SM_MODE            0x00000100UL

/* TOKEN_FLAGS_SUPPORT_SECURE_MESSAGING - if it is set, that 
 * means that token supports Secure Messaging */
#define TOKEN_FLAGS_SUPPORT_SECURE_MESSAGING      0x00000100UL

/* TOKEN_FLAGS_HAS_BUTTON - if it is set, that means
 * that token is Rutoken Touch */
#define TOKEN_FLAGS_HAS_BUTTON                    0x00000200UL

/* TOKEN_FLAGS_SUPPORT_JOURNAL - if it is set, that means 
 * that token supports journal */
#define TOKEN_FLAGS_SUPPORT_JOURNAL               0x00000400UL

/* TOKEN_FLAGS_USER_PIN_UTF8 - if it is set, that means
 * that user PIN in UTF-8 */
#define TOKEN_FLAGS_USER_PIN_UTF8                 0x00000800UL

/* TOKEN_FLAGS_ADMIN_PIN_UTF8 - if it is set, that means
 * that admin PIN in UTF-8 */
#define TOKEN_FLAGS_ADMIN_PIN_UTF8                0x00001000UL

/* TOKEN_FLAGS_FW_CHECKSUM_UNAVAILIBLE - if it is set, that means Token does not support firmware 
 * checksum request and 'ulFirmwareChecksum' can not be used */
#define TOKEN_FLAGS_FW_CHECKSUM_UNAVAILIBLE       0x40000000UL

/* TOKEN_FLAGS_FW_CHECKSUM_INVALID - if it is set, that means that 'ulFirmwareChecksum' 
 * field contains different from the reference (stored at Token) firmware checksum */
#define TOKEN_FLAGS_FW_CHECKSUM_INVALID           0x80000000UL

/* Class of token */
#define TOKEN_CLASS_UNKNOWN             0xFFFFFFFFUL
#define TOKEN_CLASS_S                   0x00UL
#define TOKEN_CLASS_ECP                 0x01UL
#define TOKEN_CLASS_LITE                0x02UL
#define TOKEN_CLASS_ECP_BT              TOKEN_CLASS_ECPDUAL
/* DEPRECATED */
#define TOKEN_CLASS_WEB                 0x03UL
#define TOKEN_CLASS_PINPAD              0x04UL
#define TOKEN_CLASS_ECPDUAL             0x09UL

/* Body color of the token */
#define TOKEN_BODY_COLOR_UNKNOWN        0UL
#define TOKEN_BODY_COLOR_WHITE          1UL
#define TOKEN_BODY_COLOR_BLACK          2UL

/* Extended error codes */
#define CKR_CORRUPTED_MAPFILE           (CKR_VENDOR_DEFINED+1UL)
#define CKR_WRONG_VERSION_FIELD         (CKR_VENDOR_DEFINED+2UL)
#define CKR_WRONG_PKCS1_ENCODING        (CKR_VENDOR_DEFINED+3UL)
#define CKR_RTPKCS11_DATA_CORRUPTED     (CKR_VENDOR_DEFINED+4UL)
#define CKR_RTPKCS11_RSF_DATA_CORRUPTED (CKR_VENDOR_DEFINED+5UL)
#define CKR_SM_PASSWORD_INVALID         (CKR_VENDOR_DEFINED+6UL)
#define CKR_LICENSE_READ_ONLY           (CKR_VENDOR_DEFINED+7UL)
#define CKR_VENDOR_EMITENT_KEY_BLOCKED  (CKR_VENDOR_DEFINED+8UL)
#define CKR_CERT_CHAIN_NOT_VERIFIED     (CKR_VENDOR_DEFINED+9UL)
#define CKR_INAPPROPRIATE_PIN           (CKR_VENDOR_DEFINED+10UL)
#define CKR_PIN_IN_HISTORY              (CKR_VENDOR_DEFINED+11UL)

/* Rutoken ECP Flash */
typedef CK_ULONG CK_VOLUME_ID_EXTENDED;
typedef CK_ULONG CK_ACCESS_MODE_EXTENDED;
typedef CK_ULONG CK_OWNER_EXTENDED;

#define CKF_ENCRYPTION                  0x01000000UL  /* DEPRECATED */

#define ACCESS_MODE_HIDDEN              0x00UL
#define ACCESS_MODE_RO                  0x01UL
#define ACCESS_MODE_RW                  0x03UL
#define ACCESS_MODE_CD                  0x05UL

/* Data structure to be used in C_EX_SlotManage - extended function with MODE_GET_IMIT argument for st23 2.0 only */
typedef struct CK_TOKEN_IMIT_DATA {
  CK_BYTE     bMode;                    /* [in] 1 - mask, 2 - mask and patch */
  CK_BYTE     pbGostSymmetricKey[32];   /* [in] byte array with symmetric gost key */
  CK_BYTE     pbImit[8];                /* [out] byte array, will contain computed MAC*/
} CK_TOKEN_IMIT_DATA;

typedef CK_TOKEN_IMIT_DATA CK_PTR CK_TOKEN_IMIT_DATA_PTR;

/* C_EX_SlotManage mode */
#define MODE_GET_IMIT        0x04UL

/* Modes of computing of MAC */
#define IMIT_MODE_MASK       0x01U
#define IMIT_MODE_MASK_PATCH 0x02U

/* Data structure to be used in C_EX_SlotManage - extended function with MODE_GET_LOCAL_PIN_INFO argument */
typedef struct CK_LOCAL_PIN_INFO {
  CK_ULONG ulPinID;             /* [in] ID of local PIN */
  CK_ULONG ulMinSize;           /* [out] minimum PIN length */
  CK_ULONG ulMaxSize;           /* [out] maximum PIN length */
  CK_ULONG ulMaxRetryCount;     /* [out] max count of unsuccessful login attempts */
  CK_ULONG ulCurrentRetryCount; /* [out] count of unsuccessful attempts left,
                                   if field equals 0 - that means that PIN is blocked */
  CK_FLAGS flags;               /* [out] information flags  */
} CK_LOCAL_PIN_INFO;

typedef CK_LOCAL_PIN_INFO CK_PTR CK_LOCAL_PIN_INFO_PTR;

/* C_EX_SlotManage mode */
#define MODE_GET_LOCAL_PIN_INFO        0x05UL

/* Local PIN flags (field "flags" from CK_LOCAL_PIN_INFO) LOCAL_PIN_FLAGS_NOT_DEFAULT - 
 * if it is set, that means that local PIN is not default */
#define LOCAL_PIN_FLAGS_NOT_DEFAULT       0x00000001UL
/* LOCAL_PIN_FLAGS_FROM_SCREEN - if it is set, that means that PIN can be entered only from screen */
#define LOCAL_PIN_FLAGS_FROM_SCREEN       0x00000002UL
 /* LOCAL_PIN_FLAGS_IS_UTF8 - if it is set, that means that PIN is in UTF-8 */
#define LOCAL_PIN_FLAGS_IS_UTF8           0x00000004UL

typedef struct CK_VOLUME_INFO_EXTENDED
{
  CK_VOLUME_ID_EXTENDED   idVolume;     /* volume identifier, from 1 up to 9 */
  CK_ULONG                ulVolumeSize; /* size of the volume (MB), not more than flash-storage capacity */
  CK_ACCESS_MODE_EXTENDED accessMode;   /* access flags */
  CK_OWNER_EXTENDED       volumeOwner;  /* person, who can change access flags */
  CK_FLAGS                flags;        /* other flags (encryption only, for now) */
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

/* CK_VENDOR_PIN_PARAMS is a structure to be used in C_EX_TokenManage extended function to change default PIN */
typedef struct CK_VENDOR_PIN_PARAMS {
  CK_USER_TYPE      userType;       /* [in] user type */
  CK_UTF8CHAR_PTR   pPinValue;      /* [in] new default PIN*/
  CK_ULONG          ulPinLength;    /* [in] length of the new default PIN (in range from 6 to 32) */
} CK_VENDOR_PIN_PARAMS;

typedef CK_VENDOR_PIN_PARAMS CK_PTR CK_VENDOR_PIN_PARAMS_PTR;

#define MODE_SET_BLUETOOTH_POWEROFF_TIMEOUT     0x01UL
#define MODE_SET_CHANNEL_TYPE                   0x02UL
#define MODE_RESET_CUSTOM_PIN_TO_STANDARD       0x03UL
#define MODE_RESET_PIN_TO_DEFAULT               0x04UL
#define MODE_CHANGE_DEFAULT_PIN                 0x05UL
#define MODE_FORCE_USER_TO_CHANGE_PIN           0x06UL

#define BLUETOOTH_POWEROFF_TIMEOUT_DEFAULT      0x00
#define BLUETOOTH_POWEROFF_TIMEOUT_MAX          0x46

#define CHANNEL_TYPE_USB                        0x00UL
#define CHANNEL_TYPE_BLUETOOTH                  0x01UL

/* DEPRECATED function C_EX_GenerateActivationPassword */
#define GENERATE_NEXT_PASSWORD                  0x00UL    /* generate password with "next serial number" */

/* DEPRECATED function C_EX_GenerateActivationPassword character sets */
#define CAPS_AND_DIGITS                         0x00UL    /* capital letters of latin alphabet excluding O and digits excluding 0 */
#define CAPS_ONLY                               0x01UL    /* capital letters of latin alphabet */

/* Rutoken S vendor defined constants */

/* Key generation mechanism GOST */
#define CKM_GOST_KEY_GEN        (CKM_VENDOR_DEFINED+1UL)    /* DEPRECATED */

/* Encryption mechanism GOST */
#define CKM_GOST                (CKM_VENDOR_DEFINED+2UL)    /* DEPRECATED */

/* Trusted key material import mechanism */
#define CKM_VENDOR_SECURE_IMPORT                (CKM_VENDOR_DEFINED+3UL)

/* Key material derivation mechanism CRISP protocol */
#define CKM_VENDOR_KDF_CRISP_CMAC               (CKM_VENDOR_DEFINED+4UL)

/* Mechanism of simultaneous encryption and MAC calculation on twin CRISP key */
#define CKM_VENDOR_CRISP_AEAD             (CKM_VENDOR_DEFINED+5UL)

/* CK_KEY_TYPE of CKO_SECRET_KEY object*/
#define CKK_GOST                (CKK_VENDOR_DEFINED+1UL)    /* DEPRECATED */

/* Attributes of a GOST key */
#define CKA_GOST_KEY_OPTIONS    (CKA_VENDOR_DEFINED+1UL)    /* DEPRECATED */
#define CKA_GOST_KEY_FLAGS      (CKA_VENDOR_DEFINED+2UL)    /* DEPRECATED */


/* Possible options for a GOST key */
#define CKO_GOST_KEY_PZ         0x00UL    /* simple replacement */                 /* DEPRECATED */
#define CKO_GOST_KEY_GAMM       0x01UL    /* gamming up (default) */               /* DEPRECATED */
#define CKO_GOST_KEY_GAMMOS     0x02UL    /* gamming with reverse connection */    /* DEPRECATED */

/* Possible GOST key flags */
#define CKF_GOST_KEY_CLOSED     0x00UL    /* private key (default) */
#define CKF_GOST_KEY_OPENLEN    0x02UL    /* key with open length */

/* Data structures needed for CMS verification functions */

typedef struct CK_VENDOR_BUFFER {
  CK_BYTE_PTR   pData;
  CK_ULONG      ulSize;
} CK_VENDOR_BUFFER;

typedef CK_VENDOR_BUFFER CK_PTR CK_VENDOR_BUFFER_PTR;
typedef CK_VENDOR_BUFFER_PTR CK_PTR CK_VENDOR_BUFFER_PTR_PTR;

typedef CK_ULONG CK_VENDOR_CRL_MODE;

typedef struct CK_VENDOR_X509_STORE {
  CK_VENDOR_BUFFER_PTR  pTrustedCertificates;
  CK_ULONG              ulTrustedCertificateCount;
  CK_VENDOR_BUFFER_PTR  pCertificates;
  CK_ULONG              ulCertificateCount;
  CK_VENDOR_BUFFER_PTR  pCrls;
  CK_ULONG              ulCrlCount;
} CK_VENDOR_X509_STORE;

typedef CK_VENDOR_X509_STORE CK_PTR CK_VENDOR_X509_STORE_PTR;

typedef CK_BYTE_PTR CK_PTR CK_BYTE_PTR_PTR;

#define CRISP_CS_1 1U
#define CRISP_CS_2 2U
#define CRISP_CS_3 3U
#define CRISP_CS_4 4U

/* CK_VENDOR_KDF_CRISP_CMAC_PARAMS provides the parameters to the CKM_VENDOR_KDF_CRISP_CMAC mechanism */
typedef struct CK_VENDOR_KDF_CRISP_CMAC_PARAMS {
  CK_ULONG       ulSeqNumLen;              /* length of sequence number in bytes */
  CK_BYTE_PTR    pSeqNum;                  /* byte string representation of sequence number (big-endian) */
  CK_ULONG       ulSourceIdentifierLen;    /* length of source identifier in bytes */
  CK_BYTE_PTR    pSourceIdentifier;        /* source identifier */
  CK_BYTE        cs;                       /* CRISP cryptographic set identifier */
} CK_VENDOR_KDF_CRISP_CMAC_PARAMS;

typedef CK_VENDOR_KDF_CRISP_CMAC_PARAMS CK_PTR CK_VENDOR_KDF_CRISP_CMAC_PARAMS_PTR;

/* CK_VENDOR_CRISP_AEAD_PARAMS provides the parameters to the CKM_VENDOR_CRISP_AEAD mechanism */
typedef struct CK_VENDOR_CRISP_AEAD_PARAMS{
    CK_MECHANISM_PTR    pEncryptMechanism;    /* data encryption mechanism */
    CK_BYTE_PTR         pAAD;                 /* byte array containing additional authenticated data */
    CK_ULONG            ulAADLen;             /* length of the pAAD field in bytes */
    CK_ULONG            ulMACLen;             /* length of integrity check value in bytes */
} CK_VENDOR_CRISP_AEAD_PARAMS;

typedef CK_VENDOR_CRISP_AEAD_PARAMS CK_PTR CK_VENDOR_CRISP_AEAD_PARAMS_PTR;

/* Flags for customizing certificate verification process, should be passed
 * in flags argument to C_EX_PKCS7VerifyInit */

/* Do not search signer certificates in certificates attached to CMS */
#define CKF_VENDOR_DO_NOT_USE_INTERNAL_CMS_CERTS 0x01UL

/* If this flag is set - chain verification will be successful
 * if at least one CA certificate from chain is in trusted certificates list */
#define CKF_VENDOR_ALLOW_PARTIAL_CHAINS 0x02UL

/* If this flag is set - certificate verification is not made
 * this flag is not compatible with CKF_VENDOR_DO_NOT_USE_INTERNAL_CMS_CERTS,
 * CKF_VENDOR_ALLOW_PARTIAL_CHAINS and CKF_VENDOR_USE_TRUSTED_CERTS_FROM_TOKEN
 * ckMode should be set to OPTIONAL_CRL_CHECK */
#define CKF_VENDOR_CHECK_SIGNATURE_ONLY 0x04UL

/* If this flag is set - trusted certificate store will be used to build a certificate chain. 
 * ulTrustedCertificateCount in certificate store should be set to 0 */
#define CKF_VENDOR_USE_TRUSTED_CERTS_FROM_TOKEN 0x08UL

/* CRL checking modes, should be passed in ckMode to C_EX_PKCS7VerifyInit */

/* If we have no suitable CRL - it won't be an error */
#define OPTIONAL_CRL_CHECK 0UL

/* Signer's CA CRL should be passed */
#define LEAF_CRL_CHECK 1UL

/* CRLs of all CA from the chain should be passed */
#define ALL_CRL_CHECK 2UL

/* Interfaces */
#define INTERFACE_TYPE_USB           0x00000001UL
#define INTERFACE_TYPE_BT            0x00000002UL
#define INTERFACE_TYPE_UART          0x00000004UL
#define INTERFACE_TYPE_ISO           0x00000008UL
#define INTERFACE_TYPE_NFC_TYPE_B    0x00000020UL
#define INTERFACE_TYPE_SPI           0x00000040UL
#define INTERFACE_TYPE_NFC_TYPE_A    0x00000080UL
#define INTERFACE_TYPE_UNKNOWN       CK_UNAVAILABLE_INFORMATION
/* DEPRECATED */
#define INTERFACE_TYPE_SD     0x00000010UL
#define INTERFACE_TYPE_NFC    INTERFACE_TYPE_NFC_TYPE_B

/* Secure messaging types */
#define SECURE_MESSAGING_DEFAULT          0x00UL
#define SECURE_MESSAGING_BUILT_IN         0x01UL
#define SECURE_MESSAGING_GOST             0x02UL
#define SECURE_MESSAGING_ENHANCED_GOST    0x03UL
#define SECURE_MESSAGING_UNSUPPORTED      0xffUL

/* Values for CKA_VENDOR_PIN_POLICY_STATE */
#define PIN_POLICY_STATE_WELL_DEFINED                         0x00UL
#define PIN_POLICY_STATE_HAS_UNKNOWN_ATTRIBUTES               0x01UL
#define PIN_POLICY_STATE_HAS_UNKNOWN_NONDEFAULT_ATTRIBUTES    0x02UL

/* Biometric types */
#define BIOMETRIC_AUTHENTICATION_NOT_SUPPORTED    0x00000000UL

/* Hardware feature types */
#define CKH_VENDOR_TOKEN_INFO        (CKH_VENDOR_DEFINED + 0x01UL)
#define CKH_VENDOR_EMITENT_KEY       (CKH_VENDOR_DEFINED + 0x02UL)
#define CKH_VENDOR_SECURE_COUNTER    (CKH_VENDOR_DEFINED + 0x03UL)
#define CKH_VENDOR_NDEF_TAG          (CKH_VENDOR_DEFINED + 0x04UL)
#define CKH_VENDOR_RNG               (CKH_VENDOR_DEFINED + 0x05UL)
#define CKH_VENDOR_PIN_POLICY        (CKH_VENDOR_DEFINED + 0x06UL)

#endif /* __RTPKCS11T_H__ */
