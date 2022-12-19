#include "semantico.h"
#define DEBUG 0

inTS     ts[MAX_STACK];
long int TOS             = 0;
int      line            = 1;
int      decVar          = 0;
int      decParam        = 0;
int      esFunc          = 0;
tData    currentType     = NONE;
int      nParams         = 0;
int      checkParams     = 0;
int      checkFunction   = 0;
int      currentFunction = -1;
int      line_if = -1;

const int TAMBUFFER = 2;

int tabs = 1;
int flag = 1;

void formatear(char *buffer, FILE *file){
  if(buffer[0] == '}') tabs--;
    
    if(flag == 1){
      char *ts = align();
      strcat(ts,buffer);
      fputs(ts,file_definitivo);
    }
    else fputs(buffer,file_definitivo);

    if(buffer[0] == '{') tabs++;

    flag = 0;
    if(buffer[0] == '\n') flag = 1;
}

void volcar(){
  char buffer[TAMBUFFER];
  closeFile();
  file = fopen("src/traduccion_temporal.txt","r");
  while (fgets(buffer,TAMBUFFER,file)){
    formatear(buffer,file_definitivo);
  }
  
  RobertEnd();

  closeFileFunciones();
  file_funciones = fopen("src/traduccion_funciones.txt","r");
  fputs("\n\n",file_definitivo);
  tabs = 0;
  while (fgets(buffer,TAMBUFFER,file_funciones)){
    formatear(buffer,file_definitivo);
  }

  remove("src/traduccion_temporal.txt");
  remove("src/traduccion_funciones.txt");
}

tData getListType(tData type) {
  switch (type) {
    case INT:
      return LIST_INT;
    case FLOAT:
      return LIST_FLOAT;
    case CHAR:
      return LIST_CHAR;
    case BOOL:
      return LIST_BOOL;
    default:
      return type;
  }
}

const char* tDataToString(tData type) {
  switch (type) {
    case NONE:
      return "NONE";
    case INT:
      return "INT";
    case FLOAT:
      return "FLOAT";
    case CHAR:
      return "CHAR";
    case BOOL:
      return "BOOL";
    case LIST_INT:
      return "LIST OF INT";
    case LIST_FLOAT:
      return "LIST OF FLOAT";
    case LIST_CHAR:
      return "LIST OF CHAR";
    case LIST_BOOL:
      return "LIST OF BOOL";
    default:
      return "NONE";
  }
}


void setType(attrs value) {
  currentType = value.type;
}

int isList(attrs e) {
  return e.type == LIST_INT || e.type == LIST_FLOAT || e.type == LIST_CHAR || e.type == LIST_BOOL;
}

tData listToType(tData listType) {
  switch (listType) {
    case LIST_INT:
      return INT;
    case LIST_FLOAT:
      return FLOAT;
    case LIST_CHAR:
      return CHAR;
    case LIST_BOOL:
      return BOOL;
    default:
      return NONE;
  }
}


int TS_AddEntry(inTS in) {
  if (TOS < MAX_STACK) {        // Comprobamos si quedan entradas en la tabla de símbolos
    ts[TOS].entry   = in.entry;
    ts[TOS].lex     = in.lex;
    ts[TOS].type    = in.type;
    ts[TOS].nParams = in.nParams;
    ts[TOS].nDim    = in.nDim;

    ts[TOS].des = in.des;
    // WIP -> ts[TOS].ended   = in.ended;

    TOS++;          // Se aumenta el contador de entradas
    //print_TS();    // Se muestra la tabla de símbolos por pantalla

    return 1;
  } else {
    printf("[Linea %d] Symbol table overflow", line);
    return -1;
  }
}

int TS_DeleteEntry() {
  if (TOS > 0) {  // Si la tabla de símbolos tiene alguna in puede eliminar la última
    TOS--;
    return 1;
  } else {
    printf("[Linea %d] Empty symbol table", line);
    return -1;
  }
}

int TS_ClearBlock() {
  int ret = -1;
  int currentTOS = -1;    // valor del TOS antes de actualizar la función actual

  if (TOS == 0)           // la TS está vacía
    return 1;

  while (TOS > 0) {       // buscamos el inicio del bloque en el que estamos mientras que no
                          // lleguemos a la base de la pila
    TOS--;                // nos desplazamos a las entradas anteriores

    if (ts[TOS].entry == MARK) {   // encuentra una entrada con la marca de inicio de bloque
      ret = 1;
      break;
    }
  }

  if (TOS == 0) {
    printf("[SEMANTIC] Analysis ended\n");
    return ret;
  }

  // dejamos TOS donde estaría la marca de bloque para comenzar a insertar ahí,
  // todo lo anterior en la TS se conserva (parámetros formales, funciones...)
  currentTOS = TOS;
  currentTOS--;

  if (ts[currentTOS].entry == PARAM) {      // si el bloque es una función, actualizamos la
                                             // función actual
    while (ts[currentTOS].entry == PARAM) {
      currentTOS--;
    }
  }
  if (ts[currentTOS].entry == FUNCTION) {   // busca en la TS la última función definida no finalizada por declarar para
                                             // convertirla en la actual (es su ámbito ahora)
    ts[currentTOS].ended = 1;                // señalamos que esta función ha finalizado, pues
                                             // hemos eliminado el cuerpo de dicha función
    updateCurrentFunction(currentTOS);
  }

  return ret;
}

void updateCurrentFunction(int lastFunc) {
  lastFunc--;
  while (ts[lastFunc].entry != FUNCTION && lastFunc > 0) {
    lastFunc--;
  }
  if (lastFunc == 0) {
    currentFunction = -1;
  } else {
    if (ts[lastFunc].ended == 0)    // si la función encontrada todavía no ha terminado de definirse
      currentFunction = lastFunc;
    else
      updateCurrentFunction(lastFunc);
  }
}

int TS_FindById(attrs e) {
  int i = TOS - 1;    

  if (TOS == 0)
    return -1;
  
  //print_TS();
  
  while (i > 0 /* && ts[i].entry != MARK */) {
    if (ts[i].entry == VAR && strcmp(e.lex, ts[i].lex) == 0) {
      //printf("ATRIBUTO FINDBYID. lex=%s, type=%d, entry=%d\n", ts[i].lex, ts[i].type, ts[i].entry);
      return i;
    }
    i--;
  }

  //printf("Linea %d] Identifier not declared: %s\n", line, e.lex);
  return -1;
}

