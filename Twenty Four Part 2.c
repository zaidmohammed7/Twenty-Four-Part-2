/****************************
 * Program 3: TwentyFour Part Two
 *
 * Course: CS 211, Spring 2023
 * System: WindowsOS using Clion
 * Student Author: Zaid Khan Mohammed
 *
 * Research references: learned about infix and postfix from- https://www.youtube.com/watch?v=Xn3FdGEOdHo
 *                      learned how to evaluate postfix from- https://www.geeksforgeeks.org/evaluation-of-postfix-expression/
 * *** **********************/

/*
* Instructions for Use:
* 1. The program is a game where the goal is to use four numbers to calculate the value 24.
* 2. You will be prompted to choose a difficulty level:
*    - E for Easy (12 puzzles)
*    - M for Medium (24 puzzles)
*    - H for Hard (11 puzzles)
* 3. After selecting a difficulty, you will be presented with four numbers.
* 4. Input your mathematical expression using each of the four numbers exactly once, 
*    combining them with basic operators (+, -, *, /) and using parentheses as needed.
* 5. The program will evaluate your expression and tell you if it equals 24.
* 6. You can choose to play again, change the difficulty, or exit the game.
* 
* Expected Input Format:
* - An expression like: (2 + 2) * 6
* - Ensure to use all four numbers provided in the prompt.
* 
* Note: Invalid symbols or incorrect use of numbers will result in an error message,
*       and you will be prompted to enter your solution again.
*/

// including libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// nodes to be used in linked list implementaiton of stack
typedef struct Node *NodePtr; // Make:  NodePtr to be a synonym for: struct Node *
struct Node {
    char data;      // The data stored at the node
    NodePtr pNext; // Pointer to the next node
};


// function to push in new elements in stack
void push(char data, NodePtr *pHead){ // basically prepend for linked lists
    NodePtr pTemp;     // pointer to new Node
    pTemp = malloc( sizeof( struct Node));
    pTemp->data=data;
    if (pHead!=NULL) {
        pTemp->pNext = *pHead;
    }
    else {pTemp->pNext=NULL;}
    *pHead=pTemp;
}


// function to pop out outer element and return the character
char pop(NodePtr *pHead){
    if (*pHead==NULL){
        return '!';
    }
    NodePtr  pTemp=*pHead;
    char data=(*pHead)->data;
    *pHead=(*pHead)->pNext;
    // making sure to free memory
    free(pTemp);
    return data;
}


// function to read from appropriate difficulty file and store puzzles in 2D array
void setDifficulty(int numPuzzles, int allPuzzles[numPuzzles][4],char fileName[]){
    FILE *filePtr = fopen(fileName, "r");
    // Check that the file was able to be opened
    if (filePtr == NULL) {
        printf("Error: could not open %s for reading\n", fileName);
        return;
    }
    char puzzle[9];
    // storing puzzle in array
    for (int i=0;i<numPuzzles;++i) {
        fgets(puzzle,9,filePtr);
        allPuzzles[i][0]=puzzle[0]-'0';
        allPuzzles[i][1]=puzzle[2]-'0';
        allPuzzles[i][2]=puzzle[4]-'0';
        allPuzzles[i][3]=puzzle[6]-'0';
    }
    fclose(filePtr);
}


// function to check precedence of operators
int precedenceCheck(char data){
    // lower precedence for +,- compared to *,/
    if (data=='+' || data=='-'){return 1;}
    else if (data=='*' || data=='/'){return 2;}
}


// function to convert infix to postfix notation
void infixToPostfix(char infix[],char postfix[]){
    NodePtr postfixStack=NULL;
    int len= strlen(infix);
    // firstly adding  ) to infix and ( to postfix stack
    infix[len-1]=')';
    infix[len]='\0';
    push('(',&postfixStack);
    postfix[0]='\0';
    int i=0;
    // postfix stack would only be NULL if the first ( was accidentally popped, which would happen only when there are too many )
    // therefore, adding ) to postfix so we can display appropriate error while evaluating the postfix expression
    while (i< strlen(infix)){
        if (postfixStack==NULL){
            len= strlen(postfix);
            postfix[len]=')';
            postfix[len+1]='\0';
            // popping all elements before returning to free all memory
            while (postfixStack!=NULL){
                pop(&postfixStack);
            }
            return;
        }
        // adding to postfix if character is a digit
        if (isdigit(infix[i])){
            len= strlen(postfix);
            postfix[len]=infix[i];
            postfix[len+1]='\0';
            ++i;
        }
        // continuing if character is a space
        else if (infix[i]==' '){
            ++i;
        }
        // checking operator precedence and pushing accordingly
        else if(infix[i]!=')' && (postfixStack->data=='(' || precedenceCheck(postfixStack->data)< precedenceCheck(infix[i]))){
            push(infix[i],&postfixStack);
            ++i;
        }
        else if(infix[i]!=')' && precedenceCheck(postfixStack->data)>= precedenceCheck(infix[i])){
            char data= pop(&postfixStack);
            len= strlen(postfix);
            postfix[len]=data;
            postfix[len+1]='\0';
            continue;
        }
        // popping elements of the stack and adding to postfix expression until corresponding ( is found or stack is empty
        else if(infix[i]==')'){
            if (postfixStack==NULL){
                len= strlen(postfix);
                postfix[len]=')';
                postfix[len+1]='\0';
                // popping all elements before returning to free all memory
                while (postfixStack!=NULL){
                    pop(&postfixStack);
                }
                return;
            }
            while (postfixStack->data!='('){
                char data=pop(&postfixStack);
                len= strlen(postfix);
                postfix[len]=data;
                postfix[len+1]='\0';
            }
            pop(&postfixStack);
            ++i;
        }
    }
    // popping all remaining stack elements and adding it to postfix expression
    while (postfixStack!=NULL){
        char data=pop(&postfixStack);
        len= strlen(postfix);
        postfix[len]=data;
        postfix[len+1]='\0';
    }
}


