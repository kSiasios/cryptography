#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "color.h"

#define TABLE_SIZE 95

char ASCII_table[] = {' ', '!', '\"', '#', '$', '%',
                      '&', '\'', '(', ')', '*', '+',
                      ',', '-', '.', '/', '0', '1',
                      '2', '3', '4', '5', '6', '7',
                      '8', '9', ':', ';', '<', '=',
                      '>', '?', '@', 'A', 'B', 'C',
                      'D', 'E', 'F', 'G', 'H', 'I',
                      'J', 'K', 'L', 'M', 'N', 'O',
                      'P', 'Q', 'R', 'S', 'T', 'U',
                      'V', 'W', 'X', 'Y', 'Z', '[',
                      '\\', ']', '^', '_', '`', 'a',
                      'b', 'c', 'd', 'e', 'f', 'g',
                      'h', 'i', 'j', 'k', 'l', 'm',
                      'n', 'o', 'p', 'q', 'r', 's',
                      't', 'u', 'v', 'w', 'x', 'y',
                      'z', '{', '|', '}', '~'};
char shuffled_ASCII[TABLE_SIZE];

// A function to generate a random permutation of arr[]
void randomize(char *array);

void initOneDArray(int array[], int size);
void printIntArray(int *array, int size);
void printCharArray(char *array, int size);
void copyCharArray(char *source, char *destination, int size);

// Encryption Functions
char *encrypt(char *, int msg_size);
char *level_One(char *message, int msg_size);
char *level_Two(char *message, int random_characters, int msg_size);
char *randomChars(int num, int modifier);

// Decryption Functions
char *decrypt(char *message);
int ret_random_chars(char c);

void swap(char *a, char *b);

char *decToHex(int num);
int hexToDec(char *num);
int my_pow(int a, int b);

int seed;

void swap(char *a, char *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void randomize(char *array)
{
    // Use a different seed value so that we don't get same
    // result each time we run this program
    srand(seed);
    int used[TABLE_SIZE];
    initOneDArray(used, TABLE_SIZE);

    //Start from the last element and swap one by one. We don't
    //need to run for the first element that's why i > 0
    int i = TABLE_SIZE - 1;
    while (i > 0)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i + 1);
        if (used[j] != 1)
        {
            // Swap array[i] with the element at random index
            swap(&array[i], &array[j]);
            i--;
            used[j] = 0;
        }
        else
        {
            continue;
        }
    }
    initOneDArray(used, TABLE_SIZE);
}

char *encrypt(char *message, int msg_size)
{
    seed = time(NULL);

    copyCharArray(ASCII_table, shuffled_ASCII, TABLE_SIZE);

    // shuffle Table
    randomize(shuffled_ASCII);

    // call level_One() and save the returned string to str
    int size1 = 0;
    while (message[size1] != '\0')
    {
        size1++;
    }
    printf("\nInitial Message:");
    yellow();
    printf(" \t%s\n", message);
    reset();
    char str[size1];
    copyCharArray(level_One(message, size1), str, size1);
    str[size1 - 1] = '\0';

    // random number from 1 to 6
    int random_char_amoumt = (rand() % 6) + 1;
    // switch case to determine the letter corresponding to our number
    char letter[2];
    int chance = (rand() % 100); // chance for the letter to be upper case or lower case
    switch (random_char_amoumt)
    {
    case 1:
        if (chance > 49)
        {
            strcpy(letter, "a");
        }
        else
        {
            strcpy(letter, "A");
        }

        break;
    case 2:
        if (chance > 49)
        {
            strcpy(letter, "b");
        }
        else
        {
            strcpy(letter, "B");
        }
        break;
    case 3:
        if (chance > 49)
        {
            strcpy(letter, "c");
        }
        else
        {
            strcpy(letter, "C");
        }
        break;
    case 4:
        if (chance > 49)
        {
            strcpy(letter, "d");
        }
        else
        {
            strcpy(letter, "D");
        }
        break;
    case 5:
        if (chance > 49)
        {
            strcpy(letter, "e");
        }
        else
        {
            strcpy(letter, "E");
        }
        break;
    default:
        if (chance > 49)
        {
            strcpy(letter, "f");
        }
        else
        {
            strcpy(letter, "F");
        }
        break;
    }

    // call level_Two() and save the returned string to arr
    int size = (random_char_amoumt + 1) * (sizeof(message)) + random_char_amoumt;
    char arr[size];

    printf("level_One() returned: \t%s\n", str);

    strcpy(arr, level_Two(str, random_char_amoumt, size1));

    // convert seed to hex
    char hex[8];

    strcpy(hex, decToHex(seed));

    char *final_encrypted_string;
    int fsize = sizeof(letter) + sizeof(hex) + sizeof(arr);
    final_encrypted_string = (char *)malloc(fsize);
    strcpy(final_encrypted_string, letter);

    strcat(final_encrypted_string, hex);

    strcat(final_encrypted_string, arr);

    return final_encrypted_string;
}

