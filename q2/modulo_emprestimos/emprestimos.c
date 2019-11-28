#include "emprestimos.h"

int cmp_empr_matricula(const void *a, const void *b)
{
	Emprestimo aux0 = *((Emprestimo *)a), aux1 = *((Emprestimo *)b);
	
	if(aux0.matricula > aux1.matricula)
		return 1;
	else if(aux0.matricula == aux1.matricula)
		return 0;
	else
		return -1;
}

int cmp_empr_matricula_dev(const void *a, const void *b)
{
	Emprestimo aux0 = *((Emprestimo *)a), aux1 = *((Emprestimo *)b);
	
	if(aux0.matricula > aux1.matricula || 
	   (aux0.matricula == aux1.matricula && difftime(mktime(&aux0.devolucao), mktime(&aux1.devolucao))>0))
		return 1;
	else
		return 0;
}

int cmp_empr_codigo(const void *a, const void *b)
{
	Emprestimo aux0 = *((Emprestimo *)a), aux1 = *((Emprestimo *)b);
	
	if(aux0.codigo > aux1.codigo)
		return 1;
	else if(aux0.codigo == aux1.codigo)
		return 0;
	else
		return -1;
}

int cmp_empr_titulo(const void *a, const void *b)
{
	Emprestimo aux0 = *((Emprestimo *)a), aux1 = *((Emprestimo *)b);
	
	return(strcmp(aux0.titulo, aux1.titulo));
}


void mudar_status_usuario(FILE *fp, int matricula, struct tm hoje)
{
	Usuario aux;
	int pos;
	/*Converte a data atual para a data EPOCH*/
	time_t suspen = mktime(&hoje);
	size_t tam_user, num_user;
	
	fseek(fp, 0, SEEK_END);
	tam_user = sizeof(Usuario);
	num_user = ftell(fp)/tam_user;
	
	/*Procura o usuário*/
	aux.matricula = matricula;
	if( (pos = search_file(fp, num_user, tam_user, &aux, cmp_matricula, 1, &aux))<0 )
	{
		printf("\nErro: matrícula inexistente na base de dados!\n");
		fclose(fp);
		return;
	}
	
	/*Muda o status*/
	if(aux.status == SUSP)
		aux.status = NORM;
	else
	{
		aux.status = SUSP;
		/*Adicina 30 dias em segundos para a data de hoje*/
		suspen += (EMPR_SUSP);
		aux.term_susp = *localtime(&suspen);
	}
	
	/*Grava as modificações no arquivo*/
	fseek(fp, pos*tam_user, SEEK_SET);
	fwrite(&aux, tam_user, 1, fp);
	fclose(fp);
	
	return;
}

void mudar_status_livro(int codigo)
{
	int pos;
	FILE *fp;
	Livro aux;
	size_t num_livro, tam_livro;
	
	/*tenta abrir o arquivo de livros*/
	if( !(fp = fopen(LIVRO_FILENAME, "rb+")) )
	{
		printf("\nErro: Nenhum livor cadastrado!\n");
		return;
	}
	fseek(fp, 0, SEEK_END);
	tam_livro = sizeof(Livro);
	num_livro = ftell(fp)/tam_livro;

	/*Procura o livro*/
	aux.codigo = codigo;
	if( (pos = search_file(fp, num_livro, tam_livro, &aux, cmp_codigo, 1, &aux))<0 )
	{
		printf("\nErro: código inexistente na base de dados.\n");
		fclose(fp);
		return;
	}

	/*Muda o status*/
	if(aux.status == DISP)
		aux.status = EMPR;
	else if(aux.status == EMPR)
		aux.status = DISP;

	/*Grava as modificações no arquivo*/
	fseek(fp, pos*tam_livro, SEEK_SET);
	fwrite(&aux, tam_livro, 1, fp);
	fclose(fp);

	return;
}


