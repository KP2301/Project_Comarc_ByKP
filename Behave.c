#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int reg[8] = {0};

int PC = 0;

// gcc -o Behave Behave.c 

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

int conBi_to_IntReg(char* bi){ //convert bi to int reg (0 - 7)
    return strtol(bi, NULL, 2);
}

char* conInt_to_Binary(int num){ // convert int to 32 bit binary
    int bits = 32;  
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

//add
void add(char* destReg,char *regA, char *regB ){
    int A = conBi_to_IntReg(regA);
    int B = conBi_to_IntReg(regB);
    if(A > 7 || A < 0 || B > 7 || B < 0){
        printf("input is only 3 bit");
        return exit(1);
    }
    int dest = conBi_to_IntReg(destReg);
    reg[dest] = reg[A] + reg[B];
}

//nand
void nand(char* destReg,char *regA, char *regB){
    int A = conBi_to_IntReg(regA);
    int B = conBi_to_IntReg(regB);
    if(A > 7 || A < 0 || B > 7 || B < 0){
        printf("input is only 3 bit");
        return exit(1);
    }
    int dest = conBi_to_IntReg(destReg);
    reg[dest] = ~(reg[A] & reg[B]) & 0xF; // To avoid the higher num and 2 com.
    //0xF is 4 bits for 8 is FF, and 16 is FFF 
}

//jumpr
void jalr(char* regA, char* regB){
    int A = conBi_to_IntReg(regA);
    int B = conBi_to_IntReg(regB);
    if(A > 7 || A < 0 || B > 7 || B < 0){
        printf("input is only 3 bit");
        return exit(1);
    }
    int Next_PC = PC + 1;
    if(reg[A] == reg[B]){
        reg[B] = Next_PC;
        PC = Next_PC;
    }else{
        reg[B] = Next_PC;
        PC = reg[A];
    }
}

void beq(char* regA, char* regB, char* offsetField){
    int A = conBi_to_IntReg(regA);
    int B = conBi_to_IntReg(regB);
    if(A > 7 || A < 0 || B > 7 || B < 0){
        printf("input is only 3 bit");
        return exit(1);
    }
    if(strlen(offsetField) == 16){
        offsetField = conInt_to_Binary(conBi_to_Int(offsetField));
    }
    if(reg[A] == reg[B]){
        PC = PC+1+conBi_to_Int(offsetField);
    }
}


int main() {
    // printf("%s",conInt_to_Binary(32768));

    //add
    // reg[0] = 5;
    // reg[1] = -1;
    // //add
    // char* a = conInt_to_Binary(0);
    // char* b = conInt_to_Binary(1);
    // char* c = conInt_to_Binary(0);
    // add(c,a,b); // reg[0] = reg[0] - reg[1]
    // printf("%d\n",reg[0]);

    // nand
    // char* a = "100";  //4
    // char* b = "010";  //2
    // char* c = "001";  //1
    // reg[conBi_to_IntReg(a)] = 12; //1100
    // reg[conBi_to_IntReg(b)] = 13; //1101
    // nand(c,a,b);
    // printf("%d\n",reg[1]); 

    //jumpr
    // reg[1] = 3; //a
    // reg[2] = 3; //b
    // char* a = "001";  //1
    // char* b = "010";  //2
    // jalr(a,b);
    // printf("reg[1] : %d, reg[2] (Next PC) : %d, PC : %d",reg[1],reg[2],PC);

    //beq
    // reg[1] = 3; //a
    // reg[2] = 3; //b
    // char* a = "001";  //1
    // char* b = "010";  //2
    // char* offset = conInt_to_Binary(-1000);
    // beq(a,b,offset);
    // printf("PC : %d",PC);


    return 0;
}