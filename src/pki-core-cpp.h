#ifndef RT_PKI_CORE_CPP_H
#define RT_PKI_CORE_CPP_H

#include <cassert>
#include <cstdint>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#ifndef RT_PKI_CORE_H
#define RT_PKI_CORE_H

#include <stddef.h>
#include <stdint.h>

#ifndef RT_PC_INTERNAL_ERRORCODE_H
#define RT_PC_INTERNAL_ERRORCODE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
#define ERROR_DECLARATION(DUMMY1, NAME, VALUE, DUMMY2) \
	rt_pc_e_ ## NAME = VALUE,
ERROR_DECLARATION(BadParams, bad_params, 2, "Bad parameters")
ERROR_DECLARATION(DeviceNotFound, device_not_found, 3, "No device has been found")
ERROR_DECLARATION(WrongCertificateCategory, certificate_category_bad, 5, "Wrong certificate category")
ERROR_DECLARATION(CertificateExists, certificate_exists, 6, "Certificate already exists")
ERROR_DECLARATION(PinChanged, pin_changed, 8, "PIN has been changed unexpectedly")
ERROR_DECLARATION(InvalidSession, session_invalid, 9, "Session state been changed unexpectedly")

ERROR_DECLARATION(KeyLabelNotUnique, key_label_not_unique, 10, "Key label is not unique")
ERROR_DECLARATION(InternalError, internal_error, 11, "Internal error")
ERROR_DECLARATION(NoCaCertificatesFound, ca_certificates_not_found, 12, "No CA certificates found")
ERROR_DECLARATION(CertificateVerification, certificate_verification_error, 13, "Certificate verification failed")
ERROR_DECLARATION(Pkcs11ModuleLoad, pkcs11_load_module_error, 14, "Failed to load pkcs#11 module")
ERROR_DECLARATION(NotEnoughMemory, not_enough_memory, 15, "Not enough memory")
ERROR_DECLARATION(InvalidPinLength, pin_length_invalid, 16, "PIN length is invalid")
ERROR_DECLARATION(IncorrectPin, pin_incorrect, 17, "PIN is incorrect")
ERROR_DECLARATION(PinLocked, pin_locked, 18, "PIN is locked")
ERROR_DECLARATION(NotLoggedIn, user_not_logged_in, 19, "User login required to perform an operation")

ERROR_DECLARATION(KeyNotFound, key_not_found, 20, "No key has been found")
ERROR_DECLARATION(KeyIdNotUnique, key_id_not_unique, 21, "Key id is not unique")
ERROR_DECLARATION(CertificateNotFound, certificate_not_found, 22, "No certificate has been found")
ERROR_DECLARATION(CertificateHashNotUnique, certificate_hash_not_unique, 23, "Certificate hash is not unique")
ERROR_DECLARATION(InvalidToken, token_invalid, 24, "Token state has been changed unexpectedly")
ERROR_DECLARATION(WrongKeyType, wrong_key_type, 25, "Key type is not supported in operation")
ERROR_DECLARATION(DeviceError, device_error, 26, "Token failed to perform operation")
ERROR_DECLARATION(InvalidData, data_invalid, 27, "Data provided is invalid")
ERROR_DECLARATION(UnsupportedByToken, unsupported_by_token, 28, "Unsupported by token")
ERROR_DECLARATION(KeyFunctionNotPermitted, key_function_not_permitted, 29, "Key is not permitted to be used in operation")

ERROR_DECLARATION(FunctionRejected, function_rejected, 30, "Function processing rejected by user")
ERROR_DECLARATION(FunctionFailed, function_failed, 31, "Function failed")
ERROR_DECLARATION(Base64Decode, base64_decode_error, 32, "Error while decoding base64")
ERROR_DECLARATION(Asn1Error, asn1_err, 33, "Something wrong happened when processing ASN1 structures")
ERROR_DECLARATION(PemError, pem_err, 34, "Something wrong happened when processing PEM-encoded data")
ERROR_DECLARATION(Base64Encode, base64_encode_error, 35, "Error while encoding base64")
ERROR_DECLARATION(X509UnableToGetIssuerCert, x509_unable_to_get_issuer_cert, 36, "unable to get issuer certificate")
ERROR_DECLARATION(X509UnableToGetCrl, x509_unable_to_get_crl, 37, "unable to get certificate CRL")
ERROR_DECLARATION(X509UnableToDecryptCertSignature, x509_unable_to_decrypt_cert_signature, 38, "unable to decrypt certificate's signature")
ERROR_DECLARATION(X509UnableToDecryptCrlSignature, x509_unable_to_decrypt_crl_signature, 39, "unable to decrypt CRL's signature")
ERROR_DECLARATION(X509UnableToDecodeIssuerPublicKey, x509_unable_to_decode_issuer_public_key, 40, "unable to decode issuer public key")
ERROR_DECLARATION(X509CertSignatureFailure, x509_cert_signature_failure, 41, "certificate signature failure")
ERROR_DECLARATION(X509CrlSignatureFailure, x509_crl_signature_failure, 42, "CRL signature failure")
ERROR_DECLARATION(X509CertNotYetValid, x509_cert_not_yet_valid, 43, "certificate is not yet valid")
ERROR_DECLARATION(X509CrlNotYetValid, x509_crl_not_yet_valid, 44, "CRL is not yet valid")
ERROR_DECLARATION(X509CertHasExpired, x509_cert_has_expired, 45, "certificate has expired")
ERROR_DECLARATION(X509CrlHasExpired, x509_crl_has_expired, 46, "CRL has expired")
ERROR_DECLARATION(X509ErrorInCertNotBeforeField, x509_error_in_cert_not_before_field, 47, "format error in certificate's notBefore field")
ERROR_DECLARATION(X509ErrorInCertNotAfterField, x509_error_in_cert_not_after_field, 48, "format error in certificate's notAfter field")
ERROR_DECLARATION(X509ErrorInCrlLastUpdateField, x509_error_in_crl_last_update_field, 49, "format error in CRL's lastUpdate field")
ERROR_DECLARATION(X509ErrorInCrlNextUpdateField, x509_error_in_crl_next_update_field, 50, "format error in CRL's nextUpdate field")
ERROR_DECLARATION(X509OutOfMem, x509_out_of_mem, 51, "out of memory")
ERROR_DECLARATION(X509DepthZeroSelfSignedCert, x509_depth_zero_self_signed_cert, 52, "self signed certificate")
ERROR_DECLARATION(X509SelfSignedCertInChain, x509_self_signed_cert_in_chain, 53, "self signed certificate in certificate chain")
ERROR_DECLARATION(X509UnableToGetIssuerCertLocally, x509_unable_to_get_issuer_cert_locally, 54, "unable to get local issuer certificate")
ERROR_DECLARATION(X509UnableToVerifyLeafSignature, x509_unable_to_verify_leaf_signature, 55, "unable to verify the first certificate")
ERROR_DECLARATION(X509CertChainTooLong, x509_cert_chain_too_long, 56, "certificate chain too long")
ERROR_DECLARATION(X509CertRevoked, x509_cert_revoked, 57, "certificate revoked")
ERROR_DECLARATION(X509InvalidCa, x509_invalid_ca, 58, "invalid CA certificate")
ERROR_DECLARATION(X509InvalidNonCa, x509_invalid_non_ca, 59, "invalid non-CA certificate (has CA markings)")
ERROR_DECLARATION(X509PathLengthExceeded, x509_path_length_exceeded, 60, "path length constraint exceeded")
ERROR_DECLARATION(X509ProxyPathLengthExceeded, x509_proxy_path_length_exceeded, 61, "proxy path length constraint exceeded")
ERROR_DECLARATION(X509ProxyCertificatesNotAllowed, x509_proxy_certificates_not_allowed, 62, "proxy certificates not allowed, please set the appropriate flag")
ERROR_DECLARATION(X509InvalidPurpose, x509_invalid_purpose, 63, "unsupported certificate purpose")
ERROR_DECLARATION(X509CertUntrusted, x509_cert_untrusted, 64, "certificate not trusted")
ERROR_DECLARATION(X509CertRejected, x509_cert_rejected, 65, "certificate rejected")
ERROR_DECLARATION(X509ApplicationVerification, x509_application_verification, 66, "application verification failure")
ERROR_DECLARATION(X509SubjectIssuerMismatch, x509_subject_issuer_mismatch, 67, "subject issuer mismatch")
ERROR_DECLARATION(X509AkidSkidMismatch, x509_akid_skid_mismatch, 68, "authority and subject key identifier mismatch")
ERROR_DECLARATION(X509AkidIssuerSerialMismatch, x509_akid_issuer_serial_mismatch, 69, "authority and issuer serial number mismatch")
ERROR_DECLARATION(X509KeyusageNoCertsign, x509_keyusage_no_certsign, 70, "key usage does not include certificate signing")
ERROR_DECLARATION(X509UnableToGetCrlIssuer, x509_unable_to_get_crl_issuer, 71, "unable to get CRL issuer certificate")
ERROR_DECLARATION(X509UnhandledCriticalExtension, x509_unhandled_critical_extension, 72, "unhandled critical extension")
ERROR_DECLARATION(X509KeyusageNoCrlSign, x509_keyusage_no_crl_sign, 73, "key usage does not include CRL signing")
ERROR_DECLARATION(X509KeyusageNoDigitalSignature, x509_keyusage_no_digital_signature, 74, "key usage does not include digital signature")
ERROR_DECLARATION(X509UnhandledCriticalCrlExtension, x509_unhandled_critical_crl_extension, 75, "unhandled critical CRL extension")
ERROR_DECLARATION(X509InvalidExtension, x509_invalid_extension, 76, "invalid or inconsistent certificate extension")
ERROR_DECLARATION(X509InvalidPolicyExtension, x509_invalid_policy_extension, 77, "invalid or inconsistent certificate policy extension")
ERROR_DECLARATION(X509NoExplicitPolicy, x509_no_explicit_policy, 78, "no explicit policy")
ERROR_DECLARATION(X509DifferentCrlScope, x509_different_crl_scope, 79, "Different CRL scope")
ERROR_DECLARATION(X509UnsupportedExtensionFeature, x509_unsupported_extension_feature, 80, "Unsupported extension feature")
ERROR_DECLARATION(X509UnnestedResource, x509_unnested_resource, 81, "RFC 3779 resource not subset of parent's resources")
ERROR_DECLARATION(X509PermittedViolation, x509_permitted_violation, 82, "permitted subtree violation")
ERROR_DECLARATION(X509ExcludedViolation, x509_excluded_violation, 83, "excluded subtree violation")
ERROR_DECLARATION(X509SubtreeMinmax, x509_subtree_minmax, 84, "name constraints minimum and maximum not supported")
ERROR_DECLARATION(X509UnsupportedConstraintType, x509_unsupported_constraint_type, 85, "unsupported name constraint type")
ERROR_DECLARATION(X509UnsupportedConstraintSyntax, x509_unsupported_constraint_syntax, 86, "unsupported or invalid name constraint syntax")
ERROR_DECLARATION(X509UnsupportedNameSyntax, x509_unsupported_name_syntax, 87, "unsupported or invalid name syntax")
ERROR_DECLARATION(X509CrlPathValidationError, x509_crl_path_validation_error, 88, "CRL path validation error")

ERROR_DECLARATION(AttributeReadOnly, attribute_read_only, 89, "Object attribute can not be changed")
ERROR_DECLARATION(CannotSavePinInCache, cannot_save_pin_in_cache, 90, "Cannot save pin in cache")
ERROR_DECLARATION(CMSCertificateAlreadyPresent, cms_certificate_already_present, 91, "certificate already present")
ERROR_DECLARATION(RdnNotFound, rdn_not_found, 92, "rdn of this type not found")
ERROR_DECLARATION(AlreadyLoggedIn, already_logged_in, 93, "Login has already been performed")
ERROR_DECLARATION(CekNotAuthentic, cek_not_authentic, 94, "CEK not authentic")
ERROR_DECLARATION(LicenceReadOnly, licence_read_only, 95, "Licence can not be changed")

ERROR_DECLARATION(AttributeSensitive, attribute_sensitive, 96, "pkcs11 attribute sensitive")
ERROR_DECLARATION(AttributeTypeInvalid, attribute_type_invalid, 97, "pkcs11 attribute type invalid")
ERROR_DECLARATION(AttributeValueInvalid, attribute_value_invalid, 98, "pkcs11 attribute value invalid")
ERROR_DECLARATION(BufferTooSmall, buffer_too_small, 99, "pkcs11 buffer too small")
ERROR_DECLARATION(Pkcs11Cancel, pkcs11_cancel, 100, "pkcs11 cancel")
ERROR_DECLARATION(Pkcs11CantLock, cant_lock, 101, "pkcs11 cant lock")
ERROR_DECLARATION(CryptokiAlreadyInitialized, cryptoki_already_initialized, 102, "pkcs11 cryptoki already initialized")
ERROR_DECLARATION(DataLenRange, data_len_range, 103, "pkcs11 data len range")
ERROR_DECLARATION(FunctionNotParallel, function_not_parallel, 104, "pkcs11 function not parallel")
ERROR_DECLARATION(Pkcs11GeneralError, pkcs11_general_error, 105, "pkcs11 general error")
ERROR_DECLARATION(Pkcs11InformationSensitive, information_sensitive, 106, "pkcs11 information sensitive")
ERROR_DECLARATION(Pkcs11DeviceRemoved, pkcs11_device_removed, 107, "pkcs11 device removed")
ERROR_DECLARATION(Pkcs11DomainParamsInvalid, pkcs11_domain_params_invalid, 108, "pkcs11 domain params invalid")
ERROR_DECLARATION(EncryptedDataInvalid, encrypted_data_invalid, 109, "pkcs11 encrypted data invalid")
ERROR_DECLARATION(EncryptedDataLenRange, encrypted_data_len_range, 110, "pkcs11 encrypted data len range")
ERROR_DECLARATION(FunctionCanceled, function_canceled, 111, "pkcs11 function canceled")
ERROR_DECLARATION(KeyHandleInvalid, key_handle_invalid, 112, "pkcs11 key handle invalid")
ERROR_DECLARATION(KeyIndigestible, key_indigestible, 113, "pkcs11 key indigestible")
ERROR_DECLARATION(KeyNeeded, key_needed, 114, "pkcs11 key needed")
ERROR_DECLARATION(KeyNotNeeded, key_not_needed, 115, "pkcs11 key not needed")
ERROR_DECLARATION(KeyNotWrappable, key_not_wrappable, 116, "pkcs11 key not wrappable")
ERROR_DECLARATION(KeySizeRange, key_size_range, 117, "pkcs11 key size range")
ERROR_DECLARATION(KeyUnextractable, key_unextractable, 118, "pkcs11 key unextractable")
ERROR_DECLARATION(SessionClosed, session_closed, 119, "pkcs11 session closed")
ERROR_DECLARATION(SessionCount, session_count, 120, "pkcs11 session count")
ERROR_DECLARATION(SessionExists, session_exists, 121, "pkcs11 session exists")
ERROR_DECLARATION(PinExpired, pin_expired, 122, "pkcs11 pin expired")
ERROR_DECLARATION(PinInvalid, pin_invalid, 123, "pkcs11 pin invalid")
ERROR_DECLARATION(Pkcs11RandomNoRng, random_no_rng, 124, "pkcs11 random no rng")
ERROR_DECLARATION(Pkcs11RandomSeedNotSupported, random_seed_not_supported, 125, "pkcs11 random seed not supported")
ERROR_DECLARATION(Pkcs11SavedStateInvalid, saved_state_invalid, 126, "pkcs11 saved state invalid")
ERROR_DECLARATION(UserPinNotInitialized, user_pin_not_initialized, 127, "pkcs11 user pin not initialized")
ERROR_DECLARATION(UserTooManyTypes, user_too_many_types, 128, "pkcs11 user too many types")
ERROR_DECLARATION(UserTypeInvalid, user_type_invalid, 129, "pkcs11 user type invalid")
ERROR_DECLARATION(TokenNotPresent, token_not_present, 130, "pkcs11 token not present")
ERROR_DECLARATION(TokenNotRecognized, token_not_recognized, 131, "pkcs11 token not recognized")
ERROR_DECLARATION(TokenWriteProtected, token_write_protected, 132, "pkcs11 token write protected")
ERROR_DECLARATION(UnknownKeyType, unknown_key_type, 133, "unknown key type")
ERROR_DECLARATION(UnknownOid, unknown_oid, 134, "unknown oid")
ERROR_DECLARATION(WrongBigNumber, wrong_big_number, 135, "wrong big number")
ERROR_DECLARATION(DidnotFindSession, didnot_find_session, 136, "unable to find any session")
ERROR_DECLARATION(SessionParallelNotSupported, session_parallel_not_supported, 137, "pkcs11 session parallel not supported")
ERROR_DECLARATION(SessionReadOnly, session_read_only, 138, "pkcs11 session read only")
ERROR_DECLARATION(SessionReadOnlyExists, session_read_only_exists, 139, "pkcs11 session read only exists")
ERROR_DECLARATION(SessionReadWriteSoExists, session_read_write_so_exists, 140, "pkcs11 session read write so exists")
ERROR_DECLARATION(Pkcs11MutexBad, mutex_bad, 141, "pkcs11 mutex bad")
ERROR_DECLARATION(Pkcs11MutexNotLocked, mutex_not_locked, 142, "pkcs11 mutex not locked")
ERROR_DECLARATION(Pkcs11ObjectHandleInvalid, object_handle_invalid, 143, "pkcs11 object handle invalid")
ERROR_DECLARATION(Pkcs11Ok, pkcs11_ok, 144, "pkcs11 ok")
ERROR_DECLARATION(Pkcs11OperationActive, operation_active, 145, "pkcs11 operation active")
ERROR_DECLARATION(Pkcs11OperationNotInitialized, operation_not_initialized, 146, "pkcs11 operation not initialized")
ERROR_DECLARATION(MechanismInvalid, mechanism_invalid, 147, "pkcs11 mechanism invalid")
ERROR_DECLARATION(MechanismParamInvalid, mechanism_param_invalid, 148, "pkcs11 mechanism param invalid")
ERROR_DECLARATION(SignatureInvalid, signature_invalid, 149, "pkcs11 signature invalid")
ERROR_DECLARATION(SignatureLenRange, signature_len_range, 150, "pkcs11 signature len range")
ERROR_DECLARATION(SlotIdInvalid, slot_id_invalid, 151, "pkcs11 slot id invalid")
ERROR_DECLARATION(Pkcs11StateUnsaveable, state_unsaveable, 152, "pkcs11 state unsaveable")
ERROR_DECLARATION(TemplateIncomplete, template_incomplete, 153, "pkcs11 template incomplete")
ERROR_DECLARATION(UserAnotherAlreadyLoggedIn, user_another_already_logged_in, 154, "pkcs11 user another already logged in")
ERROR_DECLARATION(UnwrappingKeyHandleInvalid, unwrapping_key_handle_invalid, 155, "pkcs11 unwrapping key handle invalid")
ERROR_DECLARATION(UnwrappingKeySizeRange, unwrapping_key_size_range, 156, "pkcs11 unwrapping key size range")
ERROR_DECLARATION(UnwrappingKeyTypeInconsistent, unwrapping_key_type_inconsistent, 157, "pkcs11 unwrapping key type inconsistent")
ERROR_DECLARATION(WrappedKeyLenRange, wrapped_key_len_range, 158, "pkcs11 wrapped key len range")
ERROR_DECLARATION(WrappingKeyHandleInvalid, wrapping_key_handle_invalid, 159, "pkcs11 wrapping key handle invalid")
ERROR_DECLARATION(WrappingKeySizeRange, wrapping_key_size_range, 160, "pkcs11 wrapping key size range")
ERROR_DECLARATION(WrappingKeyTypeInconsistent, wrapping_key_type_inconsistent, 161, "pkcs11 wrapping key type inconsistent")
ERROR_DECLARATION(Pkcs11NeedToCreateThreads, need_to_create_threads, 162, "pkcs11 need to create threads")
ERROR_DECLARATION(Pkcs11NewPinMode, new_pin_mode, 163, "pkcs11 new pin mode")
ERROR_DECLARATION(Pkcs11NextOtp, next_otp, 164, "pkcs11 next otp")
ERROR_DECLARATION(Pkcs11NoEvent, no_event, 165, "pkcs11 no event")
ERROR_DECLARATION(CantHardwareVerifyCMS, cant_hardware_verify_cms, 166, "Multitple signed CMS with hardware verification is not supported")
ERROR_DECLARATION(DecryptUnsuccessful, decrypt_unsuccessful, 167, "Decrypt operation was unsuccessful")

#undef ERROR_DECLARATION
} rt_pc_error_code;

#ifdef __cplusplus
}
#endif

#endif // RT_PC_INTERNAL_ERRORCODE_H


#ifndef RT_PKI_CORE_INTERNAL_PP_H
#define RT_PKI_CORE_INTERNAL_PP_H

#define RTPC_CALL(variable, function, ...) \
	(variable).methods->function((variable).object RTPC_PP_COMMA_IF(RTPC_PP_NOT_EMPTY(__VA_ARGS__)) __VA_ARGS__)

#define RTPC_PP_NOT(x) RTPC_PP_NOT_I(x)
#define RTPC_PP_NOT_I(x) RTPC_PP_COMPL(RTPC_PP_BOOL(x))

#define RTPC_PP_COMPL(x) RTPC_PP_COMPL_I(x)
#define RTPC_PP_COMPL_I(x) RTPC_PP_COMPL_ ## x

#define RTPC_PP_COMPL_0 1
#define RTPC_PP_COMPL_1 0

#define RTPC_PP_OR(p, q) RTPC_PP_BITOR(RTPC_PP_BOOL(p), RTPC_PP_BOOL(q))

#define RTPC_PP_BITOR(x, y) RTPC_PP_BITOR_I(x, y)
#define RTPC_PP_BITOR_I(x, y) RTPC_PP_BITOR_ ## x ## y

#define RTPC_PP_BITOR_00 0
#define RTPC_PP_BITOR_01 1
#define RTPC_PP_BITOR_10 1
#define RTPC_PP_BITOR_11 1

#define RTPC_PP_COMMA_IF(cond) RTPC_PP_IF(cond, RTPC_PP_COMMA, RTPC_PP_EMPTY)()

#define RTPC_PP_EMPTY()

#define RTPC_PP_COMMA() ,

#define RTPC_PP_IF(cond, t, f) RTPC_PP_IIF(RTPC_PP_BOOL(cond), t, f)

#define RTPC_PP_IIF(bit, t, f) RTPC_PP_IIF_I(bit, t, f)
#define RTPC_PP_IIF_I(bit, t, f) RTPC_PP_IIF_ ## bit(t, f)

#define RTPC_PP_IIF_0(t, f) f
#define RTPC_PP_IIF_1(t, f) t

#define RTPC_PP_BOOL(x) RTPC_PP_BOOL_I(x)
#define RTPC_PP_BOOL_I(x) RTPC_PP_BOOL_ ## x

#define RTPC_PP_BOOL_0 0
#define RTPC_PP_BOOL_1 1
#define RTPC_PP_BOOL_2 1
#define RTPC_PP_BOOL_3 1
#define RTPC_PP_BOOL_4 1
#define RTPC_PP_BOOL_5 1
#define RTPC_PP_BOOL_6 1
#define RTPC_PP_BOOL_7 1
#define RTPC_PP_BOOL_8 1
#define RTPC_PP_BOOL_9 1
#define RTPC_PP_BOOL_10 1

#define RTPC_PP_NOT_EMPTY(...) RTPC_PP_OR(RTPC_PP_HAS_COMMA(__VA_ARGS__), RTPC_PP_NOT(RTPC_PP_HAS_COMMA(RTPC_PP_TRIGGER_PARENTHESIS __VA_ARGS__())))

#define RTPC_PP_HAS_COMMA(...) RTPC_PP_HAS_COMMA_I(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define RTPC_PP_HAS_COMMA_I(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, ...) e9

#define RTPC_PP_TRIGGER_PARENTHESIS(...) ,

#endif // RT_PKI_CORE_INTERNAL_PP_H


#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	rt_pc_data_format_plain,
	rt_pc_data_format_raw,
	rt_pc_data_format_pinpad2,
	rt_pc_data_format_xml,
	rt_pc_data_format_safetouch
} rt_pc_data_format;

