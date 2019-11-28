#include "usuarios.h"

int cmp_matricula(const void *a, const void *b)
{
	Usuario aux0 = *((Usuario *)a), aux1 = (*(Usuario *)b);

	if(aux0.matricula > aux1.matricula)
		return 1;
	else if(aux0.matricula == aux1.matricula)
		return 0;
	else
		return -1;

}

int cmp_repetido(const void *a, const void *b)
{
	Usuario aux0 = *((Usuario *)a), aux1 = (*(Usuario *)b);

	if(aux0.matricula == aux1.matricula ||
			aux0.rg == aux1.rg  ||
			!strcmp(aux0.cpf, aux1.cpf))
		return 0;
	else
		return -1;

}

int cadastrar_usuario()
{
	Usuario new_user, aux_user;
	time_t agora, aux=0;
	FILE *fp;
	int ano, mes;
	size_t tam_user, num_user;
	
	/*Tenta abrir o arquivo de usuários*/
	if( !(fp = fopen(USER_FILENAME, "rb+")))
		if( !(fp = fopen(USER_FILENAME, "wb+")))
		{
			printf("\nErro ao criar base de dados!\n");
			return 1;
		}
	fseek(fp, 0, SEEK_END);
	tam_user = sizeof(Usuario);
	num_user = ftell(fp)/tam_user;

	printf("digite a matrícula: ");
	scanf("%d", &new_user.matricula);
	getchar();
	printf("digite o rg: ");
	scanf("%d", &new_user.rg);
	getchar();
	printf("digite o cpf: ");
	fgets(new_user.cpf, 13, stdin);
	new_user.cpf[strlen(new_user.cpf) - 1] = '\0';
	printf("digite o nome: ");
	fgets(new_user.nome, 52, stdin);
	new_user.nome[strlen(new_user.nome) -1] = '\0';
	printf("digite o endereço: ");
	fgets(new_user.endereco, 72, stdin);
	new_user.endereco[strlen(new_user.endereco) -1] = '\0';
	printf("digite a data de nascimento(DD/MM/AAAA): ");
	scanf("%d/%d/%d", &new_user.nasc.tm_mday, &mes, &ano);
	new_user.nasc.tm_mon = mes-1;
	new_user.nasc.tm_year = ano-1900;
	getchar();
	printf("digite o sexo(M/F): ");
	scanf("%c", &new_user.sexo);
	getchar();
	printf("digite o bairro: ");
	fgets(new_user.bairro, 32, stdin);
	new_user.bairro[strlen(new_user.bairro) -1] = '\0';
	printf("digite o telefone residencial(8 dígitos): ");
	scanf("%d", &new_user.fone_red);
	getchar();
	printf("digite o telefone móvel(8 dígitos): ");
	scanf("%d", &new_user.fone_cel);
	getchar();
	time(&agora);
	new_user.adesao = *localtime(&agora);
	new_user.status = NORM;
	new_user.num_emprest = 0;
	new_user.term_susp = *(localtime(&aux));
	
	
	/*Checa por dados repetidos*/
	fseek(fp, 0, SEEK_END);
	if( search_file(fp, num_user, tam_user, &new_user,
				cmp_repetido,
				1,
				&aux_user) >=0)
	{
		printf("\nErro: Algum dado já consta na base de dados!\n");
		fclose(fp);
		return 1;
	}
	

	/*Escreve no arquivo*/
	fseek(fp, 0, SEEK_END);
	fwrite(&new_user, tam_user, 1, fp);
	
	/*Ordena o arquivo*/
	fseek(fp, 0, SEEK_END);
	sort_file(fp, (num_user+1), tam_user, cmp_matricula);

	fclose(fp);

	return 0;
}

