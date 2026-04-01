#include "header.h"

char *menu =    "\n" \
                " ***********Please select the following options**********************\n" \
                " * This is the memory operation menu (Lab 6)                     *\n" \
                " ********************************************************************\n" \
                " * 1. Write a double-word (32-bit) to the memory                 *\n"  \
                " ********************************************************************\n" \
                " * 2. Read a byte (8-bit) data from the memory                   *\n" \
                " * 3. Read a double-word (32-bit) data from the memory           *\n" \
                " ********************************************************************\n" \
                " * 4. Generate a memory dump from any memory location            *\n" \
                " ********************************************************************\n" \
                " * e. To Exit, Type 'e'  or 'E'                                  *\n" \
                " ********************************************************************\n";

unsigned char rand_generator()
{
    return rand()%255;  
}
void free_memory(char *base_address)
{
    free(base_address);  
    return;
}
char *init_memory()
{
    char *mem = malloc(MEM_SIZE);  
   if (mem == NULL){ 
	   printf("Memory allocation failed \n"); 
	   return NULL; 
   }
   
   for (int i= 0; i < MEM_SIZE; i++){ 
	   mem[i] = 0; // Fill with 0 for Lab 7
  }
    return mem;
}

void write_dword(const char *base_address, const int offset, const unsigned int dword_data){
	unsigned int *target_address = (unsigned int *)((char *)base_address + offset);
	*target_address = dword_data;
    printf("Successfully wrote double-word %08X to offset %X\n", dword_data, offset);
}
void write_byte(const char *base_address, const int offset, const unsigned char byte_data){
    *(char *)(base_address + offset) = byte_data;
}
unsigned char read_byte(const char *base_address, const int offset){
	unsigned char data = (unsigned char)base_address[offset]; 
    printf("Byte read from offset %X: %02X\n", offset, data);
    return data;
}
unsigned int read_dword(const char *base_address, const int offset){
	unsigned int *target_address = (unsigned int *)((char *)base_address + offset);
    unsigned int data = *target_address;
    printf("Double-word read from offset %X: %08X\n", offset, data);
    return data;
}
void memory_dump(const char *base_address, const int offset, unsigned int dumpsize){
  
if (dumpsize < MIN_DUMP_SIZE || dumpsize > MEM_SIZE)
        dumpsize = MIN_DUMP_SIZE;  

    for (unsigned int i = 0; i < dumpsize; i += DUMP_LINE) {
        // Corrected pointer cast for 64-bit
        unsigned long long current_addr = (unsigned long long)(uintptr_t)(base_address + offset + i);
        printf("%I64X: ", current_addr);

        for (int j = 0; j < DUMP_LINE; j++) {
            if (i + j < dumpsize) {
                unsigned char val = (unsigned char)base_address[offset + i + j];
                printf("%02X ", val);
            } else {
                printf("   "); 
            }
        }
        printf(" --- ");
        for (int j = 0; j < DUMP_LINE; j++) {
            if (i + j < dumpsize) {
                unsigned char val = (unsigned char)base_address[offset + i + j];
                if (val >= 0x20 && val <= 0x7E) {
                    printf("%c ", val);
                } else {
                    printf(". ");
                }
            }
        }
        printf("\n"); 
    }
    return;
}

void setup_memory()
{
    char *mem = init_memory();  
    char options =0;
    unsigned int offset, dumpsize;
    char tempchar;
    unsigned int dword_data;      
    do{
        if (options != 0x0a)  
        {
            puts(menu); 
            printf ("\nThe base address of your memory is: %I64Xh (HEX)\n", (long long unsigned int)(mem));  
            puts("Please make a selection:");  
        }
            options = getchar();
            switch (options)
            {
                case '1':  
                    puts("Please input your memory's offset address (in HEX):");
                    scanf("%x", (unsigned int*)&offset);    
                    puts("Please input your DOUBLE WORD data to be written (in HEX):");
                    scanf("%x", (unsigned int*)&dword_data);    
                    write_dword (mem, offset, dword_data);  
                    continue;
                case '2':  
                    puts("Please input your memory's offset address (in HEX):");
                    scanf("%x", &offset);    
                    read_byte(mem, offset);
                    continue;
                case '3':  
                    puts("Please input your memory's offset address (in HEX):");
                    scanf("%x", &offset);    
                    read_dword(mem, offset);
                    continue;
                case '4':  
                    puts("Please input your memory's offset address (in HEX, should be a multiple of 0x10h):");
                    scanf("%x", &offset);    
                    puts("Please input the size of the memory to be dumped (a number between 256 and 1024 ):");
                    scanf("%d", &dumpsize);    
                    memory_dump(mem, offset, dumpsize);  
                    continue;
                case 'e':
                case 'E':
                    puts("Code finished, press any key to exit");
                    free_memory(mem);
                    while ((tempchar = getchar()) != '\n' && tempchar != EOF);  
                    tempchar = getchar();
                    return;  
                default:
                    continue;
            }
    }while (1);  
    return;
}