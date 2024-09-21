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
int reg[8] = {0,0,0,0,0,0,0,0};
int PC = 0;
int halted = 0;
char* fillvalue[50] = {"0"}; // .fill label and value
char* var[20] = {"0"};
long lineOffsets[100] = {0};
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

    int sizeoffill = 0; 
    int a = 0;
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        if( strcmp(opcode, ".fill") != 0 && strspn(label, " \t\n") != strlen(label)){ // keep PC of loop or function
            var[a] = strdup(label);
            var[a+1] = malloc(20 * sizeof(char));  
            sprintf(var[a+1], "%d", PC); // string to int
            fillvalue[sizeoffill] = strdup(label); // label
            fillvalue[sizeoffill+1] = strdup("0"); //value
            fillvalue[sizeoffill+2] = var[a+1];// PC
            a += 2;
            sizeoffill += 3;
        }
        if(strcmp(opcode, ".fill") == 0){ // keep .fill value in temp
            fillvalue[sizeoffill] = strdup(label); // label
            if(!isNumber(arg0)){ // in case of stAddr
                for (int j = 0; j < a; j++) {
                    if(strcmp(var[j], arg0) == 0){
                        fillvalue[sizeoffill+1] = var[j+1]; // value
                    }
                }  
            }else{
                fillvalue[sizeoffill+1] = strdup(arg0); //value
            }
            fillvalue[sizeoffill+2] = malloc(20 * sizeof(char));  
            sprintf(fillvalue[sizeoffill+2], "%d", PC); // PC
            sizeoffill+=3;
        }
        PC++;
    }
    PC = 0;

    //example of how to use movePtrTo 
    // movePtrTo(5,MAX_PC,inFilePtr,lineOffsets);
    // readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2);
    // printf("%s %s %s %s %s\n",label,opcode,arg0,arg1,arg2);


    // rewind(inFilePtr);

    // while(PC < MAX_PC){
    //     readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2);
    //     char* rs1 = conInt_to_Binary(3,arg0);
    //     char* rs2 = conInt_to_Binary(3,arg1);
    //     char* rd;
    //     char* offsetfild = {"0"};
    //     if(isNumber(arg2)){
    //         rd = conOffset(opcode, arg2);
    //         offsetfild = rd;
    //     }else{
    //         for (int j = 0; j < i; j++) {
    //             if(strcmp(fillvalue[j], arg2) == 0){
    //                 rd = fillvalue[j+1];
    //                 if(strcmp(opcode, "beq") == 0){
    //                     int beqoff = conString_base10_to_Int(fillvalue[j+2])-(PC+1);
    //                     offsetfild = malloc(20 * sizeof(char)); 
    //                     sprintf(offsetfild, "%d", beqoff);  
    //                 }else{
    //                     offsetfild = fillvalue[j+2];
    //                 }
    //                 break;
    //             }
    //         }               
    //         rd = conOffset(opcode, rd);
    //         offsetfild = conOffset(opcode, offsetfild);
    //     }
    //         if (!strcmp(opcode, "add")) {
    //             char* toBi[] = {first31to25,"000",rs1,rs2,"0000000000000",rd};
    //             machine[PC] = concatbinary(6,toBi);
    //             mem[PC] = conBi_to_Int(machine[PC]);
    //         }else if(!strcmp(opcode, "nand")){
    //             char* toBi[] = {first31to25,"001",rs1,rs2,"0000000000000",rd};
    //             machine[PC] = concatbinary(6,toBi);
    //             mem[PC] = conBi_to_Int(machine[PC]);
    //         }else if(!strcmp(opcode, "beq")){
    //             char* toBi[] = {first31to25,"100",rs1,rs2,offsetfild};
    //             machine[PC] = concatbinary(5,toBi);
    //             mem[PC] = conBi_to_Int(machine[PC]);
    //         }else if(!strcmp(opcode, "lw")){
    //             int symORnum = 0; // 0 : numeric,  1 : symbolic
    //             if(!isNumber(arg2)){
    //                 symORnum = 1;
    //             } 
    //             char* toBi[] = {first31to25,"010",rs1,rs2,offsetfild};
    //             machine[PC] = concatbinary(5,toBi);
    //             mem[PC] = conBi_to_Int(machine[PC]);
    //         }else if(!strcmp(opcode, "sw")){
    //             int symORnum = 0; // 0 : numeric,  1 : symbolic
    //             if(!isNumber(arg2)){
    //                 symORnum = 1;
    //             } 
    //             char* toBi[] = {first31to25,"011",rs1,rs2,offsetfild};
    //             machine[PC] = concatbinary(5,toBi);
    //             mem[PC] = conBi_to_Int(machine[PC]);
    //         }else if(!strcmp(opcode, "jalr")){
    //             char* toBi[] = {first31to25,"101",rs1,rs2,"0000000000000000"};
    //             machine[PC] = concatbinary(5,toBi);
    //             mem[PC] = conBi_to_Int(machine[PC]);               
    //         }else if(!strcmp(opcode, "halt")){
    //             char* toBi[] = {first31to25,"110","0000000000000000000000"};
    //             machine[PC] = concatbinary(3,toBi);
    //             mem[PC] = conBi_to_Int(machine[PC]);             
    //         }else if(!strcmp(opcode, "noop")){
    //             char* toBi[] = {first31to25,"111","0000000000000000000000"};
    //             machine[PC] = concatbinary(3,toBi);
    //             mem[PC] = conBi_to_Int(machine[PC]);            
    //         }else if(!strcmp(opcode, ".fill")){
    //             for(int a = 0; a < i; a++){
    //                 if(!strcmp(label, fillvalue[a])){
    //                     machine[PC] = conInt_to_Binary(32,fillvalue[a+1]);
    //                     mem[PC] = conBi_to_Int(machine[PC]);
    //                 }
    //             }
    //         } 
    //         PC++;
    // }

    // for(int i = 0; i < PC; i++){
    //     printf("%d\n",mem[i]);
    // }
    // printf("\n");

    for(int j = 0; j < sizeoffill; j+=3){
        printf("label : %s, value : %s, PC : %s\n",fillvalue[j],fillvalue[j+1],fillvalue[j+2]);
    }

    rewind(inFilePtr);
    while(PC < MAX_PC){
        printf("PC : %d\n",PC);
        int move = 0; // not move
        readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2);
        char* rs1 = conInt_to_Binary(3,arg0);
        char* rs2 = conInt_to_Binary(3,arg1);
        char* rd;
        char* offsetfild = {"0"};
        if(isNumber(arg2)){
            rd = conOffset(opcode, arg2);
            offsetfild = rd;
        }else{
            for (int j = 0; j < sizeoffill; j++) {
                if(strcmp(fillvalue[j], arg2) == 0){
                    rd = fillvalue[j+1];
                    if(strcmp(opcode, "beq") == 0){
                        int beqoff = conString_base10_to_Int(fillvalue[j+2])-(PC+1);
                        offsetfild = malloc(20 * sizeof(char)); 
                        sprintf(offsetfild, "%d", beqoff);  
                    }else{
                        offsetfild = fillvalue[j+2];
                    }
                    break;
                }
            }               
            rd = conOffset(opcode, rd);
            offsetfild = conOffset(opcode, offsetfild);
        }
            if (!strcmp(opcode, "add")) {
                add(rs1,rs2,rd);

            }else if(!strcmp(opcode, "nand")){
                nand(rs1,rs2,rd);

            }else if(!strcmp(opcode, "beq")){

                move = beq(rs1,rs2,offsetfild,move);
                if(move){
                    movePtrTo(PC,MAX_PC,inFilePtr,lineOffsets);
                }

            }else if(!strcmp(opcode, "lw")){
                lw(rs1,rs2,offsetfild,sizeoffill);

            }else if(!strcmp(opcode, "sw")){

                int symORnum = 0; // 0 : numeric,  1 : symbolic
                if(!isNumber(arg2)){
                    symORnum = 1;
                } 
                sw(rs1,rs2,rd);

            }else if(!strcmp(opcode, "jalr")){
                jalr(rs1,rs2);
                move = 1;
                movePtrTo(PC,MAX_PC,inFilePtr,lineOffsets);
                
            }else if(!strcmp(opcode, "halt")){
                halt();
                break;
                
            }else if(!strcmp(opcode, "noop")){
                noop();

            }
            if(!move){
                 PC++;
            }
            for(int i = 0; i < 8; i++){
                printf("reg[%d]: %d\n",i,reg[i]);
            }
            printf("--------------------------------------------------------\n");

    }
    
    return(0);
}

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