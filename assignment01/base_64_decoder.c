#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char *decode(char *inputString) {
    int len = strlen(inputString);

    char *result = (char *) malloc(2000 * sizeof(char));

    int numBits = 0;
    int value = 0;
    int resultIndex = 0;
    int paddingCount = 0;

    for (int i = 0; i < len; i += 4) {
        value = 0;
        numBits = 24;
        paddingCount = 0;

        for (int j = i; j <= i + 3 && j < len; j++) {
            int tempIndex;

            if (inputString[j] >= 'A' && inputString[j] <= 'Z')
                tempIndex = inputString[j] - 'A';
            else if (inputString[j] >= 'a' && inputString[j] <= 'z')
                tempIndex = inputString[j] - 'a' + 26;
            else if (inputString[j] >= '0' && inputString[j] <= '9')
                tempIndex = inputString[j] - '0' + 52;
            else if (inputString[j] == '+')
                tempIndex = 62;
            else if (inputString[j] == '/')
                tempIndex = 63;
            else if (inputString[j] == '=') {
                paddingCount++;
                continue;
            }

            value = (value << 6) | tempIndex;
        }

        if (paddingCount == 2) {
            value = value >> 4;
            numBits = 8;
        } else if (paddingCount == 1) {
            value = value >> 2;
            numBits = 16;
        }

        for (int j = 8; j <= numBits; j += 8) {
            int temp = value >> (numBits - j);
            int index = temp % 256;
            result[resultIndex++] = (char)index;
        }
    }

    result[resultIndex] = '\0';
    return result;
}
