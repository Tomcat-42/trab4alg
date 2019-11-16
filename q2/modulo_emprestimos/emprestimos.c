#include "emprestimos.h"
		
void sort_emprestimo()
{
	Emprestimo aux0, aux1;
	FILE *fp;
	int i, tam;

	/*Tenta abrir o arquivo*/
	if( !(fp = fopen(EMPR_FILENAME, "rb+")) ) return;
	
	/*Calcula o número de strucs do arquivo*/
	fseek(fp, 0, SEEK_END);
	tam = (int)ftell(fp)/(int)sizeof(Emprestimo);

	/*Move o ponteiro para a penúltima struct no arquivo do arquivo*/
	fseek(fp, -(2*sizeof(Usuario)), SEEK_END);
	for(i=1; i<tam; i++)
	{
		fread(&aux0, sizeof(Emprestimo), 1, fp);
		fread(&aux1, sizeof(Emprestimo), 1, fp);

		if(aux0.matricula > aux1.matricula)
		{
			fseek(fp, -(2*sizeof(Emprestimo)), SEEK_CUR);
			fwrite(&aux1, sizeof(Emprestimo), 1, fp);
			fwrite(&aux0, sizeof(Emprestimo), 1, fp);
			fseek(fp, -((2*sizeof(Emprestimo)) + (i*sizeof(Emprestimo))), SEEK_END);
		}
		else if(aux0.matricula == aux1.matricula && difftime(mktime(&aux0.emprestimo), mktime(&aux1.emprestimo))>0)
		{
			fseek(fp, -(2*sizeof(Emprestimo)), SEEK_CUR);
			fwrite(&aux1, sizeof(Emprestimo), 1, fp);
			fwrite(&aux0, sizeof(Emprestimo), 1, fp);
			fseek(fp, -((2*sizeof(Emprestimo)) + (i*sizeof(Emprestimo))), SEEK_END);
		}
		else/*A última struct está no local correto*/
		{
			fclose(fp);
			return;
		}
	}
	fclose(fp);
	return;
}

void mudar_status_usuario(int matricula, struct tm hoje)
{
	Usuario aux;
	int pos;
	FILE *fp;
	/*Converte a data atual para a data EPOCH*/
	time_t suspen = mktime(&hoje);
	
	/*Procura o usuário*/
	if( (pos = busca_generica_usuario(matricula, &aux))<0 )
		return;
	
	/*Tenta abrir o arquivo de usuarios*/
	if( !(fp = fopen(USER_FILENAME, "rb+")) )
		return;

	/*Muda o status*/
	if(aux.status == SUSP)
		aux.status = NORM;
	else
	{
		aux.status = SUSP;
		/*Adicina 30 dias em segundos para a data de hoje*/
		suspen += (30*24*60*60);
		aux.term_susp = *localtime(&suspen);
	}
	
	/*Grava as modificações no arquivo*/
	fseek(fp, pos*sizeof(Usuario), SEEK_SET);
	fwrite(&aux, sizeof(Usuario), 1, fp);
	fclose(fp);
	
	return;
}

void mudar_status_livro(int codigo)
{
	int pos;
	FILE *fp;
	Livro aux;

	/*Procura o livro*/
	if( (pos = busca_generica_livro(codigo, &aux))<0 )
		return;

	/*tenta abrir o livro*/
	if( !(fp = fopen(LIVRO_FILENAME, "rb+")) )
		return;

	/*Muda o status*/
	if(aux.status == DISP)
		aux.status = EMPR;
	else if(aux.status == EMPR)
		aux.status = DISP;

	/*Grava as modificações no arquivo*/
	fseek(fp, pos*sizeof(Livro), SEEK_SET);
	fwrite(&aux, sizeof(Livro), 1, fp);
	fclose(fp);

	return;
}

int busca_emprestimo(int matricula, Emprestimo *emp, int n)
{
	Emprestimo aux_busca;
	FILE *fp;
	int i=0, j;

	if( !(fp = fopen(EMPR_FILENAME, "rb")) )
		return -1;

	while( !feof(fp) )
	{
		fread(&aux_busca, sizeof(Emprestimo), 1, fp);
		if(!feof(fp) && (matricula == aux_busca.matricula))
		{
			for(j=0; j<n; j++)
			{
				*(emp+j) = aux_busca;
				fread(&aux_busca, sizeof(Emprestimo), 1, fp);
			}
			fclose(fp);
			return i;
		}
		else
			i++;
	}
	fclose(fp);
	return -1;
}

