#ifdef __unix__
#define CLEARCMD "clear"
#include "./modulo_usuarios/usuarios.h"
//#include "./modulo_emprestimos/emprestimos.h"
#include "./modulo_livros/livros.h"
#else
#define CLEARCMD "cls"
#include "modulo_usuarios\usuarios.h"
//#include "modulo_emprestimos\emprestimos.h"
#include "modulo_livros\livros.h"
#endif

#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int main()
{
	int menu0, menu1;
	time_t tempo;
	struct tm agora;
	while(1)
	{
		time(&tempo);
		agora = *localtime(&tempo);
		system(CLEARCMD);
		printf("Sistema da biblioteca municipal de Nárnia (SBMN)\n");
		printf("Data: %d/%d/%d\n", agora.tm_mday,
					   agora.tm_mon+1,
					   agora.tm_year+1900);
		printf( "\n0)Sair" 
			"\n1)Cadastro de Usuários"
			"\n2)Cadastro de Obras"
			"\n3)Empréstimos e Devoluções"
			"\n4)Relatórios");
		printf("\n\n>>> ");
		scanf("%d",&menu0);
		getchar();

		system(CLEARCMD);
		switch(menu0)
		{
			case 1:
				printf("Cadasto de usuários:\n");
				printf( "\n0)Voltar" 
					"\n1)Cadastrar novo usuário"
					"\n2)Atualizar cadastro"
					"\n3)Consultar usuário (por matrícula)"
					"\n4)Apagar usuário (fisicamente)");
				printf("\n\n>>> ");
				scanf("%d",&menu1);
				getchar();
				system(CLEARCMD);
				switch(menu1)
				{
					case 1:
						if(!cadastrar_usuario())
							printf("\nUsuário cadastrado com sucesso!\n");
						else
							printf("\nErro no cadastro do usuário!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						while(getchar()!='\n');
						break;
					case 2:
						if(!atualizar_cadastro())
							printf("\nCadastro atualizado com sucesso!\n");
						else
							printf("\nErro na atualização do cadastro!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						while(getchar()!='\n');
						break;
					case 3:
						if(!consultar_usuario())
							printf("\nCadastro consultado com sucesso!\n");
						else
							printf("\nErro na consulta do cadastro!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						//getchar();
						while(getchar()!='\n');
						break;
					case 4:
						if(!apagar_usuario())
							printf("\nUsuário deletado com sucesso!\n");
						else
							printf("\nErro ao deletar o usuário!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						//getchar();
						while(getchar()!='\n');
						break;
					default:
						break;

				}
				break;
			case 2:
				printf("Cadasto de Obras:\n");
				printf( "\n0)Voltar" 
					"\n1)Cadastrar nova obra"
					"\n2)Alterar status"
					"\n3)Consultar obra (por título)"
					"\n4)Apagar obra (fisicamente)");
				printf("\n\n>>> ");
				scanf("%d",&menu1);
				getchar();
				system(CLEARCMD);
				switch(menu1)
				{
					case 1:
						if(!cadastrar_livro())
							printf("\nLivro cadastrado com sucesso!\n");
						else
							printf("\nErro no cadastro do livro!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						//getchar();
						while(getchar()!='\n');
						break;
					case 2:
						if(!alterar_status_livro())
							printf("\nStatus alterado com sucesso!\n");
						else
							printf("\nErro na alteração do status!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						//getchar();
						while(getchar()!='\n');
						break;
					case 3:
						if(!consultar_livro())
							printf("\nLivro consultado com sucesso!\n");
						else
							printf("\nErro na consulta do livro!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						//getchar();
						while(getchar()!='\n');
						break;
					case 4:
						if(!apagar_livro())
							printf("\nLivro deletado com sucesso!\n");
						else
							printf("\nErro ao deletar o livro!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						//getchar();
						while(getchar()!='\n');
						break;
					default:
						break;
				}
				break;
			/*
			case 3:
				printf("Empréstimos e Devoluções:");
				printf( "\n0)Voltar" 
					"\n1)Emprestar"
					"\n2)Devolver"
					"\n3)Relatório de Empréstimos");
				printf("\n\n>>> ");
				scanf("%d",&menu1);
				getchar();
				system(CLEARCMD);
				switch(menu1)
				{
					case 1:
						if(!emprestar())
							printf("\nLivro emprestado com sucesso!\n");
						else
							printf("\nErro no empréstimo do livro!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						getchar();
						while(getchar()!='\n');
						break;
					case 2:
						if(!devolver())
							printf("\nLivro devolvido com sucesso!\n");
						else
							printf("\nErro na devolução do livro!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						getchar();
						while(getchar()!='\n');
						break;
					default:
						break;
				}
				break;
				*/
			case 4:
				printf("Relatórios:\n");
				printf( "\n0)Voltar" 
					"\n1)Relatórios de usuários"
					"\n2)Relatórios de livros"
					"\n3)Relatórios de empréstimos");
				printf("\n\n>>> ");
				scanf("%d",&menu1);
				getchar();
				system(CLEARCMD);
				switch(menu1)
				{
					case 1:
						if(!relatorio_usuario())
							printf("\nRelatório impresso com sucesso!\n");
						else
							printf("\nErro ao imprimir relatório!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						//getchar();
						while(getchar()!='\n');
						break;
					case 2:
						if(!relatorio_livro())
							printf("\nRelatório impresso com sucesso!\n");
						else
							printf("\nErro ao imprimir relatório!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						//getchar();
						while(getchar()!='\n');
						break;
					/*
					case 3:
						if(!relatorio_emprestimo())
							printf("\nRelatório impresso com sucesso!\n");
						else
							printf("\nErro ao imprimir relatório!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						getchar();
						while(getchar()!='\n');
						break;
					*/
					default:
						break;
				}
				break;
			default:
				return 0;
		}
	}
	return 0;
}
