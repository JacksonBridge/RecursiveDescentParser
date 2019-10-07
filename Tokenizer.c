/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Tokenizer.c
 * -------------------------------------------------------------------
 * Jackson Hambridge
 * CMSC 403
 * October 7, 2019
 *
 * Including the Tokenizer.h and Givens.h header, the latter to access
 * the lexics struct, this method reads a file and turns all the
 * characters into tokens from our EBNF grammar.
 * Tokens include: LEFT_PARENTHESIS, RIGHT_PARENTHESIS, LEFT_BRACKET,
 * RIGHT_BRACKET, WHILE_KEYWORD, RETURN_KEYWORD, EQUAL, COMMA, EOL,
 * VARTYPE, IDENTIFIER, BINOP, and NUMBER.
 */

#include "Tokenizer.h"
#include "Givens.h"

#include <stdbool.h> 
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <regex.h>
#include <ctype.h>



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * trim
 * -------------------------------------------------------------------
 * This method trims the whitespace around a character array.
 * It is used later in the program.
 */
char *trim(char *string){
  char *last;
  while(isspace((unsigned char)*string)){
    string++;
  }
  if(*string == 0){
    return string;
  }
  last = string + strlen(string) - 1;
  while(last > string && isspace((unsigned char)*last)) last--;
  last[1] = '\0';
  return string;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * validNumber
 * -------------------------------------------------------------------
 * This method is copied from the Givens.c file.
 * I had issues connecting them but this worked just fine, too :)
 */
// _Bool validNumber(char* aLexeme){
// 	regex_t numberRegex;
// 	regcomp(&numberRegex, "^[0-9][0-9]*$", REG_EXTENDED);
// 	return !regexec(&numberRegex, aLexeme, 0, 0, 0);	
// }



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * validIdentifier
 * -------------------------------------------------------------------
 * This method is copied from the Givens.c file.
 * :)
 */
// _Bool validIdentifier(char * aLexeme){
// 	regex_t identifierRegex;
// 	regcomp(&identifierRegex, "^[a-zA-Z][a-zA-Z0-9]*$", REG_EXTENDED);
// 	return !regexec(&identifierRegex, aLexeme, 0, 0, 0);
// }


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * combine
 * -------------------------------------------------------------------
 * This method puts two strings together!
 */
