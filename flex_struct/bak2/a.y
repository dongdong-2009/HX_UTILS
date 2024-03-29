/* Companionsource code for "flex & bison", published by O'Reilly 
 * Media, ISBN 978-0-596-15597-1 
 * Copyright (c) 2009, Taughannock Networks.All rights reserved. 
 * See the README file for license conditionsand contact info. 
 * $Header: /home/johnl/flnb/code/RCS/fb1-5.y,v2.1 2009/11/08 02:53:18 johnl Exp $ 
 */  
  
/* simplestversion of calculator */  
  
%{  
  
#  include <stdio.h>  
  
%}  
  
/* declare tokens*/  
%token NUMBER  
%token ADD SUB MUL DIV ABS  
%token OP CP  
%token EOL  
  
%%  
  
calclist: /*nothing */  
 | calclist exp EOL { printf("= %d\n>", $2); }  
 | calclist EOL { printf("> "); }/* blank line or a comment */  
 ;  
  
exp: factor  
 | exp ADD exp { $$ = $1 + $3; }  
 | exp SUB factor { $$ = $1 - $3; }  
 | exp ABS factor { $$ = $1 | $3; }  
 ;  
  
factor: term  
 | factor MUL term { $$ = $1 * $3; }  
 | factor DIV term { $$ = $1 / $3; }  
 ;  
  
term: NUMBER  
 | ABS term { $$ = $2 >= 0? $2 : - $2; }  
 | OP exp CP { $$ = $2; }  
 ;  
  
%%  
  
  

int main(int argc, char *argv[])  
{  
	FILE *fp;
	if(argc>1){
		fp = fopen(argv[1], "r");
		_yyin = fp;
		yylex(); 
		fclose(fp);
		return 0;
	}else{
	  printf("> ");  
	  yyparse();  
	  return 0;
	}
}  
  
  
  
yyerror(char *s)  
{  
  fprintf(stderr, "error: %s\n", s);  
}  