int TS_FindFunctionById(attrs e) {
  int i = TOS - 1;    

  if (TOS == 0)
    return -1;
  
  //print_TS();
  
  while (i > 0 /* && ts[i].entry != MARK */) {
    if (ts[i].entry == FUNCTION && strcmp(e.lex, ts[i].lex) == 0) {
      //printf("ATRIBUTO FINDBYID. lex=%s, type=%d, entry=%d\n", ts[i].lex, ts[i].type, ts[i].entry);
      return i;
    }
    i--;
  }

  //printf("Linea %d] Identifier not declared: %s\n", line, e.lex);
  return -1;  
}

int TS_FindByName(attrs e) {
  int i = TOS - 1;    

  if (TOS == 0)
    return -1;
  
  while (i > 0) {
    if (ts[i].entry == FUNCTION && strcmp(e.lex, ts[i].lex) == 0)
      return i;
    i--;
  }

  //printf("Linea %d] Identifier not declared: %s\n", line, e.lex);
  return -1;
}

void TS_AddId(attrs e) {
  int j = TOS - 1;    // Para añadir un id a la pila no se puede haber llegado al tope,
                      // el id no puede existir y se deben estar declarando variables
  int found = 0;
  int index;

  int numparams = ts[currentFunction].nParams;

  if (index != 1) {   // Comprobamos que la variable a añadir no es una redeclaración de un
                      // argumento de la función en la que se encuentra
    // Comparamos con los parámetros formales de la fución actual
    for (index = currentFunction+1; index < currentFunction+1+numparams; index++) {
      if (strcmp(ts[index].lex, e.lex) == 0) {  // Su nombre es el de un argumento de la función
        printf("[Linea %d] Declaration error. Id already exists: %s\n", line, e.lex);
        return;
      }
    }
  }

  if (j >= 0 && decVar == 1) {  // Caso declaración de variable
    while (ts[j].entry != MARK && j >= 0 && !found) {   // Busca entrada con el mismo nombre
                                                        // dentro del bloque
      if (strcmp(ts[j].lex, e.lex) != 0) {
        j--;
      } else {
        found = 1;
        printf("[Linea %d] Declaration error. Id already exists: %s\n", line, e.lex);
        return;
      }
    }
    if (!found) {
      inTS inFunction;
      inFunction.entry   = VAR;
      inFunction.lex     = e.lex;
      inFunction.type    = currentType;
      inFunction.nParams = 0;
      inFunction.nDim    = e.nDim;
      TS_AddEntry(inFunction);
    }
  }
}

void TS_AddMark() {
  inTS inInitScope;

  inInitScope.entry   = MARK;
  inInitScope.lex     = "{";
  inInitScope.type    = NONE;
  inInitScope.nParams = 0;
  inInitScope.nDim    = 0;

  TS_AddEntry(inInitScope);

  // Si es el cuerpo de una función, se añaden a la tabla de símbolos los parámetros/argumentos
  // de la función como variables locales de ese bloque al fin de poder ser utilizadas
  if (esFunc == 1) {
    int j = TOS - 2;    // TOS-2 para leer los parámetros formales de antes de la llave
    while (j > 0 && ts[j].entry == PARAM) {   // Mientras leamos parámetros formales los aniadimos
                                              // como variables locales al nuevo bloque
      inTS newIn;
      newIn.entry   = VAR;
      newIn.lex     = ts[j].lex;
      newIn.type    = ts[j].type;
      newIn.nParams = ts[j].nParams;
      newIn.nDim    = ts[j].nDim;
      TS_AddEntry(newIn);

      j--;
    }
  }
}

void TS_AddFunction(attrs e) {
  inTS inFunction;

  inFunction.entry   = FUNCTION;
  inFunction.lex     = e.lex;
  inFunction.type    = currentType;
  inFunction.nParams = 0;   // lo actualizaremos a continuación
  inFunction.nDim    = 0;
  inFunction.ended   = 0;

  currentFunction = TOS;    // actualizamos la función actual
  TS_AddEntry(inFunction);
}

void TS_AddParam(attrs e) {
  int j = TOS - 1;
  int found = 0;

  while (j != currentFunction && !found) {   // Comprobamos si ya existe un argumento de esa
                                            // función con el mismo nombre
    if (strcmp(ts[j].lex, e.lex) != 0) {
      j--;
    } else {
      found = 1;
      printf("[Linea %d] Declaration error. Param already exists: %s\n", line, e.lex);
    }
  }

  if (!found) {
    inTS inParam;
    inParam.entry   = PARAM;
    inParam.lex     = e.lex;
    inParam.type    = currentType;
    inParam.nParams = 0;
    inParam.nDim    = e.nDim;
    TS_AddEntry(inParam);

    if (currentFunction > -1)             // si no son los parámetros de entrada del programa
      ts[currentFunction].nParams += 1;   // actualiza el número de parámetros de la función
  }
}


void TS_CheckReturn(attrs expr, attrs *res) {
  int index = currentFunction;

  if (index > -1) {
    if (expr.type != ts[index].type) {
      printf("[Linea %d] Return type of %s not equal to function type of %s\n",
        line, tDataToString(expr.type), tDataToString(ts[index].type));
      return;
    }
    attrs tmp;
    tmp.nDim = ts[index].nDim;

    if (!comprobarMismaDimension(expr, tmp)) {
      printf("[Linea %d] Return expression not same size as return function\n", line);
    }

    res->type = expr.type;
    res->nDim = expr.nDim;
    return;
  }/* else {
    printf("[Linea %d] Result not declared into function\n", line);
    return;
  }*/
}

void TS_GetById(attrs id, attrs *res) {
  int index = TS_FindById(id);
  if (index == -1 && id.lex[0] != '"') {   // No es ninguna variable de la TS
    //if (TS[index].entry != FUNCTION)
    printf("[Linea %d] Search error. Id not found: %s\n", line, id.lex);
  } else {
    res->lex  = strdup(ts[index].lex);
    res->type = ts[index].type;
  }
}

void TS_GetByIdFunction(attrs id, attrs *res) {
  int index = TS_FindFunctionById(id);
  if (index == -1) {   // No es ninguna variable de la TS
    //if (TS[index].entry != FUNCTION)
    printf("[Linea %d] Search error. Id not found: %s\n", line, id.lex);
  } else {
    res->lex  = strdup(ts[index].lex);
    res->type = ts[index].type;
  }
}

int TS_GetType(attrs id) {
  int index = TS_FindById(id);
  if (index == -1) {   // No es ninguna variable de la TS
    //if (TS[index].entry != FUNCTION)
    printf("[Linea %d] Search error. Id not found: %s\n", line, id.lex);
    return -1;
  } else {
    return ts[index].type;
  }
}

int TS_GetNDim(attrs id) {
  int index = TS_FindById(id);
  if (index != -1)
    return ts[index].nDim;
  else
    return 0;
}

