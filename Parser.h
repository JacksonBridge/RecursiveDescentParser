/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Parser.h
 * -------------------------------------------------------------------
 * Jackson Hambridge
 * CMSC 403
 * October 7, 2019
 *
 * This defines all the functions for Parser.c and includes Givens.h
 * in order to access the custom struct lexics.
 */

#ifndef PARSER_H
#define PARSER_H

#include "Givens.h"

_Bool parser(struct lexics *someLexics, int numberOfLexics);
_Bool function(struct lexics *someLexics, int numberOfLexics);
_Bool header(struct lexics *someLexics, int numberOfLexics);
_Bool body(struct lexics *someLexics, int numberOfLexics);
_Bool bodyOp(struct lexics *someLexics, int numberOfLexics);
_Bool argDecl(struct lexics *someLexics, int numberOfLexics);
_Bool argDecl2(struct lexics *someLexics, int numberOfLexics);
_Bool statementList(struct lexics *someLexics, int numberOfLexics);
_Bool statement(struct lexics *someLexics, int numberOfLexics);
_Bool statementOp(struct lexics *someLexics, int numberOfLexics);
_Bool whileLoop(struct lexics *someLexics, int numberOfLexics);
_Bool whileLoopOp(struct lexics *someLexics, int numberOfLexics);
_Bool ret(struct lexics *someLexics, int numberOfLexics);
_Bool retOp(struct lexics *someLexics, int numberOfLexics);
_Bool assignment(struct lexics *someLexics, int numberOfLexics);
_Bool assignmentOp(struct lexics *someLexics, int numberOfLexics);
_Bool expression(struct lexics *someLexics, int numberOfLexics);
_Bool term(struct lexics *someLexics, int numberOfLexics);


#endif