int atualizar_cadastro()
{
	FILE *fp;
	int pos, mes, ano;
	Usuario aux;
	size_t tam_user, num_user;
	
	/*Tenta abrir o arquivo de usuários*/
	if( !(fp = fopen(USER_FILENAME, "rb+")))
	{
		printf("\nErro: Nenhum usuário cadastrado!\n");
		return 1;
	}
	fseek(fp, 0, SEEK_END);
	tam_user = sizeof(Usuario);
	num_user = ftell(fp)/tam_user;
	
	printf("Digite a matrícula: ");
	scanf("%d", &aux.matricula);
	getchar();

	/*Procura no arquivo por aquela matricula*/
	fseek(fp, 0, SEEK_END);
	if((pos = search_file(fp, num_user, tam_user, &aux, cmp_matricula, 1, &aux))<0)
	{
		printf("\nErro: matrícula inexistente na base de dados!\n");
		fclose(fp);
		return 1;
	}

	printf("digite o novo rg: ");
	scanf("%d", &aux.rg);
	getchar();
	printf("digite o novo cpf: ");
	fgets(aux.cpf, 13, stdin);
	aux.cpf[strlen(aux.cpf) -1] = '\0';
	printf("digite o nome: ");
	fgets(aux.nome, 52, stdin);
	aux.nome[strlen(aux.nome) -1] = '\0';
	printf("digite o endereço: ");
	fgets(aux.endereco, 72, stdin);
	aux.endereco[strlen(aux.endereco) -1] = '\0';
	printf("digite a data de nascimento(DD/MM/AA): ");
	scanf("%d/%d/%d", &aux.nasc.tm_mday, &mes, &ano);
	aux.nasc.tm_mon = mes-1;
	aux.nasc.tm_year = ano-1900;
	getchar();
	printf("digite o sexo(M/F): ");
	scanf("%c", &aux.sexo);
	getchar();
	printf("digite o bairro: ");
	fgets(aux.bairro, 32, stdin);
	aux.bairro[strlen(aux.bairro) -1] = '\0';
	printf("digite o telefone residencial(9 dígitos): ");
	scanf("%d", &aux.fone_red);
	getchar();
	printf("digite o telefone móvel(9 dígitos): ");
	scanf("%d", &aux.fone_cel);
	getchar();
		
	fseek(fp, pos*tam_user, SEEK_SET);
	fwrite(&aux, tam_user, 1, fp);
	fclose(fp);
	
	return 0;
}

int consultar_usuario()
{
	FILE *fp;
	Usuario aux;
	size_t tam_user, num_user;
	
	/*Tenta abrir o arquivo de usuários*/
	if( !(fp = fopen(USER_FILENAME, "rb+")))
	{
		printf("\nErro: Nenhum usuário cadastrado!\n");
		return 1;
	}
	fseek(fp, 0, SEEK_END);
	tam_user = sizeof(Usuario);
	num_user = ftell(fp)/tam_user;
	
	printf("Digite a matrícula: ");
	scanf("%d", &aux.matricula);
	getchar();

	/*Procura no arquivo por aquela matricula*/
	fseek(fp, 0, SEEK_END);
	if((search_file(fp, num_user, tam_user, &aux, cmp_matricula, 1, &aux))<0)
	{
		printf("\nErro: matrícula inexistente na base de dados!\n");
		fclose(fp);
		return 1;
	}
	
	printf("\nmatrícula: %d\n", aux.matricula);
	printf("rg: %d\n", aux.rg);
	printf("cpf: %s\n", aux.cpf);
	printf("nome: %s\n", aux.nome);
	printf("endereço: %s\n", aux.endereco);
	printf("data de nascimento: %02d/%02d/%d\n", aux.nasc.tm_mday, aux.nasc.tm_mon+1, aux.nasc.tm_year+1900);
	printf("sexo: %c\n", aux.sexo);
	printf("bairro: %s\n", aux.bairro);
	printf("telefone residencial: %d\n", aux.fone_red);
	printf("telefone móvel: %d\n", aux.fone_cel);
	printf("data de adesão: %02d/%02d/%d\n", aux.adesao.tm_mday, aux.adesao.tm_mon+1, aux.adesao.tm_year+1900);
	printf("status: %s\n", (aux.status) ? ("SUSPENSO"): ("NORMAL"));
	if(aux.status) printf("término da suspensão: %d/%d/%d\n", aux.term_susp.tm_mday, aux.term_susp.tm_mon+1, aux.term_susp.tm_year+1900);
	printf("volumes emprestados: %d\n", aux.num_emprest);	
	
	fclose(fp);

	return 0;
}