void TS_FunctionCall(attrs *res) {
  //printf("CF:%d\n", currentFunction);
  //print_TS();
  if (currentFunction != -1) {
    if (checkParams != ts[currentFunction].nParams) {
      printf("[Linea %d] Args error. Number of params not valid. Expected %d, but got %d\n",
        line, ts[currentFunction].nParams, checkParams);
      res->lex  = strdup(ts[currentFunction].lex);
      res->type = NONE;
      res->nDim = 0;
    } else {
      res->lex  = strdup(ts[currentFunction].lex);
      res->type = ts[currentFunction].type;
      res->nDim = ts[currentFunction].nDim;
    }
  }
}

void TS_CheckParam(attrs param) {
  checkParams += 1;
  int formalParam = currentFunction + checkParams;

  if (param.type != ts[formalParam].type && ts[formalParam].type > 0) {
    printf("[Linea %d] Args error. Param type not valid. Expected %s, but got %s\n",
      line, tDataToString(ts[formalParam].type), tDataToString(param.type));
    return;
  }
}


void print_TS() {
  int j = 0;
  char *t, *e;

  printf("------------------------------\n");

  while (j <= TOS-1) {
    if (ts[j].entry == 0) { e = "MARK"; }
    if (ts[j].entry == 1) { e = "FUNCTION"; }
    if (ts[j].entry == 2) { e = "VAR"; }
    if (ts[j].entry == 3) { e = "PARAM"; }

    if (ts[j].type == 0) { t = "NONE"; }
    if (ts[j].type == 1) { t = "INT"; }
    if (ts[j].type == 2) { t = "FLOAT"; }
    if (ts[j].type == 3) { t = "CHAR"; }
    if (ts[j].type == 4) { t = "BOOL"; }
    if (ts[j].type == 5) { t = "LIST_INT"; }
    if (ts[j].type == 6) { t = "LIST_FLOAT"; }
    if (ts[j].type == 7) { t = "LIST_CHAR"; }
    if (ts[j].type == 8) { t = "LIST_BOOL"; }

    printf("[%d]\n", j);
    printf("  -Entrada: %-12s\n", e);
    printf("  -Lexema:  %-12s\n", ts[j].lex);
    printf("  -type:    %-10s\n", t);
    printf("  -nParams: %-4d\n", ts[j].nParams);

    printf("------------------------------\n");
    j++;
  }
  char strvar[100];
  printf("(Insert any value to continue:) ");
  fgets (strvar, 100, stdin);
}

void imprimirAtributos(attrs e, char *msg) {
  char *t;

    if (e.type == 0) { t = "NONE"; }
    if (e.type == 1) { t = "INT"; }
    if (e.type == 2) { t = "FLOAT"; }
    if (e.type == 3) { t = "CHAR"; }
    if (e.type == 4) { t = "BOOL"; }
    if (e.type == 5) { t = "LIST_INT"; }
    if (e.type == 6) { t = "LIST_FLOAT"; }
    if (e.type == 7) { t = "LIST_CHAR"; }
    if (e.type == 8) { t = "LIST_BOOL"; }

  printf("------------------------------\n");
  printf("%s\n", msg);
  printf("  - Atributos: %-4d\n", e.attr);
  printf("  - Lexema:    %-12s\n", e.lex);
  printf("  - type:      %-10s\n", t);
  printf("------------------------------\n");
}

int isNone(attrs e) {
  return (e.type != BOOL)?1:0;
}

int comprobarMismaDimension(attrs e1, attrs e2) {
  return e1.nDim == e2.nDim;
}

void comprobarAsignacion(attrs e1, attrs e2) {
  int type = TS_GetType(e1);
  if (type != e2.type && e2.type != NONE && type > 0)
    printf("[%sLinea %d] Assigning expression of type %s to %s, of type %s\n", DEBUG ? "{481}" : "",line, tDataToString(e2.type), e1.lex, tDataToString(TS_GetType(e1)));
  /* WIP unnecessary -> if (!comprobarMismaDimension(e1, e2))
    printf("Linea %d] Expression and %s are not the same size\n", line, e2.lex); */
}

void comprobarBooleano(attrs e) {
  if (e.type != BOOL) {
    printf("[%sLinea %d] Expected expression of type BOOLEAN, but got type %s\n", DEBUG ? "{489}" : "",line, tDataToString(e.type));
  }
}

void comprobarBooleano_if (attrs e) {
  if (e.type != BOOL) {
    int print = line;
    if (line_if < line && line_if != -1) {
      print = line_if;
      printf("[%sLinea %d] Expected expression of type BOOLEAN, but got type %s\n", DEBUG ? "{489}" : "",print, tDataToString(e.type));
      line_if = -1;
    }
  }
}

void comprobarEntero(attrs e) {
  if (e.type != INT)
    printf("[%sLinea %d] Expected expression of type INT, but got type %s\n", DEBUG ? "{494}" : "",line, tDataToString(e.type));
}

void Check_ListSentence(attrs id) {
  int type = TS_GetType(id);
  if (type != -1 && !(type == LIST_INT || type == LIST_FLOAT || type == LIST_BOOL || type == LIST_CHAR))
    printf("[%sLinea %d] %s is not of type LIST, but type %s\n", DEBUG ? "{499}" : "",line, id.lex, tDataToString(type));
}

void comprobarNegacionConBooleano(attrs op, attrs expr, attrs *res) {
  if (expr.type != BOOL) {
    printf("[%sLinea %d] Unary operator ! expects expression of type ![BOOLEAN], but got type ![%s]\n",
      DEBUG ? "{505}" : "",line, tDataToString(expr.type));
    return ;
  }
  res->type = BOOL;
  res->nDim = 0;
}

void comprobarNumeroElementosLista(attrs op, attrs expr, attrs *res) {
  if (!isList(expr)) {
    printf("[%sLinea %d] Unary operator # expects expression of type #[LIST], but got type #[%s]\n",
      DEBUG ? "{515}" : "",line, tDataToString(expr.type));
    return ;
  }
  res->type = INT;
  res->nDim = 0;
}

void Check_OpUnaryQuest(attrs op, attrs expr, attrs *res) {
  if (!isList(expr)) {
    printf("[%sLinea %d] Unary operator ? expects expression of type ?[LIST], but got type ?[%s]\n",
      DEBUG ? "{525}" : "",line, tDataToString(expr.type));
    return ;
  }
  res->type = listToType(expr.type);
  res->nDim = 0;
}

void comprobarIncrementoDecremento(attrs op, attrs expr, attrs *res) {
  if (!(expr.type == INT || expr.type == FLOAT)) {
    printf("[%sLinea %d] Unary operator %s expects expression of type %s[INT|FLOAT], but got type %s[%s]\n",
      DEBUG ? "{535}" : "",line, op.lex, op.lex, op.lex, tDataToString(expr.type));
  }
  res->type = expr.type;
  res->nDim = expr.nDim;
}