int emprestar()
{
	Usuario usuario;
	Livro livro;
	Emprestimo emp[4], new_emprest, aux_emprestimo;
	int pos_usuario, pos_livro, i, mes, ano;
	time_t agr, aux=0;
	struct tm hoje;
	FILE *fp_empr, *fp_livro, *fp_usuario;
	size_t tam_user, tam_livro, tam_empr, num_user, num_livro, num_empr;
	
	time(&agr);
	hoje = *localtime(&agr);

	/*Tenta abrir os arquivos*/
	if( !(fp_usuario = fopen(USER_FILENAME, "rb+")) ) 
	{
		printf("\nErro: Nenhum usuário cadastrado!\n");
		return 1;
	}
	if( !(fp_livro = fopen(LIVRO_FILENAME, "rb+")) )
	{
		printf("\nErro: Nenhum livro cadastrado!\n");
		return 1;
	}
	
	if( !(fp_empr = fopen(EMPR_FILENAME, "rb+")) )
		if( !(fp_empr = fopen(EMPR_FILENAME, "wb+")) )
		{
			printf("\nErro ao criar a base de dados de empréstimos!\n");
			return 1;
		}
	
	/*Calcula os tamanhos e quantidades de structs,
	 * para uso nas funções de busca*/
	tam_user = sizeof(Usuario);
	tam_livro = sizeof(Livro);
	tam_empr = sizeof(Emprestimo);

	fseek(fp_usuario, 0, SEEK_END);
	fseek(fp_livro, 0, SEEK_END);
	fseek(fp_empr, 0 ,SEEK_END);
	
	num_user = ftell(fp_usuario)/tam_user;
	num_livro = ftell(fp_livro)/tam_livro;
	num_empr = ftell(fp_empr)/tam_empr;


	printf("digite a matrícula: ");
	scanf("%d", &usuario.matricula);
	getchar();

	/*busca e retorna o usuário*/
	if( (pos_usuario = search_file(fp_usuario, num_user, tam_user, &usuario,
					cmp_matricula,
					1,
					&usuario))<0 )
	{
		printf("\nEmpréstimo não pode ser realizado: Usuário inexistente!\n");

		fclose(fp_usuario);
		fclose(fp_livro);
		fclose(fp_empr);
		
		return 1;
	}
	
	/*O usuário está suspenso e a data de suspensão não chegou*/
	if(usuario.status == SUSP && 
	   difftime(agr, mktime(&usuario.term_susp))<0)
	{
		printf("\nEmpréstimo não pode ser realizado: Usuário suspenso até %02d/%02d/%02d.\n",
				usuario.term_susp.tm_mday,
				usuario.term_susp.tm_mon+1,
				usuario.term_susp.tm_year+1900);

		fclose(fp_usuario);
		fclose(fp_livro);
		fclose(fp_empr);
		
		return 1;
	}
	/*O usuário está suspenso e a data de suspensão já passou*/
	else if(usuario.status == SUSP && 
	   difftime(agr, mktime(&usuario.term_susp))>=0)
	{
		mudar_status_usuario(fp_usuario, usuario.matricula, hoje);
	}

	/*checa a qntd de empréstimos do usuário*/
	if(usuario.num_emprest>=4)
	{
		printf("\nEmpréstimo não pode ser realizado: Usuário possui 4 volumes em seu poder.\n");
		fclose(fp_usuario);
		fclose(fp_livro);
		fclose(fp_empr);
		return 1;
	}
	
	/*O usuário existe e não está suspenso, checa se algum de seus
	 * empréstimos está em atraso*/
	aux_emprestimo.matricula = usuario.matricula;
	if( search_file(fp_empr, num_empr, tam_empr,  &aux_emprestimo,
				cmp_empr_matricula, usuario.num_emprest, &emp[0]) >= 0)
	{
		for(i=usuario.num_emprest-1; i>=0; i--)
		{
			/*O livro atrasado mais recente*/
			if(difftime(agr, mktime(&emp[i].devolucao))>0)
			{
				mudar_status_usuario(fp_usuario, usuario.matricula, emp[i].devolucao);
				printf("\nEmpréstimo não pode ser realizado: Volume em atraso encontrado.\n");
				return 1;
			}
		}
	}
	
	printf("digite o código: ");
	scanf("%d", &livro.codigo);
	getchar();
	
	/*Procura o livro e testa se ele está disponível*/
	if( (pos_livro = search_file(fp_livro, num_livro, tam_livro, &livro,
					cmp_codigo, 1, &livro))<0)
	{
		printf("\nEmpréstimo não pode ser realizado: Volume inexistente.\n");

		fclose(fp_usuario);
		fclose(fp_livro);
		fclose(fp_empr);
		
		return 1;
	}
	else if(livro.status == MANU || livro.status == EMPR)
	{
		printf("\nEmpréstimo não pode ser realizado: Volume indisponível no momento.\n");
		
		fclose(fp_usuario);
		fclose(fp_livro);
		fclose(fp_empr);
		
		return 1;
	}
	
	new_emprest.matricula = usuario.matricula; 
	new_emprest.fone_red = usuario.fone_red; 
	new_emprest.codigo = livro.codigo;
	strcpy(new_emprest.titulo, livro.titulo);
	strcpy(new_emprest.nome, usuario.nome);
	new_emprest.emprestimo = hoje;
	printf("digite a data de devolução(DD/MM/AA): ");
	
	new_emprest.devolucao = *(localtime(&aux));
	
	scanf("%d/%d/%d", &new_emprest.devolucao.tm_mday, &mes, &ano);
	getchar();
	
	
	new_emprest.devolucao.tm_mon = mes-1;
	new_emprest.devolucao.tm_year = ano-1900;
	new_emprest.devolucao.tm_sec = 59;
	new_emprest.devolucao.tm_min = 59;
	new_emprest.devolucao.tm_hour = 23;

	fseek(fp_empr, 0, SEEK_END);
	fwrite(&new_emprest, tam_empr, 1, fp_empr);
	
	/*Atualiza a situação do livro e do usuário*/
	usuario.num_emprest++;
	fseek(fp_usuario, pos_usuario*tam_user, SEEK_SET);
	fwrite(&usuario, tam_user, 1, fp_usuario);

	livro.status = EMPR;
	fseek(fp_livro, pos_livro*tam_livro, SEEK_SET);
	fwrite(&livro, tam_livro, 1, fp_livro);
	
	/*Ordena o arquivo*/
	sort_file(fp_empr, (num_empr+1), tam_empr, cmp_empr_matricula_dev);
	
	fclose(fp_empr);
	fclose(fp_usuario);
	fclose(fp_livro);

	return 0;
}

