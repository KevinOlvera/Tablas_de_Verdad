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

int main(){

   	char prop[LMAX];
	char *vars;
	char *resp;
	bool *vals;
	int band;

    system("color 4F");

	printf("\nEste programa construye la tabla de verdad de una expresion logica.\n");
	printf("\nLos operadores logicos validos son:\n\n");
	printf("\t\"!\"\t~\tNegacion\n");
	printf("\t\"&\"\t^\tConjuncion\n");
	printf("\t\"|\"\tv\tDisyuncion\n");
	printf("\t\":\"\t->\tImplicacion\n");
	printf("\t\"=\"\t<=>\tBicondicional\n\n");
	printf("Marque con parentesis la jerarquia de operaciones de la funcion.\n");

	for(band=0;;band++){

        if(band>0)
            printf("\nIngrese 0 para terminar\n");
		printf("\nIngrese la expresion: ");
		int n = leer_entrada(prop);

        if(band>0)
            if ( *prop == '0')
                break;

		resp = new char[n];
		vars = new char[n];
		vals = new bool[n];

		for(int i=0; i<n; i++){
			resp[i] = ' ';
			vars[i] = '\0';
			vals[i] = false;
		}

		resp[n-1] = '\0';

		if(validar_sintaxis(prop, 0, n)){
			int nvars = buscar_variables(prop, vars);
			int reps = (int)pow(2, nvars);
	        printf("\n");

			for(int i=0; i<nvars; i++)
				printf("%c ", vars[i]);
			printf("| ");

			for(int i=0; i<n; i++)
				printf("%c ", prop[i]);
            printf(" F");
			printf("\n\n");

			for(int i=0; i<reps; i++){
				for(int j=0; j<nvars; j++){
					if(i%(int)pow(2, nvars-j-1) == 0){
						vals[j] = !vals[j];
					}
					if(vals[j]) printf("V "); else printf("F ");    //Valores de verdad de las variables en tabla
				}
				printf("| ");
				evaluar_proposicion(prop, vals, vars, resp, 0, n);

				for(int j=0; j<n; j++)
					printf("%c ", resp[j]);

				if(evaluar_proposicion(prop, vals, vars, resp, 0, n) == true) //Muestra el resultado final
                    printf(" V");
                else
                    printf(" F");
               // printf("%d ", evaluar_proposicion(prop, vals, vars, resp, 0, n));

				printf("\n");
			}
		}
		else{
			printf("¡ERROR! La sintaxis de la expresion es incorrecta.\n");
		}
	}

	return 0;
}

//Quita espacios y retorna numero de columnas "i"

int leer_entrada(char *prop) {
	char c; int i=0;
	while((c=getchar()) != '\n' && i < (LMAX-1))
		if(c!=' ')
			prop[i++] = c;
	prop[i] = '\0';
	return i;
}

//Busca operacion final y retorna a index

int max_precedencia(char *prop, int ini, int fin) {
	int npar=0, prec=0, anid=0, index=0;
	bool prim=true;
	for(int i=ini; i<fin; i++) {
		if(prop[i] == '(') {
			npar++;
		}
		else if(prop[i] == ')') {
			npar--;
		}
		else {
			if(npar<0) return -1;
			int p = precedencia(prop[i]);
			if(prim) {
				anid = npar;
				prec = p;
				index = i;
				prim = false;
				continue;
			}
			if(npar<anid) {
				anid = npar;
				prec = p;
				index = i;
			}
			else if(anid==npar) {
				if(p>prec) {
					prec = p;
					index = i;
				}
				else if(p==prec) {
					if(prop[index] != prop[i])
						return -1;
					if(prop[index] != '!')
						index = i;
				}
			}
		}
	}
	if(npar != 0 || prim)
        return -1;
	return index;
}

//Operadores de la funcion

int precedencia(char c) {
	switch(c) {
		case '!': return 1;	//negacion
		case '&': return 2;	//conjuncion
		case '|': return 2;	//disyuncion
		case ':': return 3;	//implicacion
		case '=': return 4;	//bicondicional
	}
	return 0;
}

/*Esta funcion se encarga de validar la sintaxis de una exprecion de forma recursiva
usando la siguiente definicion:

	P ::= p / (!P) / (P&Q) / (P|Q) / (P:Q) / (P=Q)

Devuelve verdadero si la proposicion cumple la definicion y falso si no.*/

