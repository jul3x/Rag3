/* This Bison file was machine-generated by BNFC */
%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <R3E/j3x/Absyn.h>
typedef struct yy_buffer_state *YY_BUFFER_STATE;
int yyparse(void);
int yylex(void);
YY_BUFFER_STATE yy_scan_string(const char *str);
void yy_delete_buffer(YY_BUFFER_STATE buf);
int yy_mylinenumber;
int initialize_lexer(FILE * inp);
int yywrap(void)
{
  return 1;
}
void yyerror(const char *str)
{
  extern char *yytext;
  fprintf(stderr,"error: line %d: %s at %s\n", 
    yy_mylinenumber, str, yytext);
}



static Script* YY_RESULT_Script_ = 0;
Script* pScript(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Script_;
  }
}
Script* pScript(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Script_;
  }
}

static Def* YY_RESULT_Def_ = 0;
Def* pDef(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Def_;
  }
}
Def* pDef(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Def_;
  }
}

static ListDef* YY_RESULT_ListDef_ = 0;
ListDef* pListDef(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListDef_;
  }
}
ListDef* pListDef(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListDef_;
  }
}

static Type* YY_RESULT_Type_ = 0;
Type* pType(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Type_;
  }
}
Type* pType(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Type_;
  }
}

static Expr* YY_RESULT_Expr_ = 0;
Expr* pExpr(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Expr_;
  }
}
Expr* pExpr(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Expr_;
  }
}

static ListExpr* YY_RESULT_ListExpr_ = 0;
ListExpr* pListExpr(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListExpr_;
  }
}
ListExpr* pListExpr(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListExpr_;
  }
}



%}

%union
{
  int int_;
  char char_;
  double double_;
  char* string_;
  Script* script_;
  Def* def_;
  ListDef* listdef_;
  Type* type_;
  Expr* expr_;
  ListExpr* listexpr_;
}

%token _ERROR_
%token _SYMB_0    //   =
%token _SYMB_1    //   list[
%token _SYMB_2    //   ]
%token _SYMB_3    //   +
%token _SYMB_4    //   -
%token _SYMB_5    //   *
%token _SYMB_6    //   /
%token _SYMB_7    //   [
%token _SYMB_8    //   ;
%token _SYMB_9    //   (
%token _SYMB_10    //   ,
%token _SYMB_11    //   )
%token _SYMB_12    //   bool
%token _SYMB_13    //   false
%token _SYMB_14    //   float
%token _SYMB_15    //   int
%token _SYMB_16    //   string
%token _SYMB_17    //   true
%token _SYMB_18    //   vector

%type <script_> Script
%type <def_> Def
%type <listdef_> ListDef
%type <type_> Type
%type <expr_> Expr
%type <expr_> Expr1
%type <expr_> Expr2
%type <listexpr_> ListExpr

%start Script
%token<string_> _STRING_
%token<int_> _INTEGER_
%token<double_> _DOUBLE_
%token<string_> _IDENT_

%%
Script : ListDef {  std::reverse($1->begin(),$1->end()) ;$$ = new Prog($1); YY_RESULT_Script_= $$; } 
;
Def : Type _IDENT_ _SYMB_0 Expr {  $$ = new VarDef($1, $2, $4);  } 
;
ListDef : Def {  $$ = new ListDef() ; $$->push_back($1);  } 
  | Def ListDef {  $2->push_back($1) ; $$ = $2 ;  }
;
Type : _SYMB_15 {  $$ = new Int();  } 
  | _SYMB_16 {  $$ = new Str();  }
  | _SYMB_12 {  $$ = new Bool();  }
  | _SYMB_14 {  $$ = new Float();  }
  | _SYMB_18 {  $$ = new Vector();  }
  | _SYMB_1 Type _SYMB_2 {  $$ = new ArrType($2);  }
;
Expr : Expr _SYMB_3 Expr1 {  $$ = new EAdd($1, $3);  } 
  | Expr _SYMB_4 Expr1 {  $$ = new ESub($1, $3);  }
  | Expr1 {  $$ = $1;  }
;
Expr1 : Expr1 _SYMB_5 Expr2 {  $$ = new EMul($1, $3);  } 
  | Expr1 _SYMB_6 Expr2 {  $$ = new EDiv($1, $3);  }
  | _SYMB_4 Expr2 {  $$ = new ENeg($2);  }
  | Expr2 {  $$ = $1;  }
;
Expr2 : _SYMB_7 ListExpr _SYMB_2 {  std::reverse($2->begin(),$2->end()) ;$$ = new EList($2);  } 
  | _INTEGER_ {  $$ = new ELitInt($1);  }
  | _STRING_ {  $$ = new EString($1);  }
  | _DOUBLE_ {  $$ = new EDouble($1);  }
  | _SYMB_17 {  $$ = new ELitTrue();  }
  | _SYMB_13 {  $$ = new ELitFalse();  }
  | _IDENT_ {  $$ = new EVar($1);  }
  | _SYMB_9 Expr _SYMB_10 Expr _SYMB_11 {  $$ = new EVector($2, $4);  }
  | _SYMB_9 Expr _SYMB_11 {  $$ = $2;  }
;
ListExpr : /* empty */ {  $$ = new ListExpr();  } 
  | Expr {  $$ = new ListExpr() ; $$->push_back($1);  }
  | Expr _SYMB_8 ListExpr {  $3->push_back($1) ; $$ = $3 ;  }
;

