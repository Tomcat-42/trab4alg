#include "livros.h"

int cmp_codigo(const void *a, const void *b)
{
	Livro aux0 = *((Livro *)a), aux1 = *((Livro *)b);
	if(aux0.codigo > aux1.codigo)
		return 1;
	else if(aux0.codigo == aux1.codigo)
		return 0;
	else
		return -1;
}

int cmp_titulo(const void *a, const void *b)
{
	Livro aux0 = *((Livro *)a), aux1 = *((Livro *)b);
	return strcmp(aux0.titulo, aux1.titulo);
}

int cadastrar_livro()
{
	Livro new_livro, aux;
	FILE *fp;
	size_t num_livro, tam_livro;
	
	/*Checa por erros ao abrir o arquivo*/
	if( !(fp = fopen(LIVRO_FILENAME, "rb+")))
		if( !(fp = fopen(LIVRO_FILENAME, "wb+")) )
		{
			printf("\nErro ao criar base de dados!\n");
			return 1;
		}
	
	fseek(fp, 0, SEEK_END);
	tam_livro = sizeof(Livro);
	num_livro = ftell(fp)/tam_livro;
	
	printf("digite o código: ");
	scanf("%d", &new_livro.codigo);
	getchar();
	printf("digite o ISBN: ");
	fgets(new_livro.isbn, 14, stdin);
	new_livro.isbn[strlen(new_livro.isbn) -1] = '\0';
	printf("digite o título: ");
	fgets(new_livro.titulo, 82, stdin);
	new_livro.titulo[strlen(new_livro.titulo) -1] = '\0';
	printf("digite os autores: ");
	fgets(new_livro.autores, 101, stdin);
	new_livro.autores[strlen(new_livro.autores) -1] = '\0';
	printf("digite o assunto: ");
	fgets(new_livro.assunto, 82, stdin);
	new_livro.assunto[strlen(new_livro.assunto) -1] = '\0';
	printf("digite a editora: ");
	fgets(new_livro.editora, 52, stdin);
	new_livro.editora[strlen(new_livro.editora) -1] = '\0';
	printf("digite o ano: ");
	scanf("%d", &new_livro.ano);
	getchar();
	printf("digite a edição: ");
	scanf("%d", &new_livro.edicao);
	getchar();
	new_livro.status = DISP;
	
	/*Checa por dados repetidos*/
	if( (search_file(fp, num_livro, tam_livro, &new_livro, cmp_codigo, 1, &aux))>=0 ) 
	{
		printf("\nErro: Algum dado já consta na base de dados!\n");
		fclose(fp);
		return 1;
	}

	/*Escreve no arquivo*/
	fseek(fp, 0, SEEK_END);
	fwrite(&new_livro, tam_livro, 1, fp);

	
	/*Ordena o arquivo*/
	sort_file(fp, ftell(fp)/sizeof(Livro), sizeof(Livro), cmp_codigo);

	fclose(fp);
	
	return 0;
}

int alterar_status_livro()
{
	int pos;
	FILE *fp;
	Livro aux;
	size_t num_livro, tam_livro;
	
	/*Erros ao abrir o arquivo*/
	if( !(fp = fopen(LIVRO_FILENAME, "rb+")) )
	{
		printf("\nErro: Nenhum livor cadastrado!\n");
		return 1;
	}
	fseek(fp, 0, SEEK_END);
	tam_livro = sizeof(Livro);
	num_livro = ftell(fp)/tam_livro;

	printf("Digite o código: ");
	scanf("%d", &aux.codigo);
	getchar();

	/*O código não existe*/
	if( (pos = search_file(fp, num_livro, tam_livro, &aux, cmp_codigo, 1, &aux))<0 )
	{
		printf("\nErro: código não consta na base de dados!.\n");
		return 1;
	}
	
	if(aux.status == DISP)
		aux.status = MANU;
	else if(aux.status == MANU)
		aux.status = DISP;
	else
		return 1;/*O livro está emprestado*/
	

	/*Escreve de volta no arquivo*/
	fseek(fp, pos*tam_livro, SEEK_SET);
	fwrite(&aux, tam_livro, 1, fp);

	fclose(fp);
	return 0;
}

