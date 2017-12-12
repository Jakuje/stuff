// gcc -shared -fPIC chroot_wrapper.c -o chroot_wrapper.so
int chroot (const char *path) {
	return 0;
}
