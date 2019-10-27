#ifdef __unix__
#define CLEARCMD "clear"
#include "./modulo_usuarios/usuarios.h"
//#include "./modulo_emprestimos/emprestimos.h"
//#include "./modulo_livros/livros.h"
#elif
#define CLEARCMD "cls"
#include "modulo_usuarios\usuarios.h"
//#include "modulo_emprestimos\emprestimos.h"
//#include "modulo_livros\livros.h"
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
				system(CLEARCMD);
				switch(menu1)
				{
					case 1:
						if(!cadastrar_usuario())
							printf("Usuário cadastrado com sucesso!\n");
						else
							printf("Erro no cadastro do usuário!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						while(getchar()!='\n');
						break;
					case 2:
						if(!atualizar_cadastro())
							printf("Cadastro atualizado com sucesso!\n");
						else
							printf("Erro na atualização do cadastro!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						getchar();
						while(getchar()!='\n');
						break;
					case 3:
						if(!consultar_usuario())
							printf("Cadastro consultado com sucesso!\n");
						else
							printf("Erro na consulta do cadastro!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						getchar();
						while(getchar()!='\n');
						break;
					case 4:
						if(!apagar_usuario())
							printf("Usuário deletado com sucesso!\n");
						else
							printf("Erro ao deletar o usuário!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						getchar();
						while(getchar()!='\n');
						break;
					default:
						break;

				}
				break;
			/*
			case 2:
				printf("Cadasto de Obras:");
				printf( "\n0)Voltar" 
					"\n1)Cadastrar nova obra"
					"\n2)Alterar status"
					"\n3)Consultar obra (por título)"
					"\n4)Apagar obra (fisicamente)");
				printf("\n\n>>> ");
				scanf("%d",&menu1);
				system(CLEARCMD);
				switch(menu1)
				{
					case 1:
						if(!cadastrar_livro())
							printf("Livro cadastrado com sucesso!\n");
						else
							printf("Erro no cadastro do livro!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						getchar();
						while(getchar()!='\n');
						break;
					case 2:
						if(!alterar_status())
							printf("Status alterado com sucesso!\n");
						else
							printf("Erro na alteração do status!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						getchar();
						while(getchar()!='\n');
						break;
					case 3:
						if(!consultar_livro())
							printf("Livro consultado com sucesso!\n");
						else
							printf("Erro na consulta do livro!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						getchar();
						while(getchar()!='\n');
						break;
					case 4:
						if(!apagar_livro())
							printf("Livro deletado com sucesso!\n");
						else
							printf("Erro ao deletar o livro!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						getchar();
						while(getchar()!='\n');
						break;
					default:
						break;
				}
				break;
			case 3:
				printf("Empréstimos e Devoluções:");
				printf( "\n0)Voltar" 
					"\n1)Emprestar"
					"\n2)Devolver"
					"\n3)Relatório de Empréstimos");
				printf("\n\n>>> ");
				scanf("%d",&menu1);
				system(CLEARCMD);
				switch(menu1)
				{
					case 1:
						if(!emprestar())
							printf("Livro emprestado com sucesso!\n");
						else
							printf("Erro no empréstimo do livro!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						getchar();
						while(getchar()!='\n');
						break;
					case 2:
						if(!devolver())
							printf("Livro devolvido com sucesso!\n");
						else
							printf("Erro na devolução do livro!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						getchar();
						while(getchar()!='\n');
						break;
					case 3:
						if(!relatorio())
							printf("Relatório impresso com sucesso!\n");
						else
							printf("Erro na impressão do relatório!\n");
						printf("\nPressione [ENTER] para retornar ao menu ");
						getchar();
						while(getchar()!='\n');
						break;
					default:
						break;
				}
				break;
				*/
			default:
				return 0;
		}
	}
	return 0;
}