int consultar_livro()
{
	Livro aux;
	FILE *fp;	
	size_t num_livro, tam_livro;
	
	/*Erros ao abrir o arquivo*/
	if( !(fp = fopen(LIVRO_FILENAME, "rb")) )
	{
		printf("\nErro: Nenhum livor cadastrado!\n");
		return 1;
	}
	fseek(fp, 0, SEEK_END);
	tam_livro = sizeof(Livro);
	num_livro = ftell(fp)/tam_livro;

	printf("Digite o título: ");
	fgets(aux.titulo, 81, stdin);
	aux.titulo[strlen(aux.titulo) -1] = '\0';
	printf("%s\n",aux.titulo);

	/*Procura no arquivo por aquela matricula*/
	if( (search_file(fp, num_livro, tam_livro, &aux, cmp_titulo, 1, &aux))<0 )
	{
		printf("\nErro: código não consta na base de dados!.\n");
		return 1;
	}
	
	printf("\ncódigo: %d\n", aux.codigo);
	printf("isbn: %s\n", aux.isbn);
	printf("título: %s\n", aux.titulo);
	printf("autores: %s\n", aux.autores);
	printf("assunto: %s\n", aux.assunto);
	printf("editora: %s\n", aux.editora);
	printf("ano: %d\n", aux.ano);
	printf("edição: %d\n", aux.edicao);
	printf("status: %s\n", (aux.status == DISP) ? ("DISPONÍVEL") : 
				(aux.status == EMPR) ? ("EMPRESTADO") : ("MANUTENÇÃO"));
				
	return 0;
}

int apagar_livro()
{
	FILE *fp_old, *fp_new;
	Livro aux, aux_new;
	size_t num_livro, tam_livro;
	
	if( !(fp_old = fopen(LIVRO_FILENAME, "rb")) || !(fp_new = fopen("livro_bk.dat", "wb") )) 
		return 1;
	
	fseek(fp_old, 0, SEEK_END);
	tam_livro = sizeof(Livro);
	num_livro = ftell(fp_old)/tam_livro;
	
	printf("Digite o código: ");
	scanf("%d", &aux.codigo);
	getchar();

	/*Procura no arquivo por aquele código*/
	fseek(fp_old, 0, SEEK_END);
	if((search_file(fp_old, num_livro, tam_livro, &aux, cmp_codigo, 1, &aux))<0)
	{
		printf("\nErro: código inexistente na base de dados!\n");
		fclose(fp_old);
		fclose(fp_new);
		remove("livro_bk.dat");
		return 1;
	}
	
	/*Se o livro está emprestado*/
	if(aux.status == EMPR)
	{	
		printf("\nerro: livros emprestados não podem ser apagados!\n");
		fclose(fp_old);
		fclose(fp_new);
		remove("livro_bk.dat");
		return 1;
	}


	rewind(fp_old);
	while(!feof(fp_old))
	{
		fread(&aux_new, tam_livro, 1, fp_old);
		if(!feof(fp_old) && aux.codigo != aux_new.codigo)
			fwrite(&aux_new, tam_livro, 1, fp_new);

	}
	fclose(fp_old);
	fclose(fp_new);
	rename("livro_bk.dat", LIVRO_FILENAME);
	
	return 0;
}

int relatorio_livro()
{
	/*variável que armazenará cada livro na busca*/
	Livro aux;
	char filename[51], filepath[51];
	FILE *fp,*stream;
	time_t agr;
	struct tm tempo;
	int menu;
	size_t tam_livro = sizeof(Livro);
	
	/*Tenta abrir o arquivo*/
	if( !(fp = fopen(LIVRO_FILENAME, "rb") ) ) return 1;
	
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
		sprintf(filename, "Relatorio_livros_%02d-%02d-%02d@%02d:%02d.txt", 
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
	
	system("clear");

	fprintf(stream, "Relatório de livros cadastrados:\nData: %02d/%02d/%02d @ %02d:%02d\n\n",
			tempo.tm_mday,
			tempo.tm_mon+1, 
			tempo.tm_year+1900,
			tempo.tm_hour,
			tempo.tm_min);
	
	while(!feof(fp))
	{
		fread(&aux, tam_livro, 1, fp);
		if(!feof(fp))
		{
			fprintf(stream, "\ncódigo: %d\n", aux.codigo);
			fprintf(stream, "isbn: %s\n", aux.isbn);
			fprintf(stream, "título: %s\n", aux.titulo);
			fprintf(stream, "autores: %s\n", aux.autores);
			fprintf(stream, "assunto: %s\n", aux.assunto);
			fprintf(stream, "editora: %s\n", aux.editora);
			fprintf(stream, "ano: %d\n", aux.ano);
			fprintf(stream, "edição: %d\n", aux.edicao);
			fprintf(stream, "status: %s\n", (aux.status == DISP) ? ("DISPONÍVEL") : 
							(aux.status == EMPR) ? ("EMPRESTADO") : ("MANUTENÇÃO"));
		}
		fprintf(stream, "\n");
	}
	fclose(fp);
	if(menu == 2) fclose(stream);
	return 0;
}