int devolver()
{
	Usuario usuario;
	Livro livro;
	Emprestimo empr;
	int pos_usuario, pos_livro;
	time_t agr, aux;
	//struct tm hoje;
	FILE *fp_empr, *fp_livro, *fp_usuario, *fp_tmp;
	size_t tam_user, tam_livro, tam_empr, num_user, num_livro, num_empr;
	
	/*Tenta abrir os arquivos*/
	if( !(fp_usuario = fopen(USER_FILENAME, "rb+")) ) 
	{
		printf("\nErro: Nenhum usuário cadastrado!\n");
		return 1;
	}
	if( !(fp_livro = fopen(LIVRO_FILENAME, "rb+")) )
	{
		printf("\nErro: Nenhum livro cadastrado!\n");
		return 1;
	}
	
	if( !(fp_empr = fopen(EMPR_FILENAME, "rb+")) )
	{
		printf("\nErro: Nenhum empréstimo cadastrado!\n");
		return 1;
	}
	if( !(fp_tmp = fopen("emprestimo_bk.dat", "wb+")))
		return 1;
	
	/*Calcula os tamanhos e quantidades de structs,
	 * para uso nas funções de busca*/
	tam_user = sizeof(Usuario);
	tam_livro = sizeof(Livro);
	tam_empr = sizeof(Emprestimo);

	fseek(fp_usuario, 0, SEEK_END);
	fseek(fp_livro, 0, SEEK_END);
	fseek(fp_empr, 0 ,SEEK_END);
	
	num_user = ftell(fp_usuario)/tam_user;
	num_livro = ftell(fp_livro)/tam_livro;
	num_empr = ftell(fp_empr)/tam_empr;
	
	printf("digite o código: ");
	scanf("%d", &livro.codigo);
	getchar();

	if( (pos_livro = search_file(fp_livro, num_livro, tam_livro, &livro,
					cmp_codigo, 1, &livro))<0 )
	{
		printf("\nErro: Código inválido.\n");
		return 1;
	}
	
	empr.codigo = livro.codigo;
	if( (search_file(fp_empr, num_empr, tam_empr, &empr,
					cmp_empr_codigo, 1, &empr))<0 )
	{
		printf("\nErro: volume não emprestado.\n");
		return 1;
	}

	usuario.matricula = empr.matricula;
	if( (pos_usuario = search_file(fp_usuario, num_user, tam_user, &usuario,
					cmp_matricula, 1, &usuario))<0 )
		return 1;

	/*Atualiza a situação do livro e do usuário*/
	livro.status = DISP;
	usuario.num_emprest--;
	
	/*Se o usuário atrasou a devolução*/
	time(&agr);
	if(usuario.status == NORM && difftime(agr, mktime(&empr.devolucao))>0)
	{
		usuario.status = SUSP;
		aux = agr + (EMPR_SUSP);
		usuario.term_susp = *localtime(&aux);
		printf("Volume entrege com atraso, usuário suspenso até: %02d/%02d/%04d!\n",
				usuario.term_susp.tm_mday,
				usuario.term_susp.tm_mon+1,
				usuario.term_susp.tm_year+1900);
	}
	
	
	/*Grava as alterações e apaga o empréstimo*/
	fseek(fp_usuario, pos_usuario*tam_user, SEEK_SET);
	fwrite(&usuario, tam_user, 1, fp_usuario);

	fseek(fp_livro, pos_livro*tam_livro, SEEK_SET);
	fwrite(&livro, tam_livro, 1, fp_livro);
	
	while(!feof(fp_empr))
	{
		fread(&empr, tam_empr, 1, fp_empr);
		if(!feof(fp_empr) && empr.codigo != livro.codigo)
			fwrite(&empr, tam_empr, 1, fp_tmp);

	}
	
	fclose(fp_usuario);
	fclose(fp_tmp);
	fclose(fp_livro);
	fclose(fp_empr);
	rename("emprestimo_bk.dat", EMPR_FILENAME);

	return 0;
}