void comprobarMasMenosBinario(attrs expr1, attrs op, attrs expr2, attrs *res) {
  // WIP sólo permitimos la suma y resta de enteros/flotantes (¿suma/resta de char/bool?)
  if (!(expr1.type == INT || expr1.type == LIST_INT || expr1.type == FLOAT || expr1.type == LIST_FLOAT) ||
    !(expr2.type == INT || expr2.type == LIST_INT || expr2.type == FLOAT || expr2.type == LIST_FLOAT)) {
    printf("[%sLinea %d] Binary operator %s expects expressions of types [INT|FLOAT|LIST OF INT|LIST OF FLOAT]%s[INT|FLOAT|LIST OF INT|LIST OF FLOAT], but got types [%s]%s[%s]\n",
      DEBUG ? "{544}" : "",line, op.lex, op.lex, tDataToString(expr1.type), op.lex, tDataToString(expr2.type));
    res->type = NONE;
    return ;
  }

  // casting int->float
  int resType = INT;
  if (expr1.type == FLOAT || expr1.type == LIST_FLOAT || expr2.type == FLOAT || expr2.type == LIST_FLOAT)
    resType = FLOAT;

  if (!isList(expr1) && !isList(expr2)) {
    res->type = resType;
    res->nDim = 0;
  } else {
    // alguno de ellos es array
    if (isList(expr1) && !isList(expr2)) {
      // lista+valor ó lista-valor (ambas operaciones posibles)
      res->type = resType == INT ? LIST_INT : LIST_FLOAT;  // WIP comprobar si correcto en C
      res->nDim = expr1.nDim;
    } else if (!isList(expr1) && isList(expr2)) {
      // sólo permitir valor+lista
      if (op.attr == 1) {
        printf("[%sLinea %d] Binary operator - does not allow expressions of types [NOT LIST]-[LIST], but got types [%s]-[%s]\n",
          DEBUG ? "{572}" : "",line, tDataToString(expr1.type), tDataToString(expr2.type));
        return ;
      }
      res->type = resType == INT ? LIST_INT : LIST_FLOAT;   // WIP ídem
      res->nDim = expr2.nDim;
    } else {
      printf("[%sLinea %d] Binary operator %s does not allow expressions of types [LIST]%s[LIST], but got types [%s]%s[%s]\n",
        DEBUG ? "{578}" : "",line, op.lex, op.lex, tDataToString(expr1.type), op.lex, tDataToString(expr2.type));
    }
  }
}

void comprobarMasMenos(attrs op, attrs expr, attrs *res) {
  if (!(expr.type == INT || expr.type == FLOAT)) {
    printf("[%sLinea %d] Unary operator %s expects expression of type %s[INT|FLOAT], but got type %s[%s]\n",
      DEBUG ? "{584}" : "",line, op.lex, op.lex, op.lex, tDataToString(expr.type));
    return ;
  }
  res->type = expr.type;
  res->nDim = 0;
}

void comprobaOperadorBinarioMultiplicacion(attrs expr1, attrs op, attrs expr2, attrs *res) {
  if (op.attr == 1) {  // %
    // int % int
    if (expr1.type == INT && expr2.type == INT) {
      res->type = INT;
      res->nDim = 0;
    // list % int
    } else if (isList(expr1) && expr2.type == INT) {
      res->type = expr1.type;
      res->nDim = atoi(expr2.lex); // WIP comprobar
    } else {
      printf("[%sLinea %d] Binary operator %s expects expressions of types [INT]%s[INT] or [LIST]%s[INT], but got [%s]%s[%s]\n",
        DEBUG ? "{605}" : "",line, op.lex, op.lex, op.lex, tDataToString(expr1.type), op.lex, tDataToString(expr2.type));
    }
  } 
  else {  // * /
    // WIP sólo permitimos producto/división de enteros/flotantes (¿de char/bool?)
    if (!(expr1.type == INT || expr1.type == LIST_INT || expr1.type == FLOAT || expr1.type == LIST_FLOAT) ||
      !(expr2.type == INT || expr2.type == LIST_INT || expr2.type == FLOAT || expr2.type == LIST_FLOAT)) {
      printf("[%sLinea %d] Binary operator %s expects expressions of types [INT|FLOAT|LIST OF INT|LIST OF FLOAT]%s[INT|FLOAT|LIST OF INT|LIST OF FLOAT], but got [%s]%s[%s]\n",
        DEBUG ? "{626}" : "",line, op.lex, op.lex, tDataToString(expr1.type), op.lex, tDataToString(expr2.type));
      return ;
    }

    // casting int->float
    int resType = INT;
    if ((expr1.type == FLOAT || expr1.type == LIST_FLOAT) && (expr2.type == FLOAT || expr2.type == LIST_FLOAT))
      resType = FLOAT;

    if (!isList(expr1) && !isList(expr2)) {
      res->type = resType;
      res->nDim = 0;
    } else {
      // alguno de ellos es array
      if (isList(expr1) && !isList(expr2)) {
        // lista+valor ó lista-valor (ambas operaciones posibles)
        res->type = resType == INT ? LIST_INT : LIST_FLOAT;  // WIP comprobar si correcto en C
        res->nDim = expr1.nDim;
      } else if (!isList(expr1) && isList(expr2)) {
        // sólo permitir valor+lista
        if (op.attr == 0) {
          printf("[%sLinea %d] Binary operator / does not allow expressions of types [NOT LIST]/[LIST], but got [%s]/[%s]\n",
            DEBUG ? "{648}" : "",line, tDataToString(expr1.type), tDataToString(expr2.type));
          return ;
        }
        res->type = resType == INT ? LIST_INT : LIST_FLOAT;   // WIP ídem
        res->nDim = expr2.nDim;
      } else {
        printf("[%sLinea %d] Binary operator %s does not allow expressions of types [LIST]%s[LIST], but got types [%s]%s[%s]\n",
          DEBUG ? "{655}" : "",line, op.lex, op.lex, tDataToString(expr1.type), op.lex, tDataToString(expr2.type));
      }
    }
  }
}

void comprobaOperadorBinarioConcatenarListas(attrs expr1, attrs op, attrs expr2, attrs *res) {
  if (isList(expr1) && isList(expr2)) {
    // both must be of the same type
    if (expr1.type == expr2.type) {
      res->type = expr1.type;
      res->nDim = expr1.nDim + expr2.nDim;
    } else {
      printf("[%sLinea %d] Binay operator ** expects expressions of types [LIST OF x]**[LIST OF x] (lists of same type), but got [%s]**[%s]\n",
        DEBUG ? "{615}" : "",line, tDataToString(expr1.type), tDataToString(expr2.type));
    }
  } else {
    printf("[%sLinea %d] Binary operator ** expects expressions of types [LIST]**[LIST], but got [%s]**[%s]\n",
      DEBUG ? "{619}" : "",line, tDataToString(expr1.type), tDataToString(expr2.type));
  }
}

