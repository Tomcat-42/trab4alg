#ifndef LIVROS_H
#define LIVROS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __unix__
#define LIVRO_FILENAME "modulo_livros/livros.dat"
#define PATH_REL "relatorios/"
#include "../tools/tools.h"
#else
#define LIVRO_FILENAME "modulo_livros\\livros.dat"
#define PATH_REL "relatorios\\"
#include "..\\tools\\tools.h"
#endif

/*Enum que representa o status do livro*/
enum {DISP, EMPR, MANU} STATUS_LIVRO;

/*Estrutura que representa um livro*/
typedef struct
{
	int codigo;
	char isbn[13];
	char titulo[81];
	char autores[101];
	char assunto[81];
	char editora[51];
	int ano;
	int edicao;
	int status;
}Livro;

/*Comparação por código*/
int cmp_codigo(const void *a, const void *b);

/*Comparação por título*/
int cmp_titulo(const void *a, const void *b);

/*Cadastra um livro no arquivo*/
int cadastrar_livro();

/*Muda o status de um livro*/
int alterar_status_livro();

/*Consulta um livro na base de dados*/
int consultar_livro();

/*Apaga um livro na base de dados*/
int apagar_livro();

/*Imprime todas as obras cadastradas*/
int relatorio_livro();

#endif