// function to evaluate the postfix expression
int postfixEvaluate(char postfixExpression[]){
    int result;
    int numOperators=0;
    NodePtr evaluateStack=NULL;
    int len= strlen(postfixExpression);
    // pushing digits into stack directly
    for (int i=0;i<len;++i){
        if (isdigit(postfixExpression[i])){
            push(postfixExpression[i],&evaluateStack);
        }
        // popping out last 2 elements to evaluate with operator
        else{
            int num2= pop(&evaluateStack)-'0';
            int num1= pop(&evaluateStack)-'0';
            // hardcoding the operators...
            if (postfixExpression[i]=='+'){
                push((num1+num2)+'0',&evaluateStack);
                printf("%d + %d = %d.\n",num1,num2,num1+num2);
            }
            else if (postfixExpression[i]=='-'){
                push((num1-num2)+'0',&evaluateStack);
                printf("%d - %d = %d.\n",num1,num2,num1-num2);
            }
            else if (postfixExpression[i]=='*'){
                push((num1*num2)+'0',&evaluateStack);
                printf("%d * %d = %d.\n",num1,num2,num1*num2);
            }
            else if (postfixExpression[i]=='/'){
                push((num1/num2)+'0',&evaluateStack);
                printf("%d / %d = %d.\n",num1,num2,num1/num2);
            }
            // printing appropriate error message
            else if (postfixExpression[i]==')'){
                printf("Error! Too many closing parentheses in the expression.\n\n");
                // popping all elements before returning to free all memory
                while (evaluateStack!=NULL){
                    pop(&evaluateStack);
                }
                return -1;
            }
        }
    }
    // counting the number of operators in the expression (there should be 3)
    for (int i=0;i< strlen(postfixExpression);++i){
        if (postfixExpression[i]=='+' || postfixExpression[i]=='-' || postfixExpression[i]=='*' || postfixExpression[i]=='/'){
            ++numOperators;
        }
    }
    // printing appropriate error message
    if (numOperators<3){
        printf("Error! too many values in the expression.\n\n");
        // popping all elements before returning to free all memory
        while (evaluateStack!=NULL){
            pop(&evaluateStack);
        }
        return -1;
    }
    // storing the final element of stack into result
    result=evaluateStack->data-'0';
    // popping all elements before returning to free all memory
    while (evaluateStack!=NULL){
        pop(&evaluateStack);
    }
    return result;
}


