// Global.h
#ifndef GLOBAL_H
#define GLOBAL_H
#define MAXLINELENGTH 1000

// extern FILE *inFilePtr, *outFilePtr;
// extern char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], 
//             arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
// extern char line[MAXLINELENGTH];
extern int mem[65536]; // Declare the mem array as external
extern char* machine[65536];
extern int reg[8]; // Register array
extern int PC; // Program Counter
extern int move;  // Flag to indicate if the machine is halted
extern char* fillvalue[50]; // .fill label and value
extern char* var[20];
int sizeoffill;
int MAX_PC; 

#endif // GLOBAL_H
