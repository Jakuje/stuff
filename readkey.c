/* 
 * Simple test script to verify functionality of private key decryption
 * Usage:
 * gcc -lssh -o readkey readkey.c
 * ssh-keygen -t rsa -N libssh -f id_rsa
 * ./readkey
  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libssh/libssh.h>

#define PASSPHRASE "libssh"

int my_auth_callback(const char *prompt, char *buf, size_t len, int echo,
                     int verify, void *userdata)
{
    (void) prompt;
    (void) echo;
    (void) verify;
    (void) userdata;

    strncpy(buf, PASSPHRASE, strlen(PASSPHRASE));
    buf[strlen(PASSPHRASE)] = '\0';
    printf("Asking for passphrease\n");
    return SSH_OK;
}

int main(void)
{
    ssh_key privkey = NULL;
    unsigned char *hash = NULL;
    int rv = 0;
    size_t hlen = 0;

    ssh_set_log_level(15); 

    rv = ssh_pki_import_privkey_file("id_rsa", NULL, my_auth_callback, NULL, &privkey);
    if (rv != SSH_OK) {
        printf("Failed to import key\n");
        return rv;
    }
    rv = ssh_get_publickey_hash(privkey, SSH_PUBLICKEY_HASH_SHA256, &hash, &hlen);
    if (rv != SSH_OK) {
        printf("Failed to get key hash\n");
        return rv;
    }
    ssh_print_hash(SSH_PUBLICKEY_HASH_SHA256, hash, hlen);
    return rv;
}
