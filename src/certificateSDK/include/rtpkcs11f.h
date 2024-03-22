/*************************************************************************************
* Copyright (C) Aktiv Co. 2003-2023                                                  *
* rtpkcs11f.h                                                                        *
* This file contains additional functions for work with Rutoken in PKCS#11 library   *
*************************************************************************************/

/* C_EX_GetFunctionListExtended returns the extended function list */
CK_PKCS11_FUNCTION_INFO(C_EX_GetFunctionListExtended)
#ifdef CK_NEED_ARG_LIST
(
  CK_FUNCTION_LIST_EXTENDED_PTR_PTR    ppFunctionList    /* [out] pointer to pointer to extended function list */
);
#endif


/* C_EX_InitToken initializes a token with full format */
CK_PKCS11_FUNCTION_INFO(C_EX_InitToken)
#ifdef CK_NEED_ARG_LIST
(
  CK_SLOT_ID                   slotID,      /* [in] ID of the token's slot */
  CK_UTF8CHAR_PTR              pPin,        /* [in] SO's initial PIN */
  CK_ULONG                     ulPinLen,    /* [in] length in bytes of the PIN */
  CK_RUTOKEN_INIT_PARAM_PTR    pInitInfo    /* [in] init parameters */
);
#endif


/* C_EX_GetTokenInfoExtended obtains information about a particular token in the system */
CK_PKCS11_FUNCTION_INFO(C_EX_GetTokenInfoExtended)
#ifdef CK_NEED_ARG_LIST
(
  CK_SLOT_ID                    slotID,    /* [in] ID of the token's slot */
  CK_TOKEN_INFO_EXTENDED_PTR    pInfo      /* [out] pointer to a token information */
);
#endif


/* C_EX_UnblockUserPIN unblock the blocked user's PIN.
 * Requires same conditions as a C_InitPIN. */
CK_PKCS11_FUNCTION_INFO(C_EX_UnblockUserPIN)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE    hSession    /* [in] session's handle */
);
#endif


/* C_EX_SetTokenName modifies the token symbol name (label) if User is logged in.
 * Can only be called in the "R/W User Functions" state. */
CK_PKCS11_FUNCTION_INFO(C_EX_SetTokenName)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE    hSession,     /* [in] session's handle */
  CK_CHAR_PTR          pLabel,       /* [in] new label */
  CK_ULONG             ulLabelLen    /* [in] length of the new label */
);
#endif


/* C_EX_SetLicense modifies the token license if User or SO is logged in.
 * Can only be called in the "R/W User Functions" state or "R/W SO Functions" state. */
CK_PKCS11_FUNCTION_INFO(C_EX_SetLicense)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE    hSession,        /* [in] session's handle */
  CK_ULONG             ulLicenseNum,    /* [in] number of the new license, can only be 1 or 2 */
  CK_BYTE_PTR          pLicense,        /* [in] byte buffer with the data of new license */
  CK_ULONG             ulLicenseLen     /* [in] length of the new license, can only be 72 */
);
#endif


/* C_EX_GetLicense read the token license. Can be called in the every state. */
CK_PKCS11_FUNCTION_INFO(C_EX_GetLicense)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE    hSession,        /* [in] session's handle */
  CK_ULONG             ulLicenseNum,    /* [in] number of the license, can only be 1 or 2 */
  CK_BYTE_PTR          pLicense,        /* [out] byte buffer for data of license, length of the buffer
                                           can only be not less then license length */
  CK_ULONG_PTR         pulLicenseLen    /* [in/out] license length in bytes, can only be 72 */
);
#endif


/* C_EX_GetCertificateInfoText gets text information about certificate.
 * Can be called in the every state. */
CK_PKCS11_FUNCTION_INFO(C_EX_GetCertificateInfoText)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE    hSession,     /* [in] session's handle */
  CK_OBJECT_HANDLE     hCert,        /* [in] object's (certificate) handle */
  CK_CHAR_PTR*         pInfo,        /* [out] pointer to the address of the allocated buffer with text information */
  CK_ULONG_PTR         pulInfoLen    /* [out] poin[inter to the size of the allocated buffer */
);
#endif


/* C_EX_PKCS7Sign sign data and pack it to PKCS#7 format certificate.
 * Can only be called in the "R/W User Functions" or "R User Functions" state. */
