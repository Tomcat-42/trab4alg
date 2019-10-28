#include "usuarios.h"

int busca_repetido_usuario(int matricula, int rg, char *cpf)
{
	/*variável que armazenará cada usuario na busca*/
	Usuario aux;

	FILE *fp;
	
	/*Tenta abrir o arquivo*/
	if( !(fp = fopen(USER_FILENAME, "rb") ) ) return 0;

	/*Vasculha todo o arquivo por uma entrada repetida*/
	while(!feof(fp))
	{
		fread(&aux, sizeof(Usuario), 1, fp);
		if(!feof(fp))
		{
			/*Checa por dados repetidos*/
			if( (matricula == aux.matricula) || 
			    (rg == aux.rg) ||
			    (!strcmp(cpf, aux.cpf)) )
			{
				fclose(fp);
				return 1;
			}
		}
	}
	fclose(fp);
	return 0;
}

void sort_usuario()
{
	Usuario aux0, aux1;
	FILE *fp;
	int i, tam;

	/*Tenta abrir o arquivo*/
	if( !(fp = fopen(USER_FILENAME, "rb+")) ) return;
	
	/*Calcula o número de strucs do arquivo*/
	fseek(fp, 0, SEEK_END);
	tam = (int)ftell(fp)/(int)sizeof(Usuario);
	printf("%d\n",tam);

	/*Move o ponteiro para a penúltima struct no arquivo do arquivo*/
	fseek(fp, -(2*sizeof(Usuario)), SEEK_END);
	for(i=1; i<tam; i++)
	{
		fread(&aux0, sizeof(Usuario), 1, fp);
		fread(&aux1, sizeof(Usuario), 1, fp);
		if(aux0.matricula > aux1.matricula)
		{
			fseek(fp, -(2*sizeof(Usuario)), SEEK_CUR);
			fwrite(&aux1, sizeof(Usuario), 1, fp);
			fwrite(&aux0, sizeof(Usuario), 1, fp);
			fseek(fp, -((2*sizeof(Usuario)) + (i*sizeof(Usuario))), SEEK_END);
			
		}
		else/*O o último ddo está no local correto*/
		{
			fclose(fp);
			return;
		}
	}
	fclose(fp);
	return;
}

int busca_generica_usuario(int matricula, Usuario *user)
{	
	Usuario aux_busca;
	FILE *fp;
	int i=0;

	if( !(fp = fopen(USER_FILENAME, "rb")) )
		return -1;

	while( !feof(fp) )
	{
		fread(&aux_busca, sizeof(Usuario), 1, fp);
		if(!feof(fp) && (matricula == aux_busca.matricula))
		{
			*(user) = aux_busca;
			return i;
		}
		else
			i++;
	}
	fclose(fp);
	return -1;
}

int cadastrar_usuario()
{
	Usuario new_user;
	time_t agora;
	FILE *fp;
	int ano, mes;
	
	printf("digite a matrícula: ");
	scanf("%d", &new_user.matricula);
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
	printf("digite a data de nascimento(DD/MM/AA): ");
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
	printf("digite o telefone residencial(9 dígitos): ");
	scanf("%d", &new_user.fone_red);
	getchar();
	printf("digite o telefone móvel(9 dígitos): ");
	scanf("%d", &new_user.fone_cel);
	getchar();
	time(&agora);
	new_user.adesao = *localtime(&agora);
	new_user.status = NORM;
	new_user.num_emprest = 0;
	
	/*Checa por dados repetidos*/
	if( busca_repetido_usuario(new_user.matricula, new_user.rg, new_user.cpf) ) return 1;
	
	/*Checa por erros ao abrir o arquivo*/
	if( !(fp = fopen(USER_FILENAME, "ab+"))) return 1;

	/*Escreve no arquivo*/
	fwrite(&new_user, sizeof(Usuario), 1, fp);

	fclose(fp);
	
	/*Ordena o arquivo*/
	sort_usuario();

	return 0;
}

int atualizar_cadastro()
{
	int matricula;
	FILE *fp;
	int pos, mes, ano;
	Usuario aux;
	
	printf("Digite a matrícula: ");
	scanf("%d", &matricula);

	/*Procura no arquivo por aquela matricula*/
	if( (pos = busca_generica_usuario(matricula, &aux))<0 ) return 1;

	/*Tenta abrir o arquivo para leitura*/
	if( !(fp = fopen(USER_FILENAME, "wb")) );

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
		
	fseek(fp, pos*sizeof(Usuario), SEEK_SET);
	fwrite(&aux, sizeof(Usuario), 1, fp);
	fclose(fp);
	
	return 0;
}

int consultar_usuario()
{
	int matricula;
	Usuario aux;
	
	printf("Digite a matrícula: ");
	getchar();
	scanf("%d", &matricula);

	/*Procura no arquivo por aquela matricula*/
	if( (busca_generica_usuario(matricula, &aux))<0 ) return 1;
	
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
				
	return 0;
}

int apagar_usuario()
{
	FILE *fp_old, *fp_new;
	Usuario aux;
	int matricula;
	
	printf("Digite a matrícula: ");
	scanf("%d", &matricula);

	/*Procura no arquivo por aquela matricula*/
	if( (busca_generica_usuario(matricula, &aux))<0 ) return 1;

	/*Se o usuário está suspenso*/
	if(aux.status) return 1;

	if( !(fp_old = fopen(USER_FILENAME, "rb")) || !(fp_new = fopen("usuario_bk.dat", "wb") )) return 1;

	while(!feof(fp_old))
	{
		fread(&aux, sizeof(Usuario), 1, fp_old);
		if(!feof(fp_old) && aux.matricula != matricula)
			fwrite(&aux, sizeof(aux), 1, fp_new);

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
	
	/*Tenta abrir o arquivo*/
	if( !(fp = fopen(USER_FILENAME, "rb") ) ) return 1;
	
	time(&agr);
	tempo = *localtime(&agr);
	printf("1)Tela\n2)Arquivo\n\n>>> ");
	scanf("%d", &menu);

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

		if( !(stream = fopen(filepath, "w") ) ) return 1;
	}
	else
		return 1;

	fprintf(stream, "Relatório de usuários cadastrados:\nData: %02d/%02d/%02d @ %02d:%02d\n\n",
			tempo.tm_mday,
			tempo.tm_mon+1, 
			tempo.tm_year+1900,
			tempo.tm_hour,
			tempo.tm_min);
	
	while(!feof(fp))
	{
		fread(&aux, sizeof(Usuario), 1, fp);
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
	return 0;

}
