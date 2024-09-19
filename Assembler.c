/* Assembler code fragment */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// gcc -o Assembler Assembler.c
// ./Assembler Assembly_CODE.txt Out.txt

#define MAXLINELENGTH 1000
#define first31to25 "0000000"

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int mem[65536];
char* memo[65536];
int reg[8] = {0,0,12,0,0,0,0,0};
int PC = 0;
int halted = 0;  // Flag to indicate if the machine is halted

char* concatbinary(int numStrings, char *strings[]) {
    int totalLength = 0;
    for (int i = 0; i < numStrings; i++) {
        totalLength += strlen(strings[i]);
    }
    char *result = (char *)malloc(totalLength + 1);
    if (result == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    result[0] = '\0';
    for (int i = 0; i < numStrings; i++) {
        strcat(result, strings[i]);
    }
    return result;
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

void add(char* destReg,char *regA, char *regB ){
    int A = conBi_to_IntReg(regA);
    int B = conBi_to_IntReg(regB);
    int dest = conBi_to_IntReg(destReg);
    reg[dest] = reg[A] + reg[B];
}

void nand(char* destReg,char *regA, char *regB){
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

void beq(char* regA, char* regB, char* offsetField){
    int A = conBi_to_IntReg(regA);
    int B = conBi_to_IntReg(regB);
    if(strlen(offsetField) == 16){
        offsetField = conInt_to_Binary(32,offsetField);
    }
    if(reg[A] == reg[B]){
        PC = PC+1+conBi_to_Int(offsetField);
    }
}

// lw: Load word from memory into a register
void lw(char* regA, char* regB, char* offsetField){
    int A = conBi_to_IntReg(regA); 
    int B = conBi_to_IntReg(regB);

    if(A > 7 || A < 0 || B > 7 || B < 0){
        printf("input is only 3 bit");
        return exit(1);
    }
    
    if(strlen(offsetField) == 16){
        offsetField = conInt_to_Binary(32,offsetField);
    }
    
    int address = reg[A] + conBi_to_Int(offsetField);
    
    reg[B] = mem[address];
}

// sw: Store word from a register into memory
void sw(char *regA, char *regB, char *offsetField) {
    int A = conBi_to_IntReg(regA);
    int B = conBi_to_IntReg(regB);
    
    if (A > 7 || A < 0 || B > 7 || B < 0) {
        printf("Input is only 3-bit\n");
        exit(1);
    }

    if(strlen(offsetField) == 16){
        offsetField = conInt_to_Binary(32,offsetField);
    }
    
    int address = reg[A] + conBi_to_Int(offsetField);
    
    mem[address] = reg[B];
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

int main(int argc, char *argv[])
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    // if (argc != 3) {
    //     printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
    //         argv[0]);
    //     exit(1);
    // }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    // if (inFilePtr == NULL) {
    //     printf("error in opening %s\n", inFileString);
    //     exit(1);
    // }
    outFilePtr = fopen(outFileString, "w");
    // if (outFilePtr == NULL) {
    //     printf("error in opening %s\n", outFileString);
    //     exit(1);
    // }

    /* here is an example for how to use readAndParse to read a line from
        inFilePtr */
    if (! readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
        /* reached end of file */
    }
    
    //to run this code with Assembly_CODE.txt use gcc -o assembler assembler.c to compile and ./assembler Assembly_CODE.txt to run
    printf("%s %s %s %s %s\n",label,opcode,arg0,arg1,arg2); 
    // printf("%d",isNumber(arg2));

    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    rewind(inFilePtr);
    
    // while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
    //     /* reached end of file */
    //     printf("%s %s %s %s %s\n",label,opcode,arg0,arg1,arg2); 
    // }

    /* after doing a readAndParse, you may want to do the following to test the
        opcode */
    if (!strcmp(opcode, "add")) {
        char* rd = conInt_to_Binary(3,arg0);
        char* rs1 = conInt_to_Binary(3,arg1);
        char* rs2 = conInt_to_Binary(3,arg2);
        printf("%s, %s, %s \n",rd,rs1,rs2);
        char* toBi[] = {first31to25,"000",rs2,rs1,"0000000000000",rd};
        memo[0] = concatbinary(6,toBi);
        mem[0] = conBi_to_Int(memo[0]);
        add(rd,rs1,rs2);
        printf("%s, %d, %d\n", memo[0], mem[0], reg[conBi_to_IntReg(rd)]);
    }

    return(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
    char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
	/* reached end of file */
        return(0);
    }

    /* check for line too long (by looking for a \n) */
    if (strchr(line, '\n') == NULL) {
        /* line too long */
	printf("error: line too long\n");
	exit(1);
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
	/* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]",
        opcode, arg0, arg1, arg2);
    return(1);
}

int isNumber(char *string)
{
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}