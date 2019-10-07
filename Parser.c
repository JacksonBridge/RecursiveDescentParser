/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Parser.c
 * -------------------------------------------------------------------
 * Jackson Hambridge
 * CMSC 403
 * October 7, 2019
 *
 * With the Parser.h and Givens.h headers, this program will parse
 * through a tokenized lexical structure, outputting either true or
 * false if the tokens are in the EBNF grammar or not.
 * This is a recursive descent parser, starting from the very bottom
 * where parser() is located.
 */


#include "Parser.h"
#include "Givens.h"

#include <stdio.h>

int current=0;



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * term
 * -------------------------------------------------------------------
 * This tests the tokens for either a NUMBER or IDENTIFIER
 * term --> IDENTIFIER | NUMBER
 */
_Bool term(struct lexics *someLexics, int numberOfLexics){
    if((strcmp(someLexics[current].lexeme,"IDENTIFIER")==0) || (strcmp(someLexics[current].lexeme,"NUMBER")==0)){
      current++;
      //printf("Found IDENTIFIER or NUMBER\n");
      return 1;
    }
    return 0;
  }



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * expression2
 * -------------------------------------------------------------------
 * This is an optional statement that may be called from expression
 * representing {BINOP term} on the EBNF grammar
 */
 _Bool expression2(struct lexics *someLexics, int numberOfLexics){
    if(strcmp(someLexics[current].lexeme,"BINOP")==0){
      //printf("Found BINOP\n");
      current++;
      if(term(someLexics,numberOfLexics)){
        if(expression2(someLexics, numberOfLexics)){
          return 1;
        }
      }
    }
    return 1;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * expression
 * -------------------------------------------------------------------
 * expression --> term {BINOP term} | ( expression )
 */
_Bool expression(struct lexics *someLexics, int numberOfLexics){
  if(term(someLexics,numberOfLexics)){
    if(expression2(someLexics,numberOfLexics)){
      return 1;
    }
  }

  if(strcmp(someLexics[current].lexeme,"LEFT_PARENTHESIS")==0){
    //printf("Found LEFT_PARENTHESIS\n");
    current++;
    if(expression(someLexics, numberOfLexics)){
      if(strcmp(someLexics[current].lexeme,"RIGHT_PARENTHESIS")==0){
        //printf("Found RIGHT_PARENTHESIS\n");
        current++;
        return 1;
      }
    }
  }

  if(strcmp(someLexics[current].lexeme,"LEFT_PARENTHESIS")==0){
    //printf("Found LEFT_PARENTHESIS\n");
    current++;
    if(expression(someLexics, numberOfLexics)){
      if((strcmp(someLexics[current].lexeme,"RIGHT_PARENTHESIS")==0)){
        //printf("Found RIGHT_PARENTHESIS\n");
        current++;
        return 1;
      }
    }
  }
    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * bodyOp
 * -------------------------------------------------------------------
 * This method, used for statementOp, is optional.  It will
 * output 1 if it either completes or does not run at all.
 */
_Bool bodyOp(struct lexics *someLexics, int numberOfLexics){
  if(strcmp(someLexics[current].lexeme,"LEFT_BRACKET")==0){
    //printf("Found LEFT_BRACKET\n");
    current++;
    if(strcmp(someLexics[current].lexeme,"RIGHT_BRACKET")==0){
      //printf("Found RIGHT_BRACKET\n");
      current++;
      return 1;
    }
    if(statementList(someLexics,numberOfLexics)){
      if(strcmp(someLexics[current].lexeme,"RIGHT_BRACKET")==0){
        //printf("Found RIGHT_BRACKET\n");
        current++;
        return 1;
      }
    }
    return 0;
  }
  return 1;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * assignmentOp
 * -------------------------------------------------------------------
 * This method, used for statementOp, is optional.  It will
 * output 1 if it either completes or does not run at all.
 */
 _Bool assignmentOp(struct lexics *someLexics, int numberOfLexics){
  if((strcmp(someLexics[current].lexeme,"IDENTIFIER")==0)){
    current++;
    //printf("Found IDENTIFIER\n");
    if((strcmp(someLexics[current].lexeme,"EQUAL")==0)){
      //printf("Found EQUAL\n");
      current++;
      if(expression(someLexics, numberOfLexics)){
        if((strcmp(someLexics[current].lexeme,"EOL")==0)){
          //printf("Found EOL\n");
          current++;
          return 1;
        }
      }
    }
    return 0;
  }
  return 1;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * assignment
 * -------------------------------------------------------------------
 * assignment --> IDENTIFIER EQUAL expression EOL
 */
_Bool assignment(struct lexics *someLexics, int numberOfLexics){
  if((strcmp(someLexics[current].lexeme,"IDENTIFIER")==0)){
    current++;
    //printf("Found IDENTIFIER\n");
    if((strcmp(someLexics[current].lexeme,"EQUAL")==0)){
      //printf("Found EQUAL\n");
      current++;
      if(expression(someLexics, numberOfLexics)){
        if((strcmp(someLexics[current].lexeme,"EOL")==0)){
          //printf("Found EOL\n");
          current++;
          return 1;
        }
      }
    }
  }
  return 0;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * retOp
 * -------------------------------------------------------------------
 * This method, used for statementOp, is optional.  It will
 * output 1 if it either completes or does not run at all.
 */
_Bool retOp(struct lexics *someLexics, int numberOfLexics){
  if((strcmp(someLexics[current].lexeme,"RETURN_KEYWORD")==0)){
    //printf("Found RETURN_KEYWORD\n");
    current++;
    if(expression(someLexics, numberOfLexics)){
      if((strcmp(someLexics[current].lexeme,"EOL")==0)){
        //printf("Found EOL\n");
        current++;
        return 1;
      }
    }
    return 0;
  }
  return 1;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ret
 * -------------------------------------------------------------------
 * return --> RETURN_KEYWORD expression EOL
 */
_Bool ret(struct lexics *someLexics, int numberOfLexics){
  if((strcmp(someLexics[current].lexeme,"RETURN_KEYWORD")==0)){
    //printf("Found RETURN_KEYWORD\n");
    current++;
    if(expression(someLexics, numberOfLexics)){
      if((strcmp(someLexics[current].lexeme,"EOL")==0)){
        //printf("Found EOL\n");
        current++;
        return 1;
      }
    }
  }
  return 0;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * whileLoopOp
 * -------------------------------------------------------------------
 * This method, used for statementOp, is optional.  It will
 * output 1 if it either completes or does not run at all.
 */
_Bool whileLoopOp(struct lexics *someLexics, int numberOfLexics){
  if((strcmp(someLexics[current].lexeme,"WHILE_KEYWORD")==0)){
    //printf("Found OP WHILE_KEYWORD\n");
    current++;
    if((strcmp(someLexics[current].lexeme,"LEFT_PARENTHESIS")==0)){
      //printf("Found OP LEFT_PARENTHESIS\n");
      current++;
      if(expression(someLexics, numberOfLexics)){
        if((strcmp(someLexics[current].lexeme,"RIGHT_PARENTHESIS")==0)){
          //printf("Found OP RIGHT_PARENTHESIS\n");
          current++;
          if(statement(someLexics, numberOfLexics)){
            return 1;
          }
        }
      }
    }
    return 0;
  }
  return 1;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * whileLoop
 * -------------------------------------------------------------------
 * while-loop --> WHILE_KEYWORD LEFT_PARENTHESIS expression 
 *   RIGHT_PARENTHESIS statement
 */
_Bool whileLoop(struct lexics *someLexics, int numberOfLexics){

  if((strcmp(someLexics[current].lexeme,"WHILE_KEYWORD")==0)){
    //printf("Found WHILE_KEYWORD\n");
    current++;
    if((strcmp(someLexics[current].lexeme,"LEFT_PARENTHESIS")==0)){
      //printf("Found LEFT_PARENTHESIS\n");
      current++;
      if(expression(someLexics, numberOfLexics)){
        if((strcmp(someLexics[current].lexeme,"RIGHT_PARENTHESIS")==0)){
          //printf("Found RIGHT_PARENTHESIS\n");
          current++;
          if(statement(someLexics, numberOfLexics)){
            return 1;
          }
        }
      }
    }
  }
  return 0;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * statementOp
 * -------------------------------------------------------------------
 * This method is 'optional'.  It is run numberOfLexics times,
 * representing the {statement} in statement-list --> statement {statement}
 * It will only return 0 if the methods fail.  If nothing runs in the
 * optional methods, they still return one.
 */
_Bool statementOp(struct lexics *someLexics, int numberOfLexics){
  if(whileLoopOp(someLexics,numberOfLexics)){
    if(assignmentOp(someLexics,numberOfLexics)){
      if(bodyOp(someLexics,numberOfLexics)){
        if(retOp(someLexics,numberOfLexics)){
          return 1;
        }
      }
    }
  }
  return 0;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * statement
 * -------------------------------------------------------------------
 * statement --> while-loop | return | assignment | body
 */
_Bool statement(struct lexics *someLexics, int numberOfLexics){
  if(whileLoop(someLexics,numberOfLexics)){
    return 1;
  }else if(assignment(someLexics,numberOfLexics)){
    return 1;
  }else if(body(someLexics,numberOfLexics)){
    return 1;
  }else if(ret(someLexics,numberOfLexics)){ 
    return 1;
  }else{return 0;}
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * statementList
 * -------------------------------------------------------------------
 * statement-list --> statement {statement}
 * statementOp is run numberOFLexics times to simulate {statement}
 */
_Bool statementList(struct lexics *someLexics, int numberOfLexics){
  if(statement(someLexics, numberOfLexics)){
    int i;
    for(i=0;i<numberOfLexics;i++){
      if(statementOp(someLexics,numberOfLexics)==0){
        return 0;
      }
    }
    return 1;
  }
  return 0;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * argDecl2
 * -------------------------------------------------------------------
 * This simulates the optional {COMMA VARTYPE IDENTIFIER}
 */
_Bool argDecl2(struct lexics *someLexics, int numberOfLexics){
  if(strcmp(someLexics[current].lexeme,"COMMA")==0){
    //printf("Found COMMA\n");
    current++;
    if((strcmp(someLexics[current].lexeme,"VARTYPE")==0)){
      //printf("Found VARTYPE\n");
      current++;
      if((strcmp(someLexics[current].lexeme,"IDENTIFIER")==0)){
        //printf("Found IDENTIFIER\n");
        current++;
        if((argDecl2(someLexics, numberOfLexics))){
          return 1;
        }
      }
    }
  }
  return 1;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * argDecl
 * -------------------------------------------------------------------
 * arg-decl --> VARTYPE IDENTIFIER {COMMA VARTYPE IDENTIFIER}
 */
_Bool argDecl(struct lexics *someLexics, int numberOfLexics){
  if((strcmp(someLexics[current].lexeme,"VARTYPE")==0)){
    //printf("Found VARTYPE\n");
    current++;
    if((strcmp(someLexics[current].lexeme,"IDENTIFIER")==0)){
      //printf("Found IDENTIFIER\n");
      current++;
      if(argDecl2(someLexics,numberOfLexics)){
        return 1;
      }
    }
  }
  return 1;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * body
 * -------------------------------------------------------------------
 * body --> LEFT_BRACKET [statement-list] RIGHT_BRACKET
 */
_Bool body(struct lexics *someLexics, int numberOfLexics){
  if(strcmp(someLexics[current].lexeme,"LEFT_BRACKET")==0){
    //printf("Found LEFT_BRACKET\n");
    current++;
    if(strcmp(someLexics[current].lexeme,"RIGHT_BRACKET")==0){
      //printf("Found RIGHT_BRACKET\n");
      current++;
      return 1;
    }
    if(statementList(someLexics,numberOfLexics)){
      if(strcmp(someLexics[current].lexeme,"RIGHT_BRACKET")==0){
        //printf("Found RIGHT_BRACKET\n");
        current++;
        return 1;
      }
    }
  }
  return 0;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * header
 * -------------------------------------------------------------------
 * header --> VARTYPE IDENTIFIER LEFT_PARENTHESIS [arg-decl] RIGHT_PARENTHESIS
 */
 _Bool header(struct lexics *someLexics, int numberOfLexics){
  if(strcmp(someLexics[current].lexeme,"VARTYPE")==0){
    //printf("Found VARTYPE\n");
    current++;
    if(strcmp(someLexics[current].lexeme,"IDENTIFIER")==0){
      //printf("Found IDENTIFIER\n");
      current++;
      if(strcmp(someLexics[current].lexeme,"LEFT_PARENTHESIS")==0){
        //printf("Found LEFT_PARENTHESIS\n");
        current++;
        if(argDecl(someLexics,numberOfLexics)){
          if(strcmp(someLexics[current].lexeme,"RIGHT_PARENTHESIS")==0){
            //printf("Found RIGHT_PARENTHESIS\n");
            current++;
            return 1;
          }
        }
      }
    }
  }
  return 0;
 }



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function
 * -------------------------------------------------------------------
 * function --> header body
 */
_Bool function(struct lexics *someLexics, int numberOfLexics){
  return (header(someLexics,numberOfLexics) && body(someLexics,numberOfLexics));
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * parser
 * -------------------------------------------------------------------
 * This method begins the recursive descent parsing.
 * It ensures that all the lexics were used and that the lexics are
 * apart of the EBNF grammar described.
 */
_Bool parser(struct lexics *someLexics, int numberOfLexics){
  return (function(someLexics, numberOfLexics) && current==numberOfLexics);
}