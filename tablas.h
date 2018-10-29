#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define LMAX 100

int leer_entrada(char *prop);
int max_precedencia(char *prop, int ini, int fin);
int precedencia(char c);
bool validar_sintaxis(char *prop, int ini, int fin);
bool evaluar_proposicion(char *prop, bool *vals, char *vars, char *resp, int ini, int fin);
int buscar_variables(char *prop, char *vars);