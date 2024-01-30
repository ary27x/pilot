DOCUMENTATION EXPLAINING THE SYNTAX OF PILOT PROGRAMMING LANGUAGE

The syntax of pilot programming language has very high resemblance to the english language , which makes it very easy to read and very beginner friendly.

-> COMMENTS :
Comments are denoted using the ~ (BACK TICK) operator.
Example : ~ this is a comment

-> DECLARING VARIABLES :
Variables can be declared using the "is" keyword or "="
Example : first is 10


-> PRINT:
You can print a string , number , variable or the value of a mathematical calculation onto the screen.
Every string , number , var or math calc has to be separated using the comma (,) operator in the display field
Example : display "Hello World!"
Example : display "This is the value of 7 sqaured : " , 7 times 7


-> USER INPUT:
We use the "get" keyword to take in input from the user , and we need not declare the variable first
For taking multiple inputs , the names of the corresponding variables has to be separated by the comman(,) operator
Example : display "Enter the first and the second number : "
          get firstnumber , secondnumber

-> SCOPING:
Scoping in python is done using identation , not curly brace (similar to python).
Statements with the same indentation have the same scope

-> LOOP :
Pilot currently supports "range" loop which follows the following syntax:
Example: range <value> to <value>:
              display "This is from the loop !"
In the above example , <value> can be an integer value , a predefined variable or a mathematical statement
Example : lower is 10
          upper is 20
          range lower to upper :
                  display "This would be printed 10 times ! "

You also have the ability to assign an iterator for the loop.
Example: lower is 10
         upper is 20
         range lower to upper as i:
               display "This is the value of i : " , i ~ this would print the numbers from 10 to 20


-> IF ELSE : 
Pilot programming language follows the basic construct of if else.
Syntax :
if <condition>:
    ~ if block
else :
    ~ else block

Example:
display "Enter the first and the second number : "
get first , second
if first > second : 
    display "The first number is greater than the second number "
else : 
    display "The second number is greater than the first number "

   
(For various instances of an operator, you could use a correponsding keyword which makes it very readable
Example : varialble = variable + 2 could be written as variable is variable plus 2 )
