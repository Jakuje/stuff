/* compile with
 * gcc -ldl randomtest.c -o randomtest
 */
#include "/home/jjelen/devel/OpenSC/src/pkcs11/pkcs11.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {

	CK_FUNCTION_LIST_PTR fp;
	CK_RV rv;
	CK_SLOT_ID slot_id = 0;
	CK_SLOT_INFO slot_info;
	int token_present = 0, i;
	static void *pkcs11_so;
	char *pkcs11_module = argv[1];
	CK_SESSION_HANDLE session_handle;
	unsigned char *data;

	if (argc > 1)
		pkcs11_module = argv[1];
	else
		pkcs11_module = "/usr/lib64/pkcs11/opensc-pkcs11.so";

	CK_RV (*C_GetFunctionList)(CK_FUNCTION_LIST_PTR_PTR) = 0;
	pkcs11_so = dlopen(pkcs11_module, RTLD_NOW);
	if (!pkcs11_so) {
		fprintf(stderr, "dlopen(%s) failed", pkcs11_module);
		return 1;
	}
	C_GetFunctionList = (CK_RV (*)(CK_FUNCTION_LIST_PTR_PTR)) dlsym(pkcs11_so, "C_GetFunctionList");
	rv = C_GetFunctionList(&fp);
	rv = fp->C_Initialize(NULL_PTR);

	rv = fp->C_OpenSession(0, CKF_SERIAL_SESSION | CKF_RW_SESSION,
		NULL_PTR, NULL_PTR, &session_handle);

	data = malloc(32*sizeof(unsigned char));
	rv = fp->C_GenerateRandom(session_handle, data, 32);

	fwrite(data, 32, 1, stdout);

	fp->C_Finalize(NULL_PTR);
        dlclose(pkcs11_so);
	return 0;
}
