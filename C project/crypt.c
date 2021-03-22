#include "crypt_lib.h"

int main()
{
    char buf[256];
    // char *msg = "This is a message!";
    // strcpy(buf, msg);
    red();
    printf("\n\n\n\n*****\n\n");
    printf("Write your message:\n");
    fgets(buf, sizeof buf, stdin);
    reset();
    char *enc_msg;
    enc_msg = encrypt(buf, 256);

    printf("Final Message:");
    green();
    printf(" \t\t%s\n\n", enc_msg);
    black();
    printf("\n&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&**************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n\n");
    reset();
    decrypt(enc_msg);
    printf("\n\n*****\n\n\n\n");

    // testing region
    // printf("Testing 1: %d\n", (int)ret_random_chars('b'));
    // printf("Testing 2: %d\n", (int)my_pow(2, 4));
    // printf("Testing 3: %d\n", (int)hexToDec("B268A"));
    // printf("Testing 4: %s\n", (char *)decToHex(730762));
    // printf("Testing 5: %s\n", level_Two("!!!", 5));

    return 0;
}
