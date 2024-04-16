PILOT IS A TURING COMPLETE , MULTI-LINGUAL PROGRAMMING LANGUAGE WHOSE SOURCE CODE CAN EITHER BE WRITTEN IN ENGLISH OR HINDI.

FILE EXTENSION : .pi

HOW TO SETUP AND RUN : https://github.com/ary27x/pilot/blob/main/HOWTORUN.md

FOLLOW THE PROJECT ON YOUTUBE : https://www.youtube.com/playlist?list=PLm7R-cUo29CVmWXQ2ZiaGcUIOVy0FEGaH


<h1>PILOT PROGRAMMING LANGUAGE SYNTAX DOCUMENTATION</h1>

*(The syntax of pilot programming language has very high resemblance to the english language , which makes it very easy to read and very beginner friendly)*


**COMMENTS**


Comments are denoted using the ~ (BACK TICK) operator.

    ~ this is a comment

<hr>

**VARIABLES**

Variable declaration has the following syntax :

    <variable> is <value>

The variable name has to start with either _ (underscore) or a character, but further on it has ability to have either underscore or alphanumeric characters.

    first is 10
    my_favorite_nubmer is 27
    negativeNumber is -5
    name is "aryan kumar"
    my_dog is "joey"

<hr>

**ARRAYS**

Pilot has support for multidimensional arrays.

Creating a 1D array is as simple as the following syntax :

    <array_name> are <values> ~ <values> can either be an integer or a variable
Example : 

    favorite_numbers are 27 , 141 , -2
    IDs are ID1 , ID2 , ID3 , ID4

Accesing the value in the array is done using the [ ] operator as follows:

    display "This is the first ID : " , IDs[0]

**MULTIDIMENSIONAL ARRAYS**

The general syntax for creating a n-dimensional array is as follows :

    <array_name> is <dimension1 * dimension2 * ..... * dimensionN> <values>

Here are some examples :

    favorite_numbers is <3> 27 , 141 , -2 ~ making a 1D array of size 3 
    matrix is <2*4> 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 ~ making a 2D array of size 2 x 4 

If less number of value are supplied that required into the array , the array will fill in the remaining positions with the last integer that was written into the array , the default write value being 0

Example : 

    space is <50*20*10> 0 ~ creating a 3D array named "space" , equating all the values to 0
    graph is <2*8> 1 , 2  ~ the first and second element are 1 and 2 , remaining are all equal to 2

(The reason that in the above graph array example all the values are equal to 2 is becase 2 is the last integer write value in the array , hence the remaning vacancies in the array is 
filled with 2)

<hr>


**PRINT**

You can print a string , number , variable or the value of a mathematical calculation onto the screen.
Every string , number , var or math calc has to be separated using the comma (,) operator in the display field

Example : 

    display "Hello World!"
    display "This is the value of 7 sqaured : " , 7 times 7
    display "My name is : " , name 
It also has the support for string repeat print , which goes as follows:

    display "Hello There ! " times 3 ~ this would print "Hello There !" 3 times onto the scrren
To print colored text onto the screen call the display sub function using the name of the color as follows:

    display.red    "this is a red text"
    display.green  "this is a green text"
    display.yellow "this is a yellow text"
    display.cyan   "this is a cyan text "
    display.purple "this is a purple text"
    display.blue   "this is a blue text"
    display.white  "this is a white text"
Output:<br><br>
![image](https://github.com/ary27x/pilot/assets/63790777/bf4d9903-7460-4aa7-80b5-f5f69e5d6fbc)


<hr>

**USER INPUT**

We use the "get" keyword to take in input from the user , and we need not declare the variable first
For taking multiple inputs , the names of the corresponding variables has to be separated by the comman(,) operator

    display "Enter the first and the second number : "
    get firstnumber , secondnumber
<hr>

**SCOPING**

Scoping in Pilot is done using identation , not curly brace (similar to python).
Statements with the same indentation have the same scope
<hr>

**FUNCTIONS**

The general syntax of a function in pilot is as follows:

    function <function_name> :
        ~ code        
If the function expects arguments passed to it:

    function <function_name> :: <arguments> :
        ~ code
        
**Functions can only be defined in the main scope**

Example :

    function sayHelloThrice:
        display.red   "Hello There!!"
        display.green "Hello There!!"
        display.blue  "Hello There!!"
        
    function cube :: number :
           display "The cube of : " , number , " is = " , number * number * number

    function sum :: first , second : 
           display "The sum of " , first , " and " , second , " is = " , first + second
    

To call a function , we have the following syntax :

    call <functionName>
To provide arguments while calling : 

    call <functionName> :: <arguments>
**Functions have to be defined before they called.**

Example :

    call sayHelloThrice
    call cube :: 7    
    call sum :: a , b
(Function can be called from any scope)



**CLOSURES IN FUNCTIONS**

Variables which are defined inside of the function are not global , and are destroyed when the function goes out of scope.

Example:

The below code would work as the variable is in the same scope as the function

    function closureTest:
        favoriteNumber is 27
        display "This is my favorite number : " , favoriteNumber
    
    call closureTest
The below code would raise an error , as the favoriteNumber variable is local to the function closureTest , but we are trying to access it from the global scope
    
    function closureTest:
        favoriteNumber is 27
        display "This is my favorite number : " , favoriteNumber
    
    call closureTest
    display "this is from the global scope : " , favoriteNumber ~ this should not work and will raise an error  

<hr>


**TILL LOOP**

Till loop has the following syntax:

    till <condition>:
        ~ code 

An example of the following syntax:

    till number < 10:
        display "This is the value of the number : " , number
        number is number + 1 ~ this is a very important step

**Till loops should be used carefully , as forgetting to update the value of the variable in the condition field would lead to an infinite loop**
<hr>

**RANGE LOOP**

Range loop has the following syntax:

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
Pilot also has support for complex nested loop.

Here is an example program to check for prime numbers in a given range:

    display "Enter the lower and the upper limit : "
    get lower , upper    
    range lower to upper as number : 
        factorCounter is 0
        range 1 to number as value:
                  if number mod value == 0:
                       factorCounter is factorCounter + 1
       if factorCounter == 2:
              display.red  "PRIME NUMBER : " , number
       else: 
             display.green "NOT PRIME    : " , number
Output:<br><br>
![image](https://github.com/ary27x/pilot/assets/63790777/a58883fe-7cab-477c-a515-b86e57e56fc6)

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