char *decrypt(char *message)
{
    int chars = ret_random_chars(message[0]);

    char hex_timestamp[8];
    // copy hex value from the message whitch is in the positions 1-8
    int i = 1;
    for (i = 1; i < 9; i++)
    {
        hex_timestamp[i - 1] = message[i];
    }

    int timestamp = hexToDec(hex_timestamp);

    // recreate the shuffled version of ascii
    copyCharArray(ASCII_table, shuffled_ASCII, TABLE_SIZE);
    seed = timestamp;

    randomize(shuffled_ASCII);

    i = 0;
    while (message[i + 9] != '\0')
    {
        // count the size of the encrypted message
        i++;
    }
    int msg_size = (i - chars) / (chars + 1);

    char lvl_One_message[msg_size];
    i = 9 + chars;
    int k = 0;
    while (message[i] != '\0')
    {
        // skip random chars and save valuable chars to lvl_One_msg
        lvl_One_message[k] = message[i];
        i += chars + 1;
        k++;
    }
    lvl_One_message[msg_size] = '\0';

    printf("lvl_one msg: \t\t%s\n", lvl_One_message);

    // anti-lvl_One()
    char dec_msg[msg_size];
    i = 0;
    while (lvl_One_message[i] != '\0')
    {
        int j;
        char next_char;
        for (j = 0; j < TABLE_SIZE; j++)
        {
            // iterate through ascii table
            /* when there is a match, set next_char to the corresponding 
            suffled character and terminate loop */
            if (lvl_One_message[i] == shuffled_ASCII[j])
            {
                next_char = ASCII_table[j];
                break;
            }
        }

        //appends each message character to the end of the letter
        dec_msg[i] = next_char;
        i++;
    }

    dec_msg[msg_size] = '\0';

    printf("decrypted msg: ");
    red();
    printf("\t\t%s\n", dec_msg);
}

void initOneDArray(int *array, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        array[i] = 0;
    }
    return;
}

void printIntArray(int *array, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        printf("%d\t", array[i]);
        if ((i + 1) % 5 == 0)
        {
            printf("\n");
        }
    }
    return;
}

void printCharArray(char *array, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        printf("%c\t", array[i]);
        if ((i + 1) % 5 == 0)
        {
            printf("\n");
        }
    }
    printf("--------------------------------------------\n");
    return;
}

void copyCharArray(char *source, char *destination, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        destination[i] = source[i];
    }
    return;
}

char *level_One(char *message, int msg_size)
{
    int i = 0;

    //calculate the size of the message to declare the new static char array
    char new_string[msg_size];

    while (message[i] != '\0')
    {
        // for each character in our message
        int j;
        char next_char;
        for (j = 0; j < TABLE_SIZE; j++)
        {
            // iterate through ascii table
            /* when there is a match, set next_char to the corresponding 
            suffled character and terminate loop */
            if (message[i] == ASCII_table[j])
            {
                next_char = shuffled_ASCII[j];
                break;
            }
        }

        //appends each message character to the end of the letter
        new_string[i] = next_char;
        i++;
    }

    char *final_str;
    final_str = new_string;

    return final_str;
}