CK_PKCS11_FUNCTION_INFO(C_EX_PKCS7Sign)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE       hSession,             /* [in] session's handle */
  CK_BYTE_PTR             pData,                /* [in] pointer to CK_BYTE array, that contains data for signing */
  CK_ULONG                ulDataLen,            /* [in] length of data for signing */
  CK_OBJECT_HANDLE        hCert,                /* [in] certificate's handle */
  CK_BYTE_PTR             *ppEnvelope,          /* [out] pointer to a byte array, that contains message */
  CK_ULONG_PTR            pEnvelopeLen,         /* [out] pointer to a length of calculated message */
  CK_OBJECT_HANDLE        hPrivKey,             /* [in] handle of the corresponded to certificate private key. 
                                                   If equals 0, then private key will be searched via it's ID from the certificate */
  CK_OBJECT_HANDLE_PTR    phCertificates,       /* [in] pointer to a certificate's chain (array) */
  CK_ULONG                ulCertificatesLen,    /* [in] count of phCertificates elements */
  CK_ULONG                flags                 /* [in] hash mode flag. Possible values:
                                                       0 - software hashing. Original data (pData) will be saved with signed message 
                                                       PKCS7_DETACHED_SIGNATURE - software hashing. Original data (pData) won't be saved with signed message.
                                                       USE_HARDWARE_HASH - hardware hashing */
);
#endif


/* C_EX_CreateCSR create request by certificate and pack it in PKCS#10 format.
 * Can only be called in the "R/W User Functions" or "R User Functions" state. */
CK_PKCS11_FUNCTION_INFO(C_EX_CreateCSR)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE    hSession,             /* [in] session's handle */
  CK_OBJECT_HANDLE     hPublicKey,           /* [in] handle of the public key, that is used for generating certificate */
  CK_CHAR_PTR          *dn,                  /* [in] distinctive name - an array of strings; 
                                                1st string represents a type of field in text format or object identifier(CN or "2.5.3.3") 
                                                2nd string contains field value encoded in UTF-8.
                                                Following fields are transfered in same way. Count of strings is even.*/
  CK_ULONG             dnLength,             /* [in] count of strings in an array, to which dn paramter points to. */
  CK_BYTE_PTR          *pCsr,                /* [in] pointer to a pointer to a byte array, in which generated certificate request will be placed. */
  CK_ULONG_PTR         pulCsrLength,         /* [in] pointer to length of generated request */
  CK_OBJECT_HANDLE     hPrivKey,             /* [in] handle of the corresponding to hPublicKey private key. 
                                                If equals 0, then private key will be searched via ID of the public key.*/
  CK_CHAR_PTR          *pAttributes,         /* [in] additional attributes to be included in request. Format is similar to dn parameter. */
  CK_ULONG             ulAttributesLength,   /* [in] count of attributes in pAttributes parameter */
  CK_CHAR_PTR          *pExtensions,         /* [in] extensions to be included in request. Format is similar to dn parameter. */
  CK_ULONG             ulExtensionsLength    /* [in] count of extensions in pExtensions parameter*/
);
#endif


/* C_EX_FreeBuffer free buffer, allocated in extended functions */
CK_PKCS11_FUNCTION_INFO(C_EX_FreeBuffer)
#ifdef CK_NEED_ARG_LIST
(
  CK_BYTE_PTR    pBuffer    /* [in] pointer to a buffer */
);
#endif


/* C_EX_GetTokenName return the token symbol name (label) */
CK_PKCS11_FUNCTION_INFO(C_EX_GetTokenName)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE    hSession,      /* [in] session's handle */
  CK_CHAR_PTR          pLabel,        /* [out] byte buffer for label */
  CK_ULONG_PTR         pulLabelLen    /* [in/out] pointer to the buffer length in bytes */
);
#endif


/* C_EX_SetLocalPIN modifies the local PIN for devices witch supported it */
CK_PKCS11_FUNCTION_INFO(C_EX_SetLocalPIN)
#ifdef CK_NEED_ARG_LIST
(
  CK_SLOT_ID         slotID,              /* [in] ID of the token's slot */
  CK_UTF8CHAR_PTR    pUserPin,            /* [in] current User PIN */
  CK_ULONG           ulUserPinLen,        /* [in] length of current User PIN */
  CK_UTF8CHAR_PTR    pNewLocalPin,        /* [in] new local PIN */
  CK_ULONG           ulNewLocalPinLen,    /* [in] length of the new local PIN */
  CK_ULONG           ulLocalID            /* [in] ID of the local PIN */
);
#endif


