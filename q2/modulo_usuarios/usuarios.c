#include "usuarios.h"

int busca_repetido_usuario(int matricula, int rg, char *cpf)
{
	/*variável que armazenará cada usuario na busca*/
	Usuario aux;

	FILE *fp;
	
	/*Tenta abrir o arquivo como ro*/
	if( !(fp = fopen(USER_FILENAME, "rb") ) ) return -1;

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
				return 1;
		}
	}
	return 0;
}

void sort_usuario()
{
	Usuario aux0, aux1;
	FILE *fp;
	int i, tam;

	/*Tenta abrir o arquivo*/
	if( !(fp = fopen(USER_FILENAME, "wb+")) ) return;
	
	tam = (int)ftell(fp)/(int)sizeof(Usuario); 

	/*Move o ponteiro para a penúltima struct no arquivo do arquivo*/
	fseek(fp, -(2*sizeof(Usuario)), SEEK_END);
	for(i=1; i<tam; i++)
	{
		fread(&aux0, sizeof(Usuario), 1, fp);
		fread(&aux1, sizeof(Usuario), 1, fp);
		if(aux0.matricula < aux1.matricula)
		{
			fseek(fp, -(2*sizeof(Usuario)), SEEK_CUR);
			fwrite(&aux1, sizeof(Usuario), 1, fp);
			fwrite(&aux0, sizeof(Usuario), 1, fp);
			fseek(fp, -((2*sizeof(Usuario)) + i), SEEK_CUR);
			
		}
		else/*O o último ddo está no local correto*/
		{
			fclose(fp);
			return;
		}
	}
}

int busca_generica_usuario(int matricula, Usuario *user)
{	
	Usuario aux_busca;
	FILE *fp;
	int i=0;

	if( !(fp = fopen(USER_FILENAME, "rb")) ) return -1;

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
	
	printf("digite a matrícula: ");
	scanf("%d", &new_user.matricula);
	printf("digite o rg: ");
	scanf("%d", &new_user.rg);
	printf("digite o cpf: ");
	scanf("%s", new_user.cpf);
	printf("digite o nome: ");
	scanf("%s", new_user.nome);
	printf("digite o endereço: ");
	scanf("%s", new_user.endereco);
	printf("digite a data de nascimento(DD/MM/AA): ");
	scanf("%d/%d/%d", &new_user.nasc.tm_mday, &new_user.nasc.tm_mon, &new_user.nasc.tm_year);
	printf("digite o sexo(M/F): ");
	scanf("%c", &new_user.sexo);
	printf("digite o bairro: ");
	scanf("%s", new_user.bairro);
	printf("digite o telefone residencial(9 dígitos): ");
	scanf("%d", &new_user.fone_red);
	printf("digite o telefone móvel(9 dígitos): ");
	scanf("%d", &new_user.fone_cel);
	time(&agora);
	new_user.adesao = *localtime(&agora);
	new_user.status = NORM;
	new_user.num_emprest = 0;
	
	/*Checa por dados repetidos, ou erros ao abrir o arquivo*/
	if( (busca_repetido_usuario(new_user.matricula, new_user.rg, new_user.cpf)) || !(fp = fopen(USER_FILENAME, "ab"))) return 1;

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
	int pos;
	Usuario aux;
	
	printf("Digite a matrícula: ");
	scanf("%d", &matricula);

	/*Procura no arquivo por aquela matricula*/
	if( (pos = busca_generica_usuario(matricula, &aux))<0 ) return 1;

	/*Tenta abrir o arquivo para leitura*/
	if( !(fp = fopen(USER_FILENAME, "wb")) );

	printf("digite o rg: ");
	scanf("%d", &aux.rg);
	printf("digite o cpf: ");
	scanf("%s", aux.cpf);
	printf("digite o nome: ");
	scanf("%s", aux.nome);
	printf("digite o endereço: ");
	scanf("%s", aux.endereco);
	printf("digite a data de nascimento(DD/MM/AA): ");
	scanf("%d/%d/%d", &aux.nasc.tm_mday, &aux.nasc.tm_mon, &aux.nasc.tm_year);
	printf("digite o sexo(M/F): ");
	scanf("%c", &aux.sexo);
	printf("digite o bairro: ");
	scanf("%s", aux.bairro);
	printf("digite o telefone residencial(9 dígitos): ");
	scanf("%d", &aux.fone_red);
	printf("digite o telefone móvel(9 dígitos): ");
	scanf("%d", &aux.fone_cel);
		
	fseek(fp, pos*sizeof(Usuario), SEEK_SET);
	fwrite(&aux, sizeof(Usuario), 1, fp);
	fclose(fp);
	
	return 0;
}

int consultar_cadastro()
{
	int matricula;
	Usuario aux;
	
	printf("Digite a matrícula: ");
	scanf("%d", &matricula);

	/*Procura no arquivo por aquela matricula*/
	if( (busca_generica_usuario(matricula, &aux))<0 ) return 1;
	
	printf("matrícula: %d\n", aux.matricula);
	printf("rg: %d\n", aux.rg);
	printf("cpf: %s\n", aux.cpf);
	printf("nome: %s\n", aux.nome);
	printf("endereço: %s\n", aux.endereco);
	printf("data de nascimento: %d/%d/%d\n", aux.nasc.tm_mday, aux.nasc.tm_mon+1, aux.nasc.tm_year+1900);
	printf("sexo: %c\n", aux.sexo);
	printf("bairro: %s\n", aux.bairro);
	printf("telefone residencial: %d\n", aux.fone_red);
	printf("telefone móvel: %d\n", aux.fone_cel);
	printf("data de adesão: %d/%d/%d\n", aux.adesao.tm_mday, aux.adesao.tm_mon+1, aux.adesao.tm_year+1900);
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
