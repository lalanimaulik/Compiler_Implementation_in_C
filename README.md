# Tiny_Compiler

Extended basic implementation of a 'Tiny' programming language compiler and modified all the modules of the compiler to accommodate these operations and functionalities. Following are the four modules of the compiler:
Lex.tiny: This file defines legal tokens allowed in programming language. 

parse.tiny: This file defines precedence and associativity of operators and also creates syntax tree                       
                  which will be used by the following two modules. 

Constrainer.c: This file does all the validations and decorate the tree so that all the variables and                                                           functions point to appropriate declarations and type.

Codegenerator.c: This file generates machine code and creates stack.

Following are the features added to the programming language compiler.

1) Binary and Unary operations like: +,-, and,or,=,<>,>=,<,>,-,*,**,/,mod based on precedence and associativity.
2) Added constants like true and false.
3) Made else clause optional in if and else statement.
4) Added following statements and loops: for loop in pascal style i.e. upto and downto, repeat statement, case statement, loop-pool statement.
5) Added following data types: char, const in pascal style and enumerated data type. 
6) Added Intrinsic functions: succ, pred and ord for integers, characters and enumerated data types.
7) Input and output for both Integers and Characters.
8) Added functions and procedures to tiny.
