/* compile with
 * gcc -ldl waittest.c -o waittest
 */
#include "/home/jjelen/trunk/OpenSC/src/pkcs11/pkcs11.h"
#include <dlfcn.h>
#include <stdio.h>

int main(int argc, char **argv) {

	CK_FUNCTION_LIST_PTR fp;
	CK_RV rv;
	CK_SLOT_ID slot_id = 0;
	CK_SLOT_INFO slot_info;
	int token_present = 0, i;
	static void *pkcs11_so;
	char *pkcs11_module = argv[1];

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

	do {
		printf(" [ Waiting for slot event ... ]\n");
		rv = fp->C_WaitForSlotEvent(0, &slot_id, NULL_PTR);
		if (rv == CKR_FUNCTION_NOT_SUPPORTED) {
			fprintf(stderr, "Function does not support call with blocking wait. Skipping.\n");
			return 1;
		} else if (rv != CKR_OK) {
			printf("C_WaitForSlotEvent: rv = 0x%.8lX\n", rv);
			return 1;
		}

		rv = fp->C_GetSlotInfo(slot_id, &slot_info);
		if (rv != CKR_OK) {
			printf("C_GetSlotInfo: rv = 0x%.8lX\n", rv);
			return 1;
		}

		token_present = ((slot_info.flags & CKF_TOKEN_PRESENT) != 0);
		printf(" [ Slot %lu ] %s\n", slot_id, slot_info.slotDescription);
		printf("              Status: %s\n",
			token_present ? "Token present": "No token");
	} while (!token_present);
	fp->C_Finalize(NULL_PTR);
        dlclose(pkcs11_so);
	return 0;
}
