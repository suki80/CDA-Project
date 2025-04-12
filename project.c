#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
        
    if (ALUControl == '1') { // Add
        *ALUresult = A+B; 
    }
    if (ALUControl == '2') { // Subtract
        *ALUresult = A-B; 
    }
    if (ALUControl == '3') { // Set less than
        if ((int)A<(int)B) {
            *ALUresult = 1;
        }
        else {
            *ALUresult = 0;
        } 
    }
    if (ALUControl == '4') { // Set less than (unsigned integers)
        if (A<B) {
            *ALUresult = 1;
        }
        else {
            *ALUresult = 0;
        } 
    }
    if (ALUControl == '5') { // AND
        *ALUresult = A&B; 
    }
    if (ALUControl == '6') { // OR
        *ALUresult = A|B; 
    }
    if (ALUControl == '7') { // Shift B left by 16 bits
        *ALUresult = B<<16;
    }
    if (ALUControl == '8') { // NOT A
        *ALUresult = ~A; 
    }

    // Assign Zero to 1 if the result is zero
    if (ALUresult == 0) { 
        *Zero = 1; 
    }
    *Zero = 0;

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
    *op = (instruction >> 26) & 0x3F; // shift right 26 ms bits and mask 6 ls bits
    *r1 = (instruction >> 21) & 0x1F; // shift right 21 ms bits and mask 5 ls bits
    *r2 = (instruction >> 16) & 0x1F; // shift right 16 ms bits and mask 5 ls bits
    *r3 = (instruction >> 11) & 0x1F; // shift right 16 ms bits and mask 5 ls bits
    *funct = instruction & 0x3F; // mask 6 ls bits
    *offset = instruction & 0xFFFF; // mask 16 ls bits
    *jsec = (instruction >> 6) & 0x3FFFFFF; // shift right 6 ms bits and mask 26 ls bits

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
    *data1 = Reg[r1];
    *data2 = Reg[r2];
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
   
    char ALUControl;

    // Determine ALU operation based on ALUOp and funct
    if (ALUOp == 0) {
        if (funct == 0x20) {
            ALUControl = '1'; // Add
        }
        else if (funct == 0x22) {
            ALUControl = '2'; // Subtract
        }
        else if (funct == 0x2A) {
            ALUControl = '3'; // SLT
        }
        else if (funct == 0x2B) {
            ALUControl = '4'; // SLT(unsigned)
        }
        else if (funct == 0x24) {
            ALUControl = '5'; // AND
        }
        else if (funct == 0x25) {
            ALUControl = '6'; // OR
        }
        else if (funct == 0x00) {
            ALUControl = '7'; // SLL
        }
        else if (funct == 0x28) {
            ALUControl = '8'; // NOR(NOT)
        }
        else {
            return 1; // Halt
        }
          
    }
    else if (ALUOp == 1) {
        ALUControl = '1'; // Add

    }
    else if (ALUOp == 2) {
        ALUControl = '2'; // Subtract

    }
    else if (ALUOp == 3) {
        ALUControl = '5'; // AND

    }
    else {
        return 1; // Halt
    }

    // use data2
    if (ALUSrc == '0') {
        ALU(data1, data2, ALUControl, ALUresult, Zero);
    }
    // use extended_value
    else if (ALUSrc == '1') {
        ALU(data1, extended_value, ALUControl, ALUresult, Zero);
    }
    else {
        return 1; // Halt
    }

    return 0;
  
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
    unsigned dest;
    
    if (RegWrite != '1') {
        // no write
        return;
    }
    else {  
        // write
        if (RegDst == '0') {
            // write comes from rt field bits[20-16]
            dest = r2;
   
        }
        else if (RegDst == '1') {
           // write comes from rd field bits[15-11]
           dest = r3;
        }
   
        if (MemtoReg == '0') {
           // value comes from ALU
           Reg[dest] = ALUresult;
        }
       else if (MemtoReg == '1') {
           // value comes from data memory
           Reg[dest] = memdata;
        }
    }

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

