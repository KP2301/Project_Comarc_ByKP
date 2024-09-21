// Behave.h
#ifndef BEHAVE_H
#define BEHAVE_H

int conString_base10_to_Int(char* string);
int conBi_to_Int(char *a);
int conBi_to_IntReg(char* bi);
char* conInt_to_Binary(int bits, char* nums);
void add(char* destReg, char *regA, char *regB);
void nand(char* destReg, char *regA, char *regB);
void jalr(char* regA, char* regB);
void beq(char* regA, char* regB, char* offsetField);
void lw(char* regA, char* regB, char* offsetField, int symORmum);
void sw(char *regA, char *regB, char *offsetField);
void halt();
void noop();

#endif // BEHAVE_H
