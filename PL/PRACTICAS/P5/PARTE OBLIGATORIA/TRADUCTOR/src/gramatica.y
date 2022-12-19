%{

    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "semantico.h"

    int yylex();
    void yyerror( const char * msg ) ;

    //int linea_actual = 1 ;

    #define YYERROR_VERBOSE

%}

%token OP_ASIGNACION
%token ID
%token CONST_INT CONST_FLOAT CONST_CHAR CONST_BOOL
%token DECLAR
%token PARDER
%token PARIZQ
%token METERDATOS
%token MIENTRAS
%token SI
%token SACARDATOS
%token FOR
%token INICIOBLOQUE
%token FINBLOQUE
%token PYC
%token TO 
%token CORIZQ
%token CORDER
%token COMA
%token MAIN
%token RETURN
%token INI_DECLAR_VAR
%token FIN_DECLAR_VAR
%token DO
%token SENT_LIST
%token DOLAR
%token CADENA
%token ARRARR
%token THEN
%token ELSE
%left OP_OR_LOG
%left OP_AND_LOG
%left OP_EQ_NEQ
%left OP_REL
%left MAS_MENOS MENOSMENOS MASMAS ARRARR
%left OP_MUL OP_LIST_MUL
%right NEGACION OP_LIST_UN OP_AND_LIST
%left ARROBA

%%

programa                    : { generaFich(); } MAIN bloque_main { Robert(); volcar(); closeInter(); }
                              ;
                              
bloque_main                 : INICIOBLOQUE { TS_AddMark(); nivel_bloque = 0}
                              declar_de_variables_locales
                              declar_de_subprogs 
                              sentencias
                              FINBLOQUE { TS_ClearBlock(); }
                              ;

bloque                      : INICIOBLOQUE { TS_AddMark(); nivel_bloque++; }
                              declar_de_variables_locales
                              declar_de_subprogs 
                              sentencias
                              FINBLOQUE { TS_ClearBlock(); nivel_bloque--;}
                              ;

declar_de_variables_locales : INI_DECLAR_VAR {decVar = 1; }
                              variables_locales
                              FIN_DECLAR_VAR {decVar = 0; }
                            | ;

declar_de_subprogs          : declar_de_subprogs declar_subprog 
                            | ;                                          

declar_subprog              : cabecera_subprograma { esFunc = 1; }
                              bloque { esFunc = 0;fputs("}\n",file_funciones); }
                              ;

variables_locales           : variables_locales cuerpo_declar_variables 
                            | cuerpo_declar_variables ;

cuerpo_declar_variables     : primera mas_variables PYC { pycYSalto(); }
                            | DECLAR ID error
                            | DECLAR error
                            | error ;

primera                     : DECLAR ID { $$.type = $1.type; setType($1);agregarNuevoID($2, &$$); declarID($2); }
                              ;

mas_variables               : mas_variables COMA ID 
                              {
                                declarListID($3);
                                agregarNuevoID($3, &$$);
                              }
                            | ;

cabecera_subprograma        : DECLAR ID { setType($1); decParam = 1; TS_AddFunction($2); }
                              PARIZQ declaraciones_id PARDER { decParam = 0; $4.nDim = 0; impresion_lexema($1,$2, lista_param,$$.lex);}
                              ;

declaraciones_id            : declaraciones_id COMA param {lista_param = concat(lista_param,$3.lex,',');}
                            | param {lista_param = concat(lista_param,$1.lex,',');}
                            | declaraciones_id COMA error
                            | ;

param                       : DECLAR ID { setType($1); TS_AddParam($2); juntarDeclarId($1,$2,&$$);  }
                              ;

sentencias                  : sentencias sentencia
                            | ;

sentencia                   : bloque 
                            | sentencia_asignacion {$$.eval = $1.eval;}
                            | sentencia_if {addSalida();}
                            | sentencia_for_pascal 
                            | sentencia_while 
                            | sentencia_entrada 
                            | sentencia_salida 
                            | sentencia_return 
                            | sentencia_adelante_atras 
                            | sentencia_principio_lista ;

sentencia_asignacion        : ID OP_ASIGNACION expresion PYC { comprobarAsignacion($1, $3); $$.eval = evaluar_sentencia_asig($1, $3); }
                            | error;

