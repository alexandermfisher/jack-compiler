// Simple label
(LOOP)
   @LOOP
   0;JMP


// Label with trailing slash
(LABEL)
   @LABEL
   M=D+1

// Label with double slashes (comment after label)
(LABEL)//
   @LABEL
   0;JMP

// Label with a space before comment
(LABEL) // valid comment
   @LABEL
   D=D+A


// Valid labels with actual instructions
(LOOP1)
   @LOOP1
   D=D+1

(LOOP_END)
   @LOOP_END
   0;JMP

// Label with spaces inside
//( JUMP )

// Multiple words (should cause an error)
//( MULTI WORDS )

// Reserved keyword as label (should be rejected)
//(THIS)

// Label with extra closing parenthesis
// (LABEL))
