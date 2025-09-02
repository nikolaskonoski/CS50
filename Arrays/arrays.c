#include <stdio.h>

int const size = 5; //Tamanho pré determinado do array para legibilidade e facilita caso queira mudar o tamanho

int main(void) {
    //Declare an array
    int numbers[size];
    //Para fazer um contador e usar no loop
    numbers[0] = 1;

    //Populate an array essa foi uma primeira forma de fazer
    /*numbers[0] = 1;
    numbers[1] = numbers[0] * 2;
    numbers[2] = numbers[1] * 2;
    numbers[3] = numbers[2] * 2;
    numbers[4] = numbers[3] * 2;
    */

    //int numbers [5] = {1, 2, 4, 8, 16}; Uma forma de fazer já sabendo os números

    //Forma de fazer com um loop for
    for (int i = 1; i < size; i++) {
        numbers[i] = numbers[i - 1] * 2;
    }

    //Print out the values one by one
    for (int i = 0; i < 5; i++) {
        printf("%i\n", numbers[i]);
    }
}
