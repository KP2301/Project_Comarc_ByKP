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
void lw(char* regA, char* regB, char* offsetField, int symORmum){
    int A = conBi_to_IntReg(regA); 
    int B = conBi_to_IntReg(regB);
    
    if(strlen(offsetField) == 17){
        offsetField = conInt_to_Binary(32,offsetField);
    }
    
    int address = reg[A] + conBi_to_Int(offsetField);
    if(symORmum){ // 0 : numeric,  1 : symbolic
        reg[B] = conString_base10_to_Int(fillvalue[address]);
    }else{
        reg[B] = conString_base10_to_Int(offsetField);
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
    
    char* temp;
    sprintf(temp, "%d", reg[B]);
    fillvalue[address] = temp;
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

// int main() {
//     // printf("%s",conInt_to_Binary(32768));

//     // add
//     // reg[0] = 5;
//     // reg[1] = -10000;
//     // char* a = conInt_to_Binary(3,"0");
//     // char* b = conInt_to_Binary(3,"1");
//     // char* c = conInt_to_Binary(3,"0");
//     // add(c,a,b); // reg[0] = reg[0] - reg[1]
//     // printf("%d\n",reg[0]);

//     // nand
//     // char* a = "100";  //4
//     // char* b = "010";  //2
//     // char* c = "001";  //1
//     // reg[conBi_to_IntReg(a)] = 12; //1100
//     // reg[conBi_to_IntReg(b)] = 13; //1101
//     // nand(c,a,b);
//     // printf("%d\n",reg[1]); 

//     //jalr
//     // reg[1] = 3; //a
//     // reg[2] = 3; //b
//     // char* a = "001";  //1
//     // char* b = "010";  //2
//     // jalr(a,b);
//     // printf("reg[1] : %d, reg[2] (Next PC) : %d, PC : %d",reg[1],reg[2],PC);

//     //beq
//     // reg[1] = 3; //a
//     // reg[2] = 3; //b
//     // char* a = "001";  //1
//     // char* b = "010";  //2
//     // char* offset = conInt_to_Binary(16,"-1000");
//     // beq(a,b,offset);
//     // printf("PC : %d",PC);

//     // Initialize memory values for testing 'lw' and 'sw'
//     // mem[10] = 42;
//     // mem[14] = 99;

//     // lw
//     // reg[1] = 10;
//     // char* a = "001"; // reg[1]
//     // char* b = "010"; // reg[2]
//     // char* offset = conInt_to_Binary(4);
//     // lw(a, b, offset); 
//     // printf("reg[2]: %d", reg[2]); // reg[2]: 99

//     // sw
//     // reg[1] = 40;
//     // reg[2] = 20;
//     // char* a = "001"; // reg[1]
//     // char* b = "010"; // reg[2]
//     // char* offset = conInt_to_Binary(0);
//     // sw(a, b, offset);
//     // printf("mem[40]: %d", mem[40]); // mem[40]: 20

//     // halt
//     // halt();
//     // printf("PC: %d, halted: %d", PC, halted); // PC: 1, halted: 1

//     // no-op
//     // noop();
//     // printf("PC: %d", PC); // PC: 1


//     return 0;
// }