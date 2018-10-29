#include "tablas.h"

//Quita espacios y retorna numero de columnas "i"

int leer_entrada(char *prop)
{
	char c; int i=0;
	while((c=getchar()) != '\n' && i < (LMAX-1))
		if(c!=' ')
			prop[i++] = c;
	prop[i] = '\0';
	return i;
}

//Busca operacion final y retorna a index

int max_precedencia(char *prop, int ini, int fin)
{
	int npar=0, prec=0, anid=0, index=0;
	bool prim=true;
	for(int i=ini; i<fin; i++)
    {
		if(prop[i] == '(')
			npar++;
		else if(prop[i] == ')')
			npar--;
		else
        {
			if(npar<0) return -1;
			int p = precedencia(prop[i]);
			if(prim)
            {
				anid = npar;
				prec = p;
				index = i;
				prim = false;
				continue;
			}
			if(npar<anid)
            {
				anid = npar;
				prec = p;
				index = i;
			}
			else if(anid==npar)
            {
				if(p>prec)
                {
					prec = p;
					index = i;
				}
				else if(p==prec)
                {
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

int precedencia(char c)
{
	switch(c)
    {
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

bool validar_sintaxis(char *prop, int ini, int fin)
{
	int index = max_precedencia(prop, ini, fin);
	if(index == -1)
        return false;
	if(prop[index] == '!')
    {
		int npar=0, i=index+1, f;
		for(f=i; f<fin; f++)
        {
			if(prop[f]=='(')
    			npar++;
			else if(prop[f]==')')
            {
				npar--;
				if(npar<0)
                    break;
			}
		}
		return validar_sintaxis(prop, i, f);
	}
	else if(prop[index]=='&' || prop[index]=='|' || prop[index]==':' || prop[index]=='=')
    {
		int npar=0, i, f=index;
		bool p, q;
		for(i=f-1; i>=ini; i--)
        {
			if(prop[i]==')')
				npar++;
			else if(prop[i]=='(')
            {
				npar--;
				if(npar<0)
                    break;
			}
		}
		p = validar_sintaxis(prop, i+1, f);
		npar=0; i=index+1;
		for(f=i; f<fin; f++)
        {
			if(prop[f]=='(')
				npar++;
			else if(prop[f]==')')
            {
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

bool evaluar_proposicion(char *prop, bool *vals, char *vars, char *resp, int ini, int fin)
{
	int index = max_precedencia(prop, ini, fin);
	if(prop[index] == '!')
    {
		int npar=0, i=index+1, f;
		bool b;
		for(f=i; f<fin; f++)
        {
			if(prop[f]=='(')
				npar++;
			else if(prop[f]==')')
            {
				npar--;
				if(npar<0) break;
			}
		}
		b = !evaluar_proposicion(prop, vals, vars, resp, i, f);
		if(b) resp[index] = 'V'; else resp[index] = 'F';    //Respuesta de la negacion
		return b;
	}
	else if(prop[index]=='&' || prop[index]=='|' || prop[index]==':' || prop[index]=='=')
    {
		int npar=0, i, f=index;
		bool p, q, r;
		for(i=f-1; i>=ini; i--)
        {
			if(prop[i]==')')
				npar++;
			else if(prop[i]=='(')
            {
				npar--;
				if(npar<0)
                    break;
			}
		}
		p = evaluar_proposicion(prop, vals, vars, resp, i+1, f);
		npar=0; i=index+1;
		for(f=i; f<fin; f++)
        {
			if(prop[f]=='(')
				npar++;
			else if(prop[f]==')')
            {
				npar--;
				if(npar<0) break;
			}
		}
		q = evaluar_proposicion(prop, vals, vars, resp, i, f);
		switch(prop[index])
        {
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
	else
		for(int i=0; vars[i]!='\0'; i++)
			if(prop[index] == vars[i])
            {
				if(vals[i])
					resp[index] = 'V';  //Valor de verdad de las variables
				else
					resp[index] = 'F';
				return vals[i];
			}
}

//Busca el numero de variables de la funcion

int buscar_variables(char *prop, char *vars)
{
	int nvars = 0;
	for(int i=0; prop[i]!='\0'; i++)
		if(prop[i]!='(' && prop[i]!=')' && prop[i]!='!' && prop[i]!='&' && prop[i]!='|' && prop[i]!=':' && prop[i]!='=')
        {
			bool existe = false;
			for(int j=0; j<nvars && !existe; j++)
            	if(prop[i] == vars[j])
					existe = true;
			if(!existe)
				vars[nvars++] = prop[i];
		}
	return nvars;
}