int apagar_usuario()
{
	FILE *fp_old, *fp_new;
	Usuario aux, aux_new;
	size_t tam_user, num_user;
	
	/*Tenta abrir os arquivos*/
	if( !(fp_old = fopen(USER_FILENAME, "rb")) || !(fp_new = fopen("usuario_bk.dat", "wb") )) 
		return 1;
	
	fseek(fp_old, 0, SEEK_END);
	tam_user = sizeof(Usuario);
	num_user = ftell(fp_old)/tam_user;
	
	printf("Digite a matrícula: ");
	scanf("%d", &aux.matricula);
	getchar();


	/*Procura no arquivo por aquela matricula*/
	fseek(fp_old, 0, SEEK_END);
	if((search_file(fp_old, num_user, tam_user, &aux, cmp_matricula, 1, &aux))<0)
	{
		printf("\nErro: matrícula inexistente na base de dados!\n");
		fclose(fp_old);
		fclose(fp_new);
		remove("usuario_bk.dat");
		return 1;
	}
	
	/*Se o usuário está suspenso*/
	if(aux.status)
	{
		printf("\nerro: usuários suspensos não podem ser apagados!\n");
		fclose(fp_old);
		fclose(fp_new);
		remove("usuario_bk.dat");
		return 1;
	}

	rewind(fp_old);
	while(!feof(fp_old))
	{
		fread(&aux_new, tam_user, 1, fp_old);
		if(!feof(fp_old) && aux_new.matricula != aux.matricula)
			fwrite(&aux_new, tam_user, 1, fp_new);

	}
	fclose(fp_old);
	fclose(fp_new);
	rename("usuario_bk.dat", USER_FILENAME);
	
	return 0;
}

int relatorio_usuario()
{
	/*variável que armazenará cada usuario na busca*/
	Usuario aux;
	char filename[51], filepath[51];
	FILE *fp,*stream;
	time_t agr;
	struct tm tempo;
	int menu;
	size_t tam_user = sizeof(Usuario);
	
	/*Tenta abrir o arquivo*/
	if( !(fp = fopen(USER_FILENAME, "rb") ) ) return 1;
	
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
		sprintf(filename, "Relatorio_usuarios_%02d-%02d-%02d@%02d:%02d.txt", 
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

	fprintf(stream, "Relatório de usuários cadastrados:\nData: %02d/%02d/%02d @ %02d:%02d\n\n",
			tempo.tm_mday,
			tempo.tm_mon+1, 
			tempo.tm_year+1900,
			tempo.tm_hour,
			tempo.tm_min);
	
	while(!feof(fp))
	{
		fread(&aux, tam_user, 1, fp);
		if(!feof(fp))
		{
			fprintf(stream, "\nmatrícula: %d\n", aux.matricula);
			fprintf(stream, "rg: %d\n", aux.rg);
			fprintf(stream, "cpf: %s\n", aux.cpf);
			fprintf(stream, "nome: %s\n", aux.nome);
			fprintf(stream, "endereço: %s\n", aux.endereco);
			fprintf(stream, "data de nascimento: %02d/%02d/%d\n", aux.nasc.tm_mday, aux.nasc.tm_mon+1, aux.nasc.tm_year+1900);
			fprintf(stream, "sexo: %c\n", aux.sexo);
			fprintf(stream, "bairro: %s\n", aux.bairro);
			fprintf(stream, "telefone residencial: %d\n", aux.fone_red);
			fprintf(stream, "telefone móvel: %d\n", aux.fone_cel);
			fprintf(stream, "data de adesão: %02d/%02d/%d\n", aux.adesao.tm_mday, aux.adesao.tm_mon+1, aux.adesao.tm_year+1900);
			fprintf(stream, "status: %s\n", (aux.status) ? ("SUSPENSO"): ("NORMAL"));
			if(aux.status) fprintf(stream, "término da suspensão: %d/%d/%d\n", aux.term_susp.tm_mday, aux.term_susp.tm_mon+1, aux.term_susp.tm_year+1900);
			fprintf(stream, "volumes emprestados: %d\n", aux.num_emprest);	
		}
		fprintf(stream, "\n");
	}
	fclose(fp);
	if(menu == 2) fclose(stream);
	return 0;
}
