#include "livros.h"

int busca_generica_livro(int codigo, Livro *livro)
{	
	Livro aux_busca;
	FILE *fp;
	int i=0;

	if( !(fp = fopen(LIVRO_FILENAME, "rb")) )
		return -1;

	while( !feof(fp) )
	{
		fread(&aux_busca, sizeof(Livro), 1, fp);
		if(!feof(fp) && (codigo == aux_busca.codigo))
		{
			*(livro) = aux_busca;
			return i;
		}
		else
			i++;
	}
	fclose(fp);
	return -1;
}

int busca_titulo_livro(char *titulo, Livro *livro)
{	
	Livro aux_busca;
	FILE *fp;
	int i=0;

	if( !(fp = fopen(LIVRO_FILENAME, "rb")) )
		return -1;

	while( !feof(fp) )
	{
		fread(&aux_busca, sizeof(Livro), 1, fp);
		if(!feof(fp) && (!strcmp(titulo, aux_busca.titulo)))
		{
			*(livro) = aux_busca;
			fclose(fp);
			return i;
		}
		else
			i++;
	}
	fclose(fp);
	return -1;
}

void sort_livro()
{
	Livro aux0, aux1;
	FILE *fp;
	int i, tam;

	/*Tenta abrir o arquivo*/
	if( !(fp = fopen(LIVRO_FILENAME, "rb+")) ) return;
	
	/*Calcula o número de strucs do arquivo*/
	fseek(fp, 0, SEEK_END);
	tam = (int)ftell(fp)/(int)sizeof(Livro);

	/*Move o ponteiro para a penúltima struct no arquivo do arquivo*/
	fseek(fp, -(2*sizeof(Livro)), SEEK_END);
	for(i=1; i<tam; i++)
	{
		fread(&aux0, sizeof(Livro), 1, fp);
		fread(&aux1, sizeof(Livro), 1, fp);
		if(aux0.codigo > aux1.codigo)
		{
			fseek(fp, -(2*sizeof(Livro)), SEEK_CUR);
			fwrite(&aux1, sizeof(Livro), 1, fp);
			fwrite(&aux0, sizeof(Livro), 1, fp);
			fseek(fp, -((2*sizeof(Livro)) + (i*sizeof(Livro))), SEEK_END);
			
		}
		else/*O último dado está no local correto*/
		{
			fclose(fp);
			return;
		}
	}
	fclose(fp);
	return;
}

int cadastrar_livro()
{
	Livro new_livro;
	FILE *fp;
	
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
	if( busca_generica_livro(new_livro.codigo, &new_livro)>=0 ) return 1;
	
	/*Checa por erros ao abrir o arquivo*/
	if( !(fp = fopen(LIVRO_FILENAME, "ab+"))) return 1;

	/*Escreve no arquivo*/
	fwrite(&new_livro, sizeof(Livro), 1, fp);

	fclose(fp);
	
	/*Ordena o arquivo*/
	sort_livro();

	return 0;
}

int alterar_status_livro()
{
	int pos, codigo;
	FILE *fp;
	Livro aux;

	printf("Digite o código: ");
	scanf("%d", &codigo);
	getchar();

	/*O código não existe*/
	if( (pos = busca_generica_livro(codigo, &aux))<0 ) return 1;
	
	if(aux.status == DISP)
		aux.status = MANU;
	else if(aux.status == MANU)
		aux.status = DISP;
	else
		return 1;/*O livro está emprestado*/
	
	/*Erros ao abrir o arquivo*/
	if( !(fp = fopen(LIVRO_FILENAME, "rb+")) ) return 1;

	/*Escreve de volta no arquivo*/
	fseek(fp, pos*sizeof(Livro), SEEK_SET);
	fwrite(&aux, sizeof(Livro), 1, fp);

	fclose(fp);
	return 0;
}

int consultar_livro()
{
	char titulo[81];
	Livro aux;
	
	printf("Digite o título: ");
	fgets(titulo, 81, stdin);
	titulo[strlen(titulo) -1] = '\0';

	/*Procura no arquivo por aquela matricula*/
	if( (busca_titulo_livro(titulo, &aux))<0 ) return 1;
	
	printf("\ncódigo: %d\n", aux.codigo);
	printf("isbn: %s\n", aux.isbn);
	printf("título: %s\n", aux.titulo);
	printf("autores: %s\n", aux.autores);
	printf("assunto: %s\n", aux.assunto);
	printf("editora: %s", aux.editora);
	printf("ano: %d\n", aux.ano);
	printf("edição: %d\n", aux.edicao);
	printf("status: %s\n", (aux.status == DISP) ? ("DISPONÍVEL") : 
				(aux.status == EMPR) ? ("EMPRESTADO") : ("MANUTENÇÃO"));
				
	return 0;
}

int apagar_livro()
{
	FILE *fp_old, *fp_new;
	Livro aux;
	int codigo;
	
	printf("Digite o código: ");
	scanf("%d", &codigo);
	getchar();

	/*Procura no arquivo por aquele código*/
	if( (busca_generica_livro(codigo, &aux))<0 ) return 1;

	/*Se o livro está emprestado*/
	if(aux.status == EMPR) return 1;

	if( !(fp_old = fopen(LIVRO_FILENAME, "rb")) || !(fp_new = fopen("livro_bk.dat", "wb") )) return 1;

	while(!feof(fp_old))
	{
		fread(&aux, sizeof(Livro), 1, fp_old);
		if(!feof(fp_old) && aux.codigo != codigo)
			fwrite(&aux, sizeof(Livro), 1, fp_new);

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

	fprintf(stream, "Relatório de livros cadastrados:\nData: %02d/%02d/%02d @ %02d:%02d\n\n",
			tempo.tm_mday,
			tempo.tm_mon+1, 
			tempo.tm_year+1900,
			tempo.tm_hour,
			tempo.tm_min);
	
	while(!feof(fp))
	{
		fread(&aux, sizeof(Livro), 1, fp);
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
