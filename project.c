#include "spimcore.h"
#include <stdio.h>


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    switch(ALUControl){
        case 0: // ADD
            *ALUresult = A + B; // ADD
            break;
        case 1: // SUBSTRACT
            *ALUresult = A - B;
            break;
        case 2: // SLT (signed)
            if((int) A < (int) B)
                *ALUresult = 1;
            else
                *ALUresult = 0;
            break;
        case 3: // SLT (unsigned)
            if(A < B)
                *ALUresult = 1;
            else
                *ALUresult = 0;
            break;
        case 4: // A AND B
            *ALUresult = A & B;
            break;
        case 5: // A OR B
            *ALUresult = A | B;
            break;
        case 6: // Shift left B by 16 bits
            *ALUresult = B << 16;   
            break;
        case 7: // NOT A
            *ALUresult = ~A;
            break;
    }

    // Assign Zero to 1 if the result is zero
    if(*ALUresult == 0)
        *Zero = 1;
    else
        *Zero = 0;
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    //calculating the word address ( / 4)
    unsigned word_addr = PC >> 2;

    //checking for multiple of 4
    if (PC % 4 != 0) {
        return 1; 
    }

    //fetching instruction from memory
    *instruction = Mem[word_addr];

    return 0; 

}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    *op = (instruction >> 26) & 0x3F; // instruction [31-26]
    *r1 = (instruction >> 21) & 0x1F; // instruction [25-21]
    *r2 = (instruction >> 16) & 0x1F; // instruction [20-16]
    *r3 = (instruction >> 11) & 0x1F; // instruction [15-11]
    *funct = instruction & 0x3F; // instruction [5-0]
    *offset = instruction & 0xFFFF; // instruction [15-0]
    *jsec = instruction & 0x3FFFFFF; // instruction [25-0]
}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
    // the values we set depend on which opcode is recieved
    switch(op){
        //R Type Instructions
        case 0:
            controls->RegDst = 1;
            controls->RegWrite = 1;
            controls->ALUSrc = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 7;
            break;
        //End R Type Instructions
        
        //I Type Instructions
        //addi
        case 8:
            controls->RegDst = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 1;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 0;
            break;
        
        //slt
        case 10:
            controls->RegDst = 1;
            controls->RegWrite = 1;
            controls->ALUSrc = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 2;
            break;
            
        //sltu
        case 11:
            controls->RegDst = 1;
            controls->RegWrite = 1;
            controls->ALUSrc = 0 ;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 3;
            break;
            
        //beq
        case 4:
            controls->RegDst = 2;
            controls->RegWrite = 0;
            controls->ALUSrc = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 2;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->ALUOp = 1;
            break;
            
        //lw
        case 35:
            controls->RegDst = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 1;
            controls->MemRead = 1;
            controls->MemWrite = 0;
            controls->MemtoReg = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 0;
            break;
            
        //lui
        case 15:
            controls->RegDst = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 1;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 6;
            break;
            
        //sw
        case 43:
            controls->RegDst = 2; // 2 for don't care
            controls->RegWrite = 0;
            controls->ALUSrc = 1;
            controls->MemRead = 0;
            controls->MemWrite = 1;
            controls->MemtoReg = 2;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->ALUOp = 0;
            break;
        //End I Type Instructions
        
        //J Type Instructions
        //Jump
        case 2:
            controls->RegDst = 0;
            controls->RegWrite = 0;
            controls->ALUSrc = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->Jump = 1;
            controls->Branch = 0;
            controls->ALUOp = 0;
            break; 
        //End J Type Instructions
        
        default:
            return 1;
    }
    return 0;
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
    unsigned neg = offset >> 15;
    if (neg == 1)
    {
        *extended_value = offset | 0xFFFF0000;
    }
    else
    {
        *extended_value = offset & 0x0000FFFF;
    }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    if (ALUSrc == 1) {
		data2 = extended_value;
	}

    // Determine ALU operation based on ALUOp and funct
    if (ALUOp == 7) {
        if (funct == 32) {
            ALUOp = 0; // Add
        }
        else if (funct == 34) {
            ALUOp = 1; // Subtract
        }
        else if (funct == 42) {
            ALUOp = 2; // SLT(signed)
        }
        else if (funct == 43) {
            ALUOp = 3; // SLT(unsigned)
        }
        else if (funct == 36) {
            ALUOp = 4; // AND
        }
        else if (funct == 37) {
            ALUOp = 5; // OR
        }
        else if (funct == 4) {
            ALUOp = 6; // SLL
        }
        else if (funct == 39) {
            ALUOp = 7; // NOR 
        }
        else {
            return 1; // Halt
        }

        ALU(data1, data2, ALUOp, ALUresult, Zero);

    }
    else {
        ALU(data1, data2, ALUOp, ALUresult, Zero);
    }

    return 0;

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{       
    if (MemRead == 1) {
        if (ALUresult % 4 != 0) {
            return 1; // Address not aligned
        }

        *memdata = Mem[ALUresult >> 2];
    }

    if (MemWrite == 1) {
        if (ALUresult % 4 != 0) {
            return 1; // Address not aligned
        }
        Mem[ALUresult >> 2] = data2;
    }

    return 0;

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    if(RegWrite == 1) {
        // mem to reg
        if(MemtoReg == 1 && RegDst == 0) {
            Reg[r2] = memdata;
        }
        else if(MemtoReg == 1 && RegDst == 1) {
            Reg[r3] = memdata;
        }
        // store ALUResult 
        else if(MemtoReg == 0 && RegDst == 0) {
            Reg[r2] = ALUresult;
        }
        else if(MemtoReg == 0 && RegDst == 1) {
            Reg[r3] = ALUresult;
        }
    }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    *PC += 4; // Default: move to next instruction

    if (Branch == 1 && Zero == 1)
    {
        // Branch target is PC + 4 + (sign-extended offset << 2)
        *PC += (extended_value << 2);
    }

    if (Jump == 1)
    {
        // Jump target is {PC[31:28], jsec, 00}
        *PC = (jsec << 2) | (*PC & 0xF0000000);
    }
}