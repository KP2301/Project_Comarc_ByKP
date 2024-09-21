#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Behave.h"
#include "Global.h"

// gcc -o Behave Behave.c 

void movePtrTo(int target_PC, int MAX_PC, FILE* inFilePtr, long lineOffset[]){
    if (target_PC <= MAX_PC) { // return to the targetline
        fseek(inFilePtr, lineOffset[target_PC]-1, SEEK_SET); 
    } else {
        printf("Line %d does not exist in the file.\n", target_PC);
    }
}

int conString_base10_to_Int(char* string){
    return strtol(string, NULL, 10);
}

int conBi_to_Int(char *a){
    int number = 0;
    int len = strlen(a);
    if(a[0] == '1'){ // for negative do two complement
        char* invert = (char*)malloc(len + 1); // +1 for negative num
        if (invert == NULL) {
            printf("Memory allocation failed!\n");
            return 0;
        }
        for(int i = 0; i < len; i++){
            invert[i] = (a[i] == '0') ? '1':'0';
        }
        invert[len] = '\0';
        number = strtol(invert, NULL, 2) + 1;
        number *= -1;
        free(invert);
    }else{ 
        // for positve
        number = strtol(a, NULL, 2); // convert string to long integer in base 2 
    }
    
    return number; 
}

int conBi_to_IntReg(char* bi){ 
    int result = strtol(bi, NULL, 2);
    if(result > 7 || result < 0){
        printf("input is only 3 bit");
        return -1;
    }
    return result;
}

char* conInt_to_Binary(int bits,char* nums){ // convert int to bits bit binary 
    int num = conString_base10_to_Int(nums);
    // Allocate memory for binary (+1 for \0)
    char *binary = (char *)malloc(bits + 1);  
    if (binary == NULL) {
        return NULL;  // Memory allocation failed
    }
    // go from i = 31 to i = 0
    for (int i = bits - 1; i >= 0; i--) {
        int bit = (num >> i) & 1;
        binary[bits - 1 - i] = bit ? '1' : '0';  
    }
    binary[bits] = '\0';  
    return binary; 
}

void add(char *regA, char *regB, char* destReg){
    int A = conBi_to_IntReg(regA);
    int B = conBi_to_IntReg(regB);
    int dest = conBi_to_IntReg(destReg);
    reg[dest] = reg[A] + reg[B];
}

void nand(char *regA, char *regB,char* destReg){
    int A = conBi_to_IntReg(regA);
    int B = conBi_to_IntReg(regB);
    int dest = conBi_to_IntReg(destReg);
    reg[dest] = ~(reg[A] & reg[B]) & 0xF; // To avoid the higher num and 2 com.
    //0xF is 4 bits for 8 is FF, and 16 is FFF 
}

void jalr(char* regA, char* regB){
    int A = conBi_to_IntReg(regA);
    int B = conBi_to_IntReg(regB);
    int Next_PC = PC + 1;
    if(reg[A] == reg[B]){
        reg[B] = Next_PC;
        PC = Next_PC;
    }else{
        reg[B] = Next_PC;
        PC = reg[A];
    }
}

int beq(char* regA, char* regB, char* offsetField, int move){
    int A = conBi_to_IntReg(regA);
    int B = conBi_to_IntReg(regB);
    if(strlen(offsetField) == 17){
        offsetField = conInt_to_Binary(32,offsetField);
    }
    if(reg[A] == reg[B]){
        PC = PC+1+conBi_to_Int(offsetField);
        return 1;
    }
    return 0;
}

// lw: Load word from memory into a register
void lw(char* regA, char* regB, char* offsetField){
    int A = conBi_to_IntReg(regA); 
    int B = conBi_to_IntReg(regB);
    
    if(strlen(offsetField) == 17){
        offsetField = conInt_to_Binary(32,offsetField);
    }

    int address = reg[A] + conBi_to_Int(offsetField);
    
    char* addressStr = malloc(20 * sizeof(char));
    sprintf(addressStr, "%d", address);
    for(int j = 0; j < sizeoffill; j+=3){
        if(!strcmp(addressStr,fillvalue[j+2])){
            reg[B] = conString_base10_to_Int(fillvalue[j+1]);
        }
    }
    
}

// sw: Store word from a register into memory
void sw(char *regA, char *regB, char *offsetField) {
    int A = conBi_to_IntReg(regA);
    int B = conBi_to_IntReg(regB);

    if(strlen(offsetField) == 17){
        offsetField = conInt_to_Binary(32,offsetField);
    }
    
    int address = reg[A] + conBi_to_Int(offsetField);

    char* addressStr = malloc(20 * sizeof(char));
    sprintf(addressStr, "%d", address);
    char* temp = malloc(20 * sizeof(char));
    sprintf(temp, "%d", reg[B]);
    for(int j = 0; j < sizeoffill; j+=3){
        if(!strcmp(addressStr,fillvalue[j+2])){
            fillvalue[j+1] = temp;
        }
    }
    
}

// halt: Set the halted flag and increment PC
void halt() {
    PC++;
    halted = 1;
}

// no-op: Increment PC without changing other state
void noop() {
    PC++;
}