sentencia_if                : SI PARIZQ expresion_if PARDER THEN _sentencia { comprobarBooleano_if($3); } 
                            | SI PARIZQ expresion_if PARDER THEN _sentencia ELSE sentencia {comprobarBooleano_if($3);}
                              ;

_sentencia                  : sentencia { goToEndAndElse(); }
                              ;

expresion_if                : expresion { line_if = line; $$.type = $1.type; evaluar_sentencia_if($1); insertIfElse(); goToElse($$.temp_asociado)}
                              ;

asignacion_for_pascal       : ID OP_ASIGNACION expresion TO valor { comprobarAsignacion($1, $3); comprobarEntero($3); comprobarEntero($5); insertFor(); goToFor($1,$3,$5); $$.lex = $1.lex; }
                            | error ;

sentencia_for_pascal        : FOR asignacion_for_pascal DO sentencia { evaluar_sentencia_for($2.lex); }
                              ;

valor                       : CONST_INT { $$.type = INT; }
                            | ID { TS_GetById($1,&$$); }
                              ;

sentencia_while             : MIENTRAS PARIZQ expresion_while PARDER sentencia { comprobarBooleano($3); evaluar_sentencia_while(); }
                              ;

expresion_while             : expresion { $$.type = $1.type; insertWhile(); goToWhile($$.temp_asociado,$1); }
                              ;

sentencia_entrada           : METERDATOS lista_variables PYC {evaluarSentenciaEntrada();}
                              ;

lista_variables             : lista_variables COMA ID { agregarNuevoID($3, &$$); addVariableListaMeterDatos($3); }
                            | ID { agregarNuevoID($1, &$$); addVariableListaMeterDatos($1);}
                            | error ;

sentencia_salida            : SACARDATOS lista_expresiones_o_cadena_sacar PYC { evaluarSentenciaSalida(); }
                              ;

lista_expresiones_o_cadena_sacar  : lista_expresiones_o_cadena_sacar COMA expr_cad_sacar { addVariableListaSacarDatos($3); }
                            | expr_cad_sacar { addVariableListaSacarDatos($1); }
                              ; 

expr_cad_sacar              : ID {$$.lex = $1.lex;}
                            | CADENA {$$.lex = $1.lex;}

lista_expresiones_o_cadena  : lista_expresiones_o_cadena COMA expr_cad { TS_CheckParam($3); }
                            | expr_cad {checkParams = 0; TS_CheckParam($1);$$.temp_asociado = lista_func;$$.eval = lista_evals;}
                              ; 

expr_cad                    : expresion {sacarTemporalAsociado($1); sacarEvalAsociado($1);}
                              ;

sentencia_return            : RETURN expresion PYC { TS_CheckReturn($2, &$$); imprimir_return($2); }
                              ;