typedef enum
{
	rt_pc_device_feature_journal,
	rt_pc_device_feature_pin2,
	rt_pc_device_feature_visualization,
	rt_pc_device_feature_sm,
	rt_pc_device_feature_flash_drive
} rt_pc_device_feature;

typedef enum
{
	rt_pc_key_alg_gost3410_2001,
	rt_pc_key_alg_gost3410_2012_256,
	rt_pc_key_alg_gost3410_2012_512
} rt_pc_key_alg;

typedef enum
{
	rt_pc_key_type_common,
	rt_pc_key_type_journal
} rt_pc_key_type;

typedef enum
{
	rt_pc_key_flag_need_pin2 = 0x01,
	rt_pc_key_flag_need_visualization = 0x02
} rt_pc_key_flag;

typedef struct
{
	int version;
	int flags;
	rt_pc_key_alg alg;
	rt_pc_key_type type;
	const void* cka_id;
	size_t cka_id_length;
} rt_pc_key_gen_params;

typedef enum
{
	rt_pc_gost3410_2001_key_paramset_a,
	rt_pc_gost3410_2001_key_paramset_b,
	rt_pc_gost3410_2001_key_paramset_c,
	rt_pc_gost3410_2001_key_paramset_xa,
	rt_pc_gost3410_2001_key_paramset_xb,
} rt_pc_gost3410_2001_key_paramset;

typedef enum
{
	rt_pc_gost3410_2012_256_key_paramset_a,
	rt_pc_gost3410_2012_256_key_paramset_b,
	rt_pc_gost3410_2012_256_key_paramset_c,
	rt_pc_gost3410_2012_256_key_paramset_xa,
	rt_pc_gost3410_2012_256_key_paramset_xb,
} rt_pc_gost3410_2012_256_key_paramset;

typedef enum
{
	rt_pc_gost3410_2012_512_key_paramset_a,
	rt_pc_gost3410_2012_512_key_paramset_b
} rt_pc_gost3410_2012_512_key_paramset;

typedef struct
{
	int version;
	int flags;
	rt_pc_key_alg alg;
	rt_pc_key_type type;
	const void* cka_id;
	size_t cka_id_length;
	rt_pc_gost3410_2001_key_paramset paramset;
} rt_pc_gost3410_2001_key_gen_params;

#define RT_PC_GOST3410_2001_KEY_GEN_PARAMS_INITIALIZER { \
		1, \
		0, \
		rt_pc_key_alg_gost3410_2001, \
		rt_pc_key_type_common, \
		NULL, \
		0, \
		rt_pc_gost3410_2001_key_paramset_a \
}

typedef struct
{
	int version;
	int flags;
	rt_pc_key_alg alg;
	rt_pc_key_type type;
	const void* cka_id;
	size_t cka_id_length;
	rt_pc_gost3410_2012_256_key_paramset paramset;
} rt_pc_gost3410_2012_256_key_gen_params;

#define RT_PC_GOST3410_2012_256_KEY_GEN_PARAMS_INITIALIZER { \
		1, \
		0, \
		rt_pc_key_alg_gost3410_2012_256, \
		rt_pc_key_type_common, \
		NULL, \
		0, \
		rt_pc_gost3410_2012_256_key_paramset_a \
}

typedef struct
{
	int version;
	int flags;
	rt_pc_key_alg alg;
	rt_pc_key_type type;
	const void* cka_id;
	size_t cka_id_length;
	rt_pc_gost3410_2012_512_key_paramset paramset;
} rt_pc_gost3410_2012_512_key_gen_params;

#define RT_PC_GOST3410_2012_512_KEY_GEN_PARAMS_INITIALIZER { \
		1, \
		0, \
		rt_pc_key_alg_gost3410_2012_512, \
		rt_pc_key_type_common, \
		NULL, \
		0, \
		rt_pc_gost3410_2012_512_key_paramset_a \
}

typedef struct
{
	int version;
} rt_pc_key_enum_params;

#define RT_PC_KEY_ENUM_PARAMS_INITIALIZER { 1 }

typedef enum
{
	rt_pc_cert_form_pem
} rt_pc_cert_form;

typedef enum
{
	rt_pc_sign_flag_add_user_cert = 0x02,
	rt_pc_sign_flag_add_sign_time = 0x04,
	rt_pc_sign_flag_hardware_hash = 0x08,
} rt_pc_sign_flag;

typedef enum
{
	rt_pc_verify_flag_hardware_hash = 0x01,
	rt_pc_verify_flag_verify_user_cert = 0x02,
} rt_pc_verify_flag;

typedef enum
{
	rt_pc_verify_result_success
} rt_pc_verify_result;

typedef enum
{
	rt_pc_x500_rdn_nid_common_name,
	rt_pc_x500_rdn_nid_surname,
	rt_pc_x500_rdn_nid_given_name,
	rt_pc_x500_rdn_nid_title,
	rt_pc_x500_rdn_nid_pseudonym,
	rt_pc_x500_rdn_nid_email_address,
	rt_pc_x500_rdn_nid_country_name,
	rt_pc_x500_rdn_nid_locality_name,
	rt_pc_x500_rdn_nid_state_or_province_name,
	rt_pc_x500_rdn_nid_organization,
	rt_pc_x500_rdn_nid_organizational_unit,
	rt_pc_x500_rdn_nid_street,
	rt_pc_x500_rdn_nid_ogrn,
	rt_pc_x500_rdn_nid_ogrnip,
	rt_pc_x500_rdn_nid_snils,
	rt_pc_x500_rdn_nid_inn,
	rt_pc_x500_rdn_nid_unstructured_name
} rt_pc_x500_rdn_nid;

typedef enum
{
	rt_pc_x509_key_usage_option_digital_signature,
	rt_pc_x509_key_usage_option_non_repudiation,
	rt_pc_x509_key_usage_option_key_encipherment,
	rt_pc_x509_key_usage_option_data_encipherment,
	rt_pc_x509_key_usage_option_key_agreement,
	rt_pc_x509_key_usage_option_key_cert_sign,
	rt_pc_x509_key_usage_option_crl_sign,
	rt_pc_x509_key_usage_option_encipher_only,
	rt_pc_x509_key_usage_option_decipher_only
} rt_pc_x509_key_usage_option;

typedef enum
{
	rt_pc_x509_ext_key_usage_option_email_protection,
	rt_pc_x509_ext_key_usage_option_client_auth,
	rt_pc_x509_ext_key_usage_option_server_auth,
	rt_pc_x509_ext_key_usage_option_code_signing,
	rt_pc_x509_ext_key_usage_option_time_stamping,
	rt_pc_x509_ext_key_usage_option_ms_code_ind,
	rt_pc_x509_ext_key_usage_option_ms_code_com,
	rt_pc_x509_ext_key_usage_option_ms_ctl_sign,
	rt_pc_x509_ext_key_usage_option_ocsp_signing,
	rt_pc_x509_ext_key_usage_option_any
} rt_pc_x509_ext_key_usage_option;

int rt_pc_initialize(const char* pkcs11_path);

int rt_pc_deinitialize();

int rt_pc_free_memory(void* pointer);

struct rt_pc_object_impl;

typedef struct
{
	rt_pc_object_impl* object;
} rt_pc_object;

void rt_pc_object_destruct(rt_pc_object_impl*);

typedef enum
{
	rt_pc_cms_content_type_data,
	rt_pc_cms_content_type_signed_data,
	rt_pc_cms_content_type_enveloped_data
} rt_pc_cms_content_type;

typedef enum
{
	rt_pc_x509_cert_extension_type_key_usage,
	rt_pc_x509_cert_extension_type_ext_key_usage,
	rt_pc_x509_cert_extension_type_cert_policies,
	rt_pc_x509_cert_extension_type_subject_sign_tool
} rt_pc_x509_cert_extension_type;

#define RTPC_CLASS_START(name) struct rt_pc_ ## name ## _methods_; \
	typedef struct rt_pc_ ## name ## _methods_ rt_pc_ ## name ## _methods;
#ifndef RTPC_CLASS_START
#define RTPC_CLASS_START(DUMMY)
#endif

#ifndef RTPC_CONSTRUCTOR
#define RTPC_CONSTRUCTOR(DUMMY1, DUMMY2, DUMMY3, DUMMY4)
#endif

#ifndef RTPC_METHOD
#define RTPC_METHOD(DUMMY1, DUMMY2, DUMMY3)
#endif

#ifndef RTPC_CLASS_END
#define RTPC_CLASS_END(DUMMY)
#endif

#ifndef RTPC_STATIC_METHOD
#define RTPC_STATIC_METHOD(DUMMY1, DUMMY2, DUMMY3)
#endif

RTPC_CLASS_START(public_key)
RTPC_METHOD(to_der, 3, (rt_pc_object_impl * object, uint8_t * *result, size_t * result_length))
RTPC_CLASS_END(public_key)

RTPC_CLASS_START(pkcs11_private_key)
RTPC_METHOD(get_public_key, 2, (rt_pc_object_impl * object, rt_pc_public_key * public_key))
RTPC_METHOD(get_id, 3, (rt_pc_object_impl * object, unsigned char** id, size_t * length))
RTPC_CLASS_END(pkcs11_private_key)

RTPC_CLASS_START(cert)
RTPC_METHOD(to_pem, 2, (rt_pc_object_impl * object, char** result))
RTPC_METHOD(get_subject, 2, (rt_pc_object_impl * object, rt_pc_x500_dn * subject))
RTPC_METHOD(get_serial_number, 3, (rt_pc_object_impl * object, uint8_t * *result, size_t * result_length))
RTPC_CLASS_END(cert)

RTPC_CLASS_START(external_cert)
RTPC_CONSTRUCTOR(external_cert, create_from_der, 3, (rt_pc_external_cert * object, const void* data, size_t length))
RTPC_CONSTRUCTOR(external_cert, create_from_pem, 2, (rt_pc_external_cert * object, const char* pem))
RTPC_CLASS_END(external_cert)

RTPC_CLASS_START(pkcs11_cert)
RTPC_METHOD(is_trusted, 2, (rt_pc_object_impl * object, bool* trusted))
RTPC_METHOD(get_corresponding_private_key, 2, (rt_pc_object_impl * object, rt_pc_pkcs11_private_key * key))
RTPC_CLASS_END(pkcs11_cert)

RTPC_CLASS_START(pkcs11_device)
RTPC_METHOD(get_label, 2, (rt_pc_object_impl * object, char** label))
RTPC_METHOD(get_serial_number, 2, (rt_pc_object_impl * object, char** serial_number))
RTPC_METHOD(is_logged_in, 2, (rt_pc_object_impl * object, bool* is_logged_in))
RTPC_METHOD(is_pin_default, 2, (rt_pc_object_impl * object, bool* is_pin_default))
RTPC_METHOD(is_format_supported, 3, (rt_pc_object_impl * object, rt_pc_data_format format, bool* result))
RTPC_METHOD(is_feature_supported, 3, (rt_pc_object_impl * object, rt_pc_device_feature feature, bool* result))
RTPC_METHOD(login, 2, (rt_pc_object_impl * object, const char* pin))
RTPC_METHOD(logout, 1, (rt_pc_object_impl * object))
RTPC_METHOD(change_pin, 3, (rt_pc_object_impl * object, const char* old_pin, const char* new_pin))
RTPC_METHOD(enumerate_keys, 4, (rt_pc_object_impl * object, const rt_pc_key_enum_params * params,
                                rt_pc_pkcs11_private_key * *key_list, size_t * key_count))
RTPC_METHOD(generate_key_pair, 3, (rt_pc_object_impl * object, const void* params, rt_pc_pkcs11_private_key * key))
RTPC_METHOD(delete_key_pair, 2, (rt_pc_object_impl * object, const void* key))
RTPC_METHOD(enumerate_certs, 3, (rt_pc_object_impl * object, rt_pc_pkcs11_cert * *cert_list, size_t * cert_count))
RTPC_METHOD(import_cert, 6, (rt_pc_object_impl * object, const void* value, size_t value_length, rt_pc_cert_form form,
                             bool trusted, rt_pc_pkcs11_cert * cert))
RTPC_METHOD(delete_cert, 2, (rt_pc_object_impl * object, const void* cert))
RTPC_CLASS_END(pkcs11_device)

RTPC_CLASS_START(x500_dn)
RTPC_CONSTRUCTOR(x500_dn, create, 1, (rt_pc_x500_dn * object))
RTPC_METHOD(set_rdn, 3, (rt_pc_object_impl * object, rt_pc_x500_rdn_nid nid, const char* value))
RTPC_METHOD(get_rdn, 3, (rt_pc_object_impl * object, rt_pc_x500_rdn_nid nid, char** value))
RTPC_METHOD(has_rdn, 3, (rt_pc_object_impl * object, rt_pc_x500_rdn_nid nid, bool* result))
RTPC_CLASS_END(x500_dn)

RTPC_CLASS_START(x509_cert_extension)
RTPC_METHOD(extension_type, 2, (rt_pc_object_impl * object, rt_pc_x509_cert_extension_type * value))
RTPC_METHOD(set_critical, 1, (rt_pc_object_impl * object))
RTPC_CLASS_END(x509_cert_extension)

RTPC_CLASS_START(x509_key_usage)
RTPC_CONSTRUCTOR(x509_key_usage, create, 1, (rt_pc_x509_key_usage * object))
RTPC_METHOD(add_option, 2, (rt_pc_object_impl * object, rt_pc_x509_key_usage_option option))
RTPC_CLASS_END(x509_key_usage)

RTPC_CLASS_START(x509_ext_key_usage)
RTPC_CONSTRUCTOR(x509_ext_key_usage, create, 1, (rt_pc_x509_ext_key_usage * object))
RTPC_METHOD(add_option, 2, (rt_pc_object_impl * object, rt_pc_x509_ext_key_usage_option option))
RTPC_METHOD(add_option_by_oid, 2, (rt_pc_object_impl * object, const char* oid))
RTPC_CLASS_END(x509_ext_key_usage)

RTPC_CLASS_START(x509_cert_policies)
RTPC_CONSTRUCTOR(x509_cert_policies, create, 1, (rt_pc_x509_cert_policies * object))
RTPC_METHOD(add_policy_by_oid, 2, (rt_pc_object_impl * object, const char* oid))
RTPC_CLASS_END(x509_cert_policies)

RTPC_CLASS_START(x509_subject_sign_tool)
RTPC_CONSTRUCTOR(x509_subject_sign_tool, create, 2, (rt_pc_x509_subject_sign_tool * object,
                                                     const char* value))
RTPC_CLASS_END(x509_subject_sign_tool)

RTPC_CLASS_START(pkcs10_reqinfo)
RTPC_CONSTRUCTOR(pkcs10_reqinfo, create, 1, (rt_pc_pkcs10_reqinfo * object))
RTPC_METHOD(set_subject, 2, (rt_pc_object_impl * object, const void* dn))
RTPC_METHOD(set_extension, 2, (rt_pc_object_impl * object, const void* extension))
RTPC_METHOD(set_custom_extension, 5, (rt_pc_object_impl * object, const char* oid,
                                      const void* data, size_t size, bool critical))
RTPC_METHOD(add_custom_attribute_value, 4, (rt_pc_object_impl * object, const char* oid, const void* data, size_t size))
RTPC_CLASS_END(pkcs10_reqinfo)

RTPC_CLASS_START(cms_message)
RTPC_METHOD(content_type, 2, (rt_pc_object_impl * object, rt_pc_cms_content_type * value))
RTPC_METHOD(to_ber, 3, (rt_pc_object_impl * object, uint8_t * *data, size_t * length))
RTPC_CLASS_END(cms_message)

RTPC_CLASS_START(cms_data)
RTPC_CONSTRUCTOR(cms_data, create, 3, (rt_pc_cms_data * object, const uint8_t * data, size_t length))
RTPC_METHOD(data, 3, (rt_pc_object_impl * object, uint8_t * *data, size_t * length))
RTPC_CLASS_END(cms_data)

