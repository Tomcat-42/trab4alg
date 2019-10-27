#ifndef LIVROS_H
#define LIVROS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __unix__
#define LIVRO_FILENAME "modulo_livros/livros.dat"
#elif
#define LIVRO_FILENAME "modulo_livros\livros.dat"
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

/*vai buscar o usuario e gravar na struct *user, além disso vai retornar a posição dele no arquivo(lembrar de voltar o ponteiro de arquivo dps de ler)*/
int busca_generica_livro(int codigo, Livro *livro);

/*Pega a última entrada e insere no local correto*/
void sort_livro();

/*Cadastra um livro no arquivo*/
int cadastrar_livro();

/*Muda o status de um livro*/
int alterar_status();

/*Consulta um livro na base de dados*/
int consultar_livro();

/*Apaga um livro na base de dados*/
int apagar_livro();

#endif