expresion                   : PARIZQ expresion PARDER { $$.type = $2.type; $$.nDim = $2.nDim; $$.eval = $2.eval; $$.temp_asociado = $2.temp_asociado;}
                            | MASMAS expresion { comprobarIncrementoDecremento($1, $2, &$$); evaluar_expresion_unaria($1, $2, &$$);}
                            | MENOSMENOS expresion { comprobarIncrementoDecremento($1, $2, &$$); evaluar_expresion_unaria($1, $2, &$$);}
                            | expresion MASMAS { comprobarIncrementoDecremento($2, $1, &$$); evaluar_expresion_unaria($2, $1, &$$);}
                            | expresion MENOSMENOS { comprobarIncrementoDecremento($2, $1, &$$); evaluar_expresion_unaria($2, $1, &$$);}
                            | OP_LIST_UN expresion { comprobarNumeroElementosLista($1, $2, &$$); }
                            | MAS_MENOS expresion %prec NEGACION { comprobarMasMenos($1, $2, &$$); evaluar_expresion_unaria($1, $2, &$$);}
                            | NEGACION expresion {comprobarNegacionConBooleano($1, $2, &$$); evaluar_expresion_unaria($1, $2, &$$);}
                            | OP_AND_LIST expresion // Pa que cojones se usa????
                            | expresion ARRARR expresion {comprobarPosicionLista($1,$2,$3,&$$);}
                            | expresion MAS_MENOS expresion { comprobarMasMenosBinario($1, $2, $3, &$$); evaluar_expresion($1, $2, $3, &$$); } 
                            | expresion OP_LIST_MUL expresion {comprobaOperadorBinarioConcatenarListas($1, $2, $3, &$$);}
                            | expresion OP_OR_LOG expresion { comprobarOperadorBinarioAndOr($1, $2, $3, &$$); evaluar_expresion($1, $2, $3, &$$);}
                            | expresion OP_AND_LOG expresion { comprobarOperadorBinarioAndOr($1, $2, $3, &$$); evaluar_expresion($1, $2, $3, &$$);}
                            | expresion OP_EQ_NEQ expresion { comprobarOperadorBinarioRelacion($1, $2, $3, &$$); evaluar_expresion($1, $2, $3, &$$);}
                            | expresion OP_REL expresion { comprobarOperadorBinarioRelacion($1, $2, $3, &$$); evaluar_expresion($1, $2, $3, &$$);}
                            | expresion OP_MUL expresion {comprobaOperadorBinarioMultiplicacion($1, $2, $3, &$$); evaluar_expresion($1, $2, $3, &$$);}
                            | expresion MENOSMENOS expresion {comprobarMenosMenos($1,$2,$3,&$$);}
                            | expresion MASMAS expresion ARROBA expresion {comprobarOperadorTernarioLista($1, $2, $3, $4, $5, &$$); }
                            | ID { $$.type = TS_GetType($1); $$.nDim = TS_GetNDim($1); decVar = 0; $$.eval = "";$$.temp_asociado = $1.lex;}
                            | constante { $$.type = $1.type; $$.nDim = $1.nDim; $$.eval = "";$$.temp_asociado = $1.lex; }
                            | funcion { $$.type = $1.type; $$.nDim = $1.nDim; currentFunction = -1; $$.eval = "";$$.temp_asociado = componerLlamadaFuncion($1.lex);}                   
                            | agregado { $$.type = $1.type; $$.nDim = $1.nDim; }
                              ;
                            

sentencia_adelante_atras    : ID SENT_LIST PYC ;

sentencia_principio_lista   : DOLAR ID PYC ;

constante                   : CONST_INT { $$.type = INT; }
                            | CONST_FLOAT { $$.type = FLOAT; }
                            | CONST_BOOL { $$.type = BOOL; }
                            | CONST_CHAR { $$.type = CHAR; }
                              ;

agregado                    : CORIZQ constante_lista CORDER { $$.type = $2.type; }
                              ;

constante_lista             : constante_lista_int { $$.type = LIST_INT; $$.nDim = $1.nDim; }
                            | constante_lista_float { $$.type = LIST_FLOAT; $$.nDim = $1.nDim; }
                            | constante_lista_bool { $$.type = LIST_BOOL; $$.nDim = $1.nDim; }
                            | constante_lista_char { $$.type = LIST_CHAR; $$.nDim = $1.nDim; }
                              ;

constante_lista_int         : constante_lista_int COMA CONST_INT
                            | CONST_INT ;
constante_lista_float       : constante_lista_float COMA CONST_FLOAT
                            | CONST_FLOAT ;
constante_lista_bool        : constante_lista_bool COMA CONST_BOOL
                            | CONST_BOOL ;
constante_lista_char        : constante_lista_char COMA CONST_CHAR
                            | CONST_CHAR ;

funcion                     : cabecera_funcion argumentos_funcion { $$.attr = $2.attr; $$.type = $2.type, $$.nDim = $2.nDim; if(esFunc)
                                                                                                                              fputs(lista_evals,file_funciones);
                                                                                                                              else
                                                                                                                              fputs(lista_evals,file);
                                                                                                                              lista_evals = NULL;}
                              ;

cabecera_funcion            : ID PARIZQ { comprobarLlamadaAFuncion($1);$$.lex = $1.lex }
                              ;

argumentos_funcion          : lista_expresiones_o_cadena PARDER {TS_FunctionCall(&$$);}
                              ;

%%

#include "lex.yy.c"

void yyerror(const char *msg ) 
{
    fprintf(stderr,"[Linea %d]: %s\n", line, msg) ;
}