/* DEPRECATED */
CK_PKCS11_FUNCTION_INFO(C_EX_LoadActivationKey)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE    hSession,
  CK_BYTE_PTR          key,
  CK_ULONG             keySize
);
#endif


/* DEPRECATED */
CK_PKCS11_FUNCTION_INFO(C_EX_SetActivationPassword)
#ifdef CK_NEED_ARG_LIST
(
  CK_SLOT_ID         slotID,
  CK_UTF8CHAR_PTR    password
);
#endif


CK_PKCS11_FUNCTION_INFO(C_EX_GetVolumesInfo)
#ifdef CK_NEED_ARG_LIST
(
  CK_SLOT_ID                     slotID,         /* [in] ID of the token's slot */
  CK_VOLUME_INFO_EXTENDED_PTR    pInfo,          /* [out] pointer to an array of CK_VOLUME_INFO_EXTENDED structures that contain info about volumes: 
                                                    idVolume - identificator of a volume. Possible values: from 1 to 9 
                                                    ulVolumeSize - volume size in Mb (not bigger then storage size) 
                                                    accessMode - access to volume flags. Possible values:
                                                        ACCESS_MODE_RW - Both reading and writing 
                                                        ACCESS_MODE_RO - Reading only
                                                        ACCESS_MODE_HIDDEN - hidden volume, protected from viewing in OS, reading, writing and etc
                                                        ACCESS_MODE_CD - volume that emulates CD-ROM
                                                    volumeOwner - volume owner, that has rights for volume access flag changes. Possible values:
                                                        CKU_USER - global user
                                                        CKU_SO - global admin
                                                        CKU_LOCAL_X - X is a number from 1 to 8 - local user X 
                                                    flags - other flags */
  CK_ULONG_PTR                   pulInfoCount    /* [out] pointer to array size */
);
#endif


CK_PKCS11_FUNCTION_INFO(C_EX_GetDriveSize)
#ifdef CK_NEED_ARG_LIST
(
  CK_SLOT_ID      slotID,         /* [in] ID of the token's slot */
  CK_ULONG_PTR    pulDriveSize    /* [out] flash-storage capacity (MB) */
);
#endif


CK_PKCS11_FUNCTION_INFO(C_EX_ChangeVolumeAttributes)
#ifdef CK_NEED_ARG_LIST
(
  CK_SLOT_ID                 slotID,           /* [in] ID of the token's slot */
  CK_USER_TYPE               userType,         /* [in] user type */
  CK_UTF8CHAR_PTR            pPin,             /* [in] pointer to user's PIN */
  CK_ULONG                   ulPinLen,         /* [in] length of the PIN */
  CK_VOLUME_ID_EXTENDED      idVolume,         /* [in] volume identifier */
  CK_ACCESS_MODE_EXTENDED    newAccessMode,    /* [in] new access flags */
  CK_BBOOL                   bPermanent        /* [in] flag of mode of attribute changes. Possible values:
                                                      CK_TRUE - permament change of access attribute, valid until next atributes change 
                                                      CK_FALSE - temporary change of access attribute, valid until first device disconnect from USB-port, previous access mode will be set after */
);
#endif


CK_PKCS11_FUNCTION_INFO(C_EX_FormatDrive)
#ifdef CK_NEED_ARG_LIST
(
  CK_SLOT_ID                            slotID,              /* [in] ID of the token's slot */
  CK_USER_TYPE                          userType,            /* [in] user type */
  CK_UTF8CHAR_PTR                       pPin,                /* [in] pointer to user's PIN */
  CK_ULONG                              ulPinLen,            /* [in] length of the PIN */
  CK_VOLUME_FORMAT_INFO_EXTENDED_PTR    pInitParams,         /* [in] pointer to an array of CK_VOLUME_FORMAT_INFO_EXTENDED structures, that contain detailed info about volumes:
                                                                ulVolumeSize - volume size in Mb
                                                                accessMode - access to volume flags. Possible values:
                                                                    ACCESS_MODE_RW - Both reading and writing 
                                                                    ACCESS_MODE_RO - Reading only
                                                                    ACCESS_MODE_HIDDEN - hidden volume, protected from viewing in OS, reading, writing and etc
                                                                    ACCESS_MODE_CD - volume that emulates CD-ROM 
                                                                volumeOwner - volume owner, that has rights for volume access flag changes. Possible values:
                                                                    CKU_USER - global user
                                                                    CKU_SO - global admin
                                                                    CKU_LOCAL_X - X is a number from 1 to 8 - local user X 
                                                                flags - other flags */
  CK_ULONG                              ulInitParamsCount    /* [in] count of elements in an array */
);
#endif