char* combine(const char *firstHalf, const char *secondHalf){
  char *whole = malloc(strlen(firstHalf) + strlen(secondHalf) + 1);
  strcpy(whole, firstHalf);
  strcat(whole, secondHalf);
  return whole;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * tokenizer
 * -------------------------------------------------------------------
 * This method reads a FILE, translates it into tokens, and packages
 * each token in the aLex lexics structure
 */
_Bool tokenizer(struct lexics *aLex, int *numLex, FILE *inf){
  int i=0;
  char line[256];
  char* tokens = "";

  //This loop reads each line of the FILE
  while (fgets(line, sizeof(line), inf)) {


    int end=0;
    int length = strlen(line);
    char unknown[255];
    int index=0;

      //This loop goes through each line and turns the characters into tokens!
      while(end <= length){
        index=end;
        if(line[end] == '('){
          tokens=combine(tokens," ( ");
          end++;
        }
        else if(line[end] == ')'){
          tokens=combine(tokens," ) ");
          end++;
        }
        else if(line[end] == '{'){
          tokens=combine(tokens," { ");
          end++;
        }
        else if(line[end] == '}'){
          tokens=combine(tokens," } ");
          end++;
        }
        else if(line[end] == '=' && (line[end+1] != '=' && line[end-1] != '=')){
          tokens=combine(tokens," = ");
          end++;
        }
        else if(line[end] == ','){
          tokens=combine(tokens," , ");
          end++;
        }
        else if(line[end] == ';'){
          tokens=combine(tokens," ; ");
          end++;
        }
        else if(line[end] == 'w' && line[end+1] == 'h' && line[end+2] == 'i' && line[end+3] == 'l' && line[end+4] == 'e'){
            tokens=combine(tokens," while ");
            end=end+5;
        }
        else if(line[end] == 'r' && line[end+1] == 'e' && line[end+2] == 't' && line[end+3] == 'u' && line[end+4] == 'r' && line[end+5] == 'n'){
            tokens=combine(tokens," return ");
            end=end+6;
        }
        else if((line[end] == 'i' && line[end+1] == 'n' && line[end+2] == 't') || (line[end] == 'v' && line[end+1] == 'o' && line[end+2] == 'i' && line[end+3] == 'd')){
            tokens=combine(tokens," ");
            if(line[end] == 'i' && line[end+1] == 'n' && line[end+2] == 't'){
              end=end+3;
              tokens=combine(tokens,"int ");
            }else{
              end=end+4;
              tokens=combine(tokens,"void ");
            }
        }
        else if(line[end] == '+' || line[end] == '*' || line[end] == '%' || (line[end] == '!' && line[end+1] == '=') || (line[end] == '=' && line[end] == '=')){
          tokens=combine(tokens," ");
          char test[2] = "\0";
          test[0]=line[end];
          tokens=combine(tokens,test);
          end++;
          if((line[end] == '!' && line[end+1] == '=') || (line[end] == '=' && line[end] == '=')){
            char test[2] = "\0";
            test[0]=line[end];
            tokens=combine(tokens,test);
            end++;
          }
          tokens=combine(tokens," ");
        }
        else{
          //If none of the tokens were found, it copies the character as is and deals with it in a later process
          char test[2] = "\0";
          test[0]=line[end];
          tokens=combine(tokens,test);
          end++;

        }
      }
  }

//printf("%s\n\n\n",tokens);

  int count=0;
  //This method separates each token by spaces and packages them into aLex the lexics structure
  char *piece = strtok(tokens, " "); 
    while (piece != NULL){ 
      piece=trim(piece);
      //printf("%s\n", piece);
        if(strcmp(piece, "(")==0){
          aLex[count].token=LEFT_PARENTHESIS;
          strcpy(aLex[count].lexeme,piece);
          count++;
        }else if(strcmp(piece, ")")==0){
          aLex[count].token=RIGHT_PARENTHESIS;
          strcpy(aLex[count].lexeme,piece);
          count++;
        }else if(strcmp(piece, "{")==0){
          aLex[count].token=LEFT_BRACKET;
          strcpy(aLex[count].lexeme,piece);
          count++;
        }else if(strcmp(piece, "}")==0){
          aLex[count].token=RIGHT_BRACKET;
          strcpy(aLex[count].lexeme,piece);
          count++;
        }else if(strcmp(piece, "while")==0){
          aLex[count].token=WHILE_KEYWORD;
          strcpy(aLex[count].lexeme,piece);
          count++;
        }else if(strcmp(piece, "return")==0){
          aLex[count].token=RETURN_KEYWORD;
          strcpy(aLex[count].lexeme,piece);
          count++;
        }else if(strcmp(piece, "=")==0){
          aLex[count].token=EQUAL;
          strcpy(aLex[count].lexeme,piece);
          count++;
        }else if(strcmp(piece, ",")==0){
          aLex[count].token=COMMA;
          strcpy(aLex[count].lexeme,piece);
          count++;
        }else if(strcmp(piece, ";")==0){
          aLex[count].token=EOL;
          strcpy(aLex[count].lexeme,piece);
          count++;
        }else if(strcmp(piece, "int")==0||strcmp(piece, "void")==0){
          aLex[count].token=VARTYPE;
          strcpy(aLex[count].lexeme,piece);
          count++;
        }else if((strcmp(piece, "+")==0)||(strcmp(piece, "*")==0)||(strcmp(piece, "==")==0)||(strcmp(piece, "!=")==0)||(strcmp(piece, "%")==0)){
          aLex[count].token=BINOP;
          strcpy(aLex[count].lexeme,piece);
          count++;
        }else if(validNumber(piece)){
          aLex[count].token=NUMBER;
          strcpy(aLex[count].lexeme,piece);
          count++;
        }else if(validIdentifier(piece)){
          aLex[count].token=IDENTIFIER;
          strcpy(aLex[count].lexeme,piece);
          count++;
      }
      piece = strtok(NULL, " ");
      
  }
  *numLex=count;

  return 1;

}