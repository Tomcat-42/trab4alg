#include "tools.h"

void sort_file(FILE *fp, size_t n, size_t tam,
		int (*cmp)(const void *a, const void *b))
{
	int i, j, sort=1;

	/*Posições de memória genéricas auxiliares*/
	void *aux0 = (void *)malloc(tam), *aux1 = (void *)malloc(tam);
	
	/*Começa da última posição do arquivo*/
	for(i=0; (i<n-1 && sort); i++)
	{
		sort=0;
		for(j=n-1; j>i; j--)
		{
			fseek(fp, (j-1)*tam, SEEK_SET);
			fread(aux0, tam, 1, fp);
			fread(aux1, tam, 1, fp);

			/*aux0 é maior que aux1*/
			if(cmp(aux0, aux1)>0)
			{
				sort=1;
				fseek(fp, -(2*tam), SEEK_CUR);
				fwrite(aux1, tam, 1, fp);
				fwrite(aux0, tam, 1, fp);
			}
		}
	}
	return;
}


int search_file(FILE *fp, size_t n, size_t tam,
		const void *key,
		int (*cmp)(const void *a,const void *b),
		int num,
		void *ret)
{
	int i, j;
	/*Posição genérica de memória auxiliar*/
	void *aux = (void *)malloc(tam);

	rewind(fp);
	for(i=0;(i<n && !feof(fp)); i++)
	{
		fread(aux, tam, 1, fp);
		
		/*Encontrou o elemento*/
		if(!feof(fp) && !cmp(aux, key))
		{
			/*Copia a(s) struct encontrada para retornar*/
			for(j=0; j<num; j++)
			{
				/*Parabéns, ao ver essa gambiarra você ganha um AVC de graça*/
				memcpy(((char*)ret + j*tam), aux, tam);
				fread(aux, tam, 1, fp);
			}
			return i;
		}
	}
	return -1;
}