bool validar_sintaxis(char *prop, int ini, int fin) {
	int index = max_precedencia(prop, ini, fin);
	if(index == -1)
        return false;
	if(prop[index] == '!') {
		int npar=0, i=index+1, f;
		for(f=i; f<fin; f++) {
			if(prop[f]=='('){
				npar++;
			}
			else if(prop[f]==')') {
				npar--;
				if(npar<0)
                    break;
			}
		}
		return validar_sintaxis(prop, i, f);
	}
	else if(prop[index]=='&' || prop[index]=='|' || prop[index]==':' || prop[index]=='=') {
		int npar=0, i, f=index;
		bool p, q;
		for(i=f-1; i>=ini; i--) {
			if(prop[i]==')') {
				npar++;
			}
			else if(prop[i]=='(') {
				npar--;
				if(npar<0)
                    break;
			}
		}
		p = validar_sintaxis(prop, i+1, f);
		npar=0; i=index+1;
		for(f=i; f<fin; f++) {
			if(prop[f]=='(') {
				npar++;
			}
			else if(prop[f]==')') {
				npar--;
				if(npar<0)
                    break;
			}
		}
		q = validar_sintaxis(prop, i, f);
		return (p && q);
	}
	else {
		int vars=0;
		for(int i=ini; i<fin; i++)
			if(prop[i]!='(' && prop[i]!=')')
				vars++;
		if(vars == 1)
			return true;
	}
	return false;
}

bool evaluar_proposicion(char *prop, bool *vals, char *vars, char *resp, int ini, int fin) {
	int index = max_precedencia(prop, ini, fin);
	if(prop[index] == '!') {
		int npar=0, i=index+1, f;
		bool b;
		for(f=i; f<fin; f++) {
			if(prop[f]=='(') {
				npar++;
			}
			else if(prop[f]==')') {
				npar--;
				if(npar<0) break;
			}
		}
		b = !evaluar_proposicion(prop, vals, vars, resp, i, f);
		if(b) resp[index] = 'V'; else resp[index] = 'F';    //Respuesta de la negacion
		return b;
	}
	else if(prop[index]=='&' || prop[index]=='|' || prop[index]==':' || prop[index]=='='){
		int npar=0, i, f=index;
		bool p, q, r;
		for(i=f-1; i>=ini; i--) {
			if(prop[i]==')') {
				npar++;
			}
			else if(prop[i]=='(') {
				npar--;
				if(npar<0)
                    break;
			}
		}
		p = evaluar_proposicion(prop, vals, vars, resp, i+1, f);
		npar=0; i=index+1;
		for(f=i; f<fin; f++) {
			if(prop[f]=='(') {
				npar++;
			}
			else if(prop[f]==')') {
				npar--;
				if(npar<0) break;
			}
		}
		q = evaluar_proposicion(prop, vals, vars, resp, i, f);
		switch(prop[index]) {
			case '&': r = p&&q; break;
			case '|': r = p||q; break;
			case ':': r = !p||q; break;
			case '=': r = (!p||q)&&(!q||p); break;
		}
		if(r)
            resp[index] = 'V';  //Respuestas de las operaciones logicas
        else
            resp[index] = 'F';
		return r;
	}
	else {
		for(int i=0; vars[i]!='\0'; i++)
			if(prop[index] == vars[i]){
				if(vals[i])
					resp[index] = 'V';  //Valor de verdad de las variables
				else
					resp[index] = 'F';
				return vals[i];
			}
	}
}

//Busca el numero de variables de la funcion

int buscar_variables(char *prop, char *vars){
	int nvars = 0;
	for(int i=0; prop[i]!='\0'; i++) {
		if(prop[i]!='(' && prop[i]!=')' && prop[i]!='!' && prop[i]!='&' && prop[i]!='|' && prop[i]!=':' && prop[i]!='=') {
			bool existe = false;
			for(int j=0; j<nvars && !existe; j++) {
				if(prop[i] == vars[j]) {
					existe = true;
				}
			}
			if(!existe) {
				vars[nvars++] = prop[i];
			}
		}
	}
	return nvars;
}