int busca_emprestimo_codigo(int codigo, Emprestimo *emp)
{	
	Emprestimo aux_busca;
	FILE *fp;
	int i=0;

	if( !(fp = fopen(EMPR_FILENAME, "rb")) )
		return -1;

	while( !feof(fp) )
	{
		fread(&aux_busca, sizeof(Emprestimo), 1, fp);
		if(!feof(fp) && (codigo == aux_busca.codigo))
		{
			*(emp) = aux_busca;
			return i;
		}
		else
			i++;
	}
	fclose(fp);
	return -1;
}

int emprestar()
{
	Usuario usuario;
	Livro livro;
	Emprestimo emp[4], new_emprest;
	int matricula, codigo, pos_usuario, pos_livro, i, mes, ano;
	time_t agr;
	struct tm hoje;
	FILE *fp_empr, *fp_livro, *fp_usuario;

	time(&agr);
	hoje = *localtime(&agr);
	
	printf("digite a matrícula: ");
	scanf("%d", &matricula);
	getchar();

	/*busca e retorna o usuário*/
	if( (pos_usuario = busca_generica_usuario(matricula, &usuario))<0 )
	{
		printf("Empréstimo não pode ser realizado: Usuário inexistente!\n");
		return 1;
	}
	
	/*O usuário está suspenso e a data de suspensão não chegou*/
	if(usuario.status == SUSP && 
	   difftime(mktime(&usuario.term_susp), agr)>=0)
	{
		printf("Empréstimo não pode ser realizado: Usuário suspenso até %02d/%02d/%02d.\n",
				usuario.term_susp.tm_mday,
				usuario.term_susp.tm_mon+1,
				usuario.term_susp.tm_year+1900);
		return 1;
	}
	/*O usuário está suspenso e a data de suspensão já passou*/
	else if(usuario.status == SUSP && 
	   difftime(mktime(&usuario.term_susp), agr)<0)
	{
		mudar_status_usuario(matricula, hoje);
	}

	/*Busca e retorna os empréstimos do usuário*/
	if(usuario.num_emprest<4)
		busca_emprestimo(matricula, &emp[0], usuario.num_emprest);
	else
	{
		printf("Empréstimo não pode ser realizado: Usuário possui 4 volumes em seu poder.\n");
		return 1;
	}

	/*O usuário existe e não está suspenso, checa se algum de seus
	 * empréstimos está em atraso*/
	for(i=usuario.num_emprest; i>0; i--)
	{
		/*O livro atrasado mais recente*/
		if(difftime(agr, mktime(&emp[i].devolucao))<0)
		{
			mudar_status_usuario(matricula, emp[i].devolucao);
			printf("Empréstimo não pode ser realizado: Usuário suspenso até %02d/%02d/%02d.\n",
				usuario.term_susp.tm_mday,
				usuario.term_susp.tm_mon+1,
				usuario.term_susp.tm_year+1970);
			return 1;
		}
	}
	
	printf("digite o código: ");
	scanf("%d", &codigo);
	getchar();
	
	/*Procura o livro e testa se ele está disponível*/
	if( (pos_livro = busca_generica_livro(codigo, &livro))<0)
	{
		printf("Empréstimo não pode ser realizado: Volume inexistente.\n");
		return 1;
	}
	else if(livro.status == MANU || livro.status == EMPR)
	{
		printf("Empréstimo não pode ser realizado: Volume indisponível no momento.\n");
		return 1;
	}
	
	/*Realiza o empréstimo*/
	if( !(fp_empr = fopen(EMPR_FILENAME, "ab+")) || 
	    !(fp_usuario = fopen(USER_FILENAME, "rb+")) || 
	    !(fp_livro = fopen(LIVRO_FILENAME, "rb+")))
	{
		return 1;
	}

	new_emprest.matricula = matricula; 
	new_emprest.fone_red = usuario.fone_red; 
	new_emprest.codigo = codigo;
	strcpy(new_emprest.titulo, livro.titulo);
	strcpy(new_emprest.nome, usuario.nome);
	new_emprest.emprestimo = hoje;
	printf("digite a data de devolução(DD/MM/AA): ");
	scanf("%d/%d/%d", &new_emprest.devolucao.tm_mday, &mes, &ano);
	getchar();
	new_emprest.devolucao.tm_mon = mes-1;
	new_emprest.devolucao.tm_year = ano-1900;
	new_emprest.devolucao.tm_sec = 0;
	new_emprest.devolucao.tm_min = 0;
	new_emprest.devolucao.tm_hour = 0;

	fwrite(&new_emprest, sizeof(Emprestimo), 1, fp_empr);
	
	/*Atualiza a situação do livro e do usuário*/
	usuario.num_emprest++;
	fseek(fp_usuario, pos_usuario*sizeof(Usuario), SEEK_SET);
	fwrite(&usuario, sizeof(Usuario), 1, fp_usuario);

	livro.status = EMPR;
	fseek(fp_livro, pos_livro*sizeof(Livro), SEEK_SET);
	fwrite(&livro, sizeof(Livro), 1, fp_livro);
		
	fclose(fp_empr);
	fclose(fp_usuario);
	fclose(fp_livro);
	sort_emprestimo();

	return 0;
}