int relatorio_emprestimo()
{
	/*variável que armazenará cada empréstimo na busca*/
	Emprestimo aux;
	char filename[51], filepath[51];
	FILE *fp,*stream, *fp_tmp ;
	time_t agr;
	struct tm tempo;
	int menu, i;
	size_t num_empr, tam_empr;
	
	/*Tenta abrir o arquivo*/
	if( !(fp = fopen(EMPR_FILENAME, "rb") ) )
	{
		printf("\nErro: nenhum empréstimo consta na base de dados!\n");
		return 1;
	}
	fseek(fp, 0, SEEK_END);	
	tam_empr = sizeof(Emprestimo);
	num_empr = ftell(fp) / tam_empr;
	
	/*Cria um arquivo temporário*/
	if( !(fp_tmp = tmpfile()) )
		return 1;

	/*Copia todas as structs do arquivo original para o temporário*/
	fseek(fp, 0 , SEEK_SET);
	while(!feof(fp) && fread(&aux, tam_empr, 1, fp))
		fwrite(&aux, tam_empr, 1, fp_tmp);
	
	/*Ordena o arquivo temporário por título*/
	fclose(fp);
	sort_file(fp_tmp, num_empr, tam_empr, cmp_empr_titulo);	

	/*Printa o relatório ordenado por título*/
	time(&agr);
	tempo = *localtime(&agr);
	printf("1)Tela\n2)Arquivo\n\n>>> ");
	scanf("%d", &menu);
	getchar();

	if(menu==1)
	{
		stream = stdout;
	}
	else if(menu == 2)
	{
		sprintf(filename, "Relatorio_emprestimos_%02d-%02d-%02d@%02d:%02d.txt", 
				tempo.tm_mday,
				tempo.tm_mon+1, 
				tempo.tm_year+1900,
				tempo.tm_hour,
				tempo.tm_min);

		strcat(filepath, PATH_REL);
		strcat(filepath, filename);

		if( !(stream = fopen(filepath, "w") ) )
		{
			fclose(fp);
			fclose(fp_tmp);
			return 1;
		}
	}
	else
	{
		fclose(fp);
		fclose(fp_tmp);
		return 1;
	}

	fseek(fp_tmp, 0, SEEK_SET);		
	while(!feof(fp_tmp))
	{
		fprintf(stream, "Relatório de Livros Emprestados\t\t\t   Data: %02d/%02d/%02d @ %02d:%02d\n\n",
				tempo.tm_mday,
				tempo.tm_mon+1, 
				tempo.tm_year+1900,
				tempo.tm_hour,
				tempo.tm_min);
		
		fprintf(stream, " %-19s | %-6s | %-19s | %-9s | %-9s\n",
				"Título",
				"Código",
				"Usuário",
				"Devolução ",
				"Telefone");
		for(i=0; i<75; i++) fputc('-', stream);
		fprintf(stream, "\n");

		for(i=0; i<45 && !(feof(fp_tmp)); i++)
		{
			fread(&aux, tam_empr, 1, fp_tmp);
			if(!feof(fp_tmp))
			{
				fprintf(stream, " %-19.19s | %-6.7d | %-19.19s | %02d/%02d/%04d | %-8.9d\n",
						aux.titulo,
						aux.codigo,
						aux.nome,
						aux.devolucao.tm_mday,
						aux.devolucao.tm_mon+1,
						aux.devolucao.tm_year+1900,
						aux.fone_red);
			}
		}
		for(i=0; i<75; i++) fputc('-', stream);
		fprintf(stream, "\n\f");
	}
	fclose(fp_tmp);
	if(menu == 2) fclose(stream);
	return 0;
}
