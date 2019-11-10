#ifndef EMPRESTIMOS_H
#define EMPRESTIMOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __unix__
#define EMPR_FILENAME "modulo_emprestimos/emprestimos.dat"
#include "../modulo_livros/livros.h"
#include "../modulo_usuarios/usuarios.h"
#else
#define EMPR_FILENAME "modulo_emprestimos\\emprestimos.dat"
#include "..\\modulo_livros\\livros.h"
#include "..\\modulo_usuarios\\usuarios.h"
#endif


/*Estrutura de um empréstimo*/
typedef struct
{
	int matricula;
	int fone_red;
	int codigo;
	char titulo[81];
	struct tm emprestimo, devolucao;
}Emprestimo;

/* Vai ordenar os empréstimos por matricula do usuário 
 * e data de emprestimo dos livros */
void sort_emprestimos();

/*Muda o status do usuário de SUSP para NORM ou de NORM para SUSP.Nesse último
 * caso também seta sua data de suspensão para 30 dias a partir de 'hoje' */
void mudar_status_usuario(int matricula, struct tm hoje);

/*Muda o status do livro de DISP para SUSP*/
void mudar_status_livro(int codigo);

/*Retorna os empréstimos do usuário*/
int busca_emprestimo(int matricula, Emprestimo *emp, int n);

/*Empresta um livro para um usuário*/
int emprestar();

/*Devolve um livro*/
int devolver();

/*Printa um relatório de empréstimos para um arquivo de texto*/
int relatorio_emprestimo();

#endif
