/* Assembler code fragment */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Behave.h"
#include "Global.h"

// gcc -o assembler assembler.c Behave.c
// ./Assembler Assembly_CODE.txt Out.txt

#define MAXLINELENGTH 1000
#define first31to25 "0000000"

int mem[65536];
char* machine[65536] = {0};
int reg[8] = {0,2,20,0,0,0,0,0};
int PC = 0;
int halted = 0;
char* fillvalue[50] = {"0"}; // .fill label and value
char* var[20] = {"0"};

long lineOffsets[100];
int target_PC = 0;
int MAX_PC = 0;

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);

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

char* conOffset(char* opcode, char* arg2){
    char* result;
    if((!strcmp(opcode, "beq")) || (!strcmp(opcode, "lw")) || (!strcmp(opcode, "sw"))){
        result = conInt_to_Binary(16,arg2);
    }else{
        result = conInt_to_Binary(3,arg2);
    }
    return result;
}

int main(int argc, char *argv[])
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
    char line[MAXLINELENGTH];

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

    //for return line
    while (fgets(line, MAXLINELENGTH, inFilePtr) != NULL) {
        lineOffsets[MAX_PC] = ftell(inFilePtr) - strlen(line);
        MAX_PC++;
    }
    rewind(inFilePtr);

    int i = 0; 
    int a = 0;
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        if( strcmp(opcode, ".fill") != 0 && strspn(label, " \t\n") != strlen(label)){ // keep PC of loop or function
            var[a] = strdup(label);
            var[a+1] = malloc(20 * sizeof(char));  
            sprintf(var[a+1], "%d", PC); // string to int
            a += 2;
        }
        if(strcmp(opcode, ".fill") == 0){ // keep .fill value in temp
            fillvalue[i] = strdup(label);
            if(!isNumber(arg0)){ // in case of stAddr
                for (int j = 0; j < a; j++) {
                    if(strcmp(var[j], arg0) == 0){
                        fillvalue[i+1] = var[j+1];
                    }
                }  
            }else{
                fillvalue[i+1] = strdup(arg0);
            }
            fillvalue[i+2] = malloc(20 * sizeof(char));  
            sprintf(fillvalue[i+2], "%d", PC);
            i+=3;
        }
        PC++;
    }
    PC = 0;

    // for(int i = 0; i < a; i+=2){
    //     printf("var %s:%s \n", var[i], var[i+1]);
    // }

    // for(int j = 0; j < i; j+=3){
    //     printf("temp %s:%s, PC: %s \n", fillvalue[j], fillvalue[j+1], fillvalue[j+2]);
    // }

    // printf("MAX_PC: %d\n", MAX_PC);
    // target_PC = 3;
    // if (target_PC <= MAX_PC) { // return to the targetline
    //     // Move to the start of the desired line
    //     fseek(inFilePtr, lineOffsets[target_PC - 1]-1, SEEK_SET); 
    //     // readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2);
    //     // printf("%s %s %s %s %s\n",label,opcode,arg0,arg1,arg2);
    // } else {
    //     printf("Line %d does not exist in the file.\n", target_PC);
    // }



    rewind(inFilePtr);
        readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2);
        char* rs1 = conInt_to_Binary(3,arg0);
        char* rs2 = conInt_to_Binary(3,arg1);
        char* rd;
        char* offsetfild;
        if(isNumber(arg2)){
            rd = conOffset(opcode, arg2);
            offsetfild = rd;
        }else{
            for (int j = 0; j < i; j++) {
                if(strcmp(fillvalue[j], arg2) == 0){
                    rd = fillvalue[j+1];
                    offsetfild = fillvalue[j+2];
                    break;
                }
            }               
            rd = conOffset(opcode, rd);
            offsetfild = conOffset(opcode, offsetfild);
        }
            if (!strcmp(opcode, "add")) {
                printf("%s, %s, %s \n",rs1,rs2,rd);
                char* toBi[] = {first31to25,"000",rs1,rs2,"0000000000000",rd};
                machine[PC] = concatbinary(6,toBi);
                mem[PC] = conBi_to_Int(machine[PC]);
                add(rs1,rs2,rd);
                printf("%s, %d, %d\n", machine[PC], mem[PC], reg[conBi_to_IntReg(rd)]);
            }else if(!strcmp(opcode, "beq")){
                printf("%s, %s, %s \n",rs1,rs2,rd);
                char* toBi[] = {first31to25,"100",rs1,rs2,rd};
                machine[PC] = concatbinary(5,toBi);
                mem[PC] = conBi_to_Int(machine[PC]);
                printf("%s, %d\n", machine[PC], mem[PC]);
            }else if(!strcmp(opcode, "lw")){
                printf("%s, %s, %s \n",rs1,rs2,rd);
                int symORnum = 0; // 0 : numeric,  1 : symbolic
                if(!isNumber(arg2)){
                    symORnum = 1;
                } 
                char* toBi[] = {first31to25,"010",rs1,rs2,offsetfild};
                machine[PC] = concatbinary(5,toBi);
                mem[PC] = conBi_to_Int(machine[PC]);
                // lw(rs1,rs2,rd,symORnum);// must use rd
                printf("%s, %d\n", machine[PC], mem[PC]);
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
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,char *arg1, char *arg2)
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