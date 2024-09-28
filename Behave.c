#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Behave.h"
#include "Global.h"

// gcc -o Behave Behave.c 
int stackpointer = 7;

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
    if(A == stackpointer){
        MAX_PC += reg[B];
    }
    if(B == stackpointer){
        MAX_PC += reg[A];
    }
    PC++;
    instruction++;
}

void nand(char *regA, char *regB,char* destReg){
    int A = conBi_to_IntReg(regA);
    int B = conBi_to_IntReg(regB);
    int dest = conBi_to_IntReg(destReg);
    reg[dest] = ~(reg[A] & reg[B]); 
    PC++;
    instruction++;
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
    instruction++;
}

void beq(char* regA, char* regB, char* offsetField){
    int A = conBi_to_IntReg(regA);
    int B = conBi_to_IntReg(regB);
    if(strlen(offsetField) == 17){
        offsetField = conInt_to_Binary(32,offsetField);
    }
    if(reg[A] == reg[B]){
        PC = PC+1+conBi_to_Int(offsetField);
        move = 1;
    }else{
        PC++;
    }
    instruction++;
}

// lw: Load word from memory into a register
void lw(char* regA, char* regB, char* offsetField, int MEM[]){
    int A = conBi_to_IntReg(regA); 
    int B = conBi_to_IntReg(regB);
    
    if(strlen(offsetField) == 17){
        offsetField = conInt_to_Binary(32,offsetField);
    }
    int address = reg[A] + conBi_to_Int(offsetField);
    reg[B] = MEM[address];
    PC++;
    instruction++;
}

// sw: Store word from a register into memory
void sw(char *regA, char *regB, char *offsetField, int MEM[]) {
    int A = conBi_to_IntReg(regA);
    int B = conBi_to_IntReg(regB);

    if(strlen(offsetField) == 17){
        offsetField = conInt_to_Binary(32,offsetField);
    }
    
    int address = reg[A] + conBi_to_Int(offsetField);
    MEM[address] = reg[B];
    PC++;
    instruction++;
}

// halt: Set the halted flag and increment PC
void halt() {
    PC++;
    instruction++;
}

// no-op: Increment PC without changing other state
void noop() {
    PC++;
    instruction++;
}

char* conOffset(char* opcode, char* arg2)
{
    char* result;
    if((!strcmp(opcode, "beq")) || (!strcmp(opcode, "lw")) || (!strcmp(opcode, "sw"))){
        result = conInt_to_Binary(16,arg2);
    }else{
        result = conInt_to_Binary(3,arg2);
    }
    return result;
}

