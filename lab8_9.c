/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
// Please finish the following functions for lab 8.
// Lab 8 will perform the following functions:
//   1. Fetch the code stored in memory
//   2. Decode the code and prepare for the execution of the code.
//   3. Setup the execution function for CPU.

// Lab 9 will perform the following functions:
//   4. Execute the code stored in the memory and print the results.
#include "header.h"
#include "lab8header.h"
extern int regFile[32]; // This is for the general-purpose registers which should be 32
extern char *regNameTab[N_REG];
unsigned int PCRegister; // PC register, always pointing to the next instruction.

void CPU(char *mem){
    unsigned int machineCode = 0;
    unsigned char opcode = 0;
    PCRegister = CODESECTION;  // at the beginning, PCRegister is the starting point,
                               // it should be a global integer defined in header.h
    do{
      printf("\nPC:%x\n", PCRegister);
      machineCode = CPU_fetchCode(mem, PCRegister);
      if (machineCode == 0)  // quit the program when machineCode is 0, that is the end of the code.
          break;  // break the infinite loop.
      PCRegister += 4;                                                                     // update the program counter
      opcode = CPU_Decode(machineCode);
      printf("Decoded Opcode is: %02X. \n", opcode);

      // Lab 9: Finish the execution of the code.
      // Only finish this part when the CPU_Decode is done.
       CPU_Execution(opcode, machineCode, mem);
    }while (1);  // This is an infinite while loop
                 // When you fetch a machineCode of 00000000, the loop breaks.
    printRegisterFiles();     // After the code execution, print all the register contents on screen.
    printDataMemoryDump(mem); // After the code execution, print the memory dump of the data section.
}

// Lab 8 - Step 1. Finish the CPU_fectchCode function to
//          read the code section from memory and
//          get the 32-bit machine code from the memory.
unsigned int CPU_fetchCode(char *mem, int codeOffset) {
    unsigned int machineCode = 0;
    // Little-Endian: byte 0 is the lowest bits, byte 3 is the highest
    machineCode = ((unsigned char)mem[codeOffset + 3] << 24) |
                  ((unsigned char)mem[codeOffset + 2] << 16) |
                  ((unsigned char)mem[codeOffset + 1] << 8)  |
                  ((unsigned char)mem[codeOffset]);
    return machineCode;
}

// Lab 8 - Step 2. Finish the CPU_Decode function to
//          decode the instruction and return the
//          opcode/function of the instruction.
//          Hints: Need to consider how to find the opcode/function from different types of instructions:
//                 i.e., I-, J- and R-type instructions.
//                 The return value should be a 6-bit bianry code.
unsigned char CPU_Decode(unsigned int machineCode){
    unsigned char opcode;
    opcode = (machineCode >> 26) & 0x3F;
    return opcode;
}

// Lab 9: Finish the function CPU_Execution to run all the instructions.
void CPU_Execution(unsigned char opcode, unsigned int machineCode, char *mem) {
    unsigned char rs = (machineCode >> 21) & 0x1F;
    unsigned char rt = (machineCode >> 16) & 0x1F;
    unsigned char rd = (machineCode >> 11) & 0x1F;
    short imm = (short)(machineCode & 0xFFFF);
    unsigned char func = machineCode & 0x3F;

    switch (opcode) {
        case 0x00: // R-Type
            if (func == 0x20) { // ADD instruction
                regFile[rd] = regFile[rs] + regFile[rt];
                printf("Executed: ADD $%d, $%d, $%d\n", rd, rs, rt);
            }
            break;

        case 0x04: // BEQ (Branch if Equal)
            if (regFile[rs] == regFile[rt]) {
                PCRegister += (imm << 2);
            }
            printf("Executed: BEQ $%d, $%d, offset %d\n", rs, rt, imm);
            break;

        case 0x08: // ADDI (Add Immediate)
            regFile[rt] = regFile[rs] + imm;
            printf("Executed: ADDI $%d, $%d, %d\n", rt, rs, imm);
            break;

        case 0x23: // LW (Load Word)
            regFile[rt] = ((unsigned char)mem[regFile[rs] + imm + 3] << 24) |
                          ((unsigned char)mem[regFile[rs] + imm + 2] << 16) |
                          ((unsigned char)mem[regFile[rs] + imm + 1] << 8)  |
                          ((unsigned char)mem[regFile[rs] + imm]);
            printf("Executed: LW $%d, %d($%d)\n", rt, imm, rs);
            break;

        case 0x2B: // SW (Store Word)
            mem[regFile[rs] + imm + 3] = (regFile[rt] >> 24) & 0xFF;
            mem[regFile[rs] + imm + 2] = (regFile[rt] >> 16) & 0xFF;
            mem[regFile[rs] + imm + 1] = (regFile[rt] >> 8) & 0xFF;
            mem[regFile[rs] + imm]     = (regFile[rt]) & 0xFF;
            printf("Executed: SW $%d, %d($%d)\n", rt, imm, rs);
            break;

        case 0x2F:
            regFile[rt] = imm;
            printf("Executed: LA $%d, 0x%X\n", rt, imm);
            break;

        default:
            printf("Opcode %02X not implemented.\n", opcode);
            break;
    }
}

// Lab 8 - Step 3. Print all the 32 registers in regFile and names saved in
//          regNameTab. For example, it should print
//          $zero = 0x00000000
//          $at  = ... ... etc.
void printRegisterFiles() {
    printf("\n--- Register File Dump ---\n");
    for (int i = 0; i < 32; i++) {
        printf("%s = 0x%08X\n", regNameTab[i], regFile[i]);
    }
}

 // Lab 8 - Step 4. Call function memory_dump and pass the proper parameters to dump the first 256
//           bytes from Data section.
void printDataMemoryDump(char *mem) {
    printf("\n--- Data Memory Dump (First 256 Bytes) ---\n");
    memory_dump(mem, DATASECTION, 256);
}
