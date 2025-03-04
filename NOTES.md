Designing the CLI Utility

A fully-fledged compiler would typically wrap everything into a single tool that can go from Jack source code to Hack machine code. However, since you’re implementing each stage separately, you could take one of two approaches:
1.	Separate Binaries: Each stage (jackc, vmtrans, hackasm) is its own executable. Users manually run them in sequence.
2.	Unified Compiler: A single CLI tool (jackcc) that detects input type and runs the required passes internally.


TODO:
- main:
    - complete has_extension and change_extension
      - check for most robust way strchc vs numeric length
      - unit test 
    - allows for output file argument, if not provided uses filename as in put with .hack extension
    - check main block comment
  
- assembler accepts two file pointers 
  - instantiate parser

- parser header file and typedefs 
- implement interface as stated in nand2tetris 