void comprobarOperadorBinarioAndOr(attrs expr1, attrs op, attrs expr2, attrs *res) {
  if (expr1.type == BOOL && expr2.type == BOOL) {
    res->type = BOOL;
    res->nDim = 0;
    return ;
  }
  printf("[%sLinea %d] Binary operator %s expects expressions of types [BOOLEAN]%s[BOOLEAN], but got [%s]%s[%s]\n",
    DEBUG ? "{668}" : "",line, op.lex, op.lex, tDataToString(expr1.type), op.lex, tDataToString(expr2.type));
}

void comprobarOperadorBinarioRelacion(attrs expr1, attrs op, attrs expr2, attrs *res) {
  if ((expr1.type == INT || expr1.type == FLOAT || expr1.type == CHAR) && (expr2.type == INT || expr2.type == FLOAT || expr2.type == CHAR)) {
    res->type = BOOL;
    res->nDim = 0;
    return ;
  }
  printf("[%sLinea %d] Binary operator %s expects expressions of types [INT|FLOAT|CHAR]%s[INT|FLOAT|CHAR], but got [%s]%s[%s]\n",
    DEBUG ? "{678}" : "",line, op.lex, op.lex, tDataToString(expr1.type), op.lex, tDataToString(expr2.type));
}

void Check_OpBinaryEq(attrs expr1, attrs op, attrs expr2, attrs *res) {
  if (isList(expr1) || isList(expr2)) {
    printf("[%sLinea %d] Binary operator %s expects expressions of types [NOT LIST]%s[NOT LIST], but got [%s]%s[%s]\n",
      DEBUG ? "{684}" : "",line, op.lex, op.lex, tDataToString(expr1.type), op.lex, tDataToString(expr2.type));
    return ;
  }
  if (expr1.type == expr2.type) {
    res->type = BOOL;
    res->nDim = 0;
    return ;
  }
  printf("[%sLinea %d] Binary operator %s expects expressions of types [x]%s[x] (same type), but got [%s]%s[%s]\n",
    DEBUG ? "{693}" : "",line, op.lex, op.lex, tDataToString(expr1.type), op.lex, tDataToString(expr2.type));
}

// WIP si hacemos IDENTIFIER AT expresion, debemos de hacer TS_GetById
void comprobarPosicionLista(attrs expr1, attrs op, attrs expr2, attrs *res) {
  if (isList(expr1) && expr2.type == INT) {
    res->type = listToType(expr1.type);
    res->nDim = 0;
    return ;
  }
  printf("[%sLinea %d] Binary operator @ expects expressions of types [LIST]@[INT], but got [%s]@[%s]\n",
    DEBUG ? "{704}" : "", line, tDataToString(expr1.type), tDataToString(expr2.type));
  res->type = NONE;
  res->nDim = 0;
}

void comprobarMenosMenos(attrs expr1, attrs op, attrs expr2, attrs *res) {
  if (isList(expr1) && expr2.type == INT) {
    res->type = expr1.type;
    res->nDim = expr1.nDim - 1;
  } else {
    printf("[%sLinea %d] Binary operator -- expects expressions of types [LIST]--[INT], but got [%s]--[%s]\n",
      DEBUG ? "{715}" : "", line, tDataToString(expr1.type), tDataToString(expr2.type));
  }
}

void comprobarOperadorTernarioLista(attrs expr1, attrs op1, attrs expr2, attrs op2, attrs expr3, attrs *res) {
  if (!isList(expr1) || listToType(expr1.type) != expr2.type || expr3.type != INT) {
    printf("[%sLinea %d] Ternary operator ++ @ expects expressions of types [LIST OF x]++[x]@[INT], but got types [%s]++[%s]@[%s]\n",
      DEBUG ? "{722}" : "",line, tDataToString(expr1.type), tDataToString(expr2.type), tDataToString(expr3.type));
    return ;
  }
  res->type = expr1.type;
  res->nDim = expr1.nDim + 1;
}

void comprobarLlamadaAFuncion(attrs id) {
  currentFunction = TS_FindByName(id);

  if (currentFunction == -1)
    printf("[%sLinea %d] Function with name %s not found\n", DEBUG ? "{734}" : "",line, id.lex);
}

void agregarNuevoID(attrs id, attrs *res) {
  if (decVar == 1)
    TS_AddId(id);
  else
    TS_GetById(id, res);
}

//////////////////////////
/////// Traduccion ///////
//////////////////////////

int temp = 0;
int etiq = 0;
int varPrinc=1;
int decIF = 0,decElse=0;

FILE *file = NULL;
FILE *file_definitivo = NULL;
FILE *file_funciones = NULL;

attrs lista_meterdatos[10];
int indice_meterdatos=0;

char *lista_param;
char *lista_func;
char *lista_evals;

attrs lista_sacardatos[10];
int indice_sacardatos=0;
int nivel_bloque;

char *temporal() {
	char * cadena;
	cadena = (char *) malloc(20);
	sprintf(cadena, "temp%d",temp);
	temp++;
	return cadena;
}

char *etiqueta() {
	char *cadena;
	cadena = (char *) malloc(20);
	sprintf(cadena, "etiqueta%d",etiq);
	etiq++;
	return cadena;
}

void openFile(){
  file = fopen("src/traduccion_temporal.txt","wr");
  if (file == NULL) {
    fputs ("File error",stderr); exit (1);
  }
}

void openFileDefinitivo(){
  file_definitivo = fopen("src/traduccion.c","wr");
  if (file_definitivo == NULL) {
    fputs ("File definitivo error",stderr); exit (1);
  }
}

void openFileFunciones(){
  file_funciones = fopen("src/traduccion_funciones.txt","wr");
  if (file_funciones == NULL) {
    fputs ("File funciones error",stderr); exit (1);
  }
}

// Abre un fichero para crear el código intermedio
void generaFich() {
  openFile();
  openFileDefinitivo();
  openFileFunciones();
  
	fputs("#include <stdio.h>\n",file_definitivo);
  fputs("#include <stdbool.h>\n\n",file_definitivo);

}

// Cerrar fichero
void closeInter() {
  closeFile();
  closeFileDefinitivo();
  closeFileFunciones();
}

void closeFile(){
  fclose(file);
  file = NULL;
}

void closeFileDefinitivo(){
  fclose(file_definitivo);
  file_definitivo = NULL;
}

