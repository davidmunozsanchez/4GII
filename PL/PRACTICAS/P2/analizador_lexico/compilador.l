%{
	#include "tabla_tokens.h"
	int atributo = -1;
%}

letra	[a-zA-Z_]
digito  [0-9]

%option yylineno
%option noyywrap

%%

"Robert"  								 				return(MAIN); 
"return"  								 				return(RETURN);
"while"   								 				return(MIENTRAS);
"if"      								 				return(SI);
"for"     								 				return(FOR);
"to"      								 				return(TO);
"then"    								 				return(THEN);
"else"    								 				return(ELSE);
"do"      								 				return(DO);
"int"									atributo = 0; 	return(DECLAR);
"char"          						atributo = 1; 	return(DECLAR);
"float"         						atributo = 2; 	return(DECLAR);
"bool"          						atributo = 3; 	return(DECLAR);
"list of int"   						atributo = 4; 	return(DECLAR);
"list of char"  						atributo = 5; 	return(DECLAR);
"list of float" 						atributo = 6; 	return(DECLAR);
"list of bool"  						atributo = 7; 	return(DECLAR);
")"             						 				return(PARDER);
"("             						 				return(PARIZQ);
"["             						 				return(CORIZQ);
"]"             						 				return(CORDER);
"{"             									 	return(INICIOBLOQUE);
"}"             						 				return(FINBLOQUE);
";"             						 				return(PYC);
","             						 				return(COMA);
"++"            						 				return(MASMAS);
"@"             						 				return(ARROBA);
"="             						 				return(OP_ASIGNACION);
"/" 		    						atributo = 0; 	return(OP_BINARIO);
"%"             						atributo = 1; 	return(OP_BINARIO);
"*"             						atributo = 2; 	return(OP_BINARIO);
"<" 									atributo = 3; 	return(OP_BINARIO);
"<="									atributo = 4; 	return(OP_BINARIO);
"=="									atributo = 5; 	return(OP_BINARIO);
">="									atributo = 6; 	return(OP_BINARIO);
">"										atributo = 7; 	return(OP_BINARIO);
"&"										atributo = 8; 	return(OP_BINARIO);
"|"										atributo = 9; 	return(OP_BINARIO);
"!="									atributo = 10; 	return(OP_BINARIO);
"^"										atributo = 11; 	return(OP_BINARIO);
"&&"									atributo = 12; 	return(OP_BINARIO);
"||"									atributo = 13; 	return(OP_BINARIO);
"!"													 	return(NEGACION);
"<<"									atributo = 0; 	return(SENT_LIST);
">>"									atributo = 1; 	return(SENT_LIST);
"#"										atributo = 0; 	return(OP_LIST_UN);
"?"										atributo = 1; 	return(OP_LIST_UN);
"+"										atributo = 0; 	return(UNYBIN);
"-"										atributo = 1; 	return(UNYBIN);
"$"													 	return(DOLAR);
"meter->"												return(METERDATOS);
"sacar<-"												return(SACARDATOS);
"begin_var"												return(INI_DECLAR_VAR);
"end_var"												return(FIN_DECLAR_VAR);
{digito}({digito})*						atributo = 0; 	return(CONST);
{digito}({digito})*"."({digito})*		atributo = 1; 	return(CONST);
"true"                         			atributo = 2; 	return(CONST);
"false"                           		atributo = 2; 	return(CONST);
\'[^\']\'                           	atributo = 3; 	return(CONST);
\"[^\"]*\"                           	atributo = 4; 	return(CADENA);
{letra}({letra}|{digito})*								return(ID);
[ \t]+                              					{}
\n                                  					{}
.                             							printf("Error lexico en línea %d, no se reconoce %s\n", yylineno, yytext);

%%

int main(int argc, char** argv) {
	// Comprobamos que los argumentos son correctos
  	if (argc <= 1) {
		printf("\nERROR: el numero de argumentos introducidos es incorrecto\n");
		printf("\nEjecucion correcta: %s nombre_fichero\n", argv[0]);
		exit(-1);
  	}

    // Abrimos el fichero en modo lectura (r)
    yyin = fopen(argv[1], "r");

    // En caso de no poder abrir el fichero (NULL) mostramos el error 
    if (yyin == NULL) {
        printf ("\nNo se ha podido abrir el fichero %s\n", argv[1]);
        exit (-2);
    }

    // Llamamos a yylex para lanzar el analizador lexico
  	int analyzer = yylex();

  	// Continuamos lanzando yylex() mientras quede fichero por leer
  	while (analyzer != 0) {
		// Mostramos el código del token...
    	printf("__%d__ : ", analyzer);
		if (atributo != -1) {
			// ...y su atributo en caso de que tenga.
			printf("(%d) ", atributo);
			atributo = -1;
		}
		else {
			printf("(-) ");
		}
		printf("%s\n", yytext);
    	analyzer = yylex();
  	}

	// Finaliza el programa
	exit(1);
}