    // inFileString = argv[2];
    // inFilePtr = fopen(inFileString, "r");
    // if (inFilePtr == NULL) {
    //     printf("error in opening %s\n", inFileString);
    //     exit(1);
    // }

    // //for return line
    // while (fgets(line, MAXLINELENGTH, inFilePtr) != NULL) {
    //     lineOffsets[MAX_PC] = ftell(inFilePtr) - strlen(line);
    //     MAX_PC++;
    // }
    // rewind(inFilePtr);

    // int a = 0;
    // while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
    //     if( strcmp(opcode, ".fill") != 0 && strspn(label, " \t\n") != strlen(label)){ // keep PC of loop or function
    //         var[a] = strdup(label);
    //         var[a+1] = malloc(20 * sizeof(char));  
    //         sprintf(var[a+1], "%d", PC); // string to int
    //         fillvalue[sizeoffill] = strdup(label); // label
    //         fillvalue[sizeoffill+1] = strdup("0"); //value
    //         fillvalue[sizeoffill+2] = var[a+1];// PC
    //         a += 2;
    //         sizeoffill += 3;
    //     }
    //     if(strcmp(opcode, ".fill") == 0){ // keep .fill value in temp
    //         fillvalue[sizeoffill] = strdup(label); // label
    //         if(!isNumber(arg0)){ // in case of stAddr
    //             for (int j = 0; j < a; j++) {
    //                 if(strcmp(var[j], arg0) == 0){
    //                     fillvalue[sizeoffill+1] = var[j+1]; // value
    //                 }
    //             }  
    //         }else{
    //             fillvalue[sizeoffill+1] = strdup(arg0); //value
    //         }
    //         fillvalue[sizeoffill+2] = malloc(20 * sizeof(char));  
    //         sprintf(fillvalue[sizeoffill+2], "%d", PC); // PC
    //         sizeoffill+=3;
    //     }
    //     PC++;
    // }
    // PC = 0;

    // for(int i = 0; i < 8; i++){
    //     state.reg[i] = reg[i];
    // }
    // state.pc = PC;
    // printState(&state);

    // rewind(inFilePtr);
    // while(PC < MAX_PC){
    //     // printf("PC : %d\n",PC);
    //     int move = 0; // not move
    //     readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2);
    //     char* rs1 = conInt_to_Binary(3,arg0);
    //     char* rs2 = conInt_to_Binary(3,arg1);
    //     char* rd;
    //     char* offsetfild = {"0"};
    //     if(isNumber(arg2)){
    //         rd = conOffset(opcode, arg2);
    //         offsetfild = rd;
    //     }else{
    //         for (int j = 0; j < sizeoffill; j++) {
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
    //             add(rs1,rs2,rd);

    //         }else if(!strcmp(opcode, "nand")){
    //             nand(rs1,rs2,rd);

    //         }else if(!strcmp(opcode, "beq")){

    //             move = beq(rs1,rs2,offsetfild,move);
    //             if(move){
    //                 movePtrTo(PC,MAX_PC,inFilePtr,lineOffsets);
    //             }

    //         }else if(!strcmp(opcode, "lw")){
    //             lw(rs1,rs2,offsetfild);

    //         }else if(!strcmp(opcode, "sw")){

    //             sw(rs1,rs2,offsetfild);

    //         }else if(!strcmp(opcode, "jalr")){
    //             jalr(rs1,rs2);
    //             move = 1;
    //             movePtrTo(PC,MAX_PC,inFilePtr,lineOffsets);
                
    //         }else if(!strcmp(opcode, "halt")){
    //             halt();
    //             break;
                
    //         }else if(!strcmp(opcode, "noop")){
    //             noop();

    //         }
    //         if(!move){
    //              PC++;
    //         }
    //         for(int i = 0; i < 8; i++){
    //             state.reg[i] = reg[i];
    //         }
    //         state.pc = PC;
    //         printState(&state);

    // }