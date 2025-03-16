// Basic computations
   AD=A+1      // Increment A
   AMD=A        // D = A
   AM=M        // D = M
   MD=D+A      // D = D + A
   D=D-M      // D = D - M
   D=D&M      // D = D & M
   D=D|M      // D = D | M
   D=!D       // D = NOT D
   D=-D       // D = -D
   D=-1       // D = -1

   // Destination variations
   M=D        // M = D
   A=D        // A = D
   D=0        // D = 0
   M=1        // M = 1
   A=-1       // A = -1

   // Jump instructions
   D;JGT      // Jump if D > 0
   D;JEQ      // Jump if D == 0
   D;JGE      // Jump if D >= 0
   D;JLT      // Jump if D < 0
   D;JNE      // Jump if D != 0
   D;JLE      // Jump if D <= 0
   D;JMP      // Unconditional jump

   // Combined dest + comp + jump
   D=D+A;JGT  // If D + A > 0, jump
   D=D-M;JEQ  // If D - M == 0, jump
   M=D|M;JNE  // If D | M != 0, jump
   A=!A;JMP   // Always jump

   // Edge cases
   M=-1;JMP   // Always jump with -1 in M
   A=A+1      // Increment A
   D=A-1;JGE  // Jump if A-1 >= 0