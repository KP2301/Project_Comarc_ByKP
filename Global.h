// Global.h
#ifndef GLOBAL_H
#define GLOBAL_H
#define MAXLINELENGTH 1000

extern int reg[8]; // Register array
extern int PC; // Program Counter
extern int move;  // Flag to indicate if the machine is halted
int sizeoffill;
int MAX_PC;
int instruction; 
int stackpointer;

#endif // GLOBAL_H
