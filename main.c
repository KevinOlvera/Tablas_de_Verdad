#include "tablas.c"

int main(){

   	char prop[LMAX];
	char *vars;
	char *resp;
	bool *vals;
	int band;

	printf("\nEste programa construye la tabla de verdad de una expresion logica.\n");
	printf("\nLos operadores logicos validos son:\n\n");
	printf("\t\"!\"\t~\tNegacion\n");
	printf("\t\"&\"\t^\tConjuncion\n");
	printf("\t\"|\"\tv\tDisyuncion\n");
	printf("\t\":\"\t->\tImplicacion\n");
	printf("\t\"=\"\t<=>\tBicondicional\n\n");
	printf("Marque con parentesis la jerarquia de operaciones de la funcion.\n");

	for(band=0;;band++)
	{
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

		for(int i=0; i<n; i++)
		{
			resp[i] = ' ';
			vars[i] = '\0';
			vals[i] = false;
		}

		resp[n-1] = '\0';

		if(validar_sintaxis(prop, 0, n))
		{
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

			for(int i=0; i<reps; i++)
			{
				for(int j=0; j<nvars; j++)
				{
					if(i%(int)pow(2, nvars-j-1) == 0)
						vals[j] = !vals[j];
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
		else
			printf("�ERROR! La sintaxis de la expresion es incorrecta.\n");
	}
	return 0;
}