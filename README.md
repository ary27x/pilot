PILOT IS A COMPILER WRITTEN PURELY IN C++

FILE EXTENSION : .pi

HOW TO SETUP AND RUN : https://github.com/ary27x/pilot/blob/main/HOWTORUN.md

FOLLOW THE PROJECT ON YOUTUBE : https://www.youtube.com/playlist?list=PLm7R-cUo29CVmWXQ2ZiaGcUIOVy0FEGaH


<h1>PILOT PROGRAMMING LANGUAGE SYNTAX DOCUMENTATION</h1>

*(The syntax of pilot programming language has very high resemblance to the english language , which makes it very easy to read and very beginner friendly.)*


**COMMENTS**


Comments are denoted using the ~ (BACK TICK) operator.

    ~ this is a comment

<hr>

**DECLARING VARIABLES**

Variables can be declared using the "is" keyword or "=".

The variable name has to start with either _ (underscore) or a character, but further on it has ability to have either underscore or alphanumeric characters.

    first is 10
    my_favorite_nubmer is 27

<hr>

**PRINT**

You can print a string , number , variable or the value of a mathematical calculation onto the screen.
Every string , number , var or math calc has to be separated using the comma (,) operator in the display field

Example : 

    display "Hello World!"
    display "This is the value of 7 sqaured : " , 7 times 7
<hr>

**USER INPUT**

We use the "get" keyword to take in input from the user , and we need not declare the variable first
For taking multiple inputs , the names of the corresponding variables has to be separated by the comman(,) operator

    display "Enter the first and the second number : "
              get firstnumber , secondnumber
<hr>

**SCOPING**

Scoping in python is done using identation , not curly brace (similar to python).
Statements with the same indentation have the same scope
<hr>

**LOOP**

Pilot currently supports "range" loop which follows the following syntax:

    range <value> to <value>:
        display "This is from the loop !"
        
In the above example , <value> can be an integer value , a predefined variable or a mathematical statement

    lower is 10
    upper is 20
    range lower to upper :
        display "This would be printed 10 times ! "

You also have the ability to assign an iterator for the loop.

    lower is 10
    upper is 20
    range lower to upper as i:
        display "This is the value of i : " , i ~ this would print the numbers from 10 to 20

<hr>

**IF ELSE** 

Pilot programming language follows the basic construct of if else.
Syntax :

    if <condition>:
        ~ if block
    else :
        ~ else block
        
the else block is optional, you can remove it if you wish to.
For the condition , we can make use of the following relational operators:

    if first < second  ~ less than operator
    if first <= second ~ less than equal to operator
    if first > second  ~ greater than operator
    if first >= second ~ greater than equal to operator
    if first == second ~ equals operator
    if first != second ~ not equals operator
    
Following is an example of getting two numbers from the user and printing the larger value

    display "Enter the first and the second number : "
    get first , second
    if first > second : 
        display first
    else : 
        display second

<hr>

**MATHEMATICS**

Pilot has the ability to perform basic mathematical operations using the following mathematical operators.

    result is first + second ~ addition operation
    result is first - second ~ subtraction operation
    result is first * second ~ multiplication operation
    result is first / second ~ division operation
    result is first % second ~ modulus operation
    

You can also chain the mathematical operators in the following manner:

    result is first + second * third / fourth - fifth

Pilot also employs translatable keywords for maths , which gets converted to mathematical operators.

List of all such translatable keyword :

| Keyword | Operator |
| ------  | -- |
| plus    | +  |
| minus   | -  |
| times   | *  |
| by      | /  |
| mod     | %  |

Here is an example of such keyword:

    result is first times second  ~  equivalent to result = first * second
    

<hr>





