// Global.h
#ifndef GLOBAL_H
#define GLOBAL_H

extern int mem[65536]; // Declare the mem array as external
extern char* machine[65536];
extern int reg[8]; // Register array
extern int PC; // Program Counter
extern int halted;  // Flag to indicate if the machine is halted
extern char* fillvalue[50]; // .fill label and value
extern char* var[20];
int sizeoffill; 

#endif // GLOBAL_H