char *randomChars(int num, int modifier)
{

    // index to point to the original table
    int index;
    //table to save the random characters
    char *random_Chars;
    char arr[num];
    int i;
    // randomize feed using the current time
    for (i = 0; i < num; i++)
    {

        // random index from 0 to TABLE_SIZE
        index = rand() % (TABLE_SIZE);
        char str[1];
        str[0] = ASCII_table[index];
        if (i == 0)
        {
            strcpy(arr, str);
        }
        else
        {
            strcat(arr, str);
        }
    }
    random_Chars = arr;
    return random_Chars;
}

char *level_Two(char *message, int random_characters, int msg_size)
{
    // This function adds X random characters to the front,
    // end and between each character to our message

    // if X is the amount of random characters
    // and Y the size of our message,
    // the size of the new string will be (X + 1) * Y + X
    int size = (random_characters + 1) * (msg_size) + random_characters;

    char msg[size];
    int j = 0;
    while (j < msg_size)
    {
        msg[j] = message[j];
        j++;
    }

    char arr[size];
    char *new_message;
    strcpy(arr, randomChars(random_characters, 1)); //add to the front

    int i = 0;
    while (msg[i] != '\0')
    {
        char string[1];
        string[0] = msg[i];

        // for each character in our message, add X random chars after
        strcat(arr, string);
        strcat(arr, randomChars(random_characters, i));
        i++;
    }
    new_message = arr;

    return new_message;
}

char *decToHex(int num)
{
    // From GeeksForGeeks
    // char array to store hexadecimal number
    char hexaDeciNum[100];

    // counter for hexadecimal number array
    int i = 0;
    while (num != 0)
    {

        // temporary variable to store remainder
        int temp = 0;

        // storing remainder in temp variable.
        temp = num % 16;

        // check if temp < 10
        if (temp < 10)
        {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else
        {
            hexaDeciNum[i] = temp + 55;
            i++;
        }

        num = num / 16;
    }

    hexaDeciNum[i] = '\0';
    // revert table

    int j = i - 1;
    i = 0;
    while (j >= i)
    {
        char temp = hexaDeciNum[i];
        hexaDeciNum[i] = hexaDeciNum[j];
        hexaDeciNum[j] = temp;
        j--;
        i++;
    }

    char *retVal;
    retVal = hexaDeciNum;
    return retVal;
}

int hexToDec(char *num)
{
    int k = 0;
    while (num[k] != '\0')
    {
        k++;
    }

    int pwr = k - 1;

    int sum = 0;
    k = 0;
    while (num[k] != '\0')
    {
        switch (num[k])
        {
        case '1':
            sum += my_pow(16, pwr);
            break;
        case '2':
            sum += 2 * my_pow(16, pwr);
            break;
        case '3':
            sum += 3 * my_pow(16, pwr);
            break;
        case '4':
            sum += 4 * my_pow(16, pwr);
            break;
        case '5':
            sum += 5 * my_pow(16, pwr);
            break;
        case '6':
            sum += 6 * my_pow(16, pwr);
            break;
        case '7':
            sum += 7 * my_pow(16, pwr);
            break;
        case '8':
            sum += 8 * my_pow(16, pwr);
            break;
        case '9':
            sum += 9 * my_pow(16, pwr);
            break;
        case 'A':
            sum += 10 * my_pow(16, pwr);
            break;
        case 'B':
            sum += 11 * my_pow(16, pwr);
            break;
        case 'C':
            sum += 12 * my_pow(16, pwr);
            break;
        case 'D':
            sum += 13 * my_pow(16, pwr);
            break;
        case 'E':
            sum += 14 * my_pow(16, pwr);
            break;
        case 'F':
            sum += 15 * my_pow(16, pwr);
            break;
        default:
            break;
        }
        k++;
        pwr--;
    }
    return sum;
}

int ret_random_chars(char c)
{
    int i = 0;
    char *letters = "aAbBcCdDeEfF";
    for (i = 0; i < 10; i += 2)
    {
        if (c == letters[i] || c == letters[i + 1])
        {
            return (i / 2) + 1;
        }
    }
}

int my_pow(int base, int pow)
{
    int res = 1, i = 0;
    for (i = 0; i < pow; i++)
    {
        res *= base;
    }
    return res;
}