void closeFileFunciones(){
  fclose(file_funciones);
  file_funciones = NULL;
}

char *align() {
  char *ts = (char *) malloc(tabs);
  for (int i = 0; i < tabs; i++){
    ts[i] = '\t';
  }
  return ts;
}

void saltoLinea() {
  fputs("\n",file);
}
void pyc() {
  if(esFunc){
    fputs(";",file_funciones);
  }else if(nivel_bloque)
    fputs(";",file);
  else
    fputs(";", file_definitivo);
}
void pycYSalto() {
  if(esFunc){
    fputs(";\n",file_funciones);
  }else if(nivel_bloque)
    fputs(";\n",file);
  else
    fputs(";\n",file_definitivo);
}

const char *cadtolower(const char *cadena) {
  char *cad = (char *) malloc(strlen(cadena));
  for (int i = 0; i < strlen(cadena); i++) {
    cad[i] = tolower(cadena[i]);
  }
  return cad;
}

const char *replace(const char *cadena, char f, char r) {
  char *cad = (char *) malloc(strlen(cadena));
  for (int i = 0; i < strlen(cadena); i++) {
    cad[i] = cadena[i];
    if (cad[i] == f) cad[i] = r;
  }
  return cad;
}

void declarID(attrs e) {
  const char *type = replace(tDataToString(e.type), ' ', '_');
  char *id = e.lex;
  char *declar = (char *) malloc(strlen(type)+strlen(id) + 1);
  sprintf(declar,"%s %s",cadtolower(type),id);
  if(esFunc){
    fputs(declar,file_funciones);
  }else if(nivel_bloque)
    fputs(declar,file);
  else
    fputs(declar,file_definitivo);
}

void declarListID(attrs e) {
  char *id = e.lex;
  char *declar = (char *) malloc(strlen(id) + 2);
  sprintf(declar,", %s",id);
  if(esFunc) fputs(declar,file_funciones);
  else if(nivel_bloque) fputs(declar,file);
  else fputs(declar,file_definitivo);
}

void Robert() {
  fputs("\nint main(){\n",file_definitivo);
}

void RobertEnd() {
  fputs("\n\treturn 0;\n}", file_definitivo);
}

void iniBloque() { 
  fputs("{\n",file);
}

void finBloque() {
  fputs("}\n",file);
}

void concatenaLex(attrs expr1, attrs op, attrs expr2, attrs *res) {
  res->lex = (char *) malloc(strlen(expr1.lex)+strlen(op.lex)+strlen(expr2.lex));
  sprintf(res->lex,"%s%s%s",expr1.lex,op.lex,expr2.lex);
}

char *eliminarPos(char *cad, int index) {
  int size = strlen(cad);
  if (size > 0) {
    int contador = 0;
    char *cadena = (char *) malloc(size-1);
    for (int i = 0; i < size; i++) {
      if (i != index) {
        cadena[contador] = cad[i];
        contador++;
      }
    }
    return cadena;
  }
  return "";
}

char *concat(char *cad1, char *cad2, char sep) {
  if(!cad1) return cad2;
  else{
    char *cad = (char *) malloc(strlen(cad1)+strlen(cad2)+1);
    sprintf(cad,"%s%c%s",cad1,sep,cad2);
    return cad;
  } 
  
}

void evaluar_expresion(attrs expr1, attrs op, attrs expr2, attrs *res)
{
  char *temp = temporal();
  const char *type = cadtolower(tDataToString(expr1.type));
  int size = strlen(expr1.eval)+strlen(expr2.eval)+strlen(type)+strlen(temp)*2+strlen(expr1.temp_asociado)+strlen(op.lex)+strlen(expr2.temp_asociado)+10;
  char * cadena_eval = (char *) malloc(size);

  sprintf(cadena_eval,"%s",expr1.eval);
  sprintf(cadena_eval,"%s%s",cadena_eval, expr2.eval);
  sprintf(cadena_eval,"%s\n%s %s;",cadena_eval,type,temp);
  sprintf(cadena_eval,"%s\n%s = %s %s %s;",cadena_eval,temp,expr1.temp_asociado,op.lex,expr2.temp_asociado);

  res->temp_asociado = temp;
  res->eval = cadena_eval;
}

void evaluar_expresion_unaria(attrs op, attrs expr1, attrs *res)
{
  const char *type = cadtolower(tDataToString(expr1.type));
  char *temp = temporal();
  int size = strlen(type)+strlen(temp)*2+strlen(op.lex)+strlen(expr1.temp_asociado)+10;
  char *cadena_eval = (char *) malloc(size);

  sprintf(cadena_eval," \n %s %s\n %s = %s%s;",type,temp,temp,op.lex,expr1.temp_asociado);

  res->temp_asociado = temp;
  res->eval = cadena_eval;
}

char *evaluar_sentencia_asig(attrs id, attrs exp) {
  int size = strlen(exp.eval)+strlen(id.lex)+strlen(exp.temp_asociado)+4;
  char *cadena = (char *) malloc(size);
  
  sprintf(cadena,"%s",exp.eval);
  sprintf(cadena,"%s\n%s = %s;\n",cadena,id.lex,exp.temp_asociado);
  

  cadena = eliminarPos(cadena,0);

  if(esFunc){
    fputs("{\n",file_funciones);
    fputs(cadena,file_funciones);
    fputs("}\n",file_funciones);
  }else{
    iniBloque();
    fputs(cadena, file);
    finBloque();
  }
  

  return cadena;
}

///////////// IF ELSE //////////////////

char *searchElse () {
  char *etiq ;
  int i = TOS - 1 ;
  int pos = -1 ;
  while ( i > 0 && pos == -1 ) {
    if ( ts[i].entry == CONDITION ) pos = i ;
    i-- ;
  }
  etiq = ts[pos].des->EtiquetaElse ;
  char *copy = (char *) malloc(strlen(etiq));
  sprintf(copy,"%s",etiq);
  return (copy) ;
}

char *searchSalida () {
  char *etiq ;
  int i = TOS - 1 ;
  int pos = -1 ;
  while ( i > 0 && pos == -1 ) {
    if ( ts[i].entry == CONDITION ) pos = i ;
    i-- ;
  }
  
  etiq = ts[pos].des->EtiquetaSalida ;
  char *copy = (char *) malloc(strlen(etiq));
  sprintf(copy,"%s",etiq);
  return (copy) ;
}

void insertIfElse() {
  inTS newIn ;
  DDIC *dp = (DDIC *) malloc(sizeof(DDIC));
  
  //dp.EtiquetaEntrada = etiqueta();
  dp->EtiquetaElse = etiqueta();
  dp->EtiquetaSalida = etiqueta();
  

  newIn.entry = CONDITION;
  newIn.lex = "ifthenelse";
  newIn.type = NONE;
  newIn.nDim = 0;
  newIn.nParams = 0;

  newIn.des = dp;
  TS_AddEntry(newIn);
}

