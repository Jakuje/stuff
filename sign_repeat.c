/* compile with
 * gcc -ldl sign_repeat.c -o sign_repeat
 */
#include "/home/jjelen/devel/OpenSC/src/pkcs11/pkcs11.h"
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define SHORT_MESSAGE_DIGEST	"\x30\x21\x30\x09\x06\x05\x2b\x0e" \
				"\x03\x02\x1a\x05\x00\x04\x14\xd9" \
				"\xdd\xa3\x76\x44\x2f\x50\xe1\xec" \
				"\xd3\x8b\xcd\x6f\xc6\xce\x4e\xfd" \
				"\xd3\x1a\x3f"

int main(int argc, char **argv) {

	CK_FUNCTION_LIST_PTR fp;
	CK_RV rv;
	CK_SLOT_ID slot_id = 0;
	CK_SLOT_INFO slot_info;
	int token_present = 0, i;
	static void *pkcs11_so;
	char *pkcs11_module = argv[1];
	CK_SESSION_HANDLE session_handle;
	CK_OBJECT_CLASS keyClass = CKO_PRIVATE_KEY;
	CK_KEY_TYPE keyType = CKK_RSA;
	CK_ATTRIBUTE filter[] = {
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_KEY_TYPE, &keyType, sizeof(keyType)},
	};
	CK_ULONG filter_size = 1;
	CK_OBJECT_HANDLE object_handle = CK_INVALID_HANDLE;
	CK_OBJECT_HANDLE public_handle = CK_INVALID_HANDLE;
	CK_ULONG object_count;
	CK_BYTE *message = SHORT_MESSAGE_DIGEST;
	CK_ULONG message_length = 35;
	CK_BYTE sign[2048];
	CK_ULONG sign_length = 2048;
	CK_BYTE pin[] = "123456";

	CK_MECHANISM sign_mechanism = { CKM_RSA_PKCS, NULL_PTR, 0 };

	if (argc > 1)
		pkcs11_module = argv[1];
	else
		pkcs11_module = "/home/jjelen/devel/OpenSC/src/pkcs11/.libs/opensc-pkcs11.so";

	CK_RV (*C_GetFunctionList)(CK_FUNCTION_LIST_PTR_PTR) = 0;
	pkcs11_so = dlopen(pkcs11_module, RTLD_NOW);
	if (!pkcs11_so) {
		fprintf(stderr, "dlopen(%s) failed", pkcs11_module);
		return 1;
	}
	C_GetFunctionList = (CK_RV (*)(CK_FUNCTION_LIST_PTR_PTR)) dlsym(pkcs11_so, "C_GetFunctionList");
	rv = C_GetFunctionList(&fp);
	rv = fp->C_Initialize(NULL_PTR);

	fp->C_OpenSession(5, CKF_SERIAL_SESSION | CKF_RW_SESSION, NULL_PTR, NULL_PTR, &session_handle);

	rv = fp->C_Login(session_handle, CKU_USER, pin, pin == NULL ? 0 : strlen(pin));
	printf("C_Login() done, rv = %d\n", rv);

	/* XXX assuming the first key is the one we are looking for */
	fp->C_FindObjectsInit(session_handle, filter, filter_size);
	fp->C_FindObjects(session_handle, &object_handle, 1, &object_count);
	fp->C_FindObjectsFinal(session_handle);

	/* XXX assuming the first key is the one we are looking for */
	keyClass = CKO_PUBLIC_KEY;
	fp->C_FindObjectsInit(session_handle, filter, filter_size);
	fp->C_FindObjects(session_handle, &public_handle, 1, &object_count);
	fp->C_FindObjectsFinal(session_handle);

repeat:
	rv = fp->C_SignInit(session_handle, &sign_mechanism, object_handle);
	printf("C_SignInit() done, rv = %d\n", rv);

	/* XXX always authenticate */
	rv = fp->C_Login(session_handle, CKU_CONTEXT_SPECIFIC, (CK_UTF8CHAR *) pin, pin == NULL ? 0 : strlen(pin));
	printf("C_Login(CKU_CONTEXT_SPECIFIC) done, rv = %d\n", rv);

	rv = fp->C_Sign(session_handle, message, message_length, sign, &sign_length);
	printf("C_Sign() done, rv = %d\n", rv);


	fp->C_VerifyInit(session_handle, &sign_mechanism, public_handle);
	rv = fp->C_Verify(session_handle, message, message_length, sign, sign_length);
	printf("C_Verify() done, rv = %d\n", rv);

	sleep(10);
	goto repeat;

	fp->C_Finalize(NULL_PTR);
        dlclose(pkcs11_so);
	return 0;
}
