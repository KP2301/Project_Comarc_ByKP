// Behave.h
#ifndef BEHAVE_H
#define BEHAVE_H

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000
typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

int conString_base10_to_Int(char* string);
int conBi_to_Int(char *a);
int conBi_to_IntReg(char* bi);
char* conInt_to_Binary(int bits, char* nums);
void add(char* destReg, char *regA, char *regB);
void nand(char *regA, char *regB,char* destReg);
void jalr(char* regA, char* regB);
int beq(char* regA, char* regB, char* offsetField, int move);
void lw(char* regA, char* regB, char* offsetField, int sizeofF);
void sw(char *regA, char *regB, char *offsetField, int sizeofF);
void halt();
void noop();
void movePtrTo(int target_PC, int MAX_PC, FILE* inFilePtr, long lineOffset[]);
void printState(stateType *);

#endif // BEHAVE_H