void goToElse(char *cond) {
  char *etiq_else = searchElse();
  char *ifsentence = (char *) malloc(strlen(cond)+strlen(etiq_else)+13) ;

  strcat(ifsentence, "if (!") ;
  strcat(ifsentence, cond) ;
  strcat(ifsentence, ") goto ") ;
  strcat(ifsentence, etiq_else) ;
  strcat(ifsentence, ";\n{\n") ;
  
  //printf("%s",ifsentence);
  if(esFunc){
    fputs(ifsentence,file_funciones);
  }else{
    fputs(ifsentence,file) ;
  }
  
}

void goToEndAndElse() {
  char *etiq_else = searchElse();
  char *etiq_salida = searchSalida();
  char *result = (char *) malloc(strlen(etiq_else)+strlen(etiq_salida)+9);
  
  sprintf(result, "}\ngoto %s;\n",etiq_salida);
  sprintf(result, "%s%s:\n{\n",result,etiq_else);
  
  if(esFunc){
    fputs(result, file_funciones);
  }else{
    fputs(result, file);
  }
  
}

void addSalida() {
  char *etiq_salida = searchSalida();
  char *result = (char *) malloc(strlen(etiq_salida)+4);

  sprintf(result,"}\n%s:;\n",etiq_salida);

  if(esFunc){
    fputs(result, file_funciones);
  }else{
    fputs(result, file);
  }
  
}

void evaluar_sentencia_if (attrs expr) {
  char *cadena = (char *) malloc(strlen(expr.eval)+1);
  expr.eval = eliminarPos(expr.eval,0);
  
  sprintf(cadena,"%s\n",expr.eval);
  
  if(esFunc){
    fputs(cadena,file_funciones);
  }else{
    fputs(cadena, file);
  }
  
}

DDIC *searchCondition(const char *lex) {
  int i = TOS - 1 ;
  int pos = -1 ;
  while ( i > 0 && pos == -1 ) {
    if ( ts[i].entry == CONDITION && strcmp(ts[i].lex,lex) == 0) pos = i ;
    i-- ;
  }
  DDIC *des = ts[pos].des;
  return (des) ; 
}

void insertWhile() {
  inTS newIn ;
  DDIC *dp = (DDIC *) malloc(sizeof(DDIC));
  
  dp->EtiquetaEntrada = etiqueta();
  //dp->EtiquetaElse = etiqueta();
  dp->EtiquetaSalida = etiqueta();
  

  newIn.entry = CONDITION;
  newIn.lex = "while";
  newIn.type = NONE;
  newIn.nDim = 0;
  newIn.nParams = 0;

  newIn.des = dp;
  TS_AddEntry(newIn);
}

void goToWhile(char *cond, attrs expr) {
  // Pillamos las etiquetas
  DDIC *des = searchCondition("while") ;
  char *etiq_in = des->EtiquetaEntrada ;
  char *etiq_sal = des->EtiquetaSalida;
  char *whilesentence = (char *) malloc(strlen(etiq_in)+strlen(expr.eval)+strlen(cond)+strlen(etiq_sal)+18);

  // Añadimos a whilesentences las etiquetas y las condiciones
  sprintf(whilesentence,"\n%s",etiq_in);
  sprintf(whilesentence,"%s:;",whilesentence) ;
  sprintf(whilesentence,"%s%s\n",whilesentence,expr.eval);
  sprintf(whilesentence, "\n%sif(!%s) goto %s;\n{\n",whilesentence,cond, etiq_sal); //expr.eval

  if(esFunc){
    fputs(whilesentence,file_funciones);
  }else{
    fputs(whilesentence,file) ; 
  }
  
}

void evaluar_sentencia_while() {
  DDIC *inOutWhile = searchCondition("while");
  char *etiq_entrada = inOutWhile->EtiquetaEntrada;
  char *etiq_salida = inOutWhile->EtiquetaSalida;
  char *cadena = (char *) malloc(strlen(etiq_entrada)+strlen(etiq_salida)+12);

  //sprintf(cadena,"%s\n",expr.eval);
  sprintf(cadena,"}\ngoto %s;\n",etiq_entrada);
  sprintf(cadena,"%s%s:\n",cadena, etiq_salida);
  if(esFunc){
    fputs(cadena,file_funciones);
  }else{
    fputs(cadena, file);
  }
  
}

void insertFor(){
  inTS newIn ;
  DDIC *dp = (DDIC *) malloc(sizeof(DDIC));
  
  dp->EtiquetaEntrada = etiqueta();
  //dp->EtiquetaElse = etiqueta();
  dp->EtiquetaSalida = etiqueta();
  

  newIn.entry = CONDITION;
  newIn.lex = "for";
  newIn.type = NONE;
  newIn.nDim = 0;
  newIn.nParams = 0;

  newIn.des = dp;
  TS_AddEntry(newIn);
}

void goToFor(attrs id, attrs expr, attrs val){
  DDIC *des = searchCondition("for") ;
  char *etiq_in = des->EtiquetaEntrada ;
  char *etiq_sal = des->EtiquetaSalida;
  char *forsentence = (char *) malloc(strlen(expr.eval)+strlen(id.lex)*2+strlen(expr.temp_asociado)+strlen(etiq_in)+strlen(val.lex)+strlen(etiq_sal)+23);

  
  sprintf(forsentence, "%s\n%s=%s",expr.eval,id.lex, expr.temp_asociado);
  sprintf(forsentence, "%s;\n%s:",forsentence,etiq_in);
  sprintf(forsentence, "%s\nif(!(%s<%s)) goto %s;\n{\n",forsentence,id.lex,val.lex,etiq_sal);
  
  //printf("%s",expr.eval);

  //printf();
/*
  sprintf(forsentence,"\n%s",etiq_in);
  sprintf(forsentence,"%s:",forsentence) ;
  sprintf(forsentence,"%s%s\n",forsentence,expr.eval);
  sprintf(forsentence, "\n%sif(!%s) goto %s;\n{\n",forsentence,cond, etiq_sal);*/

  if(esFunc){
    fputs(forsentence,file_funciones);
  }else{
    fputs(forsentence,file) ;
  }
  
}

