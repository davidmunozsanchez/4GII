%{
	//#include "tabla_tokens.h"
	#include <string.h>
	int atributo = -1;
	//extern yylval;
%}

letra	[a-zA-Z_]
digito  [0-9]

%option yylineno
%option noyywrap

%%

"Robert"  								{yylval.lex = strdup(yytext); 					return(MAIN);}
"return"								{yylval.lex = strdup(yytext); 					return(RETURN);}
"while"   				 				{yylval.lex = strdup(yytext); 					return(MIENTRAS);}
"if"      								{yylval.lex = strdup(yytext); 					return(SI);}
"for"     								{yylval.lex = strdup(yytext); 					return(FOR);}
"to"      				 				{yylval.lex = strdup(yytext); 					return(TO);}
"then"    				 				{yylval.lex = strdup(yytext); 					return(THEN);}
"else"    				 				{yylval.lex = strdup(yytext); 					return(ELSE);}
"do"      				 				{yylval.lex = strdup(yytext); 					return(DO);}
"int"									{yylval.type = INT; 		  					return(DECLAR);}
"char"          						{yylval.type = CHAR; 		  					return(DECLAR);}
"float"         						{yylval.type = FLOAT;   	  					return(DECLAR);}
"bool"          						{yylval.type = BOOLEAN; 	  					return(DECLAR);}
"list of int"   						{yylval.type = LIST_INT;	  					return(DECLAR);}
"list of char"  						{yylval.type = LIST_CHAR;     					return(DECLAR);}
"list of float" 						{yylval.type = LIST_FLOAT;    					return(DECLAR);}
"list of bool"  						{yylval.type = LIST_BOOLEAN;  					return(DECLAR);}
")"             						{yylval.lex = strdup(yytext); 					return(PARDER);}
"("             						{yylval.lex = strdup(yytext); 					return(PARIZQ);}
"["             						{yylval.lex = strdup(yytext); 					return(CORIZQ);}
"]"             						{yylval.lex = strdup(yytext); 					return(CORDER);}
"{"             						{yylval.lex = strdup(yytext); 					return(INICIOBLOQUE);}
"}"             						{yylval.lex = strdup(yytext); 					return(FINBLOQUE);}
";"             						{yylval.lex = strdup(yytext); 					return(PYC);}
","             						{yylval.lex = strdup(yytext); 					return(COMA);}

"++"            						{yylval.lex = strdup(yytext); 					return (MASMAS);}
"--"									{yylval.lex = strdup(yytext); 					return(MENOSMENOS);}
"@"             						{yylval.lex = strdup(yytext); 					return(ARROBA);}
"@@"									{yylval.lex = strdup(yytext); 					return(ARRARR);}
"="             						{yylval.lex = strdup(yytext); 					return(OP_ASIGNACION);}
"/" 		    						{yylval.lex = strdup(yytext); yylval.attr = 0;	return(OP_MUL);}
"%"             						{yylval.lex = strdup(yytext); yylval.attr = 1;	return(OP_MUL);}
"*"             						{yylval.lex = strdup(yytext); 					return(ASTERISCO);}
"<" 									{yylval.lex = strdup(yytext); yylval.attr = 0; 	return(OP_REL);}
"<="									{yylval.lex = strdup(yytext); yylval.attr = 1; 	return(OP_REL);}
">="									{yylval.lex = strdup(yytext); yylval.attr = 2; 	return(OP_REL);}
">"										{yylval.lex = strdup(yytext); yylval.attr = 3;	return(OP_REL);}
"&"										{yylval.lex = strdup(yytext); 					return(OP_AND_LIST);}
"=="									{yylval.lex = strdup(yytext); yylval.attr = 0; 	return(OP_EQ_NEQ);}
"!="									{yylval.lex = strdup(yytext); yylval.attr = 1; 	return(OP_EQ_NEQ);}
"&&"									{yylval.lex = strdup(yytext);					return(OP_AND_LOG);}
"||"									{yylval.lex = strdup(yytext);					return(OP_OR_LOG);}
"**"									{yylval.lex = strdup(yytext);					return(OP_LIST_MUL);}
"!"										{yylval.lex = strdup(yytext); 					return(NEGACION);}
"<<"									{yylval.lex = strdup(yytext); yylval.attr = 0; 	return(SENT_LIST);}
">>"									{yylval.lex = strdup(yytext); yylval.attr = 1; 	return(SENT_LIST);}
"#"										{yylval.lex = strdup(yytext); yylval.attr = 0; 	return(OP_LIST_UN);}
"?"										{yylval.lex = strdup(yytext); yylval.attr = 1; 	return(OP_LIST_UN);}
"+"										{yylval.lex = strdup(yytext); yylval.attr = 0; 	return(MAS_MENOS);}
"-"										{yylval.lex = strdup(yytext); yylval.attr = 1; 	return(MAS_MENOS);}
"$"										{yylval.lex = strdup(yytext); 					return(DOLAR);}
"meter->"								{yylval.lex = strdup(yytext);					return(METERDATOS);}
"sacar<-"								{yylval.lex = strdup(yytext);					return(SACARDATOS);}
"begin_var"								{yylval.lex = strdup(yytext);					return(INI_DECLAR_VAR);}
"end_var"								{yylval.lex = strdup(yytext);					return(FIN_DECLAR_VAR);}
{digito}({digito})*						{yylval.lex = strdup(yytext); yylval.attr = 0; 	return(CONST);}
{digito}({digito})*"."({digito})*		{yylval.lex = strdup(yytext); yylval.attr = 1; 	return(CONST);}
"true"                         			{yylval.lex = strdup(yytext); yylval.attr = 2; 	return(CONST);}
"false"                           		{yylval.lex = strdup(yytext); yylval.attr = 3; 	return(CONST);}
\'[^\']\'                           	{yylval.lex = strdup(yytext); yylval.attr = 4; 	return(CONST);}
\"[^\"]*\"                           	{yylval.lex = strdup(yytext);					return(CADENA);}
{letra}({letra}|{digito})*				{yylval.lex = strdup(yytext); 					return(ID);}
[ \t]+                              	{}
\n                                  	{linea_actual++;}
.                             			{printf("[Linea %d]: lexical error, no se reconoce %s\n", yylineno, yytext);}

%%