int devolver()
{
	Usuario usuario;
	Livro livro;
	Emprestimo empr;
	int codigo, pos_usuario, pos_livro;
	time_t agr, aux;
	//struct tm hoje;
	FILE *fp_empr, *fp_livro, *fp_usuario, *fp_tmp;
	
	printf("digite o código: ");
	scanf("%d", &codigo);
	getchar();

	if( (pos_livro = busca_generica_livro(codigo, &livro))<0 )
	{
		printf("Erro: Código inválido.\n");
		return 1;
	}
	if( (busca_emprestimo_codigo(codigo, &empr))<0 )
		return 1;

	if( (pos_usuario = busca_generica_usuario(empr.matricula, &usuario))<0 )
		return 1;

	/*Atualiza a situação do livro e do usuário*/
	livro.status = DISP;
	usuario.num_emprest--;
	
	/*Se o usuário atrasou a devolução*/
	time(&agr);
	if(difftime(agr, mktime(&empr.devolucao))<=0)
	{
		usuario.status = SUSP;
		aux = agr + (30*24*60*60);
		usuario.term_susp = *localtime(&aux);
	}
	
	if( !(fp_empr = fopen(EMPR_FILENAME, "rb+")) || 
	    !(fp_usuario = fopen(USER_FILENAME, "rb+")) || 
	    !(fp_livro = fopen(LIVRO_FILENAME, "rb+")) ||
	    !(fp_tmp = fopen("emprestimo_bk.dat", "wb+")))
	{
		return 1;
	}
	printf("aaaaaaaaaaaaaaaa\n");
	
	/*Grava as alterações e apaga o empréstimo*/
	fseek(fp_usuario, pos_usuario*sizeof(Usuario), SEEK_SET);
	fwrite(&usuario, sizeof(Usuario), 1, fp_usuario);

	fseek(fp_livro, pos_livro*sizeof(Livro), SEEK_SET);
	fwrite(&livro, sizeof(Livro), 1, fp_livro);
	
	while(!feof(fp_empr))
	{
		fread(&empr, sizeof(Emprestimo), 1, fp_empr);
		if(!feof(fp_empr) && empr.codigo != codigo)
			fwrite(&empr, sizeof(Emprestimo), 1, fp_tmp);

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
	FILE *fp,*stream;
	time_t agr;
	struct tm tempo;
	int menu, i;
	
	/*Tenta abrir o arquivo*/
	if( !(fp = fopen(EMPR_FILENAME, "rb") ) ) return 1;
	
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
			return 1;
		}
	}
	else
	{
		fclose(fp);
		return 1;
	}

	
	while(!feof(fp))
	{
		fprintf(stream, "Relatório de Livros Emprestados\t\t\t      Data: %02d/%02d/%02d @ %02d:%02d\n\n",
				tempo.tm_mday,
				tempo.tm_mon+1, 
				tempo.tm_year+1900,
				tempo.tm_hour,
				tempo.tm_min);
		
		fprintf(stream, " %-20s | %-7s | %-20s | %-10s | %-10s\n",
				"Título",
				"Código",
				"Usuário",
				"Devolução ",
				"Telefone");
		for(i=0; i<78; i++) fputc('-', stream);
		fprintf(stream, "\n");

		for(i=0; i<45 && !(feof(fp)); i++)
		{
			fread(&aux, sizeof(Emprestimo), 1, fp);
			if(!feof(fp))
			{
				fprintf(stream, " %-20s| %-7d| %-20s| %02d/%02d/%04d | %-8d\n",
						aux.titulo,
						aux.codigo,
						aux.nome,
						aux.devolucao.tm_mday,
						aux.devolucao.tm_mon+1,
						aux.devolucao.tm_year+1900,
						aux.fone_red);
			}
		}
		for(i=0; i<78; i++) fputc('-', stream);
		fprintf(stream, "\n\f");
	}
	fclose(fp);
	return 0;
}
