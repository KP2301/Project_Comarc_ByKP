/* instruction-level simulator */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Behave.h"
#include "Global.h"

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

int reg[8] = {0,0,0,0,0,0,0,0};
long lineOffsets[100] = {0};
int MAX_PC = 0;
int PC = 0;
int move = 0;
char* fillvalue[50] = {"0"};

char Opcode[5]; 
char regA[4];
char regB[4];
char rd[4];
char offset[17];
char* binary[8];
typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);

void RTYPE(){
    int pos = 10;
    int rdpos = 29;
    for(int i = 0; i < 3; i++){
        regA[i] = binary[PC][pos];
        regB[i] = binary[PC][pos+3];
        rd[i] = binary[PC][rdpos + i];
        pos++;
    }
}

void ITYPE(){
    int pos = 10;
    for(int i = 0; i < 3; i++){
        regA[i] = binary[PC][pos];
        regB[i] = binary[PC][pos+3];
        pos++;
    }
    int posoff = 0;
    for(int j = 17; j < 32; j++){
        offset[posoff] = binary[PC][j];
        posoff++;
    }
}

void JTYPE(){
    int pos = 10;
    for(int i = 0; i < 3; i++){
        regA[i] = binary[PC][pos];
        regB[i] = binary[PC][pos+3];
        pos++;
    }
}

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
	printf("error: usage: %s <machine-code file>\n", argv[0]);
	exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;state.numMemory++) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    char* mach = malloc(20 * sizeof(char));
    state.pc = 0;

    for(int i = 0; i < 8; i++){
        state.reg[i] = reg[i];
    }
    printState(&state);

    while(PC < state.numMemory){
            move = 0;
            sprintf(mach, "%d", state.mem[PC]);
            binary[PC] = conInt_to_Binary(32,mach);
            int pos = 7; 
            for(int i = 0; i < 3; i++){
                Opcode[i] = binary[PC][pos];
                pos++;
            }
            // Opcode[0] = binary[PC][7]; // Bit 24:  7 = 31-24
            Opcode[5] = '\0';

            // printf("%s\n",binary[state.pc]);
            if(!strcmp(Opcode, "000")){
                strcpy(Opcode, "add");
                RTYPE();
                add(regA,regB,rd);
            }else if(!strcmp(Opcode, "001")){
                strcpy(Opcode, "nand");
                RTYPE();
                nand(regA,regB,rd);
            }else if(!strcmp(Opcode, "010")){
                strcpy(Opcode, "lw");
                ITYPE();
                lw(regA,regB,offset,state.mem);
            }else if(!strcmp(Opcode, "011")){
                strcpy(Opcode, "sw");
                ITYPE();
                sw(regA,regB,offset,state.mem);
            }else if(!strcmp(Opcode, "100")){
                strcpy(Opcode, "beq");
                ITYPE();
                beq(regA,regB,offset);
            }else if(!strcmp(Opcode, "101")){
                strcpy(Opcode, "jalr");
                JTYPE();
                jalr(regA,regB);
            }else if(!strcmp(Opcode, "110")){
                strcpy(Opcode, "halt");
                halt();
                break;
            }else if(!strcmp(Opcode, "111")){
                strcpy(Opcode, "noop");
                noop();
            }
            if(!move){
                PC++;  
            }
            state.pc = PC;
            for(int i = 0; i < 8; i++){
                state.reg[i] = reg[i];
            }
            printState(&state);
            // printf("Opcode: %s, regA : %s, regB : %s, rd : %s, offset : %s \n",Opcode,regA,regB,rd,offset);
        
    }


    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
	for (i=0; i<statePtr->numMemory; i++) {
	    printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
	}
    printf("\tregisters:\n");
	for (i=0; i<NUMREGS; i++) {
	    printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
	}
    printf("end state\n");
}