// function to check for user entered error, before trying to convert to postfix and evaluate
int errorCheck(char userSolution[],int puzzle[]){
    int userNos[5]={-1,-1,-1,-1,-1}; // storing user entered values in array
    int userNosIndex=0;
    int foundUserNos[4]={0,0,0,0}; // keeping track of number of user nos found in puzzle
    int foundPuzzleNos[4]={0,0,0,0}; // keeping track of number of puzzles nos found by user
    int numOpeningParenthesis=0;
    int numClosingParenthesis=0;
    for (int i=0;i< strlen(userSolution)-1;++i){ // checking for invalid symbols
        if (!isdigit(userSolution[i]) && userSolution[i]!='+' && userSolution[i]!='-' && userSolution[i]!='*' && userSolution[i]!='/' && userSolution[i]!='(' && userSolution[i]!=')' && userSolution[i]!=' '){
            printf("Error! Invalid symbol entered. Please try again.\n\n");
            return -1;
        }
        if (isdigit(userSolution[i])){ // adding user entered nos to array
            userNos[userNosIndex]=userSolution[i]-'0';
            ++userNosIndex;
        }
        if (userSolution[i]=='('){++numOpeningParenthesis;}
        else if (userSolution[i]==')'){++numClosingParenthesis;}
    }
    // if userNos[3]==-1 then user entered less than 4 nos, and if userNos[4]!=-1 then user entered more than 4 nos
    if (userNos[3]==-1 || userNos[4]!=-1){ // printing error message
        printf("Error! You must use all four numbers, and use each one only once. Please try again.\n\n");
        for (int i=0;i<4;++i){
        }
        return -1;
    }
    for (int i=0;i<4;++i){
        for (int j=0;j<4;++j){
            if (puzzle[i]==userNos[j]){
                foundUserNos[i]=1;
            }
        }
    }
    for (int i=0;i<4;++i){
        // if one of the numbers in puzzle is not entered by user
        if (foundUserNos[i]==0){
            printf("Error! You must use all four numbers, and use each one only once. Please try again.\n\n");
            return -1;
        }
    }
    for (int i=0;i<4;++i){
        // if one of the numbers entered by user is not found in puzzle
        for (int j=0;j<4;++j){
            if (userNos[i]==puzzle[j]){
                foundPuzzleNos[i]=1;
            }
        }
    }
    for (int i=0;i<4;++i){
        if (foundPuzzleNos[i]==0){
            printf("Error! You must use all four numbers, and use each one only once. Please try again.\n\n");
            return -1;
        }
    }
    if (numOpeningParenthesis==numClosingParenthesis){return 0;}
    else {return 1;}
}

int main() {
    srand(1);
    setbuf( stdout, 0);
    char easyFile[]="easy.txt";
    char mediumFile[]="medium.txt";
    char hardFile[]="hard.txt";

    // declaring all variables to be used
    int userChoice=2;
    char bufferDump;
    char difficultyChoice[99];
    int difficulty; // 12 for easy, 24 for medium, 11 for hard
    int allPuzzles[30][4];
    int puzzleNum;
    int puzzle[4];
    char userSolution[99];
    char postfixExpression[99];
    NodePtr pHead;
    float result;
    int error=0;

    // initial instruction
    printf("Welcome to the game of TwentyFour Part Two!\n");
    printf("Use each of the four numbers shown exactly once, \n");
    printf("combining them somehow with the basic mathematical operators (+,-,*,/) \n");
    printf("to yield the value twenty-four.\n");
    while (userChoice!=3) {
        if (userChoice==2) {
            // setting difficulty if and only if user choice is 2
            pHead = NULL;
            printf("Choose your difficulty level: E for easy, M for medium, and H for hard (default is easy).\nYour choice --> ");//fflush(stdin);
            scanf("%s", &difficultyChoice);
            scanf("%c", &bufferDump);
            if (difficultyChoice[0] == 'H') {
                setDifficulty(11, allPuzzles, hardFile);
                difficulty = 11;
            } else if (difficultyChoice[0] == 'M') {
                setDifficulty(24, allPuzzles, mediumFile);
                difficulty = 24;
            } else {
                setDifficulty(12, allPuzzles, easyFile);
                difficulty = 12;
            }
        }
        // playing the rest of the game regardless of user choice being 1 or 2
        if (userChoice==1 || userChoice==2) {
            while (1) {
                puzzleNum = rand() % difficulty;
                //printf("The rand value is: %d",puzzleNum);
                puzzle[0] = allPuzzles[puzzleNum][0];
                puzzle[1] = allPuzzles[puzzleNum][1];
                puzzle[2] = allPuzzles[puzzleNum][2];
                puzzle[3] = allPuzzles[puzzleNum][3];
                printf("The numbers to use are: %d, %d, %d, %d.\n", puzzle[0], puzzle[1], puzzle[2], puzzle[3]);
                printf("Enter your solution: ");
                fgets(userSolution, 99, stdin);
                error = errorCheck(userSolution, puzzle);
                if (error == -1) {
                    continue; // replaying if error was found
                } else {
                    break;
                }
            }
            // converting user entered expression to postfix and evaluating it
            infixToPostfix(userSolution, postfixExpression);
            result = postfixEvaluate(postfixExpression);
            // checking result and printing appropriate message
            if (result!=-1 && result==24) {
                printf("Well done! You are a math genius.\n\n");
            }
            else if (result!=-1){
                printf("Sorry. Your solution did not evaluate to 24.\n\n");
            }
            // reprinting instructions
            printf("Enter: \t1 to play again,\n");
            printf("\t2 to change the difficulty level and then play again, or\n");
            printf("\t3 to exit the program.\n");
            printf("Your choice --> ");
            scanf("%d", &userChoice);
            scanf("%c", &bufferDump);
        }
    }
    // exiting the program when user choice is 3
    printf("\nThanks for playing!\n");
    printf("Exiting...\n");

    return 0;
}