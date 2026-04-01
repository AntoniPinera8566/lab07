/*
 ============================================================================
 Name        : main.c
 Author      : Austin Tian
 Description : main code in C
 ============================================================================
 */
#include "header.h"

// Actual definitions of the global variables for the linker
MIPS_Instruction Instruction_storage[MAX_SIZE];
int regFile[N_REG];
char Data_storage[MAX_SIZE];
unsigned int totalDataByte;
labelType labelTab[MAX_LABEL];

int main (int argc, char *argv[]) {        
	    char *mem = NULL;  
        FILE *fp = NULL;
        int stopChar;

		if (argc < 2) {
            puts("\nIncorrect number of arguments.");
            puts("Usage: ProgramName.exe MIPSCode.asm \n");
            stopChar = getchar();
            return EXIT_FAILURE;  
        }
        if ((fp = fopen(argv[1], "r"))== NULL) {  
                printf("Input file could not be opened.");
                stopChar = getchar();
                return EXIT_FAILURE;  
        }
        parse_MIPS(fp);  
		
        mem = init_memory();  
        stopChar = getchar();
        
        puts("----Lab 7 Code Starts to Parse the ASM Code----");
        loadCodeToMem(mem);
        stopChar = getchar();

        free_memory(mem);  
        return 0;
}