void evaluar_sentencia_for(char *lex){
  DDIC *inOutWhile = searchCondition("for");
  char *etiq_entrada = inOutWhile->EtiquetaEntrada;
  char *etiq_salida = inOutWhile->EtiquetaSalida;
  char *cadena = (char *) malloc(strlen(etiq_entrada)+strlen(etiq_salida)+strlen(lex)+16);

  sprintf(cadena,"}\n++%s;",lex);
  sprintf(cadena,"%s\ngoto %s;\n",cadena,etiq_entrada);
  sprintf(cadena,"%s%s:;\n",cadena, etiq_salida);
  
  if(esFunc){
    fputs(cadena,file_funciones);
  }else{
    fputs(cadena, file);
  }
  
}

void addVariableListaMeterDatos(attrs id){
  attrs *res;
  TS_GetById(id,res);
  if(indice_meterdatos < 10){
    lista_meterdatos[indice_meterdatos] = *res;
    indice_meterdatos++;
  }
}

// Para procesar sentencias de entrada
void evaluarSentenciaEntrada(){
  char * entre_comillas = (char *) malloc(500);
  
  sprintf(entre_comillas,"scanf(\"");
  for(int i = 0; i < indice_meterdatos; i++){
    if(lista_meterdatos[i].type == INT || lista_meterdatos[i].type == BOOL){
      sprintf(entre_comillas,"%s%s",entre_comillas,"%d");
    }else if(lista_meterdatos[i].type == FLOAT){
      sprintf(entre_comillas,"%s%s",entre_comillas,"%f");
    }else if(lista_meterdatos[i].type == CHAR){
      sprintf(entre_comillas,"%s%s",entre_comillas,"%s");
    }
  }
  sprintf(entre_comillas,"%s\"",entre_comillas);

  for(int i=0; i < indice_meterdatos; i++){
    sprintf(entre_comillas, "%s,&%s",entre_comillas,lista_meterdatos[i].lex);
  }
  
  sprintf(entre_comillas,"%s);\n",entre_comillas);
  indice_meterdatos = 0;

  if(esFunc){
    fputs(entre_comillas, file_funciones);
  }else{
    fputs(entre_comillas, file);
  }
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Para procesar sentencias de salida
//
// Esto:
//    sacar<-("blablabla", a, "bla", b+c, "bla");
//
// pasaria a ser:
//    printf("blablabla%tbla%tbla", a, b+c);
//
// Entonces necesitamos:
//    - un "entre_comillas_cadena" para guardar la cadena.
//    - un "entre_comillas_varibles" para guardar las variables utilizadas en la cadena.
//    - un "entre_comillas" para guardar la concatenacion de lo anterior.
//
// sprintf(entre_comillas,"%s%s",entre_comillas_cadena,entre_comillas_variables);
//
// PROCESO:
// ???
////////////////////////////////////////////////////////////////////////////////////////////////////

void addVariableListaSacarDatos(attrs id){
  attrs res;
  int index = TS_FindById(id);

  res.type = STRING;
  res.lex = id.lex;
  if (index >= 0) {
    res.lex  = ts[index].lex; 
    res.type = ts[index].type;
  }
  
  if (indice_sacardatos< 10) {
    lista_sacardatos[indice_sacardatos] = res;
    indice_sacardatos++;
  }
  
}

void evaluarSentenciaSalida(){
  char * entre_comillas = (char *) malloc(500);
  
  sprintf(entre_comillas,"printf(\"");
  for(int i = 0; i < indice_sacardatos; i++){
    if(lista_sacardatos[i].type == INT || lista_sacardatos[i].type == BOOL){
      sprintf(entre_comillas,"%s %s ",entre_comillas,"%d");
    }else if(lista_sacardatos[i].type == FLOAT){
      sprintf(entre_comillas,"%s %s ",entre_comillas,"%f");
    }else if(lista_sacardatos[i].type == CHAR || lista_sacardatos[i].type == STRING){
      sprintf(entre_comillas,"%s %s ",entre_comillas,"%s");
    }
  }
  sprintf(entre_comillas,"%s \"",entre_comillas);

  for(int i=0; i < indice_sacardatos; i++){
    sprintf(entre_comillas, "%s,%s",entre_comillas,lista_sacardatos[i].lex);
  }
  
  
  sprintf(entre_comillas,"%s);\n",entre_comillas);
  indice_sacardatos = 0;

  if(esFunc){
    fputs(entre_comillas, file_funciones);
  }else{
    fputs(entre_comillas, file);
  }
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void imprimePila(){
  attrs id;
  char * buffer_intermedio = (char*) malloc(500);
  
  for (int i = 0; i < TOS; i++) {
    id.lex = ts[i].lex;
    int index = TS_FindById(id);
    if (index >= 0) {
      printf("%s -> %s\n",ts[index].lex, tDataToString(ts[index].type));
    }
  }
}

void impresion_lexema(attrs declar, attrs id, char* var, char *res)
{
  char *imprimir = (char*) malloc(strlen(cadtolower(tDataToString(declar.type))) + strlen(id.lex) + strlen(var) + 5);
  
  sprintf(imprimir,"%s %s(%s);\n",cadtolower(tDataToString(declar.type)), id.lex, var);

  //printf("\n45%s\n",var);
  res = imprimir;
  fputs( imprimir,file_definitivo);
  
  fputs( eliminarPos(imprimir, strlen(imprimir)-2),file_funciones);
  fputs("{\n",file_funciones);
  //Vaciado lista_param
  //printf("%s",lista_param);
  for(int i = 0; i < strlen(lista_param); i++){
    lista_param[i] = '\0';
  }
  lista_param = NULL;

}

void juntarDeclarId(attrs declar, attrs id, attrs *res){
  char * devolver = (char *) malloc(strlen(declar.lex) + strlen(id.lex) + 1);
  sprintf(devolver, "%s %s", cadtolower(tDataToString(declar.type)), id.lex);
  res->lex = devolver;
}


void imprimir_return(attrs expr){
  char *cadena = (char *) malloc(strlen(expr.temp_asociado)+strlen(expr.eval)+10);

  sprintf(cadena, "%s\n", expr.eval);
  sprintf(cadena, "%sreturn %s;\n", cadena,expr.temp_asociado);

  fputs(cadena, file_funciones);
}

void sacarEvalAsociado(attrs expr){
  char * cadena = (char*) malloc (strlen(expr.eval) );
  sprintf(cadena, "%s", expr.eval);
  lista_evals = concat(lista_evals ,cadena,'\n');
}

void sacarTemporalAsociado(attrs expr){
  char * cadena = (char*) malloc (strlen(expr.temp_asociado) );
  sprintf(cadena, "%s", expr.temp_asociado);
  lista_func = concat(lista_func,cadena,',');
}

char* componerLlamadaFuncion(char *nombreFunc){
  char * cadena = (char *) malloc(strlen(lista_func) + strlen(nombreFunc) + 3);
  sprintf(cadena,"%s(%s)",nombreFunc,lista_func);
  lista_func = NULL;
  return cadena;
}