RTPC_CLASS_START(cms_sign_params)
RTPC_CONSTRUCTOR(cms_sign_params, create, 2, (rt_pc_cms_sign_params * object, const rt_pc_pkcs11_cert * cert))
RTPC_METHOD(set_flags, 2, (rt_pc_object_impl * object, int flags))
RTPC_METHOD(add_cert, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_CLASS_END(cms_sign_params)

RTPC_CLASS_START(cms_verify_params)
RTPC_CONSTRUCTOR(cms_verify_params, create, 2, (rt_pc_cms_verify_params * object, const rt_pc_pkcs11_device * device))
RTPC_METHOD(set_flags, 2, (rt_pc_object_impl * object, int flags))
RTPC_METHOD(add_user_cert, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_METHOD(add_ca_cert, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_METHOD(add_crl, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_METHOD(set_data, 3, (rt_pc_object_impl * object, const void* data, size_t length))
RTPC_CLASS_END(cms_verify_params)

RTPC_CLASS_START(cms_signed_data)
RTPC_CONSTRUCTOR(cms_signed_data, parse, 3, (rt_pc_cms_signed_data * object, const uint8_t * data, size_t length))
RTPC_METHOD(assign, 3, (rt_pc_object_impl * object, const uint8_t * cms, size_t cms_length))
RTPC_CLASS_END(cms_signed_data)

RTPC_CLASS_START(cms_envelop_params)
RTPC_CONSTRUCTOR(cms_envelop_params, create, 2, (rt_pc_cms_envelop_params * object, const rt_pc_pkcs11_device * device))
RTPC_METHOD(set_flags, 2, (rt_pc_object_impl * object, int flags))
RTPC_METHOD(add_recipient, 2, (rt_pc_object_impl * object, const char* cert))
RTPC_CLASS_END(cms_envelop_params)

RTPC_CLASS_START(cms_decrypt_params)
RTPC_CONSTRUCTOR(cms_decrypt_params, create, 2, (rt_pc_cms_decrypt_params * object, const rt_pc_pkcs11_cert * cert))
RTPC_CLASS_END(cms_decrypt_params)

RTPC_CLASS_START(cms_enveloped_data)
RTPC_CONSTRUCTOR(cms_enveloped_data, parse, 3, (rt_pc_cms_enveloped_data * object, const uint8_t * data, size_t length))
RTPC_CLASS_END(cms_enveloped_data)

RTPC_STATIC_METHOD(cms_encrypt, 5, (const void* data, size_t data_length, const rt_pc_cms_envelop_params * params,
                                    unsigned char** cms, size_t * cms_length))
RTPC_STATIC_METHOD(cms_verify, 4, (const void* cms, size_t cms_length, const rt_pc_cms_verify_params * params,
                                   rt_pc_verify_result * result))
RTPC_STATIC_METHOD(cms_sign, 8, (const void* data, size_t data_length, const rt_pc_cms_sign_params * params, bool detached,
                                 const void* signed_data, size_t signed_data_length, unsigned char** cms, size_t * cms_length))
RTPC_STATIC_METHOD(cms_decrypt, 5, (const void* cms, size_t cms_length, const rt_pc_cms_decrypt_params * params,
                                    unsigned char** result, size_t * result_length))
RTPC_STATIC_METHOD(sign_pkcs10_request, 3, (const void* key, const void* request, char** result))
RTPC_STATIC_METHOD(sign, 5, (const void* key, const void* data, size_t data_length, unsigned char** sign, size_t * sign_length))

RTPC_STATIC_METHOD(enumerate_devices, 2, (rt_pc_pkcs11_device * *device_list, size_t * device_count))

#undef RTPC_STATIC_METHOD
#undef RTPC_CLASS_END
#undef RTPC_METHOD
#undef RTPC_CONSTRUCTOR
#undef RTPC_CLASS_START


#define RTPC_CLASS_START(name) typedef struct { \
		rt_pc_object_impl* object; \
		rt_pc_ ## name ## _methods * methods;
#define RTPC_CLASS_END(name) } rt_pc_ ## name;
#ifndef RTPC_CLASS_START
#define RTPC_CLASS_START(DUMMY)
#endif

#ifndef RTPC_CONSTRUCTOR
#define RTPC_CONSTRUCTOR(DUMMY1, DUMMY2, DUMMY3, DUMMY4)
#endif

#ifndef RTPC_METHOD
#define RTPC_METHOD(DUMMY1, DUMMY2, DUMMY3)
#endif

#ifndef RTPC_CLASS_END
#define RTPC_CLASS_END(DUMMY)
#endif

#ifndef RTPC_STATIC_METHOD
#define RTPC_STATIC_METHOD(DUMMY1, DUMMY2, DUMMY3)
#endif

RTPC_CLASS_START(public_key)
RTPC_METHOD(to_der, 3, (rt_pc_object_impl * object, uint8_t * *result, size_t * result_length))
RTPC_CLASS_END(public_key)

RTPC_CLASS_START(pkcs11_private_key)
RTPC_METHOD(get_public_key, 2, (rt_pc_object_impl * object, rt_pc_public_key * public_key))
RTPC_METHOD(get_id, 3, (rt_pc_object_impl * object, unsigned char** id, size_t * length))
RTPC_CLASS_END(pkcs11_private_key)

RTPC_CLASS_START(cert)
RTPC_METHOD(to_pem, 2, (rt_pc_object_impl * object, char** result))
RTPC_METHOD(get_subject, 2, (rt_pc_object_impl * object, rt_pc_x500_dn * subject))
RTPC_METHOD(get_serial_number, 3, (rt_pc_object_impl * object, uint8_t * *result, size_t * result_length))
RTPC_CLASS_END(cert)

RTPC_CLASS_START(external_cert)
RTPC_CONSTRUCTOR(external_cert, create_from_der, 3, (rt_pc_external_cert * object, const void* data, size_t length))
RTPC_CONSTRUCTOR(external_cert, create_from_pem, 2, (rt_pc_external_cert * object, const char* pem))
RTPC_CLASS_END(external_cert)

RTPC_CLASS_START(pkcs11_cert)
RTPC_METHOD(is_trusted, 2, (rt_pc_object_impl * object, bool* trusted))
RTPC_METHOD(get_corresponding_private_key, 2, (rt_pc_object_impl * object, rt_pc_pkcs11_private_key * key))
RTPC_CLASS_END(pkcs11_cert)

RTPC_CLASS_START(pkcs11_device)
RTPC_METHOD(get_label, 2, (rt_pc_object_impl * object, char** label))
RTPC_METHOD(get_serial_number, 2, (rt_pc_object_impl * object, char** serial_number))
RTPC_METHOD(is_logged_in, 2, (rt_pc_object_impl * object, bool* is_logged_in))
RTPC_METHOD(is_pin_default, 2, (rt_pc_object_impl * object, bool* is_pin_default))
RTPC_METHOD(is_format_supported, 3, (rt_pc_object_impl * object, rt_pc_data_format format, bool* result))
RTPC_METHOD(is_feature_supported, 3, (rt_pc_object_impl * object, rt_pc_device_feature feature, bool* result))
RTPC_METHOD(login, 2, (rt_pc_object_impl * object, const char* pin))
RTPC_METHOD(logout, 1, (rt_pc_object_impl * object))
RTPC_METHOD(change_pin, 3, (rt_pc_object_impl * object, const char* old_pin, const char* new_pin))
RTPC_METHOD(enumerate_keys, 4, (rt_pc_object_impl * object, const rt_pc_key_enum_params * params,
                                rt_pc_pkcs11_private_key * *key_list, size_t * key_count))
RTPC_METHOD(generate_key_pair, 3, (rt_pc_object_impl * object, const void* params, rt_pc_pkcs11_private_key * key))
RTPC_METHOD(delete_key_pair, 2, (rt_pc_object_impl * object, const void* key))
RTPC_METHOD(enumerate_certs, 3, (rt_pc_object_impl * object, rt_pc_pkcs11_cert * *cert_list, size_t * cert_count))
RTPC_METHOD(import_cert, 6, (rt_pc_object_impl * object, const void* value, size_t value_length, rt_pc_cert_form form,
                             bool trusted, rt_pc_pkcs11_cert * cert))
RTPC_METHOD(delete_cert, 2, (rt_pc_object_impl * object, const void* cert))
RTPC_CLASS_END(pkcs11_device)

RTPC_CLASS_START(x500_dn)
RTPC_CONSTRUCTOR(x500_dn, create, 1, (rt_pc_x500_dn * object))
RTPC_METHOD(set_rdn, 3, (rt_pc_object_impl * object, rt_pc_x500_rdn_nid nid, const char* value))
RTPC_METHOD(get_rdn, 3, (rt_pc_object_impl * object, rt_pc_x500_rdn_nid nid, char** value))
RTPC_METHOD(has_rdn, 3, (rt_pc_object_impl * object, rt_pc_x500_rdn_nid nid, bool* result))
RTPC_CLASS_END(x500_dn)

RTPC_CLASS_START(x509_cert_extension)
RTPC_METHOD(extension_type, 2, (rt_pc_object_impl * object, rt_pc_x509_cert_extension_type * value))
RTPC_METHOD(set_critical, 1, (rt_pc_object_impl * object))
RTPC_CLASS_END(x509_cert_extension)

RTPC_CLASS_START(x509_key_usage)
RTPC_CONSTRUCTOR(x509_key_usage, create, 1, (rt_pc_x509_key_usage * object))
RTPC_METHOD(add_option, 2, (rt_pc_object_impl * object, rt_pc_x509_key_usage_option option))
RTPC_CLASS_END(x509_key_usage)

RTPC_CLASS_START(x509_ext_key_usage)
RTPC_CONSTRUCTOR(x509_ext_key_usage, create, 1, (rt_pc_x509_ext_key_usage * object))
RTPC_METHOD(add_option, 2, (rt_pc_object_impl * object, rt_pc_x509_ext_key_usage_option option))
RTPC_METHOD(add_option_by_oid, 2, (rt_pc_object_impl * object, const char* oid))
RTPC_CLASS_END(x509_ext_key_usage)

RTPC_CLASS_START(x509_cert_policies)
RTPC_CONSTRUCTOR(x509_cert_policies, create, 1, (rt_pc_x509_cert_policies * object))
RTPC_METHOD(add_policy_by_oid, 2, (rt_pc_object_impl * object, const char* oid))
RTPC_CLASS_END(x509_cert_policies)

RTPC_CLASS_START(x509_subject_sign_tool)
RTPC_CONSTRUCTOR(x509_subject_sign_tool, create, 2, (rt_pc_x509_subject_sign_tool * object,
                                                     const char* value))
RTPC_CLASS_END(x509_subject_sign_tool)

RTPC_CLASS_START(pkcs10_reqinfo)
RTPC_CONSTRUCTOR(pkcs10_reqinfo, create, 1, (rt_pc_pkcs10_reqinfo * object))
RTPC_METHOD(set_subject, 2, (rt_pc_object_impl * object, const void* dn))
RTPC_METHOD(set_extension, 2, (rt_pc_object_impl * object, const void* extension))
RTPC_METHOD(set_custom_extension, 5, (rt_pc_object_impl * object, const char* oid,
                                      const void* data, size_t size, bool critical))
RTPC_METHOD(add_custom_attribute_value, 4, (rt_pc_object_impl * object, const char* oid, const void* data, size_t size))
RTPC_CLASS_END(pkcs10_reqinfo)

RTPC_CLASS_START(cms_message)
RTPC_METHOD(content_type, 2, (rt_pc_object_impl * object, rt_pc_cms_content_type * value))
RTPC_METHOD(to_ber, 3, (rt_pc_object_impl * object, uint8_t * *data, size_t * length))
RTPC_CLASS_END(cms_message)

RTPC_CLASS_START(cms_data)
RTPC_CONSTRUCTOR(cms_data, create, 3, (rt_pc_cms_data * object, const uint8_t * data, size_t length))
RTPC_METHOD(data, 3, (rt_pc_object_impl * object, uint8_t * *data, size_t * length))
RTPC_CLASS_END(cms_data)

RTPC_CLASS_START(cms_sign_params)
RTPC_CONSTRUCTOR(cms_sign_params, create, 2, (rt_pc_cms_sign_params * object, const rt_pc_pkcs11_cert * cert))
RTPC_METHOD(set_flags, 2, (rt_pc_object_impl * object, int flags))
RTPC_METHOD(add_cert, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_CLASS_END(cms_sign_params)

RTPC_CLASS_START(cms_verify_params)
RTPC_CONSTRUCTOR(cms_verify_params, create, 2, (rt_pc_cms_verify_params * object, const rt_pc_pkcs11_device * device))
RTPC_METHOD(set_flags, 2, (rt_pc_object_impl * object, int flags))
RTPC_METHOD(add_user_cert, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_METHOD(add_ca_cert, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_METHOD(add_crl, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_METHOD(set_data, 3, (rt_pc_object_impl * object, const void* data, size_t length))
RTPC_CLASS_END(cms_verify_params)

RTPC_CLASS_START(cms_signed_data)
RTPC_CONSTRUCTOR(cms_signed_data, parse, 3, (rt_pc_cms_signed_data * object, const uint8_t * data, size_t length))
RTPC_METHOD(assign, 3, (rt_pc_object_impl * object, const uint8_t * cms, size_t cms_length))
RTPC_CLASS_END(cms_signed_data)

RTPC_CLASS_START(cms_envelop_params)
RTPC_CONSTRUCTOR(cms_envelop_params, create, 2, (rt_pc_cms_envelop_params * object, const rt_pc_pkcs11_device * device))
RTPC_METHOD(set_flags, 2, (rt_pc_object_impl * object, int flags))
RTPC_METHOD(add_recipient, 2, (rt_pc_object_impl * object, const char* cert))
RTPC_CLASS_END(cms_envelop_params)

RTPC_CLASS_START(cms_decrypt_params)
RTPC_CONSTRUCTOR(cms_decrypt_params, create, 2, (rt_pc_cms_decrypt_params * object, const rt_pc_pkcs11_cert * cert))
RTPC_CLASS_END(cms_decrypt_params)

RTPC_CLASS_START(cms_enveloped_data)
RTPC_CONSTRUCTOR(cms_enveloped_data, parse, 3, (rt_pc_cms_enveloped_data * object, const uint8_t * data, size_t length))
RTPC_CLASS_END(cms_enveloped_data)

RTPC_STATIC_METHOD(cms_encrypt, 5, (const void* data, size_t data_length, const rt_pc_cms_envelop_params * params,
                                    unsigned char** cms, size_t * cms_length))
RTPC_STATIC_METHOD(cms_verify, 4, (const void* cms, size_t cms_length, const rt_pc_cms_verify_params * params,
                                   rt_pc_verify_result * result))
RTPC_STATIC_METHOD(cms_sign, 8, (const void* data, size_t data_length, const rt_pc_cms_sign_params * params, bool detached,
                                 const void* signed_data, size_t signed_data_length, unsigned char** cms, size_t * cms_length))
RTPC_STATIC_METHOD(cms_decrypt, 5, (const void* cms, size_t cms_length, const rt_pc_cms_decrypt_params * params,
                                    unsigned char** result, size_t * result_length))
RTPC_STATIC_METHOD(sign_pkcs10_request, 3, (const void* key, const void* request, char** result))
RTPC_STATIC_METHOD(sign, 5, (const void* key, const void* data, size_t data_length, unsigned char** sign, size_t * sign_length))

RTPC_STATIC_METHOD(enumerate_devices, 2, (rt_pc_pkcs11_device * *device_list, size_t * device_count))

#undef RTPC_STATIC_METHOD
#undef RTPC_CLASS_END
#undef RTPC_METHOD
#undef RTPC_CONSTRUCTOR
#undef RTPC_CLASS_START


#define RTPC_CLASS_START(name) struct rt_pc_ ## name ## _methods_ {
#define RTPC_METHOD(name, count, args) int(*name)args;
#define RTPC_CLASS_END(name) };
#ifndef RTPC_CLASS_START
#define RTPC_CLASS_START(DUMMY)
#endif

#ifndef RTPC_CONSTRUCTOR
#define RTPC_CONSTRUCTOR(DUMMY1, DUMMY2, DUMMY3, DUMMY4)
#endif

#ifndef RTPC_METHOD
#define RTPC_METHOD(DUMMY1, DUMMY2, DUMMY3)
#endif

#ifndef RTPC_CLASS_END
#define RTPC_CLASS_END(DUMMY)
#endif

#ifndef RTPC_STATIC_METHOD
#define RTPC_STATIC_METHOD(DUMMY1, DUMMY2, DUMMY3)
#endif

RTPC_CLASS_START(public_key)
RTPC_METHOD(to_der, 3, (rt_pc_object_impl * object, uint8_t * *result, size_t * result_length))
RTPC_CLASS_END(public_key)

RTPC_CLASS_START(pkcs11_private_key)
RTPC_METHOD(get_public_key, 2, (rt_pc_object_impl * object, rt_pc_public_key * public_key))
RTPC_METHOD(get_id, 3, (rt_pc_object_impl * object, unsigned char** id, size_t * length))
RTPC_CLASS_END(pkcs11_private_key)

RTPC_CLASS_START(cert)
RTPC_METHOD(to_pem, 2, (rt_pc_object_impl * object, char** result))
RTPC_METHOD(get_subject, 2, (rt_pc_object_impl * object, rt_pc_x500_dn * subject))
RTPC_METHOD(get_serial_number, 3, (rt_pc_object_impl * object, uint8_t * *result, size_t * result_length))
RTPC_CLASS_END(cert)

RTPC_CLASS_START(external_cert)
RTPC_CONSTRUCTOR(external_cert, create_from_der, 3, (rt_pc_external_cert * object, const void* data, size_t length))
RTPC_CONSTRUCTOR(external_cert, create_from_pem, 2, (rt_pc_external_cert * object, const char* pem))
RTPC_CLASS_END(external_cert)

RTPC_CLASS_START(pkcs11_cert)
RTPC_METHOD(is_trusted, 2, (rt_pc_object_impl * object, bool* trusted))
RTPC_METHOD(get_corresponding_private_key, 2, (rt_pc_object_impl * object, rt_pc_pkcs11_private_key * key))
RTPC_CLASS_END(pkcs11_cert)

RTPC_CLASS_START(pkcs11_device)
RTPC_METHOD(get_label, 2, (rt_pc_object_impl * object, char** label))
RTPC_METHOD(get_serial_number, 2, (rt_pc_object_impl * object, char** serial_number))
RTPC_METHOD(is_logged_in, 2, (rt_pc_object_impl * object, bool* is_logged_in))
RTPC_METHOD(is_pin_default, 2, (rt_pc_object_impl * object, bool* is_pin_default))
RTPC_METHOD(is_format_supported, 3, (rt_pc_object_impl * object, rt_pc_data_format format, bool* result))
RTPC_METHOD(is_feature_supported, 3, (rt_pc_object_impl * object, rt_pc_device_feature feature, bool* result))
RTPC_METHOD(login, 2, (rt_pc_object_impl * object, const char* pin))
RTPC_METHOD(logout, 1, (rt_pc_object_impl * object))
RTPC_METHOD(change_pin, 3, (rt_pc_object_impl * object, const char* old_pin, const char* new_pin))
RTPC_METHOD(enumerate_keys, 4, (rt_pc_object_impl * object, const rt_pc_key_enum_params * params,
                                rt_pc_pkcs11_private_key * *key_list, size_t * key_count))
RTPC_METHOD(generate_key_pair, 3, (rt_pc_object_impl * object, const void* params, rt_pc_pkcs11_private_key * key))
RTPC_METHOD(delete_key_pair, 2, (rt_pc_object_impl * object, const void* key))
RTPC_METHOD(enumerate_certs, 3, (rt_pc_object_impl * object, rt_pc_pkcs11_cert * *cert_list, size_t * cert_count))
RTPC_METHOD(import_cert, 6, (rt_pc_object_impl * object, const void* value, size_t value_length, rt_pc_cert_form form,
                             bool trusted, rt_pc_pkcs11_cert * cert))
RTPC_METHOD(delete_cert, 2, (rt_pc_object_impl * object, const void* cert))
RTPC_CLASS_END(pkcs11_device)

RTPC_CLASS_START(x500_dn)
RTPC_CONSTRUCTOR(x500_dn, create, 1, (rt_pc_x500_dn * object))
RTPC_METHOD(set_rdn, 3, (rt_pc_object_impl * object, rt_pc_x500_rdn_nid nid, const char* value))
RTPC_METHOD(get_rdn, 3, (rt_pc_object_impl * object, rt_pc_x500_rdn_nid nid, char** value))
RTPC_METHOD(has_rdn, 3, (rt_pc_object_impl * object, rt_pc_x500_rdn_nid nid, bool* result))
RTPC_CLASS_END(x500_dn)

RTPC_CLASS_START(x509_cert_extension)
RTPC_METHOD(extension_type, 2, (rt_pc_object_impl * object, rt_pc_x509_cert_extension_type * value))
RTPC_METHOD(set_critical, 1, (rt_pc_object_impl * object))
RTPC_CLASS_END(x509_cert_extension)

RTPC_CLASS_START(x509_key_usage)
RTPC_CONSTRUCTOR(x509_key_usage, create, 1, (rt_pc_x509_key_usage * object))
RTPC_METHOD(add_option, 2, (rt_pc_object_impl * object, rt_pc_x509_key_usage_option option))
RTPC_CLASS_END(x509_key_usage)

RTPC_CLASS_START(x509_ext_key_usage)
RTPC_CONSTRUCTOR(x509_ext_key_usage, create, 1, (rt_pc_x509_ext_key_usage * object))
RTPC_METHOD(add_option, 2, (rt_pc_object_impl * object, rt_pc_x509_ext_key_usage_option option))
RTPC_METHOD(add_option_by_oid, 2, (rt_pc_object_impl * object, const char* oid))
RTPC_CLASS_END(x509_ext_key_usage)

RTPC_CLASS_START(x509_cert_policies)
RTPC_CONSTRUCTOR(x509_cert_policies, create, 1, (rt_pc_x509_cert_policies * object))
RTPC_METHOD(add_policy_by_oid, 2, (rt_pc_object_impl * object, const char* oid))
RTPC_CLASS_END(x509_cert_policies)

RTPC_CLASS_START(x509_subject_sign_tool)
RTPC_CONSTRUCTOR(x509_subject_sign_tool, create, 2, (rt_pc_x509_subject_sign_tool * object,
                                                     const char* value))
RTPC_CLASS_END(x509_subject_sign_tool)

RTPC_CLASS_START(pkcs10_reqinfo)
RTPC_CONSTRUCTOR(pkcs10_reqinfo, create, 1, (rt_pc_pkcs10_reqinfo * object))
RTPC_METHOD(set_subject, 2, (rt_pc_object_impl * object, const void* dn))
RTPC_METHOD(set_extension, 2, (rt_pc_object_impl * object, const void* extension))
RTPC_METHOD(set_custom_extension, 5, (rt_pc_object_impl * object, const char* oid,
                                      const void* data, size_t size, bool critical))
RTPC_METHOD(add_custom_attribute_value, 4, (rt_pc_object_impl * object, const char* oid, const void* data, size_t size))
RTPC_CLASS_END(pkcs10_reqinfo)

RTPC_CLASS_START(cms_message)
RTPC_METHOD(content_type, 2, (rt_pc_object_impl * object, rt_pc_cms_content_type * value))
RTPC_METHOD(to_ber, 3, (rt_pc_object_impl * object, uint8_t * *data, size_t * length))
RTPC_CLASS_END(cms_message)

RTPC_CLASS_START(cms_data)
RTPC_CONSTRUCTOR(cms_data, create, 3, (rt_pc_cms_data * object, const uint8_t * data, size_t length))
RTPC_METHOD(data, 3, (rt_pc_object_impl * object, uint8_t * *data, size_t * length))
RTPC_CLASS_END(cms_data)

RTPC_CLASS_START(cms_sign_params)
RTPC_CONSTRUCTOR(cms_sign_params, create, 2, (rt_pc_cms_sign_params * object, const rt_pc_pkcs11_cert * cert))
RTPC_METHOD(set_flags, 2, (rt_pc_object_impl * object, int flags))
RTPC_METHOD(add_cert, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_CLASS_END(cms_sign_params)

RTPC_CLASS_START(cms_verify_params)
RTPC_CONSTRUCTOR(cms_verify_params, create, 2, (rt_pc_cms_verify_params * object, const rt_pc_pkcs11_device * device))
RTPC_METHOD(set_flags, 2, (rt_pc_object_impl * object, int flags))
RTPC_METHOD(add_user_cert, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_METHOD(add_ca_cert, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_METHOD(add_crl, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_METHOD(set_data, 3, (rt_pc_object_impl * object, const void* data, size_t length))
RTPC_CLASS_END(cms_verify_params)

RTPC_CLASS_START(cms_signed_data)
RTPC_CONSTRUCTOR(cms_signed_data, parse, 3, (rt_pc_cms_signed_data * object, const uint8_t * data, size_t length))
RTPC_METHOD(assign, 3, (rt_pc_object_impl * object, const uint8_t * cms, size_t cms_length))
RTPC_CLASS_END(cms_signed_data)

RTPC_CLASS_START(cms_envelop_params)
RTPC_CONSTRUCTOR(cms_envelop_params, create, 2, (rt_pc_cms_envelop_params * object, const rt_pc_pkcs11_device * device))
RTPC_METHOD(set_flags, 2, (rt_pc_object_impl * object, int flags))
RTPC_METHOD(add_recipient, 2, (rt_pc_object_impl * object, const char* cert))
RTPC_CLASS_END(cms_envelop_params)

RTPC_CLASS_START(cms_decrypt_params)
RTPC_CONSTRUCTOR(cms_decrypt_params, create, 2, (rt_pc_cms_decrypt_params * object, const rt_pc_pkcs11_cert * cert))
RTPC_CLASS_END(cms_decrypt_params)

RTPC_CLASS_START(cms_enveloped_data)
RTPC_CONSTRUCTOR(cms_enveloped_data, parse, 3, (rt_pc_cms_enveloped_data * object, const uint8_t * data, size_t length))
RTPC_CLASS_END(cms_enveloped_data)

RTPC_STATIC_METHOD(cms_encrypt, 5, (const void* data, size_t data_length, const rt_pc_cms_envelop_params * params,
                                    unsigned char** cms, size_t * cms_length))
RTPC_STATIC_METHOD(cms_verify, 4, (const void* cms, size_t cms_length, const rt_pc_cms_verify_params * params,
                                   rt_pc_verify_result * result))
RTPC_STATIC_METHOD(cms_sign, 8, (const void* data, size_t data_length, const rt_pc_cms_sign_params * params, bool detached,
                                 const void* signed_data, size_t signed_data_length, unsigned char** cms, size_t * cms_length))
RTPC_STATIC_METHOD(cms_decrypt, 5, (const void* cms, size_t cms_length, const rt_pc_cms_decrypt_params * params,
                                    unsigned char** result, size_t * result_length))
RTPC_STATIC_METHOD(sign_pkcs10_request, 3, (const void* key, const void* request, char** result))
RTPC_STATIC_METHOD(sign, 5, (const void* key, const void* data, size_t data_length, unsigned char** sign, size_t * sign_length))

RTPC_STATIC_METHOD(enumerate_devices, 2, (rt_pc_pkcs11_device * *device_list, size_t * device_count))

#undef RTPC_STATIC_METHOD
#undef RTPC_CLASS_END
#undef RTPC_METHOD
#undef RTPC_CONSTRUCTOR
#undef RTPC_CLASS_START


#define RTPC_CLASS_START(name) rt_pc_ ## name rt_pc_ ## name ## _cast(rt_pc_object_impl*);
#define RTPC_CONSTRUCTOR(className, name, count, args) int rt_pc_ ## className ## _ ## name args;
#ifndef RTPC_CLASS_START
#define RTPC_CLASS_START(DUMMY)
#endif

#ifndef RTPC_CONSTRUCTOR
#define RTPC_CONSTRUCTOR(DUMMY1, DUMMY2, DUMMY3, DUMMY4)
#endif

#ifndef RTPC_METHOD
#define RTPC_METHOD(DUMMY1, DUMMY2, DUMMY3)
#endif

#ifndef RTPC_CLASS_END
#define RTPC_CLASS_END(DUMMY)
#endif

#ifndef RTPC_STATIC_METHOD
#define RTPC_STATIC_METHOD(DUMMY1, DUMMY2, DUMMY3)
#endif

RTPC_CLASS_START(public_key)
RTPC_METHOD(to_der, 3, (rt_pc_object_impl * object, uint8_t * *result, size_t * result_length))
RTPC_CLASS_END(public_key)

RTPC_CLASS_START(pkcs11_private_key)
RTPC_METHOD(get_public_key, 2, (rt_pc_object_impl * object, rt_pc_public_key * public_key))
RTPC_METHOD(get_id, 3, (rt_pc_object_impl * object, unsigned char** id, size_t * length))
RTPC_CLASS_END(pkcs11_private_key)

RTPC_CLASS_START(cert)
RTPC_METHOD(to_pem, 2, (rt_pc_object_impl * object, char** result))
RTPC_METHOD(get_subject, 2, (rt_pc_object_impl * object, rt_pc_x500_dn * subject))
RTPC_METHOD(get_serial_number, 3, (rt_pc_object_impl * object, uint8_t * *result, size_t * result_length))
RTPC_CLASS_END(cert)

RTPC_CLASS_START(external_cert)
RTPC_CONSTRUCTOR(external_cert, create_from_der, 3, (rt_pc_external_cert * object, const void* data, size_t length))
RTPC_CONSTRUCTOR(external_cert, create_from_pem, 2, (rt_pc_external_cert * object, const char* pem))
RTPC_CLASS_END(external_cert)

RTPC_CLASS_START(pkcs11_cert)
RTPC_METHOD(is_trusted, 2, (rt_pc_object_impl * object, bool* trusted))
RTPC_METHOD(get_corresponding_private_key, 2, (rt_pc_object_impl * object, rt_pc_pkcs11_private_key * key))
RTPC_CLASS_END(pkcs11_cert)

RTPC_CLASS_START(pkcs11_device)
RTPC_METHOD(get_label, 2, (rt_pc_object_impl * object, char** label))
RTPC_METHOD(get_serial_number, 2, (rt_pc_object_impl * object, char** serial_number))
RTPC_METHOD(is_logged_in, 2, (rt_pc_object_impl * object, bool* is_logged_in))
RTPC_METHOD(is_pin_default, 2, (rt_pc_object_impl * object, bool* is_pin_default))
RTPC_METHOD(is_format_supported, 3, (rt_pc_object_impl * object, rt_pc_data_format format, bool* result))
RTPC_METHOD(is_feature_supported, 3, (rt_pc_object_impl * object, rt_pc_device_feature feature, bool* result))
RTPC_METHOD(login, 2, (rt_pc_object_impl * object, const char* pin))
RTPC_METHOD(logout, 1, (rt_pc_object_impl * object))
RTPC_METHOD(change_pin, 3, (rt_pc_object_impl * object, const char* old_pin, const char* new_pin))
RTPC_METHOD(enumerate_keys, 4, (rt_pc_object_impl * object, const rt_pc_key_enum_params * params,
                                rt_pc_pkcs11_private_key * *key_list, size_t * key_count))
RTPC_METHOD(generate_key_pair, 3, (rt_pc_object_impl * object, const void* params, rt_pc_pkcs11_private_key * key))
RTPC_METHOD(delete_key_pair, 2, (rt_pc_object_impl * object, const void* key))
RTPC_METHOD(enumerate_certs, 3, (rt_pc_object_impl * object, rt_pc_pkcs11_cert * *cert_list, size_t * cert_count))
RTPC_METHOD(import_cert, 6, (rt_pc_object_impl * object, const void* value, size_t value_length, rt_pc_cert_form form,
                             bool trusted, rt_pc_pkcs11_cert * cert))
RTPC_METHOD(delete_cert, 2, (rt_pc_object_impl * object, const void* cert))
RTPC_CLASS_END(pkcs11_device)

RTPC_CLASS_START(x500_dn)
RTPC_CONSTRUCTOR(x500_dn, create, 1, (rt_pc_x500_dn * object))
RTPC_METHOD(set_rdn, 3, (rt_pc_object_impl * object, rt_pc_x500_rdn_nid nid, const char* value))
RTPC_METHOD(get_rdn, 3, (rt_pc_object_impl * object, rt_pc_x500_rdn_nid nid, char** value))
RTPC_METHOD(has_rdn, 3, (rt_pc_object_impl * object, rt_pc_x500_rdn_nid nid, bool* result))
RTPC_CLASS_END(x500_dn)

RTPC_CLASS_START(x509_cert_extension)
RTPC_METHOD(extension_type, 2, (rt_pc_object_impl * object, rt_pc_x509_cert_extension_type * value))
RTPC_METHOD(set_critical, 1, (rt_pc_object_impl * object))
RTPC_CLASS_END(x509_cert_extension)

RTPC_CLASS_START(x509_key_usage)
RTPC_CONSTRUCTOR(x509_key_usage, create, 1, (rt_pc_x509_key_usage * object))
RTPC_METHOD(add_option, 2, (rt_pc_object_impl * object, rt_pc_x509_key_usage_option option))
RTPC_CLASS_END(x509_key_usage)

RTPC_CLASS_START(x509_ext_key_usage)
RTPC_CONSTRUCTOR(x509_ext_key_usage, create, 1, (rt_pc_x509_ext_key_usage * object))
RTPC_METHOD(add_option, 2, (rt_pc_object_impl * object, rt_pc_x509_ext_key_usage_option option))
RTPC_METHOD(add_option_by_oid, 2, (rt_pc_object_impl * object, const char* oid))
RTPC_CLASS_END(x509_ext_key_usage)

RTPC_CLASS_START(x509_cert_policies)
RTPC_CONSTRUCTOR(x509_cert_policies, create, 1, (rt_pc_x509_cert_policies * object))
RTPC_METHOD(add_policy_by_oid, 2, (rt_pc_object_impl * object, const char* oid))
RTPC_CLASS_END(x509_cert_policies)

RTPC_CLASS_START(x509_subject_sign_tool)
RTPC_CONSTRUCTOR(x509_subject_sign_tool, create, 2, (rt_pc_x509_subject_sign_tool * object,
                                                     const char* value))
RTPC_CLASS_END(x509_subject_sign_tool)

RTPC_CLASS_START(pkcs10_reqinfo)
RTPC_CONSTRUCTOR(pkcs10_reqinfo, create, 1, (rt_pc_pkcs10_reqinfo * object))
RTPC_METHOD(set_subject, 2, (rt_pc_object_impl * object, const void* dn))
RTPC_METHOD(set_extension, 2, (rt_pc_object_impl * object, const void* extension))
RTPC_METHOD(set_custom_extension, 5, (rt_pc_object_impl * object, const char* oid,
                                      const void* data, size_t size, bool critical))
RTPC_METHOD(add_custom_attribute_value, 4, (rt_pc_object_impl * object, const char* oid, const void* data, size_t size))
RTPC_CLASS_END(pkcs10_reqinfo)

RTPC_CLASS_START(cms_message)
RTPC_METHOD(content_type, 2, (rt_pc_object_impl * object, rt_pc_cms_content_type * value))
RTPC_METHOD(to_ber, 3, (rt_pc_object_impl * object, uint8_t * *data, size_t * length))
RTPC_CLASS_END(cms_message)

RTPC_CLASS_START(cms_data)
RTPC_CONSTRUCTOR(cms_data, create, 3, (rt_pc_cms_data * object, const uint8_t * data, size_t length))
RTPC_METHOD(data, 3, (rt_pc_object_impl * object, uint8_t * *data, size_t * length))
RTPC_CLASS_END(cms_data)

RTPC_CLASS_START(cms_sign_params)
RTPC_CONSTRUCTOR(cms_sign_params, create, 2, (rt_pc_cms_sign_params * object, const rt_pc_pkcs11_cert * cert))
RTPC_METHOD(set_flags, 2, (rt_pc_object_impl * object, int flags))
RTPC_METHOD(add_cert, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_CLASS_END(cms_sign_params)

RTPC_CLASS_START(cms_verify_params)
RTPC_CONSTRUCTOR(cms_verify_params, create, 2, (rt_pc_cms_verify_params * object, const rt_pc_pkcs11_device * device))
RTPC_METHOD(set_flags, 2, (rt_pc_object_impl * object, int flags))
RTPC_METHOD(add_user_cert, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_METHOD(add_ca_cert, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_METHOD(add_crl, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_METHOD(set_data, 3, (rt_pc_object_impl * object, const void* data, size_t length))
RTPC_CLASS_END(cms_verify_params)

RTPC_CLASS_START(cms_signed_data)
RTPC_CONSTRUCTOR(cms_signed_data, parse, 3, (rt_pc_cms_signed_data * object, const uint8_t * data, size_t length))
RTPC_METHOD(assign, 3, (rt_pc_object_impl * object, const uint8_t * cms, size_t cms_length))
RTPC_CLASS_END(cms_signed_data)

RTPC_CLASS_START(cms_envelop_params)
RTPC_CONSTRUCTOR(cms_envelop_params, create, 2, (rt_pc_cms_envelop_params * object, const rt_pc_pkcs11_device * device))
RTPC_METHOD(set_flags, 2, (rt_pc_object_impl * object, int flags))
RTPC_METHOD(add_recipient, 2, (rt_pc_object_impl * object, const char* cert))
RTPC_CLASS_END(cms_envelop_params)

RTPC_CLASS_START(cms_decrypt_params)
RTPC_CONSTRUCTOR(cms_decrypt_params, create, 2, (rt_pc_cms_decrypt_params * object, const rt_pc_pkcs11_cert * cert))
RTPC_CLASS_END(cms_decrypt_params)

RTPC_CLASS_START(cms_enveloped_data)
RTPC_CONSTRUCTOR(cms_enveloped_data, parse, 3, (rt_pc_cms_enveloped_data * object, const uint8_t * data, size_t length))
RTPC_CLASS_END(cms_enveloped_data)

RTPC_STATIC_METHOD(cms_encrypt, 5, (const void* data, size_t data_length, const rt_pc_cms_envelop_params * params,
                                    unsigned char** cms, size_t * cms_length))
RTPC_STATIC_METHOD(cms_verify, 4, (const void* cms, size_t cms_length, const rt_pc_cms_verify_params * params,
                                   rt_pc_verify_result * result))
RTPC_STATIC_METHOD(cms_sign, 8, (const void* data, size_t data_length, const rt_pc_cms_sign_params * params, bool detached,
                                 const void* signed_data, size_t signed_data_length, unsigned char** cms, size_t * cms_length))
RTPC_STATIC_METHOD(cms_decrypt, 5, (const void* cms, size_t cms_length, const rt_pc_cms_decrypt_params * params,
                                    unsigned char** result, size_t * result_length))
RTPC_STATIC_METHOD(sign_pkcs10_request, 3, (const void* key, const void* request, char** result))
RTPC_STATIC_METHOD(sign, 5, (const void* key, const void* data, size_t data_length, unsigned char** sign, size_t * sign_length))

RTPC_STATIC_METHOD(enumerate_devices, 2, (rt_pc_pkcs11_device * *device_list, size_t * device_count))

#undef RTPC_STATIC_METHOD
#undef RTPC_CLASS_END
#undef RTPC_METHOD
#undef RTPC_CONSTRUCTOR
#undef RTPC_CLASS_START


#define RTPC_STATIC_METHOD(name, count, args) \
	int rt_pc_ ## name args;
#ifndef RTPC_CLASS_START
#define RTPC_CLASS_START(DUMMY)
#endif

#ifndef RTPC_CONSTRUCTOR
#define RTPC_CONSTRUCTOR(DUMMY1, DUMMY2, DUMMY3, DUMMY4)
#endif

#ifndef RTPC_METHOD
#define RTPC_METHOD(DUMMY1, DUMMY2, DUMMY3)
#endif

#ifndef RTPC_CLASS_END
#define RTPC_CLASS_END(DUMMY)
#endif

#ifndef RTPC_STATIC_METHOD
#define RTPC_STATIC_METHOD(DUMMY1, DUMMY2, DUMMY3)
#endif

RTPC_CLASS_START(public_key)
RTPC_METHOD(to_der, 3, (rt_pc_object_impl * object, uint8_t * *result, size_t * result_length))
RTPC_CLASS_END(public_key)

RTPC_CLASS_START(pkcs11_private_key)
RTPC_METHOD(get_public_key, 2, (rt_pc_object_impl * object, rt_pc_public_key * public_key))
RTPC_METHOD(get_id, 3, (rt_pc_object_impl * object, unsigned char** id, size_t * length))
RTPC_CLASS_END(pkcs11_private_key)

RTPC_CLASS_START(cert)
RTPC_METHOD(to_pem, 2, (rt_pc_object_impl * object, char** result))
RTPC_METHOD(get_subject, 2, (rt_pc_object_impl * object, rt_pc_x500_dn * subject))
RTPC_METHOD(get_serial_number, 3, (rt_pc_object_impl * object, uint8_t * *result, size_t * result_length))
RTPC_CLASS_END(cert)

RTPC_CLASS_START(external_cert)
RTPC_CONSTRUCTOR(external_cert, create_from_der, 3, (rt_pc_external_cert * object, const void* data, size_t length))
RTPC_CONSTRUCTOR(external_cert, create_from_pem, 2, (rt_pc_external_cert * object, const char* pem))
RTPC_CLASS_END(external_cert)

RTPC_CLASS_START(pkcs11_cert)
RTPC_METHOD(is_trusted, 2, (rt_pc_object_impl * object, bool* trusted))
RTPC_METHOD(get_corresponding_private_key, 2, (rt_pc_object_impl * object, rt_pc_pkcs11_private_key * key))
RTPC_CLASS_END(pkcs11_cert)

RTPC_CLASS_START(pkcs11_device)
RTPC_METHOD(get_label, 2, (rt_pc_object_impl * object, char** label))
RTPC_METHOD(get_serial_number, 2, (rt_pc_object_impl * object, char** serial_number))
RTPC_METHOD(is_logged_in, 2, (rt_pc_object_impl * object, bool* is_logged_in))
RTPC_METHOD(is_pin_default, 2, (rt_pc_object_impl * object, bool* is_pin_default))
RTPC_METHOD(is_format_supported, 3, (rt_pc_object_impl * object, rt_pc_data_format format, bool* result))
RTPC_METHOD(is_feature_supported, 3, (rt_pc_object_impl * object, rt_pc_device_feature feature, bool* result))
RTPC_METHOD(login, 2, (rt_pc_object_impl * object, const char* pin))
RTPC_METHOD(logout, 1, (rt_pc_object_impl * object))
RTPC_METHOD(change_pin, 3, (rt_pc_object_impl * object, const char* old_pin, const char* new_pin))
RTPC_METHOD(enumerate_keys, 4, (rt_pc_object_impl * object, const rt_pc_key_enum_params * params,
                                rt_pc_pkcs11_private_key * *key_list, size_t * key_count))
RTPC_METHOD(generate_key_pair, 3, (rt_pc_object_impl * object, const void* params, rt_pc_pkcs11_private_key * key))
RTPC_METHOD(delete_key_pair, 2, (rt_pc_object_impl * object, const void* key))
RTPC_METHOD(enumerate_certs, 3, (rt_pc_object_impl * object, rt_pc_pkcs11_cert * *cert_list, size_t * cert_count))
RTPC_METHOD(import_cert, 6, (rt_pc_object_impl * object, const void* value, size_t value_length, rt_pc_cert_form form,
                             bool trusted, rt_pc_pkcs11_cert * cert))
RTPC_METHOD(delete_cert, 2, (rt_pc_object_impl * object, const void* cert))
RTPC_CLASS_END(pkcs11_device)

RTPC_CLASS_START(x500_dn)
RTPC_CONSTRUCTOR(x500_dn, create, 1, (rt_pc_x500_dn * object))
RTPC_METHOD(set_rdn, 3, (rt_pc_object_impl * object, rt_pc_x500_rdn_nid nid, const char* value))
RTPC_METHOD(get_rdn, 3, (rt_pc_object_impl * object, rt_pc_x500_rdn_nid nid, char** value))
RTPC_METHOD(has_rdn, 3, (rt_pc_object_impl * object, rt_pc_x500_rdn_nid nid, bool* result))
RTPC_CLASS_END(x500_dn)

RTPC_CLASS_START(x509_cert_extension)
RTPC_METHOD(extension_type, 2, (rt_pc_object_impl * object, rt_pc_x509_cert_extension_type * value))
RTPC_METHOD(set_critical, 1, (rt_pc_object_impl * object))
RTPC_CLASS_END(x509_cert_extension)

RTPC_CLASS_START(x509_key_usage)
RTPC_CONSTRUCTOR(x509_key_usage, create, 1, (rt_pc_x509_key_usage * object))
RTPC_METHOD(add_option, 2, (rt_pc_object_impl * object, rt_pc_x509_key_usage_option option))
RTPC_CLASS_END(x509_key_usage)

RTPC_CLASS_START(x509_ext_key_usage)
RTPC_CONSTRUCTOR(x509_ext_key_usage, create, 1, (rt_pc_x509_ext_key_usage * object))
RTPC_METHOD(add_option, 2, (rt_pc_object_impl * object, rt_pc_x509_ext_key_usage_option option))
RTPC_METHOD(add_option_by_oid, 2, (rt_pc_object_impl * object, const char* oid))
RTPC_CLASS_END(x509_ext_key_usage)

RTPC_CLASS_START(x509_cert_policies)
RTPC_CONSTRUCTOR(x509_cert_policies, create, 1, (rt_pc_x509_cert_policies * object))
RTPC_METHOD(add_policy_by_oid, 2, (rt_pc_object_impl * object, const char* oid))
RTPC_CLASS_END(x509_cert_policies)

RTPC_CLASS_START(x509_subject_sign_tool)
RTPC_CONSTRUCTOR(x509_subject_sign_tool, create, 2, (rt_pc_x509_subject_sign_tool * object,
                                                     const char* value))
RTPC_CLASS_END(x509_subject_sign_tool)

RTPC_CLASS_START(pkcs10_reqinfo)
RTPC_CONSTRUCTOR(pkcs10_reqinfo, create, 1, (rt_pc_pkcs10_reqinfo * object))
RTPC_METHOD(set_subject, 2, (rt_pc_object_impl * object, const void* dn))
RTPC_METHOD(set_extension, 2, (rt_pc_object_impl * object, const void* extension))
RTPC_METHOD(set_custom_extension, 5, (rt_pc_object_impl * object, const char* oid,
                                      const void* data, size_t size, bool critical))
RTPC_METHOD(add_custom_attribute_value, 4, (rt_pc_object_impl * object, const char* oid, const void* data, size_t size))
RTPC_CLASS_END(pkcs10_reqinfo)

RTPC_CLASS_START(cms_message)
RTPC_METHOD(content_type, 2, (rt_pc_object_impl * object, rt_pc_cms_content_type * value))
RTPC_METHOD(to_ber, 3, (rt_pc_object_impl * object, uint8_t * *data, size_t * length))
RTPC_CLASS_END(cms_message)

RTPC_CLASS_START(cms_data)
RTPC_CONSTRUCTOR(cms_data, create, 3, (rt_pc_cms_data * object, const uint8_t * data, size_t length))
RTPC_METHOD(data, 3, (rt_pc_object_impl * object, uint8_t * *data, size_t * length))
RTPC_CLASS_END(cms_data)

RTPC_CLASS_START(cms_sign_params)
RTPC_CONSTRUCTOR(cms_sign_params, create, 2, (rt_pc_cms_sign_params * object, const rt_pc_pkcs11_cert * cert))
RTPC_METHOD(set_flags, 2, (rt_pc_object_impl * object, int flags))
RTPC_METHOD(add_cert, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_CLASS_END(cms_sign_params)

RTPC_CLASS_START(cms_verify_params)
RTPC_CONSTRUCTOR(cms_verify_params, create, 2, (rt_pc_cms_verify_params * object, const rt_pc_pkcs11_device * device))
RTPC_METHOD(set_flags, 2, (rt_pc_object_impl * object, int flags))
RTPC_METHOD(add_user_cert, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_METHOD(add_ca_cert, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_METHOD(add_crl, 2, (rt_pc_object_impl * object, const char* pem))
RTPC_METHOD(set_data, 3, (rt_pc_object_impl * object, const void* data, size_t length))
RTPC_CLASS_END(cms_verify_params)

RTPC_CLASS_START(cms_signed_data)
RTPC_CONSTRUCTOR(cms_signed_data, parse, 3, (rt_pc_cms_signed_data * object, const uint8_t * data, size_t length))
RTPC_METHOD(assign, 3, (rt_pc_object_impl * object, const uint8_t * cms, size_t cms_length))
RTPC_CLASS_END(cms_signed_data)

RTPC_CLASS_START(cms_envelop_params)
RTPC_CONSTRUCTOR(cms_envelop_params, create, 2, (rt_pc_cms_envelop_params * object, const rt_pc_pkcs11_device * device))
RTPC_METHOD(set_flags, 2, (rt_pc_object_impl * object, int flags))
RTPC_METHOD(add_recipient, 2, (rt_pc_object_impl * object, const char* cert))
RTPC_CLASS_END(cms_envelop_params)

RTPC_CLASS_START(cms_decrypt_params)
RTPC_CONSTRUCTOR(cms_decrypt_params, create, 2, (rt_pc_cms_decrypt_params * object, const rt_pc_pkcs11_cert * cert))
RTPC_CLASS_END(cms_decrypt_params)

RTPC_CLASS_START(cms_enveloped_data)
RTPC_CONSTRUCTOR(cms_enveloped_data, parse, 3, (rt_pc_cms_enveloped_data * object, const uint8_t * data, size_t length))
RTPC_CLASS_END(cms_enveloped_data)

RTPC_STATIC_METHOD(cms_encrypt, 5, (const void* data, size_t data_length, const rt_pc_cms_envelop_params * params,
                                    unsigned char** cms, size_t * cms_length))
RTPC_STATIC_METHOD(cms_verify, 4, (const void* cms, size_t cms_length, const rt_pc_cms_verify_params * params,
                                   rt_pc_verify_result * result))
RTPC_STATIC_METHOD(cms_sign, 8, (const void* data, size_t data_length, const rt_pc_cms_sign_params * params, bool detached,
                                 const void* signed_data, size_t signed_data_length, unsigned char** cms, size_t * cms_length))
RTPC_STATIC_METHOD(cms_decrypt, 5, (const void* cms, size_t cms_length, const rt_pc_cms_decrypt_params * params,
                                    unsigned char** result, size_t * result_length))
RTPC_STATIC_METHOD(sign_pkcs10_request, 3, (const void* key, const void* request, char** result))
RTPC_STATIC_METHOD(sign, 5, (const void* key, const void* data, size_t data_length, unsigned char** sign, size_t * sign_length))

RTPC_STATIC_METHOD(enumerate_devices, 2, (rt_pc_pkcs11_device * *device_list, size_t * device_count))

#undef RTPC_STATIC_METHOD
#undef RTPC_CLASS_END
#undef RTPC_METHOD
#undef RTPC_CONSTRUCTOR
#undef RTPC_CLASS_START


#ifdef __cplusplus
}
#endif

#endif // RT_PKI_CORE_H


#define DERIVE_OBJECT(name, coreName, parentName) \
public: \
	static name cast(Object && v) \
	{ \
		return name(std::move(v)); \
	} \
    \
	name(name && v) \
		: parentName(std::move(v)) \
	{ \
		m_object = v.m_object; \
	} \
    \
	name& operator=(name&& v) \
	{ \
		parentName::operator=(std::move(v)); \
		m_object = v.m_object; \
		return *this; \
	} \
    \
protected: \
	/*! @cond INTERNAL */ \
	name(Object && v) \
		: parentName(std::move(v)) \
		, m_object(rt_pc_ ## coreName ## _cast(parentName::m_object.object)) \
	{} \
    \
	void construct(rt_pc_object_impl * object) \
	{ \
		parentName::construct(object); \
		m_object = rt_pc_ ## coreName ## _cast(object); \
	} \
    \
	rt_pc_ ## coreName m_object; \
	/*! @endcond */ \
private:
/*! \brief Пространство имен, содержащее сущности, разработанные подразделением Рутокен компании "Актив".
 *
 */
namespace rutoken
{
/*! \brief Пространство имен, содержащее сущности, разработанные в рамках проекта pki-core.
 *
 */
namespace pkicore
{
/// @cond INTERNAL
namespace internal
{
struct Crypto;

class Noncopyable
{
protected:
	Noncopyable() {}

private:
	Noncopyable(const Noncopyable&);
	Noncopyable& operator=(const Noncopyable&);
};

class MemoryHolder: Noncopyable
{
private:
	void* m_ptr;
public:
	explicit MemoryHolder(void* ptr)
		: m_ptr(ptr) {}
	~MemoryHolder()
	{
		rt_pc_free_memory(m_ptr);
	}
};

#define FREE_ON_EXIT(pointer) \
	internal::MemoryHolder rtPcMemoryHolder ## pointer(pointer)

template<typename T>
class ObjectArrayHolder: Noncopyable
{
private:
	T* m_ptr;
	size_t m_size;
public:
	explicit ObjectArrayHolder(T* ptr, size_t size)
		: m_ptr(ptr), m_size(size) {}
	T* begin() { return m_ptr; }
	T* end() { return m_ptr + m_size; }
	~ObjectArrayHolder()
	{
		for (auto object = begin(); object != end(); ++object)
		{
			if (object->object)
				rt_pc_object_destruct(object->object);
		}
		rt_pc_free_memory(m_ptr);
	}
};
}
/// @endcond

namespace error
{
class Exception: public std::exception
{
public:
	virtual const char* what() const throw() = 0;
};

#define ERROR_DECLARATION(NAME, DUMMY, DUMMY2, DESC) \
	class NAME ## Exception: public Exception \
	{ \
public: \
		virtual const char* what() const throw() { return DESC; \
		}; \
	};

ERROR_DECLARATION(BadParams, bad_params, 2, "Bad parameters")
ERROR_DECLARATION(DeviceNotFound, device_not_found, 3, "No device has been found")
ERROR_DECLARATION(WrongCertificateCategory, certificate_category_bad, 5, "Wrong certificate category")
ERROR_DECLARATION(CertificateExists, certificate_exists, 6, "Certificate already exists")
ERROR_DECLARATION(PinChanged, pin_changed, 8, "PIN has been changed unexpectedly")
ERROR_DECLARATION(InvalidSession, session_invalid, 9, "Session state been changed unexpectedly")

ERROR_DECLARATION(KeyLabelNotUnique, key_label_not_unique, 10, "Key label is not unique")
ERROR_DECLARATION(InternalError, internal_error, 11, "Internal error")
ERROR_DECLARATION(NoCaCertificatesFound, ca_certificates_not_found, 12, "No CA certificates found")
ERROR_DECLARATION(CertificateVerification, certificate_verification_error, 13, "Certificate verification failed")
ERROR_DECLARATION(Pkcs11ModuleLoad, pkcs11_load_module_error, 14, "Failed to load pkcs#11 module")
ERROR_DECLARATION(NotEnoughMemory, not_enough_memory, 15, "Not enough memory")
ERROR_DECLARATION(InvalidPinLength, pin_length_invalid, 16, "PIN length is invalid")
ERROR_DECLARATION(IncorrectPin, pin_incorrect, 17, "PIN is incorrect")
ERROR_DECLARATION(PinLocked, pin_locked, 18, "PIN is locked")
ERROR_DECLARATION(NotLoggedIn, user_not_logged_in, 19, "User login required to perform an operation")

ERROR_DECLARATION(KeyNotFound, key_not_found, 20, "No key has been found")
ERROR_DECLARATION(KeyIdNotUnique, key_id_not_unique, 21, "Key id is not unique")
ERROR_DECLARATION(CertificateNotFound, certificate_not_found, 22, "No certificate has been found")
ERROR_DECLARATION(CertificateHashNotUnique, certificate_hash_not_unique, 23, "Certificate hash is not unique")
ERROR_DECLARATION(InvalidToken, token_invalid, 24, "Token state has been changed unexpectedly")
ERROR_DECLARATION(WrongKeyType, wrong_key_type, 25, "Key type is not supported in operation")
ERROR_DECLARATION(DeviceError, device_error, 26, "Token failed to perform operation")
ERROR_DECLARATION(InvalidData, data_invalid, 27, "Data provided is invalid")
ERROR_DECLARATION(UnsupportedByToken, unsupported_by_token, 28, "Unsupported by token")
ERROR_DECLARATION(KeyFunctionNotPermitted, key_function_not_permitted, 29, "Key is not permitted to be used in operation")

ERROR_DECLARATION(FunctionRejected, function_rejected, 30, "Function processing rejected by user")
ERROR_DECLARATION(FunctionFailed, function_failed, 31, "Function failed")
ERROR_DECLARATION(Base64Decode, base64_decode_error, 32, "Error while decoding base64")
ERROR_DECLARATION(Asn1Error, asn1_err, 33, "Something wrong happened when processing ASN1 structures")
ERROR_DECLARATION(PemError, pem_err, 34, "Something wrong happened when processing PEM-encoded data")
ERROR_DECLARATION(Base64Encode, base64_encode_error, 35, "Error while encoding base64")
ERROR_DECLARATION(X509UnableToGetIssuerCert, x509_unable_to_get_issuer_cert, 36, "unable to get issuer certificate")
ERROR_DECLARATION(X509UnableToGetCrl, x509_unable_to_get_crl, 37, "unable to get certificate CRL")
ERROR_DECLARATION(X509UnableToDecryptCertSignature, x509_unable_to_decrypt_cert_signature, 38, "unable to decrypt certificate's signature")
ERROR_DECLARATION(X509UnableToDecryptCrlSignature, x509_unable_to_decrypt_crl_signature, 39, "unable to decrypt CRL's signature")
ERROR_DECLARATION(X509UnableToDecodeIssuerPublicKey, x509_unable_to_decode_issuer_public_key, 40, "unable to decode issuer public key")
ERROR_DECLARATION(X509CertSignatureFailure, x509_cert_signature_failure, 41, "certificate signature failure")
ERROR_DECLARATION(X509CrlSignatureFailure, x509_crl_signature_failure, 42, "CRL signature failure")
ERROR_DECLARATION(X509CertNotYetValid, x509_cert_not_yet_valid, 43, "certificate is not yet valid")
ERROR_DECLARATION(X509CrlNotYetValid, x509_crl_not_yet_valid, 44, "CRL is not yet valid")
ERROR_DECLARATION(X509CertHasExpired, x509_cert_has_expired, 45, "certificate has expired")
ERROR_DECLARATION(X509CrlHasExpired, x509_crl_has_expired, 46, "CRL has expired")
ERROR_DECLARATION(X509ErrorInCertNotBeforeField, x509_error_in_cert_not_before_field, 47, "format error in certificate's notBefore field")
ERROR_DECLARATION(X509ErrorInCertNotAfterField, x509_error_in_cert_not_after_field, 48, "format error in certificate's notAfter field")
ERROR_DECLARATION(X509ErrorInCrlLastUpdateField, x509_error_in_crl_last_update_field, 49, "format error in CRL's lastUpdate field")
ERROR_DECLARATION(X509ErrorInCrlNextUpdateField, x509_error_in_crl_next_update_field, 50, "format error in CRL's nextUpdate field")
ERROR_DECLARATION(X509OutOfMem, x509_out_of_mem, 51, "out of memory")
ERROR_DECLARATION(X509DepthZeroSelfSignedCert, x509_depth_zero_self_signed_cert, 52, "self signed certificate")
ERROR_DECLARATION(X509SelfSignedCertInChain, x509_self_signed_cert_in_chain, 53, "self signed certificate in certificate chain")
ERROR_DECLARATION(X509UnableToGetIssuerCertLocally, x509_unable_to_get_issuer_cert_locally, 54, "unable to get local issuer certificate")
ERROR_DECLARATION(X509UnableToVerifyLeafSignature, x509_unable_to_verify_leaf_signature, 55, "unable to verify the first certificate")
ERROR_DECLARATION(X509CertChainTooLong, x509_cert_chain_too_long, 56, "certificate chain too long")
ERROR_DECLARATION(X509CertRevoked, x509_cert_revoked, 57, "certificate revoked")
ERROR_DECLARATION(X509InvalidCa, x509_invalid_ca, 58, "invalid CA certificate")
ERROR_DECLARATION(X509InvalidNonCa, x509_invalid_non_ca, 59, "invalid non-CA certificate (has CA markings)")
ERROR_DECLARATION(X509PathLengthExceeded, x509_path_length_exceeded, 60, "path length constraint exceeded")
ERROR_DECLARATION(X509ProxyPathLengthExceeded, x509_proxy_path_length_exceeded, 61, "proxy path length constraint exceeded")
ERROR_DECLARATION(X509ProxyCertificatesNotAllowed, x509_proxy_certificates_not_allowed, 62, "proxy certificates not allowed, please set the appropriate flag")
ERROR_DECLARATION(X509InvalidPurpose, x509_invalid_purpose, 63, "unsupported certificate purpose")
ERROR_DECLARATION(X509CertUntrusted, x509_cert_untrusted, 64, "certificate not trusted")
ERROR_DECLARATION(X509CertRejected, x509_cert_rejected, 65, "certificate rejected")
ERROR_DECLARATION(X509ApplicationVerification, x509_application_verification, 66, "application verification failure")
ERROR_DECLARATION(X509SubjectIssuerMismatch, x509_subject_issuer_mismatch, 67, "subject issuer mismatch")
ERROR_DECLARATION(X509AkidSkidMismatch, x509_akid_skid_mismatch, 68, "authority and subject key identifier mismatch")
ERROR_DECLARATION(X509AkidIssuerSerialMismatch, x509_akid_issuer_serial_mismatch, 69, "authority and issuer serial number mismatch")
ERROR_DECLARATION(X509KeyusageNoCertsign, x509_keyusage_no_certsign, 70, "key usage does not include certificate signing")
ERROR_DECLARATION(X509UnableToGetCrlIssuer, x509_unable_to_get_crl_issuer, 71, "unable to get CRL issuer certificate")
ERROR_DECLARATION(X509UnhandledCriticalExtension, x509_unhandled_critical_extension, 72, "unhandled critical extension")
ERROR_DECLARATION(X509KeyusageNoCrlSign, x509_keyusage_no_crl_sign, 73, "key usage does not include CRL signing")
ERROR_DECLARATION(X509KeyusageNoDigitalSignature, x509_keyusage_no_digital_signature, 74, "key usage does not include digital signature")
ERROR_DECLARATION(X509UnhandledCriticalCrlExtension, x509_unhandled_critical_crl_extension, 75, "unhandled critical CRL extension")
ERROR_DECLARATION(X509InvalidExtension, x509_invalid_extension, 76, "invalid or inconsistent certificate extension")
ERROR_DECLARATION(X509InvalidPolicyExtension, x509_invalid_policy_extension, 77, "invalid or inconsistent certificate policy extension")
ERROR_DECLARATION(X509NoExplicitPolicy, x509_no_explicit_policy, 78, "no explicit policy")
ERROR_DECLARATION(X509DifferentCrlScope, x509_different_crl_scope, 79, "Different CRL scope")
ERROR_DECLARATION(X509UnsupportedExtensionFeature, x509_unsupported_extension_feature, 80, "Unsupported extension feature")
ERROR_DECLARATION(X509UnnestedResource, x509_unnested_resource, 81, "RFC 3779 resource not subset of parent's resources")
ERROR_DECLARATION(X509PermittedViolation, x509_permitted_violation, 82, "permitted subtree violation")
ERROR_DECLARATION(X509ExcludedViolation, x509_excluded_violation, 83, "excluded subtree violation")
ERROR_DECLARATION(X509SubtreeMinmax, x509_subtree_minmax, 84, "name constraints minimum and maximum not supported")
ERROR_DECLARATION(X509UnsupportedConstraintType, x509_unsupported_constraint_type, 85, "unsupported name constraint type")
ERROR_DECLARATION(X509UnsupportedConstraintSyntax, x509_unsupported_constraint_syntax, 86, "unsupported or invalid name constraint syntax")
ERROR_DECLARATION(X509UnsupportedNameSyntax, x509_unsupported_name_syntax, 87, "unsupported or invalid name syntax")
ERROR_DECLARATION(X509CrlPathValidationError, x509_crl_path_validation_error, 88, "CRL path validation error")

ERROR_DECLARATION(AttributeReadOnly, attribute_read_only, 89, "Object attribute can not be changed")
ERROR_DECLARATION(CannotSavePinInCache, cannot_save_pin_in_cache, 90, "Cannot save pin in cache")
ERROR_DECLARATION(CMSCertificateAlreadyPresent, cms_certificate_already_present, 91, "certificate already present")
ERROR_DECLARATION(RdnNotFound, rdn_not_found, 92, "rdn of this type not found")
ERROR_DECLARATION(AlreadyLoggedIn, already_logged_in, 93, "Login has already been performed")
ERROR_DECLARATION(CekNotAuthentic, cek_not_authentic, 94, "CEK not authentic")
ERROR_DECLARATION(LicenceReadOnly, licence_read_only, 95, "Licence can not be changed")

ERROR_DECLARATION(AttributeSensitive, attribute_sensitive, 96, "pkcs11 attribute sensitive")
ERROR_DECLARATION(AttributeTypeInvalid, attribute_type_invalid, 97, "pkcs11 attribute type invalid")
ERROR_DECLARATION(AttributeValueInvalid, attribute_value_invalid, 98, "pkcs11 attribute value invalid")
ERROR_DECLARATION(BufferTooSmall, buffer_too_small, 99, "pkcs11 buffer too small")
ERROR_DECLARATION(Pkcs11Cancel, pkcs11_cancel, 100, "pkcs11 cancel")
ERROR_DECLARATION(Pkcs11CantLock, cant_lock, 101, "pkcs11 cant lock")
ERROR_DECLARATION(CryptokiAlreadyInitialized, cryptoki_already_initialized, 102, "pkcs11 cryptoki already initialized")
ERROR_DECLARATION(DataLenRange, data_len_range, 103, "pkcs11 data len range")
ERROR_DECLARATION(FunctionNotParallel, function_not_parallel, 104, "pkcs11 function not parallel")
ERROR_DECLARATION(Pkcs11GeneralError, pkcs11_general_error, 105, "pkcs11 general error")
ERROR_DECLARATION(Pkcs11InformationSensitive, information_sensitive, 106, "pkcs11 information sensitive")
ERROR_DECLARATION(Pkcs11DeviceRemoved, pkcs11_device_removed, 107, "pkcs11 device removed")
ERROR_DECLARATION(Pkcs11DomainParamsInvalid, pkcs11_domain_params_invalid, 108, "pkcs11 domain params invalid")
ERROR_DECLARATION(EncryptedDataInvalid, encrypted_data_invalid, 109, "pkcs11 encrypted data invalid")
ERROR_DECLARATION(EncryptedDataLenRange, encrypted_data_len_range, 110, "pkcs11 encrypted data len range")
ERROR_DECLARATION(FunctionCanceled, function_canceled, 111, "pkcs11 function canceled")
ERROR_DECLARATION(KeyHandleInvalid, key_handle_invalid, 112, "pkcs11 key handle invalid")
ERROR_DECLARATION(KeyIndigestible, key_indigestible, 113, "pkcs11 key indigestible")
ERROR_DECLARATION(KeyNeeded, key_needed, 114, "pkcs11 key needed")
ERROR_DECLARATION(KeyNotNeeded, key_not_needed, 115, "pkcs11 key not needed")
ERROR_DECLARATION(KeyNotWrappable, key_not_wrappable, 116, "pkcs11 key not wrappable")
ERROR_DECLARATION(KeySizeRange, key_size_range, 117, "pkcs11 key size range")
ERROR_DECLARATION(KeyUnextractable, key_unextractable, 118, "pkcs11 key unextractable")
ERROR_DECLARATION(SessionClosed, session_closed, 119, "pkcs11 session closed")
ERROR_DECLARATION(SessionCount, session_count, 120, "pkcs11 session count")
ERROR_DECLARATION(SessionExists, session_exists, 121, "pkcs11 session exists")
ERROR_DECLARATION(PinExpired, pin_expired, 122, "pkcs11 pin expired")
ERROR_DECLARATION(PinInvalid, pin_invalid, 123, "pkcs11 pin invalid")
ERROR_DECLARATION(Pkcs11RandomNoRng, random_no_rng, 124, "pkcs11 random no rng")
ERROR_DECLARATION(Pkcs11RandomSeedNotSupported, random_seed_not_supported, 125, "pkcs11 random seed not supported")
ERROR_DECLARATION(Pkcs11SavedStateInvalid, saved_state_invalid, 126, "pkcs11 saved state invalid")
ERROR_DECLARATION(UserPinNotInitialized, user_pin_not_initialized, 127, "pkcs11 user pin not initialized")
ERROR_DECLARATION(UserTooManyTypes, user_too_many_types, 128, "pkcs11 user too many types")
ERROR_DECLARATION(UserTypeInvalid, user_type_invalid, 129, "pkcs11 user type invalid")
ERROR_DECLARATION(TokenNotPresent, token_not_present, 130, "pkcs11 token not present")
ERROR_DECLARATION(TokenNotRecognized, token_not_recognized, 131, "pkcs11 token not recognized")
ERROR_DECLARATION(TokenWriteProtected, token_write_protected, 132, "pkcs11 token write protected")
ERROR_DECLARATION(UnknownKeyType, unknown_key_type, 133, "unknown key type")
ERROR_DECLARATION(UnknownOid, unknown_oid, 134, "unknown oid")
ERROR_DECLARATION(WrongBigNumber, wrong_big_number, 135, "wrong big number")
ERROR_DECLARATION(DidnotFindSession, didnot_find_session, 136, "unable to find any session")
ERROR_DECLARATION(SessionParallelNotSupported, session_parallel_not_supported, 137, "pkcs11 session parallel not supported")
ERROR_DECLARATION(SessionReadOnly, session_read_only, 138, "pkcs11 session read only")
ERROR_DECLARATION(SessionReadOnlyExists, session_read_only_exists, 139, "pkcs11 session read only exists")
ERROR_DECLARATION(SessionReadWriteSoExists, session_read_write_so_exists, 140, "pkcs11 session read write so exists")
ERROR_DECLARATION(Pkcs11MutexBad, mutex_bad, 141, "pkcs11 mutex bad")
ERROR_DECLARATION(Pkcs11MutexNotLocked, mutex_not_locked, 142, "pkcs11 mutex not locked")
ERROR_DECLARATION(Pkcs11ObjectHandleInvalid, object_handle_invalid, 143, "pkcs11 object handle invalid")
ERROR_DECLARATION(Pkcs11Ok, pkcs11_ok, 144, "pkcs11 ok")
ERROR_DECLARATION(Pkcs11OperationActive, operation_active, 145, "pkcs11 operation active")
ERROR_DECLARATION(Pkcs11OperationNotInitialized, operation_not_initialized, 146, "pkcs11 operation not initialized")
ERROR_DECLARATION(MechanismInvalid, mechanism_invalid, 147, "pkcs11 mechanism invalid")
ERROR_DECLARATION(MechanismParamInvalid, mechanism_param_invalid, 148, "pkcs11 mechanism param invalid")
ERROR_DECLARATION(SignatureInvalid, signature_invalid, 149, "pkcs11 signature invalid")
ERROR_DECLARATION(SignatureLenRange, signature_len_range, 150, "pkcs11 signature len range")
ERROR_DECLARATION(SlotIdInvalid, slot_id_invalid, 151, "pkcs11 slot id invalid")
ERROR_DECLARATION(Pkcs11StateUnsaveable, state_unsaveable, 152, "pkcs11 state unsaveable")
ERROR_DECLARATION(TemplateIncomplete, template_incomplete, 153, "pkcs11 template incomplete")
ERROR_DECLARATION(UserAnotherAlreadyLoggedIn, user_another_already_logged_in, 154, "pkcs11 user another already logged in")
ERROR_DECLARATION(UnwrappingKeyHandleInvalid, unwrapping_key_handle_invalid, 155, "pkcs11 unwrapping key handle invalid")
ERROR_DECLARATION(UnwrappingKeySizeRange, unwrapping_key_size_range, 156, "pkcs11 unwrapping key size range")
ERROR_DECLARATION(UnwrappingKeyTypeInconsistent, unwrapping_key_type_inconsistent, 157, "pkcs11 unwrapping key type inconsistent")
ERROR_DECLARATION(WrappedKeyLenRange, wrapped_key_len_range, 158, "pkcs11 wrapped key len range")
ERROR_DECLARATION(WrappingKeyHandleInvalid, wrapping_key_handle_invalid, 159, "pkcs11 wrapping key handle invalid")
ERROR_DECLARATION(WrappingKeySizeRange, wrapping_key_size_range, 160, "pkcs11 wrapping key size range")
ERROR_DECLARATION(WrappingKeyTypeInconsistent, wrapping_key_type_inconsistent, 161, "pkcs11 wrapping key type inconsistent")
ERROR_DECLARATION(Pkcs11NeedToCreateThreads, need_to_create_threads, 162, "pkcs11 need to create threads")
ERROR_DECLARATION(Pkcs11NewPinMode, new_pin_mode, 163, "pkcs11 new pin mode")
ERROR_DECLARATION(Pkcs11NextOtp, next_otp, 164, "pkcs11 next otp")
ERROR_DECLARATION(Pkcs11NoEvent, no_event, 165, "pkcs11 no event")
ERROR_DECLARATION(CantHardwareVerifyCMS, cant_hardware_verify_cms, 166, "Multitple signed CMS with hardware verification is not supported")
ERROR_DECLARATION(DecryptUnsuccessful, decrypt_unsuccessful, 167, "Decrypt operation was unsuccessful")


#undef ERROR_DECLARATION
}

/// @cond INTERNAL
namespace internal
{
class UnknownErrorCodeException: public error::Exception
{
	std::string description;
public:
	explicit UnknownErrorCodeException(int errorCode)
	{
		std::ostringstream stream;
		stream << "Unknown error with code " << std::dec << errorCode;
		description = stream.str();
	}
	virtual ~UnknownErrorCodeException() throw() {}
	virtual const char* what() const throw() { return description.c_str(); }
};

inline void checkReturnCode(int errorCode) {
	switch (errorCode)
	{
		case 0: return;
#define ERROR_DECLARATION(NAME, CODE, DUMMY, DUMMY2) \
	case rt_pc_e_ ## CODE: throw error:: NAME ## Exception();
ERROR_DECLARATION(BadParams, bad_params, 2, "Bad parameters")
ERROR_DECLARATION(DeviceNotFound, device_not_found, 3, "No device has been found")
ERROR_DECLARATION(WrongCertificateCategory, certificate_category_bad, 5, "Wrong certificate category")
ERROR_DECLARATION(CertificateExists, certificate_exists, 6, "Certificate already exists")
ERROR_DECLARATION(PinChanged, pin_changed, 8, "PIN has been changed unexpectedly")
ERROR_DECLARATION(InvalidSession, session_invalid, 9, "Session state been changed unexpectedly")

ERROR_DECLARATION(KeyLabelNotUnique, key_label_not_unique, 10, "Key label is not unique")
ERROR_DECLARATION(InternalError, internal_error, 11, "Internal error")
ERROR_DECLARATION(NoCaCertificatesFound, ca_certificates_not_found, 12, "No CA certificates found")
ERROR_DECLARATION(CertificateVerification, certificate_verification_error, 13, "Certificate verification failed")
ERROR_DECLARATION(Pkcs11ModuleLoad, pkcs11_load_module_error, 14, "Failed to load pkcs#11 module")
ERROR_DECLARATION(NotEnoughMemory, not_enough_memory, 15, "Not enough memory")
ERROR_DECLARATION(InvalidPinLength, pin_length_invalid, 16, "PIN length is invalid")
ERROR_DECLARATION(IncorrectPin, pin_incorrect, 17, "PIN is incorrect")
ERROR_DECLARATION(PinLocked, pin_locked, 18, "PIN is locked")
ERROR_DECLARATION(NotLoggedIn, user_not_logged_in, 19, "User login required to perform an operation")

ERROR_DECLARATION(KeyNotFound, key_not_found, 20, "No key has been found")
ERROR_DECLARATION(KeyIdNotUnique, key_id_not_unique, 21, "Key id is not unique")
ERROR_DECLARATION(CertificateNotFound, certificate_not_found, 22, "No certificate has been found")
ERROR_DECLARATION(CertificateHashNotUnique, certificate_hash_not_unique, 23, "Certificate hash is not unique")
ERROR_DECLARATION(InvalidToken, token_invalid, 24, "Token state has been changed unexpectedly")
ERROR_DECLARATION(WrongKeyType, wrong_key_type, 25, "Key type is not supported in operation")
ERROR_DECLARATION(DeviceError, device_error, 26, "Token failed to perform operation")
ERROR_DECLARATION(InvalidData, data_invalid, 27, "Data provided is invalid")
ERROR_DECLARATION(UnsupportedByToken, unsupported_by_token, 28, "Unsupported by token")
ERROR_DECLARATION(KeyFunctionNotPermitted, key_function_not_permitted, 29, "Key is not permitted to be used in operation")

ERROR_DECLARATION(FunctionRejected, function_rejected, 30, "Function processing rejected by user")
ERROR_DECLARATION(FunctionFailed, function_failed, 31, "Function failed")
ERROR_DECLARATION(Base64Decode, base64_decode_error, 32, "Error while decoding base64")
ERROR_DECLARATION(Asn1Error, asn1_err, 33, "Something wrong happened when processing ASN1 structures")
ERROR_DECLARATION(PemError, pem_err, 34, "Something wrong happened when processing PEM-encoded data")
ERROR_DECLARATION(Base64Encode, base64_encode_error, 35, "Error while encoding base64")
ERROR_DECLARATION(X509UnableToGetIssuerCert, x509_unable_to_get_issuer_cert, 36, "unable to get issuer certificate")
ERROR_DECLARATION(X509UnableToGetCrl, x509_unable_to_get_crl, 37, "unable to get certificate CRL")
ERROR_DECLARATION(X509UnableToDecryptCertSignature, x509_unable_to_decrypt_cert_signature, 38, "unable to decrypt certificate's signature")
ERROR_DECLARATION(X509UnableToDecryptCrlSignature, x509_unable_to_decrypt_crl_signature, 39, "unable to decrypt CRL's signature")
ERROR_DECLARATION(X509UnableToDecodeIssuerPublicKey, x509_unable_to_decode_issuer_public_key, 40, "unable to decode issuer public key")
ERROR_DECLARATION(X509CertSignatureFailure, x509_cert_signature_failure, 41, "certificate signature failure")
ERROR_DECLARATION(X509CrlSignatureFailure, x509_crl_signature_failure, 42, "CRL signature failure")
ERROR_DECLARATION(X509CertNotYetValid, x509_cert_not_yet_valid, 43, "certificate is not yet valid")
ERROR_DECLARATION(X509CrlNotYetValid, x509_crl_not_yet_valid, 44, "CRL is not yet valid")
ERROR_DECLARATION(X509CertHasExpired, x509_cert_has_expired, 45, "certificate has expired")
ERROR_DECLARATION(X509CrlHasExpired, x509_crl_has_expired, 46, "CRL has expired")
ERROR_DECLARATION(X509ErrorInCertNotBeforeField, x509_error_in_cert_not_before_field, 47, "format error in certificate's notBefore field")
ERROR_DECLARATION(X509ErrorInCertNotAfterField, x509_error_in_cert_not_after_field, 48, "format error in certificate's notAfter field")
ERROR_DECLARATION(X509ErrorInCrlLastUpdateField, x509_error_in_crl_last_update_field, 49, "format error in CRL's lastUpdate field")
ERROR_DECLARATION(X509ErrorInCrlNextUpdateField, x509_error_in_crl_next_update_field, 50, "format error in CRL's nextUpdate field")
ERROR_DECLARATION(X509OutOfMem, x509_out_of_mem, 51, "out of memory")
ERROR_DECLARATION(X509DepthZeroSelfSignedCert, x509_depth_zero_self_signed_cert, 52, "self signed certificate")
ERROR_DECLARATION(X509SelfSignedCertInChain, x509_self_signed_cert_in_chain, 53, "self signed certificate in certificate chain")
ERROR_DECLARATION(X509UnableToGetIssuerCertLocally, x509_unable_to_get_issuer_cert_locally, 54, "unable to get local issuer certificate")
ERROR_DECLARATION(X509UnableToVerifyLeafSignature, x509_unable_to_verify_leaf_signature, 55, "unable to verify the first certificate")
ERROR_DECLARATION(X509CertChainTooLong, x509_cert_chain_too_long, 56, "certificate chain too long")
ERROR_DECLARATION(X509CertRevoked, x509_cert_revoked, 57, "certificate revoked")
ERROR_DECLARATION(X509InvalidCa, x509_invalid_ca, 58, "invalid CA certificate")
ERROR_DECLARATION(X509InvalidNonCa, x509_invalid_non_ca, 59, "invalid non-CA certificate (has CA markings)")
ERROR_DECLARATION(X509PathLengthExceeded, x509_path_length_exceeded, 60, "path length constraint exceeded")
ERROR_DECLARATION(X509ProxyPathLengthExceeded, x509_proxy_path_length_exceeded, 61, "proxy path length constraint exceeded")
ERROR_DECLARATION(X509ProxyCertificatesNotAllowed, x509_proxy_certificates_not_allowed, 62, "proxy certificates not allowed, please set the appropriate flag")
ERROR_DECLARATION(X509InvalidPurpose, x509_invalid_purpose, 63, "unsupported certificate purpose")
ERROR_DECLARATION(X509CertUntrusted, x509_cert_untrusted, 64, "certificate not trusted")
ERROR_DECLARATION(X509CertRejected, x509_cert_rejected, 65, "certificate rejected")
ERROR_DECLARATION(X509ApplicationVerification, x509_application_verification, 66, "application verification failure")
ERROR_DECLARATION(X509SubjectIssuerMismatch, x509_subject_issuer_mismatch, 67, "subject issuer mismatch")
ERROR_DECLARATION(X509AkidSkidMismatch, x509_akid_skid_mismatch, 68, "authority and subject key identifier mismatch")
ERROR_DECLARATION(X509AkidIssuerSerialMismatch, x509_akid_issuer_serial_mismatch, 69, "authority and issuer serial number mismatch")
ERROR_DECLARATION(X509KeyusageNoCertsign, x509_keyusage_no_certsign, 70, "key usage does not include certificate signing")
ERROR_DECLARATION(X509UnableToGetCrlIssuer, x509_unable_to_get_crl_issuer, 71, "unable to get CRL issuer certificate")
ERROR_DECLARATION(X509UnhandledCriticalExtension, x509_unhandled_critical_extension, 72, "unhandled critical extension")
ERROR_DECLARATION(X509KeyusageNoCrlSign, x509_keyusage_no_crl_sign, 73, "key usage does not include CRL signing")
ERROR_DECLARATION(X509KeyusageNoDigitalSignature, x509_keyusage_no_digital_signature, 74, "key usage does not include digital signature")
ERROR_DECLARATION(X509UnhandledCriticalCrlExtension, x509_unhandled_critical_crl_extension, 75, "unhandled critical CRL extension")
ERROR_DECLARATION(X509InvalidExtension, x509_invalid_extension, 76, "invalid or inconsistent certificate extension")
ERROR_DECLARATION(X509InvalidPolicyExtension, x509_invalid_policy_extension, 77, "invalid or inconsistent certificate policy extension")
ERROR_DECLARATION(X509NoExplicitPolicy, x509_no_explicit_policy, 78, "no explicit policy")
ERROR_DECLARATION(X509DifferentCrlScope, x509_different_crl_scope, 79, "Different CRL scope")
ERROR_DECLARATION(X509UnsupportedExtensionFeature, x509_unsupported_extension_feature, 80, "Unsupported extension feature")
ERROR_DECLARATION(X509UnnestedResource, x509_unnested_resource, 81, "RFC 3779 resource not subset of parent's resources")
ERROR_DECLARATION(X509PermittedViolation, x509_permitted_violation, 82, "permitted subtree violation")
ERROR_DECLARATION(X509ExcludedViolation, x509_excluded_violation, 83, "excluded subtree violation")
ERROR_DECLARATION(X509SubtreeMinmax, x509_subtree_minmax, 84, "name constraints minimum and maximum not supported")
ERROR_DECLARATION(X509UnsupportedConstraintType, x509_unsupported_constraint_type, 85, "unsupported name constraint type")
ERROR_DECLARATION(X509UnsupportedConstraintSyntax, x509_unsupported_constraint_syntax, 86, "unsupported or invalid name constraint syntax")
ERROR_DECLARATION(X509UnsupportedNameSyntax, x509_unsupported_name_syntax, 87, "unsupported or invalid name syntax")
ERROR_DECLARATION(X509CrlPathValidationError, x509_crl_path_validation_error, 88, "CRL path validation error")

ERROR_DECLARATION(AttributeReadOnly, attribute_read_only, 89, "Object attribute can not be changed")
ERROR_DECLARATION(CannotSavePinInCache, cannot_save_pin_in_cache, 90, "Cannot save pin in cache")
ERROR_DECLARATION(CMSCertificateAlreadyPresent, cms_certificate_already_present, 91, "certificate already present")
ERROR_DECLARATION(RdnNotFound, rdn_not_found, 92, "rdn of this type not found")
ERROR_DECLARATION(AlreadyLoggedIn, already_logged_in, 93, "Login has already been performed")
ERROR_DECLARATION(CekNotAuthentic, cek_not_authentic, 94, "CEK not authentic")
ERROR_DECLARATION(LicenceReadOnly, licence_read_only, 95, "Licence can not be changed")

ERROR_DECLARATION(AttributeSensitive, attribute_sensitive, 96, "pkcs11 attribute sensitive")
ERROR_DECLARATION(AttributeTypeInvalid, attribute_type_invalid, 97, "pkcs11 attribute type invalid")
ERROR_DECLARATION(AttributeValueInvalid, attribute_value_invalid, 98, "pkcs11 attribute value invalid")
ERROR_DECLARATION(BufferTooSmall, buffer_too_small, 99, "pkcs11 buffer too small")
ERROR_DECLARATION(Pkcs11Cancel, pkcs11_cancel, 100, "pkcs11 cancel")
ERROR_DECLARATION(Pkcs11CantLock, cant_lock, 101, "pkcs11 cant lock")
ERROR_DECLARATION(CryptokiAlreadyInitialized, cryptoki_already_initialized, 102, "pkcs11 cryptoki already initialized")
ERROR_DECLARATION(DataLenRange, data_len_range, 103, "pkcs11 data len range")
ERROR_DECLARATION(FunctionNotParallel, function_not_parallel, 104, "pkcs11 function not parallel")
ERROR_DECLARATION(Pkcs11GeneralError, pkcs11_general_error, 105, "pkcs11 general error")
ERROR_DECLARATION(Pkcs11InformationSensitive, information_sensitive, 106, "pkcs11 information sensitive")
ERROR_DECLARATION(Pkcs11DeviceRemoved, pkcs11_device_removed, 107, "pkcs11 device removed")
ERROR_DECLARATION(Pkcs11DomainParamsInvalid, pkcs11_domain_params_invalid, 108, "pkcs11 domain params invalid")
ERROR_DECLARATION(EncryptedDataInvalid, encrypted_data_invalid, 109, "pkcs11 encrypted data invalid")
ERROR_DECLARATION(EncryptedDataLenRange, encrypted_data_len_range, 110, "pkcs11 encrypted data len range")
ERROR_DECLARATION(FunctionCanceled, function_canceled, 111, "pkcs11 function canceled")
ERROR_DECLARATION(KeyHandleInvalid, key_handle_invalid, 112, "pkcs11 key handle invalid")
ERROR_DECLARATION(KeyIndigestible, key_indigestible, 113, "pkcs11 key indigestible")
ERROR_DECLARATION(KeyNeeded, key_needed, 114, "pkcs11 key needed")
ERROR_DECLARATION(KeyNotNeeded, key_not_needed, 115, "pkcs11 key not needed")
ERROR_DECLARATION(KeyNotWrappable, key_not_wrappable, 116, "pkcs11 key not wrappable")
ERROR_DECLARATION(KeySizeRange, key_size_range, 117, "pkcs11 key size range")
ERROR_DECLARATION(KeyUnextractable, key_unextractable, 118, "pkcs11 key unextractable")
ERROR_DECLARATION(SessionClosed, session_closed, 119, "pkcs11 session closed")
ERROR_DECLARATION(SessionCount, session_count, 120, "pkcs11 session count")
ERROR_DECLARATION(SessionExists, session_exists, 121, "pkcs11 session exists")
ERROR_DECLARATION(PinExpired, pin_expired, 122, "pkcs11 pin expired")
ERROR_DECLARATION(PinInvalid, pin_invalid, 123, "pkcs11 pin invalid")
ERROR_DECLARATION(Pkcs11RandomNoRng, random_no_rng, 124, "pkcs11 random no rng")
ERROR_DECLARATION(Pkcs11RandomSeedNotSupported, random_seed_not_supported, 125, "pkcs11 random seed not supported")
ERROR_DECLARATION(Pkcs11SavedStateInvalid, saved_state_invalid, 126, "pkcs11 saved state invalid")
ERROR_DECLARATION(UserPinNotInitialized, user_pin_not_initialized, 127, "pkcs11 user pin not initialized")
ERROR_DECLARATION(UserTooManyTypes, user_too_many_types, 128, "pkcs11 user too many types")
ERROR_DECLARATION(UserTypeInvalid, user_type_invalid, 129, "pkcs11 user type invalid")
ERROR_DECLARATION(TokenNotPresent, token_not_present, 130, "pkcs11 token not present")
ERROR_DECLARATION(TokenNotRecognized, token_not_recognized, 131, "pkcs11 token not recognized")
ERROR_DECLARATION(TokenWriteProtected, token_write_protected, 132, "pkcs11 token write protected")
ERROR_DECLARATION(UnknownKeyType, unknown_key_type, 133, "unknown key type")
ERROR_DECLARATION(UnknownOid, unknown_oid, 134, "unknown oid")
ERROR_DECLARATION(WrongBigNumber, wrong_big_number, 135, "wrong big number")
ERROR_DECLARATION(DidnotFindSession, didnot_find_session, 136, "unable to find any session")
ERROR_DECLARATION(SessionParallelNotSupported, session_parallel_not_supported, 137, "pkcs11 session parallel not supported")
ERROR_DECLARATION(SessionReadOnly, session_read_only, 138, "pkcs11 session read only")
ERROR_DECLARATION(SessionReadOnlyExists, session_read_only_exists, 139, "pkcs11 session read only exists")
ERROR_DECLARATION(SessionReadWriteSoExists, session_read_write_so_exists, 140, "pkcs11 session read write so exists")
ERROR_DECLARATION(Pkcs11MutexBad, mutex_bad, 141, "pkcs11 mutex bad")
ERROR_DECLARATION(Pkcs11MutexNotLocked, mutex_not_locked, 142, "pkcs11 mutex not locked")
ERROR_DECLARATION(Pkcs11ObjectHandleInvalid, object_handle_invalid, 143, "pkcs11 object handle invalid")
ERROR_DECLARATION(Pkcs11Ok, pkcs11_ok, 144, "pkcs11 ok")
ERROR_DECLARATION(Pkcs11OperationActive, operation_active, 145, "pkcs11 operation active")
ERROR_DECLARATION(Pkcs11OperationNotInitialized, operation_not_initialized, 146, "pkcs11 operation not initialized")
ERROR_DECLARATION(MechanismInvalid, mechanism_invalid, 147, "pkcs11 mechanism invalid")
ERROR_DECLARATION(MechanismParamInvalid, mechanism_param_invalid, 148, "pkcs11 mechanism param invalid")
ERROR_DECLARATION(SignatureInvalid, signature_invalid, 149, "pkcs11 signature invalid")
ERROR_DECLARATION(SignatureLenRange, signature_len_range, 150, "pkcs11 signature len range")
ERROR_DECLARATION(SlotIdInvalid, slot_id_invalid, 151, "pkcs11 slot id invalid")
ERROR_DECLARATION(Pkcs11StateUnsaveable, state_unsaveable, 152, "pkcs11 state unsaveable")
ERROR_DECLARATION(TemplateIncomplete, template_incomplete, 153, "pkcs11 template incomplete")
ERROR_DECLARATION(UserAnotherAlreadyLoggedIn, user_another_already_logged_in, 154, "pkcs11 user another already logged in")
ERROR_DECLARATION(UnwrappingKeyHandleInvalid, unwrapping_key_handle_invalid, 155, "pkcs11 unwrapping key handle invalid")
ERROR_DECLARATION(UnwrappingKeySizeRange, unwrapping_key_size_range, 156, "pkcs11 unwrapping key size range")
ERROR_DECLARATION(UnwrappingKeyTypeInconsistent, unwrapping_key_type_inconsistent, 157, "pkcs11 unwrapping key type inconsistent")
ERROR_DECLARATION(WrappedKeyLenRange, wrapped_key_len_range, 158, "pkcs11 wrapped key len range")
ERROR_DECLARATION(WrappingKeyHandleInvalid, wrapping_key_handle_invalid, 159, "pkcs11 wrapping key handle invalid")
ERROR_DECLARATION(WrappingKeySizeRange, wrapping_key_size_range, 160, "pkcs11 wrapping key size range")
ERROR_DECLARATION(WrappingKeyTypeInconsistent, wrapping_key_type_inconsistent, 161, "pkcs11 wrapping key type inconsistent")
ERROR_DECLARATION(Pkcs11NeedToCreateThreads, need_to_create_threads, 162, "pkcs11 need to create threads")
ERROR_DECLARATION(Pkcs11NewPinMode, new_pin_mode, 163, "pkcs11 new pin mode")
ERROR_DECLARATION(Pkcs11NextOtp, next_otp, 164, "pkcs11 next otp")
ERROR_DECLARATION(Pkcs11NoEvent, no_event, 165, "pkcs11 no event")
ERROR_DECLARATION(CantHardwareVerifyCMS, cant_hardware_verify_cms, 166, "Multitple signed CMS with hardware verification is not supported")
ERROR_DECLARATION(DecryptUnsuccessful, decrypt_unsuccessful, 167, "Decrypt operation was unsuccessful")

#undef ERROR_DECLARATION
		default: throw internal::UnknownErrorCodeException(errorCode);
	}
}
}
/// @endcond

class Pkcs11Device;

namespace cms
{
class SignParams;
class VerifyParams;
class EnvelopParams;
class EnvelopedData;
class SignedData;
}       // namespace cms

/// @cond INTERNAL
class Object: internal::Noncopyable
{
protected:
	Object()
	{
		m_object.object = nullptr;
	}

	Object(Object&& v)
		: m_object(v.m_object)
	{
		v.m_object.object = nullptr;
	}

	Object& operator=(Object&& v)
	{
		m_object = v.m_object;
		v.m_object.object = nullptr;
		return *this;
	}

	~Object()
	{
		if (m_object.object)
			rt_pc_object_destruct(m_object.object);
	}

	void construct(rt_pc_object_impl* object)
	{
		m_object.object = object;
	}

	void release()
	{
		m_object.object = nullptr;
	}

	rt_pc_object m_object;
};
/// @endcond

/*! \brief Ключ проверки подписи.
 */
class PublicKey: public Object
{
	DERIVE_OBJECT(PublicKey, public_key, Object);
	friend class Pkcs11PrivateKey;

public:
	std::vector<uint8_t> toDer() const
	{
		uint8_t* value;
		size_t size;

		int r = RTPC_CALL(m_object, to_der, &value, &size);
		internal::checkReturnCode(r);
		FREE_ON_EXIT(value);

		return std::vector<uint8_t>(value, value + size);
	}

private:
	explicit PublicKey(rt_pc_public_key&& object)
		: m_object(object)
	{
		Object::construct(m_object.object);
		object.object = nullptr;
	}
};

/*! \brief Уникальное имя X.500.
 *
 */
class X500Dn: public Object
{
	DERIVE_OBJECT(X500Dn, x500_dn, Object);
	friend class Pkcs10RequestInfo;
	friend class Cert;

public:
	/// Идентификаторы относительных уникальных имен X.500.
	struct RdnId
	{
		enum Type
		{
			commonName = rt_pc_x500_rdn_nid_common_name,                     ///< Имя
			surname = rt_pc_x500_rdn_nid_surname,                            ///< Фамилия
			givenName = rt_pc_x500_rdn_nid_given_name,                       ///< Отчество
			title = rt_pc_x500_rdn_nid_title,                                ///< Должность
			pseudonym = rt_pc_x500_rdn_nid_pseudonym,                        ///< Псевдоним
			emailAddress = rt_pc_x500_rdn_nid_email_address,                 ///< Адрес электронной почты
			countryName = rt_pc_x500_rdn_nid_country_name,                   ///< Страна
			localityName = rt_pc_x500_rdn_nid_locality_name,                 ///< Город
			stateOrProvinceName = rt_pc_x500_rdn_nid_state_or_province_name, ///< Субъект федерации
			organization = rt_pc_x500_rdn_nid_organization,                  ///< Название организации
			organizationalUnit = rt_pc_x500_rdn_nid_organizational_unit,     ///< Название подразделения организации
			street = rt_pc_x500_rdn_nid_street,                              ///< Адрес
			ogrn = rt_pc_x500_rdn_nid_ogrn,                                  ///< ОГРН
			ogrnip = rt_pc_x500_rdn_nid_ogrnip,                              ///< ОГРН ИП
			snils = rt_pc_x500_rdn_nid_snils,                                ///< СНИЛС
			inn = rt_pc_x500_rdn_nid_inn,                                    ///< ИНН
			unstructuredName = rt_pc_x500_rdn_nid_unstructured_name,         ///< Неструктурированное имя
		};
	};

	X500Dn()
	{
		int r = rt_pc_x500_dn_create(&m_object);
		internal::checkReturnCode(r);
		Object::construct(m_object.object);
	}

	//@{
	/*! \brief Установка относительного уникального имени.
	 *
	 * \param rdnId - тип относительного уникального имени.
	 * \param value - значение относительного уникального имени.
	 */
	X500Dn& setRdn(RdnId::Type rdnId, const std::string& value)
#if !defined(_MSC_VER) || _MSC_VER >= 1900
	&
#endif
	{
		int r = RTPC_CALL(m_object, set_rdn, static_cast<rt_pc_x500_rdn_nid>(rdnId), value.data());
		internal::checkReturnCode(r);
		return *this;
	}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
	X500Dn && setRdn(RdnId::Type rdnId, const std::string & value) &&
	{
		return std::move(setRdn(rdnId, value));
	}
#endif
	//@}

	/*! \brief Получение значения относительного уникального имени.
	 *
	 * \param rdnId - тип относительного уникального имени.
	 */
	std::string getRdn(RdnId::Type rdnId)
	{
		char* value = NULL;
		int r = RTPC_CALL(m_object, get_rdn, static_cast<rt_pc_x500_rdn_nid>(rdnId), &value);
		internal::checkReturnCode(r);
		FREE_ON_EXIT(value);

		return value;
	}

	/*! \brief Проверка наличия относительного уникального имени.
	 *
	 * \param rdnId - тип относительного уникального имени.
	 */
	bool hasRdn(RdnId::Type rdnId)
	{
		bool result = false;
		int r = RTPC_CALL(m_object, has_rdn, static_cast<rt_pc_x500_rdn_nid>(rdnId), &result);
		internal::checkReturnCode(r);

		return result;
	}
private:
	explicit X500Dn(rt_pc_x500_dn&& object)
		: m_object(object)
	{
		Object::construct(m_object.object);
		object.object = nullptr;
	}
};

/*! \brief Расширение сертификата X509.
 *
 */
class X509CertExtension: public Object
{
	DERIVE_OBJECT(X509CertExtension, x509_cert_extension, Object);
	friend class Pkcs10RequestInfo;

public:
	/// Тип расширения сертификата.
	struct ExtensionType
	{
		enum Type
		{
			keyUsage = rt_pc_x509_cert_extension_type_key_usage,                ///< Расширение сертификата Key Usage
			extendedKeyUsage = rt_pc_x509_cert_extension_type_ext_key_usage,    ///< Расширение сертификата Extended Key Usage
			certificatePolicies = rt_pc_x509_cert_extension_type_cert_policies, ///< Расширение сертификата Certificate Policies
			subjectSignTool = rt_pc_x509_cert_extension_type_subject_sign_tool, ///< Расширение сертификата "Название СКЗИ, используемое для подписи"
		};
	};

public:
	/*! \brief Возвращает действительный тип расширения сертификата.
	 */
	ExtensionType::Type type() const
	{
		rt_pc_x509_cert_extension_type v;
		auto r = RTPC_CALL(m_object, extension_type, &v);
		internal::checkReturnCode(r);
		return static_cast<ExtensionType::Type>(v);
	}

	//@{
	/*! \brief Установка статуса "критично" расширения сертификата.
	 */
	X509CertExtension& setCritical()
#if !defined(_MSC_VER) || _MSC_VER >= 1900
	&
#endif
	{
		auto r = RTPC_CALL(m_object, set_critical);
		internal::checkReturnCode(r);
		return *this;
	}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
	X509CertExtension && setCritical() &&
	{
		return std::move(setCritical());
	}
#endif
	//@}

protected:
	/// @cond INTERNAL
	X509CertExtension() {}
	/// @endcond
};

/*! \brief Расширение сертификата Key Usage.
 *
 *  Это расширение устанавливает ограничения по использованию ключа подписи.
 */
class X509KeyUsage: public X509CertExtension
{
	DERIVE_OBJECT(X509KeyUsage, x509_key_usage, X509CertExtension);

public:
	/*! \brief Параметры расширения сертификата Key Usage.
	 */
	struct Option
	{
		enum Type
		{
			digitalSignature = rt_pc_x509_key_usage_option_digital_signature, ///< Электронная подпись
			nonRepudiation = rt_pc_x509_key_usage_option_non_repudiation,     ///< Неотзываемый
			keyEncipherment = rt_pc_x509_key_usage_option_key_encipherment,   ///< Шифрование ключей
			dataEncipherment = rt_pc_x509_key_usage_option_data_encipherment, ///< Шифрование данных
			keyAgreement = rt_pc_x509_key_usage_option_key_agreement,         ///< Выработка ключа согласования
			keyCertSign = rt_pc_x509_key_usage_option_key_cert_sign,          ///< Подпись сертификатов
			crlSign = rt_pc_x509_key_usage_option_crl_sign,                   ///< Подпись списка отзыва сертификатов
			encipherOnly = rt_pc_x509_key_usage_option_encipher_only,         ///< Только для шифрования
			decipherOnly = rt_pc_x509_key_usage_option_decipher_only,         ///< Только для расшифрования
		};
	};

	X509KeyUsage()
	{
		int r = rt_pc_x509_key_usage_create(&m_object);
		internal::checkReturnCode(r);
		X509CertExtension::construct(m_object.object);
	}

	//@{
	/*! \brief Добавление параметра к расширению.
	 */
	X509KeyUsage& addOption(Option::Type option)
#if !defined(_MSC_VER) || _MSC_VER >= 1900
	&
#endif
	{
		int r = RTPC_CALL(m_object, add_option, static_cast<rt_pc_x509_key_usage_option>(option));
		internal::checkReturnCode(r);
		return *this;
	}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
	X509KeyUsage && addOption(Option::Type option) &&
	{
		return std::move(addOption(option));
	}
#endif
	//@}

	//@{
	/*! \brief Установка статуса "критично" расширения сертификата.
	 */
	X509KeyUsage& setCritical()
#if !defined(_MSC_VER) || _MSC_VER >= 1900
	&
#endif
	{
		X509CertExtension::setCritical();
		return *this;
	}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
	X509KeyUsage && setCritical() &&
	{
		return std::move(setCritical());
	}
#endif
	//@}
};

/*! \brief Расширение сертификата Extended Key Usage.
 *
 *  Это расширение устанавливает дополнительные ограничения по использованию ключа подписи.
 */
class X509ExtendedKeyUsage: public X509CertExtension
{
	DERIVE_OBJECT(X509ExtendedKeyUsage, x509_ext_key_usage, X509CertExtension);

public:
	/*! \brief Параметры расширения сертификата Extended Key Usage.
	 */
	struct Option
	{
		enum Type
		{
			emailProtection = rt_pc_x509_ext_key_usage_option_email_protection, ///< Защита электронной почты (S/MIME)
			clientAuth = rt_pc_x509_ext_key_usage_option_client_auth,           ///< Аутентификация клиента SSL/TLS
			serverAuth = rt_pc_x509_ext_key_usage_option_server_auth,           ///< Аутентификация сервера SSL/TLS
			codeSigning = rt_pc_x509_ext_key_usage_option_code_signing,         ///< Подпись исполняемого кода
			timeStamping = rt_pc_x509_ext_key_usage_option_time_stamping,       ///< Добавление меток времени
			msCodeInd = rt_pc_x509_ext_key_usage_option_ms_code_ind,            ///< Использование для Microsoft Individual Code Signing
			msCodeCom = rt_pc_x509_ext_key_usage_option_ms_code_com,            ///< Использование для Microsoft Commercial Code Signing
			msCtlSign = rt_pc_x509_ext_key_usage_option_ms_ctl_sign,            ///< Использование для Microsoft Trust List Signing
			ocspSigning = rt_pc_x509_ext_key_usage_option_ocsp_signing,         ///< Подпись OSCP ответов
			any = rt_pc_x509_ext_key_usage_option_any,
		};
	};

	X509ExtendedKeyUsage()
	{
		int r = rt_pc_x509_ext_key_usage_create(&m_object);
		internal::checkReturnCode(r);
		X509CertExtension::construct(m_object.object);
	}

	//@{
	/*! \brief Добавление параметра к расширению.
	 */
	X509ExtendedKeyUsage& addOption(Option::Type option)
#if !defined(_MSC_VER) || _MSC_VER >= 1900
	&
#endif
	{
		int r = RTPC_CALL(m_object, add_option, static_cast<rt_pc_x509_ext_key_usage_option>(option));
		internal::checkReturnCode(r);
		return *this;
	}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
	X509ExtendedKeyUsage && addOption(Option::Type option) &&
	{
		return std::move(addOption(option));
	}
#endif
	//@}

	//@{
	/*! \brief Добавление дополнительного параметра к расширению.
	 */
	X509ExtendedKeyUsage& addOptionByOid(const std::string& oid)
#if !defined(_MSC_VER) || _MSC_VER >= 1900
	&
#endif
	{
		int r = RTPC_CALL(m_object, add_option_by_oid, oid.c_str());
		internal::checkReturnCode(r);
		return *this;
	}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
	X509ExtendedKeyUsage && addOptionByOid(const std::string & oid) &&
	{
		return std::move(addOptionByOid(oid));
	}
#endif
	//@}

	//@{
	/*! \brief Установка статуса "критично" расширения сертификата.
	 */
	X509ExtendedKeyUsage& setCritical()
#if !defined(_MSC_VER) || _MSC_VER >= 1900
	&
#endif
	{
		X509CertExtension::setCritical();
		return *this;
	}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
	X509ExtendedKeyUsage && setCritical() &&
	{
		return std::move(this->setCritical());
	}
#endif
	//@}
};

/*! \brief Расширение сертификата Certificate Policies.
 */
class X509CertPolicies: public X509CertExtension
{
	DERIVE_OBJECT(X509CertPolicies, x509_cert_policies, X509CertExtension);

public:
	X509CertPolicies()
	{
		int r = rt_pc_x509_cert_policies_create(&m_object);
		internal::checkReturnCode(r);
		X509CertExtension::construct(m_object.object);
	}

	//@{
	/*! \brief Добавление политики сертификатов к расширению.
	 */
	X509CertPolicies& addPolicyByOid(const std::string& oid)
#if !defined(_MSC_VER) || _MSC_VER >= 1900
	&
#endif
	{
		int r = RTPC_CALL(m_object, add_policy_by_oid, oid.c_str());
		internal::checkReturnCode(r);
		return *this;
	}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
	X509CertPolicies && addPolicyByOid(const std::string & oid) &&
	{
		return std::move(addPolicyByOid(oid));
	}
#endif
	//@}

	//@{
	/*! \brief Установка статуса "критично" расширения сертификата.
	 */
	X509CertPolicies& setCritical()
#if !defined(_MSC_VER) || _MSC_VER >= 1900
	&
#endif
	{
		X509CertExtension::setCritical();
		return *this;
	}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
	X509CertPolicies && setCritical() &&
	{
		return std::move(this->setCritical());
	}
#endif
	//@}
};

/*! \brief Расширение сертификата "Название СКЗИ, используемого для подписи".
 */
class X509SubjectSignTool: public X509CertExtension
{
	DERIVE_OBJECT(X509SubjectSignTool, x509_subject_sign_tool, X509CertExtension);

public:
	/*! \brief Создание объекта расширения сертификата
	 *
	 * \param subjectSignTool - название СКЗИ, используемого для подписи.
	 */
	X509SubjectSignTool(const std::string& subjectSignTool)
	{
		int r = rt_pc_x509_subject_sign_tool_create(&m_object, subjectSignTool.c_str());
		internal::checkReturnCode(r);
		X509CertExtension::construct(m_object.object);
	}

	//@{
	/*! \brief Установка статуса "критично" расширения сертификата.
	 */
	X509SubjectSignTool& setCritical()
#if !defined(_MSC_VER) || _MSC_VER >= 1900
	&
#endif
	{
		X509CertExtension::setCritical();
		return *this;
	}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
	X509SubjectSignTool && setCritical() &&
	{
		return std::move(this->setCritical());
	}
#endif
	//@}
};

/*! \brief Ключ подписи, хранящийся на устройстве.
 *
 * Он может быть получен с помощью вызовов Pkcs11Device::enumeratePrivateKeys или Pkcs11Device::generateKeyPair.
 */
class Pkcs11PrivateKey: public Object
{
	DERIVE_OBJECT(Pkcs11PrivateKey, pkcs11_private_key, Object);
	friend class Pkcs11Device;
	friend class Pkcs11Cert;
	friend struct internal::Crypto;

private:
	explicit Pkcs11PrivateKey(rt_pc_pkcs11_private_key&& object)
		: m_object(object)
	{
		Object::construct(m_object.object);
		object.object = nullptr;
	}

public:
	PublicKey getPublicKey()
	{
		rt_pc_public_key publicKey;

		int r = RTPC_CALL(m_object, get_public_key, &publicKey);
		internal::checkReturnCode(r);

		return PublicKey(std::move(publicKey));
	}

	std::vector<uint8_t> getId() const
	{
		uint8_t* buf;
		size_t size;
		auto r = RTPC_CALL(m_object, get_id, &buf, &size);
		internal::checkReturnCode(r);
		FREE_ON_EXIT(buf);

		return std::vector<uint8_t>(buf, buf + size);
	}
};

class Cert: public Object
{
	DERIVE_OBJECT(Cert, cert, Object);

public:
	/*! \brief Получение сертификата в формате PEM.
	 */
	std::string toPem() const
	{
		char* pemValue;
		auto r = RTPC_CALL(m_object, to_pem, &pemValue);
		internal::checkReturnCode(r);
		FREE_ON_EXIT(pemValue);

		return pemValue;
	}

	/*! \brief Получение свойства Subject сертификата.
	 */
	X500Dn getSubject() const
	{
		rt_pc_x500_dn subject;
		int r = RTPC_CALL(m_object, get_subject, &subject);
		internal::checkReturnCode(r);

		return X500Dn(std::move(subject));
	}

	/*! \brief Получение свойства Serial Number сертификата.
	 */
	std::vector<uint8_t> getSerialNumber() const
	{
		uint8_t* buf;
		size_t size;
		int r = RTPC_CALL(m_object, get_serial_number, &buf, &size);
		internal::checkReturnCode(r);
		FREE_ON_EXIT(buf);

		return std::vector<uint8_t>(buf, buf + size);
	}

protected:
	/// @cond INTERNAL
	Cert() {}
	/// @endcond
};

/*! \brief Сертификат, не хранящийся на устройстве.
 *
 */
class ExternalCert: public Cert
{
	DERIVE_OBJECT(ExternalCert, external_cert, Cert);

public:
	/*! \brief Конструктор.
	 *
	 * \param pemValue - значение сертификата в формате PEM
	 */
	explicit ExternalCert(const std::string& pemValue)
	{
		auto r = rt_pc_external_cert_create_from_pem(&m_object, pemValue.c_str());
		internal::checkReturnCode(r);
		Cert::construct(m_object.object);
	}

	/*! \brief Конструктор из данных в формате DER.
	 *
	 * \param data - указатель на буфер с сертифкатом в формате DER.
	 * \param length - размер буфера в байтах.
	 */
	ExternalCert(const void* data, size_t length)
	{
		auto r = rt_pc_external_cert_create_from_der(&m_object, data, length);
		internal::checkReturnCode(r);
		Cert::construct(m_object.object);
	}
};

/*! \brief Сертификат, хранящийся на устройстве.
 *
 * Он может быть получен при перечислении сертификатов на устройстве (Pkcs11Device::enumerateCerts)
 * или при импорте сертификата на устройство (Pkcs11Device::importCert).
 */
class Pkcs11Cert: public Cert
{
	DERIVE_OBJECT(Pkcs11Cert, pkcs11_cert, Cert);
	friend class Pkcs11Device;
	friend class cms::SignParams;
	friend class cms::EnvelopedData;
	friend struct internal::Crypto;

public:
	/*! \brief Является ли сертификат доверенным в рамках текущего устройства.
	 */
	bool isTrusted() const
	{
		bool trusted = false;
		int r = RTPC_CALL(m_object, is_trusted, &trusted);
		internal::checkReturnCode(r);
		return trusted;
	}

	/*! \brief Получение ключа подписи, соответствующего данному сертификату.
	 *
	 */
	Pkcs11PrivateKey getCorrespondingPrivateKey()
	{
		rt_pc_pkcs11_private_key key;
		int r = RTPC_CALL(m_object, get_corresponding_private_key, &key);
		internal::checkReturnCode(r);

		return Pkcs11PrivateKey(std::move(key));
	}

protected:
	/// @cond INTERNAL
	Pkcs11Cert() {}

	explicit Pkcs11Cert(rt_pc_pkcs11_cert&& object)
		: m_object(object)
	{
		Cert::construct(m_object.object);
		object.object = nullptr;
	}
	/// @endcond
};

/*! \brief Список отзыва сертификатов.
 *
 * Используется при проверке подписи.
 */
class Crl
{
	friend class cms::VerifyParams;

private:
	std::string m_pemValue;

public:
	/*! \brief Конструктор.
	 *
	 * \param pemValue - список отзыва сертификатов в формате PEM.
	 */
	explicit Crl(const std::string pemValue) : m_pemValue(pemValue) {}
};

/// Набор параметров эллиптической кривой.
struct Gost34102001Paramset
{
	enum Type
	{
		/// Набор параметров I (paramset A).
		a = rt_pc_gost3410_2001_key_paramset_a,
		/// Набор параметров II (paramset B).
		b = rt_pc_gost3410_2001_key_paramset_b,
		/// Набор параметров III (paramset C).
		c = rt_pc_gost3410_2001_key_paramset_c,
		/// Набор параметров IV (paramset XchA).
		xa = rt_pc_gost3410_2001_key_paramset_xa,
		/// Набор параметров V (paramset XchB).
		xb = rt_pc_gost3410_2001_key_paramset_xb,
	};
};

struct Gost34102012_256Paramset
{
	enum Type
	{
		/// Набор параметров I (paramset A).
		a = rt_pc_gost3410_2012_256_key_paramset_a,
		/// Набор параметров II (paramset B).
		b = rt_pc_gost3410_2012_256_key_paramset_b,
		/// Набор параметров III (paramset C).
		c = rt_pc_gost3410_2012_256_key_paramset_c,
		/// Набор параметров IV (paramset XchA).
		xa = rt_pc_gost3410_2012_256_key_paramset_xa,
		/// Набор параметров V (paramset XchB).
		xb = rt_pc_gost3410_2012_256_key_paramset_xb,
	};
};

struct Gost34102012_512Paramset
{
	enum Type
	{
		/// Набор параметров I (paramset A).
		a = rt_pc_gost3410_2012_512_key_paramset_a,
		/// Набор параметров II (paramset B).
		b = rt_pc_gost3410_2012_512_key_paramset_b
	};
};

/*! \brief Данные запроса на сертификат.
 */
class Pkcs10RequestInfo: public Object
{
	DERIVE_OBJECT(Pkcs10RequestInfo, pkcs10_reqinfo, Object);
	friend struct internal::Crypto;

public:
	Pkcs10RequestInfo()
	{
		int r = rt_pc_pkcs10_reqinfo_create(&m_object);
		internal::checkReturnCode(r);
		Object::construct(m_object.object);
	}

	//@{
	/*! \brief Установка свойства Subject запроса на сертификат.
	 *
	 */
	Pkcs10RequestInfo& setSubject(X500Dn&& subjectDn)
#if !defined(_MSC_VER) || _MSC_VER >= 1900
	&
#endif
	{
		int r = RTPC_CALL(m_object, set_subject, subjectDn.m_object.object);
		internal::checkReturnCode(r);
		subjectDn.release();
		return *this;
	}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
	Pkcs10RequestInfo && setSubject(X500Dn && subjectDn) &&
	{
		return std::move(setSubject(std::move(subjectDn)));
	}
#endif
	//@}

	//@{
	/*! \brief Добавление расширения сертификата к запросу на сертификат.
	 */
	Pkcs10RequestInfo& setExtension(X509CertExtension&& extension)
#if !defined(_MSC_VER) || _MSC_VER >= 1900
	&
#endif
	{
		int r = RTPC_CALL(m_object, set_extension, extension.m_object.object);
		internal::checkReturnCode(r);
		extension.release();
		return *this;
	}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
	Pkcs10RequestInfo && setExtension(X509CertExtension && extension) &&
	{
		return std::move(setExtension(std::move(extension)));
	}
#endif
	//@}

	//@{
	/*! \brief Добавление произвольного расширения сертификата.
	 *
	 * \param oid - oid произвольного расширения.
	 * \param data - указатель на буфер с ASN1 структурой - значением расширения сертификата.
	 * \param size - размер буфера.
	 * \param critical
	 */
	Pkcs10RequestInfo& setCustomExtension(const std::string& oid, const void* data, size_t size, bool critical = false)
#if !defined(_MSC_VER) || _MSC_VER >= 1900
	&
#endif
	{
		int r = RTPC_CALL(m_object, set_custom_extension, oid.data(), data, size, critical);
		internal::checkReturnCode(r);
		return *this;
	}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
	Pkcs10RequestInfo && setCustomExtension(const std::string & oid, const void* data, size_t size, bool critical = false) &&
	{
		return std::move(setCustomExtension(oid, data, size, critical));
	}
#endif
	//@}

	//@{
	/*! \brief Добавление произвольного атрибута.
	 *
	 * \param oid - oid атрибута.
	 * \param data - указатель на буфер с ASN1 структурой - значением атрибута (значение добавляется в набор значений этого атрибута).
	 * \param size - размер буфера.
	 */
	Pkcs10RequestInfo& addCustomAttributeValue(const std::string& oid, const void* data, size_t size)
#if !defined(_MSC_VER) || _MSC_VER >= 1900
	&
#endif
	{
		int r = RTPC_CALL(m_object, add_custom_attribute_value, oid.data(), data, size);
		internal::checkReturnCode(r);
		return *this;
	}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
	Pkcs10RequestInfo && addCustomAttributeValue(const std::string & oid, const void* data, size_t size) &&
	{
		return std::move(addCustomAttributeValue(oid, data, size));
	}
#endif
	//@}
};

/*! \brief PKCS#10 запрос на сертификат.
 */
class Pkcs10Request
{
	friend struct internal::Crypto;

public:
	/*! \brief Получение запроса на сертификат в формате PEM.
	 */
	std::string toPem() const
	{
		return m_pem;
	}

private:
	Pkcs10Request(const std::string& pem)
		: m_pem(pem)
	{}

	std::string m_pem;
};

/*! \brief Устройство, подключенное к компьютеру.
 *
 * Может быть получено с помощью вызова Pkcs11Device::enumerate.
 */
class Pkcs11Device: public Object
{
	DERIVE_OBJECT(Pkcs11Device, pkcs11_device, Object);
	friend class cms::VerifyParams;
	friend class cms::EnvelopParams;
	friend class cms::SignedData;
	friend struct internal::Crypto;

public:
	/// Форматы данных, которые устройство может подписывать.
	struct Format
	{
		enum Type
		{
			plain = rt_pc_data_format_plain,         ///< Текст
			raw = rt_pc_data_format_raw,             ///< Неформатированные данные
			pinpad2 = rt_pc_data_format_pinpad2,     ///< Формат PINPad 2
			xml = rt_pc_data_format_xml,             ///< xml
			safetouch = rt_pc_data_format_safetouch, ///< Формат SafeTouch
		};
	};

	/// Дополнительная функциональность, поддерживаемая устройством.
	struct Feature
	{
		enum Type
		{
			journal = rt_pc_device_feature_journal,             ///< Наличие журнала выполненных операций подписи
			pin2 = rt_pc_device_feature_pin2,                   ///< Наличие возможности запрашивать ввод дополнительнго PIN-кода на экране устройства
			visualization = rt_pc_device_feature_visualization, ///< Наличие возможности отображать подписываемые данные на экране устройства
			sm = rt_pc_device_feature_sm,                       ///< Поддержка secure messaging
			flash_drive = rt_pc_device_feature_flash_drive,     ///< Наличие flash-памяти в устройстве
		};
	};

	/*! \brief Общие параметры генерации ключевой пары для всех типов ключей независимо от алгоритма.
	 *
	 */
	class CommonKeyGenParams
	{
		friend class Pkcs11Device;

public:
		/// Особые свойства ключевой пары.
		struct Flag
		{
			enum Type
			{
				/// Только для PINPad. Ключевая пара требует ввода PIN-кода "PIN2" для использования.
				needPin2 = rt_pc_key_flag_need_pin2,
				/// Только для PINPad. Ключевая пара требует визуализации данных при подписи.
				needVisualization = rt_pc_key_flag_need_visualization,
			};
		};

		/// Тип ключевой пары.
		struct KeyType
		{
			enum Type
			{
				/// Обычная ключевая пара
				common = rt_pc_key_type_common,
				/// Журнальная ключевая пара
				journal = rt_pc_key_type_journal,
			};
		};

protected:
		/// @cond INTERNAL
		int m_flags;
		KeyType::Type m_keyType;
		const void* m_ckaId;
		size_t m_ckaIdLength;
		/// @endcond

public:
		/*! \brief Конструктор.
		 *
		 * \param keyType - тип ключевой пары.
		 * \param flags - комбинация Flag флагов особых свойств ключевой пары.
		 * \param ckaId - указатель на буфер с cka id.
		 * \param ckaIdLength - размер буфера с cka id в байтах.
		 */
		explicit CommonKeyGenParams(KeyType::Type keyType = CommonKeyGenParams::KeyType::common, int flags = 0, const void* ckaId = NULL, size_t ckaIdLength = 0)
			: m_flags(flags),
			m_keyType(keyType),
			m_ckaId(ckaId),
			m_ckaIdLength(ckaIdLength)
		{}
	};

	/*! \brief Параметры генерации ключевой пары специфичные для алгоритма ГОСТ 34.10-2001.
	 *
	 */
	class Gost34102001KeyGenParams: private CommonKeyGenParams
	{
		friend class Pkcs11Device;

private:
		Gost34102001Paramset::Type m_paramset;

		rt_pc_gost3410_2001_key_gen_params params() const
		{
			rt_pc_gost3410_2001_key_gen_params params = RT_PC_GOST3410_2001_KEY_GEN_PARAMS_INITIALIZER;

			params.flags = m_flags;
			params.type = static_cast<rt_pc_key_type>(m_keyType);
			params.paramset = static_cast<rt_pc_gost3410_2001_key_paramset>(m_paramset);
			params.cka_id = m_ckaId;
			params.cka_id_length = m_ckaIdLength;

			return params;
		}

public:
		/*! \brief Конструктор.
		 *
		 * \param paramset - набор параметров эллиптической кривой.
		 * \param params - общие параметры генерации ключевой пары.
		 */
		explicit Gost34102001KeyGenParams(Gost34102001Paramset::Type paramset = Gost34102001Paramset::a,
		                                  const CommonKeyGenParams& params = CommonKeyGenParams())
			: CommonKeyGenParams(params), m_paramset(paramset) {}
	};

	/*! \brief Параметры генерации ключевой пары специфичные для алгоритма ГОСТ 34.10-2012 256 бит.
	 *
	 */
	class Gost34102012_256KeyGenParams: private CommonKeyGenParams
	{
		friend class Pkcs11Device;

private:
		Gost34102012_256Paramset::Type m_paramset;

		rt_pc_gost3410_2012_256_key_gen_params params() const
		{
			rt_pc_gost3410_2012_256_key_gen_params params = RT_PC_GOST3410_2012_256_KEY_GEN_PARAMS_INITIALIZER;

			params.flags = m_flags;
			params.type = static_cast<rt_pc_key_type>(m_keyType);
			params.paramset = static_cast<rt_pc_gost3410_2012_256_key_paramset>(m_paramset);
			params.cka_id = m_ckaId;
			params.cka_id_length = m_ckaIdLength;

			return params;
		}

public:
		/*! \brief Конструктор.
		 *
		 * \param paramset - набор параметров эллиптической кривой.
		 * \param params - общие параметры генерации ключевой пары.
		 */
		explicit Gost34102012_256KeyGenParams(Gost34102012_256Paramset::Type paramset = Gost34102012_256Paramset::a,
		                                      const CommonKeyGenParams& params = CommonKeyGenParams())
			: CommonKeyGenParams(params), m_paramset(paramset) {}
	};

	/*! \brief Параметры генерации ключевой пары специфичные для алгоритма ГОСТ 34.10-2012 512 бит.
	 *
	 */
	class Gost34102012_512KeyGenParams: private CommonKeyGenParams
	{
		friend class Pkcs11Device;

private:
		Gost34102012_512Paramset::Type m_paramset;

		rt_pc_gost3410_2012_512_key_gen_params params() const
		{
			rt_pc_gost3410_2012_512_key_gen_params params = RT_PC_GOST3410_2012_512_KEY_GEN_PARAMS_INITIALIZER;

			params.flags = m_flags;
			params.type = static_cast<rt_pc_key_type>(m_keyType);
			params.paramset = static_cast<rt_pc_gost3410_2012_512_key_paramset>(m_paramset);
			params.cka_id = m_ckaId;
			params.cka_id_length = m_ckaIdLength;

			return params;
		}

public:
		/*! \brief Конструктор.
		 *
		 * \param paramset - набор параметров эллиптической кривой.
		 * \param params - общие параметры генерации ключевой пары.
		 */
		explicit Gost34102012_512KeyGenParams(Gost34102012_512Paramset::Type paramset = Gost34102012_512Paramset::a,
		                                      const CommonKeyGenParams& params = CommonKeyGenParams())
			: CommonKeyGenParams(params), m_paramset(paramset) {}
	};

	/*! \brief Получение списка подключенных устройств.
	 *
	 * Вызов этой функции делает объекты, полученные при предыдущем вызове, недействительными.
	 */
	static std::vector<Pkcs11Device> enumerate()
	{
		rt_pc_pkcs11_device* deviceList = NULL;
		size_t size = 0;
		int r = rt_pc_enumerate_devices(&deviceList, &size);
		internal::checkReturnCode(r);
		internal::ObjectArrayHolder<rt_pc_pkcs11_device> arrayHolder(deviceList, size);

		std::vector<Pkcs11Device> devices;
		devices.reserve(size);
		for (auto object = arrayHolder.begin(); object != arrayHolder.end(); ++object)
		{
			devices.push_back(Pkcs11Device(std::move(*object)));
		}

		return devices;
	}

	/*! \brief Авторизация на устройстве.
	 *
	 * \param pin - PIN-код устройства.
	 */
	void login(const std::string& pin)
	{
		int r = RTPC_CALL(m_object, login, pin.data());
		internal::checkReturnCode(r);
	}

	/*! \brief Сброс прав доступа на устройстве.
	 *
	 */
	void logout()
	{
		int r = RTPC_CALL(m_object, logout);
		internal::checkReturnCode(r);
	}

	/*! \brief Смена PIN-кода устройства.
	 *
	 * \param oldPin - старый PIN-код устройства.
	 * \param newPin - новый PIN-код устройства.
	 */
	void changePin(const std::string& oldPin, const std::string& newPin)
	{
		int r = RTPC_CALL(m_object, change_pin, oldPin.data(), newPin.data());
		internal::checkReturnCode(r);
	}

	/*! \brief Получение метки устройства.
	 *
	 */
	std::string getLabel()
	{
		char* label = NULL;
		int r = RTPC_CALL(m_object, get_label, &label);
		internal::checkReturnCode(r);
		FREE_ON_EXIT(label);

		return label;
	}

	/*! \brief Получение серийного номера устройства.
	 *
	 */
	std::string getSerialNumber()
	{
		char* serial = NULL;
		int r = RTPC_CALL(m_object, get_serial_number, &serial);
		internal::checkReturnCode(r);
		FREE_ON_EXIT(serial);

		return serial;
	}

	/*! \brief Наличие прав для совершения операций с ключом подписи.
	 *
	 */
	bool isLoggedIn()
	{
		bool result;
		int r = RTPC_CALL(m_object, is_logged_in, &result);
		internal::checkReturnCode(r);
		return result;
	}

	/*! \brief Установлен ли на токене PIN-код по умолчанию.
	 *
	 */
	bool isPinDefault()
	{
		bool result;
		int r = RTPC_CALL(m_object, is_pin_default, &result);
		internal::checkReturnCode(r);
		return result;
	}

	/*! \brief Получение информации о поддерживаемых устройством форматах данных.
	 *
	 * \param format - формат, поддержку которого необходимо проверить.
	 */
	bool isFormatSupported(Format::Type format)
	{
		bool result;
		int r = RTPC_CALL(m_object, is_format_supported, static_cast<rt_pc_data_format>(format), &result);
		internal::checkReturnCode(r);
		return result;
	}

	/*! \brief Получение информации о поддерживаемых устройством дополнительных возможностях.
	 *
	 * \param feature - дополнительная возможность, поддержку которой необходимо проверить.
	 */
	bool isFeatureSupported(Feature::Type feature)
	{
		bool result;
		int r = RTPC_CALL(m_object, is_feature_supported, static_cast<rt_pc_device_feature>(feature), &result);
		internal::checkReturnCode(r);
		return result;
	}

	/*! \brief Получение списка ключей, хранящихся на устройстве.
	 *
	 */
	std::vector<Pkcs11PrivateKey> enumeratePrivateKeys()
	{
		rt_pc_pkcs11_private_key* keyList = NULL;
		size_t size = 0;
		rt_pc_key_enum_params params = RT_PC_KEY_ENUM_PARAMS_INITIALIZER;
		int r = RTPC_CALL(m_object, enumerate_keys, &params, &keyList, &size);
		internal::checkReturnCode(r);
		internal::ObjectArrayHolder<rt_pc_pkcs11_private_key> arrayHolder(keyList, size);

		std::vector<Pkcs11PrivateKey> keys;
		keys.reserve(size);
		for (auto object = arrayHolder.begin(); object != arrayHolder.end(); ++object)
		{
			keys.push_back(Pkcs11PrivateKey(std::move(*object)));
		}

		return keys;
	}

	/*! \brief Генерация ключевой пары на устройстве.
	 *
	 * \param keyGenParams - параметры генерации ключевой пары.
	 * \return Ключ подписи сгенерированной ключевой пары.
	 */
	template<class T>
	Pkcs11PrivateKey generateKeyPair(const T& keyGenParams)
	{
		rt_pc_pkcs11_private_key key;
		auto params = keyGenParams.params();
		int r = RTPC_CALL(m_object, generate_key_pair, &params, &key);
		internal::checkReturnCode(r);

		return Pkcs11PrivateKey(std::move(key));
	}

	/*! \brief Удаление ключевой пары.
	 *
	 * \param key - ключ подписи ключевой пары, которую необходимо удалить.
	 */
	void deleteKeyPair(const Pkcs11PrivateKey& key)
	{
		int r = RTPC_CALL(m_object, delete_key_pair, key.m_object.object);
		internal::checkReturnCode(r);
	}

	/* \brief Получение списка всех сертификатов, хранящихся на устройстве.
	 *
	 */
	std::vector<Pkcs11Cert> enumerateCerts()
	{
		rt_pc_pkcs11_cert* certList = NULL;
		size_t size;

		int r = RTPC_CALL(m_object, enumerate_certs, &certList, &size);
		internal::checkReturnCode(r);
		internal::ObjectArrayHolder<rt_pc_pkcs11_cert> arrayHolder(certList, size);

		std::vector<Pkcs11Cert> certs;
		certs.reserve(size);
		for (auto object = arrayHolder.begin(); object != arrayHolder.end(); ++object)
		{
			certs.push_back(Pkcs11Cert(std::move(*object)));
		}

		return certs;
	}

	/*! \brief Сохранение сертификата на устройстве.
	 *
	 * \param externalCert - сертификат, который будет сохранен на устройстве.
	 * \param trusted - считать ли данный сертификат доверенным в рамках устройства.
	 */
	Pkcs11Cert importCert(const ExternalCert& externalCert, bool trusted = false)
	{
		rt_pc_pkcs11_cert cert;
		std::string pem = externalCert.toPem();
		int r = RTPC_CALL(m_object, import_cert, pem.data(),
		                  pem.length(), rt_pc_cert_form_pem,
		                  trusted, &cert);
		internal::checkReturnCode(r);

		return Pkcs11Cert(std::move(cert));
	}

	/*! \brief Удаление сертификата с устройства.
	 *
	 * \param cert - сертификат, который необходимо удалить.
	 */
	void deleteCert(const Pkcs11Cert& cert)
	{
		int r = RTPC_CALL(m_object, delete_cert, cert.m_object.object);
		internal::checkReturnCode(r);
	}
private:
	explicit Pkcs11Device(rt_pc_pkcs11_device&& object)
		: m_object(object)
	{
		Object::construct(m_object.object);
		object.object = nullptr;
	}
};

/*! \brief Пространство имен с функциями и типами, которые относятся к CMS.
 *
 */
namespace cms
{
/*! \brief Параметры подписи сообщений.
 *
 */
class SignParams: public Object
{
	DERIVE_OBJECT(SignParams, cms_sign_params, Object);
	friend struct internal::Crypto;
	friend class SignedData;

public:
	/// Флаги подписи.
	struct Flag
	{
		enum Type
		{
			addUserCert = rt_pc_sign_flag_add_user_cert,  ///< Добавлять пользовательский сертификат к подписи.
			addSignTime = rt_pc_sign_flag_add_sign_time,  ///< Добавлять метку времени к подписи.
			hardwareHash = rt_pc_sign_flag_hardware_hash, ///< Вычислять хеш на устройстве.
		};
	};

	/*! \brief Конструктор.
	 *
	 * \param cert - сертификат, ключ подписи которого, будет использован для подписи.
	 * \param flags - комбинация Flag флагов, определяющих параметры подписи.
	 */
	explicit SignParams(const Pkcs11Cert& cert, int flags = 0)
	{
		auto r = rt_pc_cms_sign_params_create(&m_object, &cert.m_object);
		internal::checkReturnCode(r);
		construct(m_object.object);

		r = RTPC_CALL(m_object, set_flags, flags);
		internal::checkReturnCode(r);
	}

	/*! \brief Добавление сертификата, который будет присоединен к подписи.
	 *
	 */
	SignParams& addCert(const ExternalCert& cert)
	{
		auto r = RTPC_CALL(m_object, add_cert, cert.toPem().c_str());
		internal::checkReturnCode(r);
		return *this;
	}
};

/*! \brief Параметры проверки подписи.
 *
 */
class VerifyParams: public Object
{
	DERIVE_OBJECT(VerifyParams, cms_verify_params, Object);
	friend class cms::SignedData;

public:
	/// Флаги проверки подписи.
	struct Flag
	{
		enum Type
		{
			/// Проверять сертификат пользователя, подписавшего сообщение.
			verifyUserCert = rt_pc_verify_flag_verify_user_cert
		};
	};

	/*! \brief Конструктор.
	 *
	 * \param device - устройство, на котором будет выполняться проверка подписи.
	 * \param flags - комбинация Flags флагов проверки подписи.
	 */
	explicit VerifyParams(const Pkcs11Device& device, int flags = 0)
	{
		auto r = rt_pc_cms_verify_params_create(&m_object, &device.m_object);
		internal::checkReturnCode(r);
		construct(m_object.object);

		r = RTPC_CALL(m_object, set_flags, flags);
		internal::checkReturnCode(r);
	}

	/*! \brief Добавление сертификата пользователя к параметрам проверки подписи.
	 *
	 * Если сертификат пользователя, которым подписано сообщение, не прикреплен к сообщению,
	 * то его необходимо добавить к параметрам проверки подписи с помощью этой функции.
	 * \param cert - сертификат, который использоваля для подписи сообщения.
	 */
	VerifyParams& addUserCert(const ExternalCert& cert)
	{
		auto r = RTPC_CALL(m_object, add_user_cert, cert.toPem().c_str());
		internal::checkReturnCode(r);
		return *this;
	}

	/*! \brief Добавление корневого сертфиката.
	 *
	 * Добавляет корневой сертификат, который может подтверждать подлинность сертификата,
	 * к параметрам проверки подписи. Сертификаты, добавленные при помощи этой функции, будут использованы,
	 * если установлен флаг Flag::verifyUserCert.
	 * \param cert - корневой сертификат.
	 */
	VerifyParams& addCaCert(const ExternalCert& cert)
	{
		auto r = RTPC_CALL(m_object, add_ca_cert, cert.toPem().c_str());
		internal::checkReturnCode(r);
		return *this;
	}

	/*! \brief Добавление списка отзыва сертификатов.
	 *
	 * Добавляет список отзыва сертификатов к параметрам проверки подписи. Он будет использован, если установлен
	 * флаг Flag::verifyUserCert.
	 * \param crl - список отзыва сертификатов.
	 */
	VerifyParams& addCrl(const Crl& crl)
	{
		auto r = RTPC_CALL(m_object, add_crl, crl.m_pemValue.c_str());
		internal::checkReturnCode(r);
		return *this;
	}

	/*! \brief Установка подписанных данных.
	 *
	 * Если подписанные данные не включены в сообщение, необходимо добавить их с помощью этой функции.
	 * \param data - указатель на буфер с данными.
	 * \param size - размер буфера с данными.
	 */
	VerifyParams& setData(const void* data, size_t size)
	{
		auto r = RTPC_CALL(m_object, set_data, data, size);
		internal::checkReturnCode(r);
		return *this;
	}
};

/// Возможные результаты проверки подписи.
struct VerifyResult
{
	enum Type
	{
		/// Подпись верна.
		success = rt_pc_verify_result_success
	};
};

/*! \brief Параметры шифрования сообщений.
 *
 */
class EnvelopParams: public Object
{
	DERIVE_OBJECT(EnvelopParams, cms_envelop_params, Object);
	friend struct internal::Crypto;

public:
	/*! \brief Конструктор.
	 *
	 * \param device - устройство, на котором будет выполняться шифрование.
	 */
	explicit EnvelopParams(const Pkcs11Device& device)
	{
		auto r = rt_pc_cms_envelop_params_create(&m_object, &device.m_object);
		internal::checkReturnCode(r);
		construct(m_object.object);
	}

	//@{
	/*! \brief Добавление получателя зашифрованного сообщения.
	 *
	 * \param cert - сертификат получателя зашифрованного сообщения.
	 */
	EnvelopParams& addRecipient(const ExternalCert& cert)
#if !defined(_MSC_VER) || _MSC_VER >= 1900
	&
#endif
	{
		int r = RTPC_CALL(m_object, add_recipient, cert.toPem().c_str());
		internal::checkReturnCode(r);
		return *this;
	}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
	EnvelopParams && addRecipient(const ExternalCert &cert) &&
	{
		return std::move(addRecipient(cert));
	}
#endif
	//@}
};

/*! \brief Типы CMS сообщений.
 */
struct ContentType
{
	enum Type
	{
		data = rt_pc_cms_content_type_data,
		signedData = rt_pc_cms_content_type_signed_data,
		envelopedData = rt_pc_cms_content_type_enveloped_data
	};
};

/*! \brief Базовый класс для всех CMS сообщений.
 */
class Message: public Object
{
	DERIVE_OBJECT(Message, cms_message, Object);

public:
	/*! \brief Возвращает действительный тип сообщения.
	 */
	ContentType::Type contentType() const
	{
		rt_pc_cms_content_type v;
		auto r = RTPC_CALL(m_object, content_type, &v);
		internal::checkReturnCode(r);
		return static_cast<ContentType::Type>(v);
	}

protected:
	/// @cond INTERNAL
	Message() {}

	std::vector<uint8_t> toBer() const
	{
		uint8_t* buf;
		size_t size;
		auto r = RTPC_CALL(m_object, to_ber, &buf, &size);
		internal::checkReturnCode(r);
		FREE_ON_EXIT(buf);

		return std::vector<uint8_t>(buf, buf + size);
	}
	/// @endcond
};

class Data: public Message
{
	DERIVE_OBJECT(Data, cms_data, Message);
	friend struct internal::Crypto;

public:
	/*! \brief Создание объекта, который будет содержать переданные данные.
	 *
	 * \param begin - указатель на первый элемент контейнера. Контейнер должен хранить все свои элементы в непрерывной области памяти.
	 * \param end - указатель на элемент расположенный за последним элементом контейнера.
	 */
	template<typename T>
	Data(T begin, T end)
	{
		construct(begin, end);
	}

	/*! \brief Создание объекта, который будет содержать переданные данные.
	 *
	 * \param container - STL контейнер с элементами типа uint8_t. Контейнер должен хранить все свои элементы в непрерывной области памяти.
	 */
	template<typename T>
	Data(const T& container)
	{
		construct(container.begin(), container.end());
	}

	/*! \brief Данные, которые содержатся в сообщении.
	 */
	std::vector<uint8_t> data() const
	{
		uint8_t* buf;
		size_t size;
		auto r = RTPC_CALL(m_object, data, &buf, &size);
		internal::checkReturnCode(r);
		FREE_ON_EXIT(buf);

		return std::vector<uint8_t>(buf, buf + size);
	}

private:
	template<typename T>
	void construct(T begin, T end)
	{
		auto dist = std::distance(begin, end);
		auto r = rt_pc_cms_data_create(&m_object, dist ? &(*begin) : NULL, dist);
		internal::checkReturnCode(r);
		Message::construct(m_object.object);
	}
};

class SignedData: public Message
{
	DERIVE_OBJECT(SignedData, cms_signed_data, Message);

public:
	/*! \brief Создание объекта из данных в формате BER.
	 *
	 * \param begin - указатель на первый элемент контейнера. Контейнер должен хранить все свои элементы в непрерывной области памяти.
	 * \param end - указатель на элемент расположенный за последним элементом контейнера.
	 */
	template<typename T>
	static SignedData parse(T begin, T end)
	{
		return SignedData(begin, end);
	}

	/*! \brief Создание объекта из данных в формате BER.
	 *
	 * \param container - STL контейнер с элементами типа uint8_t. Контейнер должен хранить все свои элементы в непрерывной области памяти.
	 */
	template<typename T>
	static SignedData parse(const T& container)
	{
		return SignedData(container.begin(), container.end());
	}

	/*! \brief Получение данного сообщения в BER формате.
	 */
	std::vector<uint8_t> toBer() const
	{
		return Message::toBer();
	}

	/*! \brief Проверка подписи данного сообщения.
	 */
	VerifyResult::Type verify(const VerifyParams& params)
	{
		auto ber = this->toBer();

		rt_pc_verify_result result;
		int r = rt_pc_cms_verify(ber.data(), ber.size(), &params.m_object, &result);
		internal::checkReturnCode(r);

		return static_cast<cms::VerifyResult::Type>(result);
	}

	//@{
	/*! \brief Подпись содержимого конверта новым подписантом.
	 *
	 * \param signParams - параметры подписи.
	 */
	SignedData& sign(const SignParams& signParams)
#if !defined(_MSC_VER) || _MSC_VER >= 1900
	&
#endif
	{
		sign(signParams, false, nullptr, 0);
		return *this;
	}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
	SignedData && sign(const SignParams &signParams) &&
	{
		return std::move(sign(signParams));
	}
#endif
	//@}

	//@{
	/*! \brief Подпись содержимого конверта новым подписантом при отсоединенной подписи.
	 *
	 * \param signParams - параметры подписи.
	 * \param eContent - содержимое конверта. Передается, eсли подпись отсоединенная.
	 * \param eContentSize - размер содержимого конверта.
	 */
	SignedData& sign(const SignParams& signParams, const void* eContent, size_t eContentSize)
#if !defined(_MSC_VER) || _MSC_VER >= 1900
	&
#endif
	{
		sign(signParams, true, eContent, eContentSize);
		return *this;
	}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
	SignedData && sign(const SignParams &signParams, const void* eContent, size_t eContentSize) &&
	{
		return std::move(sign(signParams, eContent, eContentSize));
	}
#endif
	//@}

private:
	template<typename T>
	SignedData(T begin, T end)
	{
		auto dist = std::distance(begin, end);
		auto r = rt_pc_cms_signed_data_parse(&m_object, dist ? &(*begin) : NULL, dist);
		internal::checkReturnCode(r);
		Message::construct(m_object.object);
	}

	void sign(const cms::SignParams& params, bool detached, const void* message, size_t message_size)
	{
		auto signedData = this->toBer();

		uint8_t* cms = NULL;
		size_t cmsLen = 0;
		int r = rt_pc_cms_sign(message, message_size, &params.m_object, detached, signedData.data(), signedData.size(), &cms, &cmsLen);
		internal::checkReturnCode(r);
		FREE_ON_EXIT(cms);

		r = RTPC_CALL(m_object, assign, cms, cmsLen);
		internal::checkReturnCode(r);
	}
};

class EnvelopedData: public Message
{
	DERIVE_OBJECT(EnvelopedData, cms_enveloped_data, Message);

public:
	/*! \brief Параметры расшифрования сообщений.
	 *
	 */
	class DecryptParams: public Object
	{
		DERIVE_OBJECT(DecryptParams, cms_decrypt_params, Object);
		friend class EnvelopedData;

public:
		/*! \brief Конструктор.
		 *
		 * \param cert - сертификат пользователя, соответствующий ключ которого, будет использован для расшифрования.
		 */
		explicit DecryptParams(const Pkcs11Cert& cert)
		{
			auto r = rt_pc_cms_decrypt_params_create(&m_object, &cert.m_object);
			internal::checkReturnCode(r);
			construct(m_object.object);
		}
	};

	/*! \brief Создание объекта из данных в формате BER.
	 *
	 * \param begin - указатель на первый элемент контейнера. Контейнер должен хранить все свои элементы в непрерывной области памяти.
	 * \param end - указатель на элемент расположенный за последним элементом контейнера.
	 */
	template<typename T>
	static EnvelopedData parse(T begin, T end)
	{
		return EnvelopedData(begin, end);
	}

	/*! \brief Создание объекта из данных в формате BER.
	 *
	 * \param container - STL контейнер с элементами типа uint8_t. Контейнер должен хранить все свои элементы в непрерывной области памяти.
	 */
	template<typename T>
	static EnvelopedData parse(const T& container)
	{
		return EnvelopedData(container.begin(), container.end());
	}

	/*! \brief Получение данного сообщения в BER формате.
	 */
	std::vector<uint8_t> toBer() const
	{
		return Message::toBer();
	}

	/*! \brief Расшифрование содержимого данного сообщения.
	 */
	Message decrypt(const DecryptParams& params)
	{
		auto ber = this->toBer();

		uint8_t* message = NULL;
		size_t messageLen = 0;
		int r = rt_pc_cms_decrypt(ber.data(), ber.size(), &params.m_object, &message, &messageLen);
		internal::checkReturnCode(r);
		FREE_ON_EXIT(message);

		return Data(message, message + messageLen);
	}

private:
	template<typename T>
	EnvelopedData(T begin, T end)
	{
		auto dist = std::distance(begin, end);
		auto r = rt_pc_cms_enveloped_data_parse(&m_object, dist ? &(*begin) : NULL, dist);
		internal::checkReturnCode(r);
		Message::construct(m_object.object);
	}
};
}       // namespace cms

/// @cond INTERNAL
namespace internal
{
struct Crypto
{
	static Pkcs10Request createPkcs10Request(const Pkcs11PrivateKey& key, const Pkcs10RequestInfo& info)
	{
		char* result = NULL;
		int r = rt_pc_sign_pkcs10_request(key.m_object.object, info.m_object.object, &result);
		internal::checkReturnCode(r);
		FREE_ON_EXIT(result);

		return Pkcs10Request(result);
	}

	template<typename T>
	static std::vector<uint8_t> sign(const Pkcs11PrivateKey& key, T begin, T end)
	{
		uint8_t* sign = NULL;
		size_t signLen = 0;
		auto dist = std::distance(begin, end);
		int r = rt_pc_sign(key.m_object.object, dist ? &(*begin) : NULL, dist, &sign, &signLen);
		internal::checkReturnCode(r);
		FREE_ON_EXIT(sign);

		return std::vector<uint8_t>(sign, sign + signLen);
	}

	static cms::SignedData sign(const cms::Data& message, const cms::SignParams& params, bool detached)
	{
		auto data = message.data();

		uint8_t* cms = NULL;
		size_t cmsLen = 0;
		int r = rt_pc_cms_sign(data.data(), data.size(), &params.m_object, detached, NULL, 0, &cms, &cmsLen);
		internal::checkReturnCode(r);
		FREE_ON_EXIT(cms);

		return cms::SignedData::parse(cms, cms + cmsLen);
	}

	static cms::EnvelopedData encrypt(const cms::Data& message, const cms::EnvelopParams& params)
	{
		auto data = message.data();

		uint8_t* cms = NULL;
		size_t cmsLen = 0;
		int r = rt_pc_cms_encrypt(data.data(), data.size(), &params.m_object, &cms, &cmsLen);
		internal::checkReturnCode(r);
		FREE_ON_EXIT(cms);

		return cms::EnvelopedData::parse(cms, cms + cmsLen);
	}
};
}
/// @endcond

/*! \brief Создание запроса на сертификат.
 *
 * \param key - ключ, которым будет подписан запрос на сертификат.
 * \param info - данные запроса на сертификат.
 */
inline Pkcs10Request createPkcs10Request(const Pkcs11PrivateKey& key, const Pkcs10RequestInfo& info)
{
	return internal::Crypto::createPkcs10Request(key, info);
}

/*! \brief Хеширование и подпись данных.
 *
 * \param key - ключ, которым будут подписаны данные.
 * \param begin - указатель на первый элемент контейнера, содержащего данные.
 * \param end - указатель на элемент расположенный за последним элементом контейнера, содержащего данные.
 */
template<typename T>
std::vector<uint8_t> sign(const Pkcs11PrivateKey& key, T begin, T end)
{
	return internal::Crypto::sign(key, begin, end);
}

namespace cms
{
/*! \brief Подпись сообщения.
 *
 * \param data - подписываемое сообщение.
 * \param signParams - параметры подписи.
 * \param detached - генерировать отсоединенную подпись.
 */
inline cms::SignedData sign(const cms::Data& data, const SignParams& signParams, bool detached = false)
{
	return internal::Crypto::sign(data, signParams, detached);
}

/*! \brief Шифрование сообщения.
 *
 * \param data - шифруемое сообщение.
 * \param envelopParams - параметры шифрования.
 */
inline cms::EnvelopedData envelop(const cms::Data& data, const EnvelopParams& envelopParams)
{
	return internal::Crypto::encrypt(data, envelopParams);
}
}       //namespace cms

/*! \brief Инициализация pki-core.
 *
 * \param path - путь до каталога с библиотекой rtPKCS11ECP.
 */
inline void initialize(const std::string& path)
{
	int r = rt_pc_initialize(path.data());
	internal::checkReturnCode(r);
}

/*! \brief Деинициализация pki-core.
 */
inline void deinitialize()
{
	int r = rt_pc_deinitialize();
	internal::checkReturnCode(r);
}

#undef FREE_ON_EXIT
}
}

#undef DERIVE_OBJECT

#endif
