#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char *encode(char *inputString) {
    int len = strlen(inputString);
    char set64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    char *result = (char *) malloc(2000 * sizeof(char));

    int numBits = 0;
    int padding = (3 - len % 3) % 3;
    int value = 0;
    int resultIndex = 0;

    for (int i = 0; i < len; i += 3) {
        value = 0;
        numBits = 0;

        for (int j = i; j <= i + 2 && j < len; j++) {
            value = (value << 8) | inputString[j];
            numBits += 8;
        }

        if (numBits == 8) {
            value = value << 4;
            numBits = 12;
        } else if (numBits == 16) {
            value = value << 2;
            numBits = 18;
        }

        for (int j = 6; j <= numBits; j += 6) {
            int temp = value >> (numBits - j);
            int index = temp % 64;
            char ch = set64[index];
            result[resultIndex++] = ch;
        }
    }

    for (int j = 0; j < padding; j++) {
        result[resultIndex++] = '=';
    }

    result[resultIndex] = '\0';

    return result;
}
