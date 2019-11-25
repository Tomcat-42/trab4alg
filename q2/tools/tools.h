#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*Função de ordenação em arquivos genérica*/
void sort_file(FILE *fp, size_t n, size_t tam,
		int (*cmp)(const void *a, const void *b));

/*Função de busca em arquivos genérica*/
int search_file(FILE *fp, size_t n, size_t tam,
		const void *key,
		int (*cmp)(const void *a,const void *b),
		int num,
		void *ret);

#endif
