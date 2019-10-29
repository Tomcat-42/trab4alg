#ifndef USUARIOS_H
#define USUARIOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __unix__
#define USER_FILENAME "modulo_usuarios/usuarios.dat"
#define PATH_REL "relatorios/"
#elif
#define USER_FILENAME "modulo_usuarios\\usuarios.dat"
#define PATH_REL "relatorios\\"
#endif

/*Enum que representa o status do usuário*/
enum {NORM, SUSP} STATUS_USER;

/*Estrutura que representa um usuário*/
typedef struct
{
	int matricula;
	int rg;
	char cpf[12];
	char nome[51];
	char endereco[71];
	struct tm nasc;
	char sexo;
	char bairro[31];
	int fone_red;
	int fone_cel;
	struct tm adesao;
	int status;
	struct tm term_susp;
	int num_emprest;
	
}Usuario;

/*Busca uma entrada que contém dados repetidos*/
int busca_repetido_usuario(int matricula, int rg, char *cpf);

/*Pega a última entrada e insere no local correto*/
void sort_usuario();

/*vai buscar o usuario e gravar na struct *user, além disso vai retornar a posição dele no arquivo*/
int busca_generica_usuario(int matricula, Usuario *user);

/*Cadastra um usuário no arquivo*/
int cadastrar_usuario();

/*atualiza o cadastro de um usuário*/
int atualizar_cadastro();

/*Consulta um usuário na base de dados*/
int consultar_usuario();

/*Apaga um usuário na base de dados*/
int apagar_usuario();

/*imprime todos os usários no arquivo*/
int relatorio_usuario();

#endif