CK_PKCS11_FUNCTION_INFO(C_EX_TokenManage)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE    hSession,    /* [in] session's handle */
  CK_ULONG             ulMode,      /* [in] function's working mode. Possible values:
                                           MODE_SET_BLUETOOTH_POWEROFF_TIMEOUT - set a timeout for wireless connection channel
                                           MODE_SET_CHANNEL_TYPE - set the type of used connection channel */
  CK_VOID_PTR          pValue       /* [in] if ulMode is MODE_SET_BLUETOOTH_POWEROFF_TIMEOUT :
                                           1 .. 0x46 - timeout value, from 1 to 70 minutes 
                                           BLUETOOTH_POWEROFF_TIMEOUT_DEFAULT - default timeout
                                           BLUETOOTH_POWEROFF_TIMEOUT_MAX - maximum value (70 minutes) 
                                       if ulMode is MODE_SET_CHANNEL_TYPE:
                                           CHANNEL_TYPE_USB - through USB bus
                                           CHANNEL_TYPE_BLUETOOTH  - through Bluetooth */
);
#endif


/* DEPRECATED */
CK_PKCS11_FUNCTION_INFO(C_EX_GenerateActivationPassword)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE    hSession,
  CK_ULONG             ulPasswordNumber,
  CK_UTF8CHAR_PTR      pPassword,
  CK_ULONG_PTR         pulPasswordSize,
  CK_ULONG             ulPasswordCharacterSet
);
#endif


CK_PKCS11_FUNCTION_INFO(C_EX_GetJournal)
#ifdef CK_NEED_ARG_LIST
(
  CK_SLOT_ID      slotID,
  CK_BYTE_PTR     pJournal,
  CK_ULONG_PTR    pulJournalSize
);
#endif


/* DEPRECATED */
CK_PKCS11_FUNCTION_INFO(C_EX_SignInvisibleInit)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE    hSession,      /* session's handle */
  CK_MECHANISM_PTR     pMechanism,    /* signature mechanism */
  CK_OBJECT_HANDLE     hKey           /* handle of signature key */
);
#endif


/* DEPRECATED */
CK_PKCS11_FUNCTION_INFO(C_EX_SignInvisible)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE    hSession,
  CK_BYTE_PTR          pData,
  CK_ULONG             ulDataLen,
  CK_BYTE_PTR          pSignature,
  CK_ULONG_PTR         pulSignatureLen
);
#endif


CK_PKCS11_FUNCTION_INFO(C_EX_SlotManage)
#ifdef CK_NEED_ARG_LIST
(
  CK_SLOT_ID     slotID,    /* [in] ID of the token's slot */
  CK_ULONG       ulMode,    /* [in] function's working mode. Possible values:
                                   MODE_GET_LOCAL_PIN_INFO - get info about local PIN codes
                                   MODE_GET_PIN_SET_TO_BE_CHANGED - get info about forced PIN change */
  CK_VOID_PTR    pValue     /* [out] pointer to an information about
                               1) local PIN codes (array of a CK_LOCAL_PIN_INFO structures), if ulMode is set to MODE_GET_LOCAL_PIN_INFO:
                               2) user type (CKO_USER/CKO_SO), for whom info about forced PIN changed is requested */
);
#endif


