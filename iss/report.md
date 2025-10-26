# Lab 1 Assignment Report

## Student Information

Please type your name and student ID at here:

- Student Name: 劉天恩
- Student ID: E24137019

## Questions (20 pts)

There are **five** questions, and it is **4 pts** for each question.

### Question 1

**What is the benefit for a processor of using the MMIO scheme to control I/O devices?**

#### You Answer

Memory-mapped I/O lets the processor control devices by reading and writing to specific memory addresses. Instead of having special I/O instructions, the CPU just uses normal load and store instructions. In our lab code, each device like ROM, MainMem, TextBuffer, and Halt has its own address range set in iss.c using Core_add_device. When the processor writes to an address like 0xfffffff8, the MemoryMap_generic_store function finds that this address belongs to the text buffer and calls its store function, which prints a character on the screen. This makes the design easier and more flexible since all devices are controlled through the same memory access mechanism.

### Question 2

**What components in a RISC-V processor are considered part of the architectural visible state, and why are they important for program execution?**

#### Your Answer

The architectural visible state of a RISC-V processor includes the program counter (PC) and the 32 general-purpose registers (x0–x31). In the lab, these are stored in arch_state_t inside the Core structure. The PC holds the address of the current instruction, while the registers store data and addresses used during execution. They are important because they define the program’s current state, and any change to them directly affects how instructions are executed.

### Question 3

**Why does RISC-V define conditional set instructions? Can we use branch instructions only to achieve the same functionality? *If yes, what is the benefit of having the additional conditional set instructions?***

#### Your Answer

RISC-V defines conditional set instructions like SLT and SLTI to compare two values and store the result (0 or 1) in a register. This makes it easy to use the comparison result in later arithmetic or logical operations.

It is possible to do the same thing with branch instructions by checking a condition and jumping to different code paths, but that usually needs extra instructions and control flow changes. Conditional set instructions are faster and simpler because they don’t change the program counter. They avoid pipeline stalls caused by branching and make the code shorter and easier for the compiler to optimize.

### Question 4

**What is the different between `ROM` and `MainMem`? Answer this question by tracing the source code.**

#### Your Answer

ROM and MainMem are both memory devices, but they work differently based on how they handle read and write operations. From the source code, ROM is defined in rom.c and uses the ROM_ctor function. Its store function calls Panic("ROM should not be modified!"), which means it is read-only and cannot be written after initialization. The program instructions are loaded into ROM during startup by the load_elf function.

MainMem on the other hand is defined in main_mem.c, has both load and store functions that copy data between the internal memory array and the processor. This makes it read–write memory, where data can be changed during program execution. So, ROM is for fixed code or data that doesn’t change, while MainMem is for dynamic data and stack usage during execution.

### Question 5

**Consider the instruction: jalr xN, xN, imm, where N is a positive number from 1 to 31.**
**What potential problem might arise when implementing the JALR instruction in an ISS, particularly for this special case where the destination register is the same as the source register?**

#### Your Answer

When executing jalr xN, xN, imm, the instruction needs to store the return address (PC + 4) into xN and then jump to the new address given by xN + imm.

The problem happens if the simulator updates xN before calculating the jump target. Once xN is overwritten with PC + 4, its original value is lost, so the jump address becomes incorrect. This can cause the program to jump to the wrong location. To fix this, the simulator should first compute the target address using the old value of xN, then write the return address into xN.

### Question 6 (2 pts for bonus)

**Why do the encoding formats for JAL and BRANCH instructions omit the least-significant bit (LSB, `imm[0]`) of immediates?**

#### Your Answer

Instruction targets are at least 2-byte aligned in RISC-V, so the least-significant bit of any valid jump or branch target is always 0. JAL and BRANCH therefore encode their immediates in half-word units and omit imm[0]. The hardware just shifts the decoded immediate left by 1 when adding it to the PC.

This saves a bit that would always be 0 and effectively doubles the reachable range for the same field width. It also keeps the encoding compact. In the lab headers you can see this idea: the immediate fields are named imm_10_1, imm_4_1, etc., which means “bits 10..1,” and the missing bit 0 is implied to be 0 when forming the byte offset.

## Reflection Report (0 pts)

In this section, please write a short reflection about the lab:

1. What did you learn from completing this assignment?
2. What challenges did you encounter, and how did you solve them?
3. What feedback would you like to give to the TAs regarding this lab?

### You Report

1. I learned how a basic RISC-V processor works through the fetch, decode, and execute stages. I also understood how memory-mapped I/O connects different devices like ROM, MainMem, and TextBuffer to the CPU.

2. The biggest challenge was understanding the C code since I am not very familiar with the language. I solved it by using ChatGPT to help explain and translate the code into simpler terms so I could follow the logic.

3. I think the TAs could provide clearer and easier-to-follow instructions for the lab. The assignment was quite challenging, so having more examples or step by step instructions would make it easier for students to complete and learn from.
