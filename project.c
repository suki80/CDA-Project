#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    *Zero = 0;

    if (ALUControl == 1) { // Add
        *ALUresult = A+B;
        
    }

    if (ALUControl == 2) { // Subtract
        *ALUresult = A-B;
    }

    if (ALUControl == 3) { // A<B
        if ((int)A<(int)B) {
            *ALUresult = 1;
        }
        else {
            *ALUresult = 0;
        }
    }

    if (ALUControl == 4) { // A<B (unsigned integers)
        if (A<B) {
            *ALUresult = 1;
        }
        else {
            *ALUresult = 0;
        }
    }

    if (ALUControl == 5) { // AND
        *ALUresult = A&B;
    }

    if (ALUControl == 6) { // OR
        *ALUresult = A|B;
    }

    if (ALUControl == 7) { // Shift B left by 16 bits
        *ALUresult = B<<16;
    }

    if (ALUControl == 8) { // NOT A
        *ALUresult = ~A;
    }

    if (*ALUresult == 0) {
        *Zero = 1;
    }

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    //checking for multiple of 4
    if (PC % 4 != 0) {
        return 1; 
    }

    // checking for beyond memory bounds (0x0000 to 0xFFFF)
    if (PC > 0xFFFF) {
        return 1; 
    }

    //calculating the word address ( / 4)
    unsigned word_addr = PC >> 2;

    //checking if word address is within bounds
    if (word_addr >= (65536 >> 2)) {
        return 1; 
    }

    //fetching instruction from memory
    instruction = Mem[word_addr];

    //checking illegal instruction
    if (instruction == 0x00000000) {
        return 1; 
    }

    return 0; 

}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