/* DEPRECATED */
CK_PKCS11_FUNCTION_INFO(C_EX_WrapKey)
#ifdef CK_NEED_ARG_LIST
(
 CK_SESSION_HANDLE       hSession,
 CK_MECHANISM_PTR        pGenerationMechanism,
 CK_ATTRIBUTE_PTR        pKeyTemplate,
 CK_ULONG                ulKeyAttributeCount,
 CK_MECHANISM_PTR        pDerivationMechanism,
 CK_OBJECT_HANDLE        hBaseKey,
 CK_MECHANISM_PTR        pWrappingMechanism,
 CK_BYTE_PTR             pWrappedKey,
 CK_ULONG_PTR            pulWrappedKeyLen,
 CK_OBJECT_HANDLE_PTR    phKey
);
#endif


/* DEPRECATED */
CK_PKCS11_FUNCTION_INFO(C_EX_UnwrapKey)
#ifdef CK_NEED_ARG_LIST
(
 CK_SESSION_HANDLE       hSession,
 CK_MECHANISM_PTR        pDerivationMechanism,
 CK_OBJECT_HANDLE        hBaseKey,
 CK_MECHANISM_PTR        pUnwrappingMechanism,
 CK_BYTE_PTR             pWrappedKey,
 CK_ULONG                ulWrappedKeyLen,
 CK_ATTRIBUTE_PTR        pKeyTemplate,
 CK_ULONG                ulKeyAttributeCount,
 CK_OBJECT_HANDLE_PTR    phKey
);
#endif


CK_PKCS11_FUNCTION_INFO(C_EX_PKCS7VerifyInit)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE           hSession,     /* [in] session's handle*/
  CK_BYTE_PTR                 pCms,         /* [in] pointer to a byte array, that contains PKCS#7 formatted message for verification */
  CK_ULONG                    ulCmsSize,    /* [in] message length */
  CK_VENDOR_X509_STORE_PTR    pStore,       /* [in] pointer to a CK_VENDOR_X509_STORE structure, which contains pointers to trusted certificates, which are needed for signature veirification, certificates of signing side and revoked certificates */
  CK_VENDOR_CRL_MODE          ckMode,       /* [in] revoked certificates list checking policy. Possible values:
                                                   OPTIONAL_CRL_CHECK - lack of the list doesn't cause an error
                                                   LEAF_CRL_CHECK - list is required for signing side certificate verification
                                                   ALL_CRL_CHECK - list is required for verification of each certificate from the chain */
  CK_FLAGS                    flags         /* [in] signature verification options. Possible values:
                                                   CKF_VENDOR_DO_NOT_USE_INTERNAL_CMS_CERTS - don't use certificates from CMS message for certificate search of signing side (certificates must be set in CK_VENDOR_X509_STORE);
                                                   CKF_VENDOR_ALLOW_PARTIAL_CHAINS - at least 1 certificate of CA from signing side certificates chain should be trusted for signature verification */
);
#endif


CK_PKCS11_FUNCTION_INFO(C_EX_PKCS7Verify)
#ifdef CK_NEED_ARG_LIST
(
 CK_SESSION_HANDLE           hSession,                     /* [in] session's handle */
 CK_BYTE_PTR_PTR             ppData,                       /* [out] pointer to a byte array containing data, which was signed */
 CK_ULONG_PTR                pulDataSize,                  /* [out] pointer to the data size */
 CK_VENDOR_BUFFER_PTR_PTR    ppSignerCertificates,         /* [out] pointer to an array, that contains certificates of signing side */
 CK_ULONG_PTR                pulSignerCertificatesCount    /* [out] pointer to the count of certificates */
);
#endif


CK_PKCS11_FUNCTION_INFO(C_EX_PKCS7VerifyUpdate)
#ifdef CK_NEED_ARG_LIST
(
 CK_SESSION_HANDLE    hSession,     /* [in] session's handle */
 CK_BYTE_PTR          pData,        /* [in] byte array containing data that was signed */
 CK_ULONG             ulDataSize    /* [in] data size */
);
#endif


CK_PKCS11_FUNCTION_INFO(C_EX_PKCS7VerifyFinal)
#ifdef CK_NEED_ARG_LIST
(
 CK_SESSION_HANDLE           hSession,                     /* [in] session's handle */
 CK_VENDOR_BUFFER_PTR_PTR    ppSignerCertificates,         /* [out] pointer to an array, that contains certificates of signing side*/
 CK_ULONG_PTR                pulSignerCertificatesCount    /* [out] point to a count of certificates */
);
#endif
