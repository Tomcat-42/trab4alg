/* Equipe: Pablo AS Hugen, Carlos Stum, Heloísa Alves 
 * Questão 01: Matrizes Esparsas com Arqivos */
#include <stdio.h>
#include <stdlib.h>

/* Número de casas decimais dos valores impressos */
#define DEC 3
/* Justifica PAD casas à direita */
#define PAD 4
/*Comando do OS para 'limpar' a stdout*/
#define CLEARCMD "clear"
/*Tamanho máximo de nome de arquivo*/
#define MAX_FILENAME 50
/*Tamanho máximo de linha nos arquivos texto*/
#define MAX_LINE 300



/*Células da matriz: Armazenam um ponteiro para a celula a direita a para a celula
 * abaixo, além da sua linha e coluna e valor armazenado */
typedef struct Cell
{
	struct Cell *Right, *Down;
	int i, j;
	double Data;
}Cell;

/*Matriz: Armazena um ponteiro para a cabeça "Central" - que possui ponteiros para
 * as cabeça da primeira linha e a cabeça da primeira coluna-, além disso, possui o número de linhas, colunas,
 * e a quantidade de elementos não zeros da matriz*/
typedef struct Matrix
{
	struct Cell *Head;
	int Rows, Cols;
	int nonZero;
}Matrix;


/*Estruturas para escrita no arquivo, sem os ponteiros que ocupariam memória denecessária.*/
typedef struct Cell_file
{
	int i, j;
	double Data;
}Cell_file;

typedef struct Matrix_file
{
	int Rows, Cols;
	int nonZero;
}Matrix_file;


/*Funções principais para correção */
int salvar(Matrix *a);
int abrir(Matrix *a);
int exportar(Matrix *a);
int importar(Matrix *a);


/*Funções auxiliares */

/*Imprime os elementos da matriz a, inclusive os iguais a 0*/
void printMatrix(FILE *stream, Matrix *a);
/*Lê uma matriz*/
void scanMatrix(Matrix *a);
/*Retorna uma matriz soma,a veriicação de limites deve ser feita antes*/
Matrix sumMatrix(Matrix *a, Matrix *b);
/*Retorna uma matriz produto,a verificação de limites deve ser feita antes*/
Matrix prodMatrix(Matrix *a, Matrix *b);
/*Insere um elemento a[i,j] na matriz a*/
void insertMatrix(Matrix *a, int i, int j, double Data);

/*Inicia a matriz: aloca uma cabeça central, 'm' celulas cabeças para as linhas e 'n' células 
 * cabeças para as colunas*/
void initMatrix(Matrix *a, int Rows, int Cols);
/*Remove um elemento a[i,j] na matriz a, caso o elemento não existir nada acontece */
void removeMatrix(Matrix *a, int i, int j);
/*Desaloca todos os nós de uma matriz*/
void freeMatrix(Matrix *a);
/*Retorna um ponteiro para o elemento a[i,j] se ele for diferente de zero,
 * caso contrário retorna NULL*/
Cell *cellPtr(Matrix *a, int i, int j);



int main()
{
	/*matrizes disponíveis para operações */
	Matrix matrizes[4];
	/*Controle do menu*/
	int menu = 0;
	/*indices das matrizes usadas nas operações*/
	int op1,op2,res;
	/*Variáveis para a inserção individual*/
	int i,j;
	double Dado;

	/*Inicia as matrizes com 0 linhas e 0 colunas*/
	initMatrix(&matrizes[0], 0, 0);
	initMatrix(&matrizes[1], 0, 0);
	initMatrix(&matrizes[2], 0, 0);
	initMatrix(&matrizes[3], 0, 0);
	
	while(1)
	{
		system(CLEARCMD);
		printf("Calculadora de Matrizes:\n");
		printf( "\n0)Sair" 
			"\n1)Definir uma Matriz"
			"\n2)Inserir uma elemento em uma Matriz(já definida)"
			"\n3)Imprimir uma Matriz"
			"\n4)Somar Matrizes"
			"\n5)Multiplicar Matrizes"
			"\n6)Salvar uma Matriz"
			"\n7)Abrir uma Matriz"
			"\n8)Exportar uma Matriz"
			"\n9)Importar uma Matriz");
		printf("\n\n>>> ");
		scanf("%d",&menu);
		
		system(CLEARCMD);
		switch(menu)
		{
			case 1:
				printf("Definir uma Matriz:\n\n");
				printf("0)MatA\t1)MatB\n2)MatC\t3)MatD\n");
				printf("\n>>> ");
				scanf("%d", &res);
				system(CLEARCMD);
				freeMatrix(&matrizes[res]); // limpa a matriz anterior
				scanMatrix(&matrizes[res]);
				break;
			case 2:
				printf("Inserir uma elemento em uma Matriz\n\n");
				printf("0)MatA\t1)MatB\n2)MatC\t3)MatD\n");
				printf("\n>>> ");
				scanf("%d", &res);
				printf("Digite uma tripla 'i j Dado': ");
				scanf("%d %d %lf", &i, &j, &Dado);
				if(i<matrizes[res].Rows && j< matrizes[res].Cols)
					insertMatrix(&matrizes[res], i, j, Dado);
				system(CLEARCMD);
				break;
			case 3:
				printf("Imprimir uma Matriz:\n\n");
				printf("0)MatA\t1)MatB\n2)MatC\t3)MatD\n");
				printf("\n>>> ");
				scanf("%d",&res);
				system(CLEARCMD);
				printMatrix(stdout, &matrizes[res]);
				printf("\nPressione [ENTER] para retornar ao menu ");
				getchar();
				while(getchar()!='\n');
				break;
			case 4:
				printf("Somar Matrizes:\n\n");
				printf("0)MatA\t1)MatB\n2)MatC\t3)MatD\n");
				printf("\nexemplo: Para armazenar a soma das matrizes MatA e MatB na matriz MatC '2 <= 0 + 1'");
				printf("\n>>> ");
				scanf("%d <= %d + %d", &res, &op1, &op2);
				if(matrizes[op1].Rows == matrizes[op2].Rows && matrizes[op1].Cols == matrizes[op2].Cols)
				{
					freeMatrix(&matrizes[res]); // limpa a matriz anterior
					matrizes[res] = sumMatrix(&matrizes[op1], &matrizes[op2]);
				}
				break;
			case 5:
				printf("Multiplicar Matrizes:\n\n");
				printf("0)MatA\t1)MatB\n2)MatC\t3)MatD\n");
				printf("\nexemplo: Para armazenar o produto das matrizes MatA e MatB na matriz MatC '2 <= 0 * 1'");
				printf("\n>>> ");
				scanf("%d <= %d * %d", &res, &op1, &op2);
				if(matrizes[op1].Cols == matrizes[op2].Rows)
				{
					freeMatrix(&matrizes[res]); // limpa a matriz anterior
					matrizes[res] = prodMatrix(&matrizes[op1], &matrizes[op2]);
				}
				break;
			case 6:
				printf("Salvar uma Matriz:\n\n");
				printf("0)MatA\t1)MatB\n2)MatC\t3)MatD\n");
				printf("\n>>> ");
				scanf("%d",&res);
				system(CLEARCMD);
				if(!salvar(&matrizes[res]))
					printf("\nMatriz salva com sucesso.\n");
				else
					printf("\nErro na operação de salvar.");

				printf("\nPressione [ENTER] para retornar ao menu ");
				getchar();
				while(getchar()!='\n');
				break;
			case 7:
				printf("Abrir uma Matriz:\n\n");
				printf("0)MatA\t1)MatB\n2)MatC\t3)MatD\n");
				printf("\n>>> ");
				scanf("%d",&res);
				system(CLEARCMD);
				if(!abrir(&matrizes[res]))
					printf("\nMatriz aberta com sucesso.\n");
				else
					printf("\nErro na operação de abrir.");

				printf("\nPressione [ENTER] para retornar ao menu ");
				getchar();
				while(getchar()!='\n');
				break;
			case 8:
				printf("Exportar uma Matriz:\n\n");
				printf("0)MatA\t1)MatB\n2)MatC\t3)MatD\n");
				printf("\n>>> ");
				scanf("%d",&res);
				system(CLEARCMD);
				if(!exportar(&matrizes[res]))
					printf("\nMatriz exportada com sucesso.\n");
				else
					printf("\nErro na exportação.");

				printf("\nPressione [ENTER] para retornar ao menu ");
				getchar();
				while(getchar()!='\n');
				break;
			case 9:
				printf("Importar uma Matriz:\n\n");
				printf("0)MatA\t1)MatB\n2)MatC\t3)MatD\n");
				printf("\n>>> ");
				scanf("%d",&res);
				system(CLEARCMD);
				if(!importar(&matrizes[res]))
					printf("\nMatriz importada com sucesso.\n");
				else
					printf("\nErro na importação.");

				printf("\nPressione [ENTER] para retornar ao menu ");
				getchar();
				while(getchar()!='\n');
				break;
			default:
				//limpa as matrizes
				freeMatrix(&matrizes[0]);
				freeMatrix(&matrizes[1]);
				freeMatrix(&matrizes[2]);
				freeMatrix(&matrizes[3]);
				exit(0);
		}
	}
	return 0;
}

int salvar(Matrix *a)
{
	FILE *save_file;
	char filename[MAX_FILENAME];
	Cell *aux0 = NULL, *aux1 = NULL;
	Cell_file auxC;
	Matrix_file auxM;

	printf("Digite um nome(caminho) de arquivo: ");
	scanf("%s", filename);
	if( !(save_file = fopen(filename, "wb")) )
		return 1;
	
	/*Salva a cabeça da matriz*/
	auxM.nonZero = a->nonZero;
	auxM.Rows = a->Rows;
	auxM.Cols = a->Cols;
	fwrite(&auxM, sizeof(Matrix_file), 1, save_file);
	
	/*Salva todos os nós da matriz*/
	aux0 = a->Head->Down;
	while(aux0 != a->Head)
	{
		aux1 = aux0->Right;
		while(aux1 != aux0)
		{
			auxC.i = aux1->i;
			auxC.j = aux1->j;
			auxC.Data = aux1->Data;
			
			printf("%.2lf\n",auxC.Data);
			fwrite(&auxC, sizeof(Cell_file), 1, save_file);	
			
			aux1 = aux1->Right;
		}
		aux0 = aux0->Down;
	}

	fclose(save_file);
	return 0;
}

int abrir(Matrix *a)
{
	FILE *open_file;
	char filename[MAX_FILENAME];
	Cell_file auxC;
	Matrix_file auxM;

	printf("Digite um nome(caminho) de arquivo: ");
	scanf("%s", filename);
	if( !(open_file = fopen(filename, "rb")) )
		return 1;
	
	freeMatrix(a);
	
	/*Lê os dados da matriz*/
	fread(&auxM, sizeof(Matrix_file), 1, open_file);
	initMatrix(a, auxM.Rows, auxM.Cols);


	/*Lê os dados dos nós*/
	for(int i =0; !feof(open_file) && i<auxM.nonZero; i++)
	{
		if(!feof(open_file))
		{
			fread(&auxC, sizeof(Cell_file), 1, open_file);
			insertMatrix(a, auxC.i, auxC.j, auxC.Data);
		}
	}
	fclose(open_file);
	return 0;
}

int exportar(Matrix *a)
{
	FILE *export_file;
	char filename[MAX_FILENAME];
	
	printf("Digite um nome(caminho) de arquivo: ");
	scanf("%s", filename);
	if( !(export_file = fopen(filename, "w")) )
		return 1;
	
	printMatrix(export_file, a);
	fclose(export_file);
	
	return 0;
}

int importar(Matrix *a)
{
	FILE *import_file;
	char filename[MAX_FILENAME];
	char line[MAX_LINE];
	char *line_tmp;
	double val;

	printf("Digite um nome(caminho) de arquivo: ");
	scanf("%s", filename);
	
	if( !(import_file = fopen(filename, "r")) )
		return 1;

	freeMatrix(a);

	/*Lê as dimensões da matriz*/
	fgets(line, MAX_LINE, import_file);
	sscanf(line, "%*[^0-9]%d",&a->Rows);
	fgets(line, MAX_LINE, import_file);
	sscanf(line, "%*[^0-9]%d",&a->Cols);
	fgets(line, MAX_LINE, import_file);
	sscanf(line, "%*[^0-9]%d",&a->nonZero);
	
	initMatrix(a, a->Rows, a->Cols);

	/*Lê os valores da matriz*/
	fseek(import_file, sizeof(char), SEEK_CUR);
	for(int i=0; !feof(import_file) && i<a->Rows; i++)
	{
		fgets(line, MAX_LINE, import_file);
		line_tmp = line;
		for(int j=0; !feof(import_file) && j<a->Cols; j++)
		{
			val = strtod(line_tmp, &line_tmp);
			if(val)
				insertMatrix(a, i, j, val);
		}
	}
	fclose(import_file);
	return 0;
}

void printMatrix(FILE *stream, Matrix *a)
{
	int i,j;
	Cell *aux0 = NULL, *aux1 = NULL;	
	fprintf(stream, "Número de linhas: %d\nNúmero de colunas: %d\nNúmero de elementos não nulos: %d\n\n",
			a->Rows, a->Cols, a->nonZero);	
	/* Visita todos as cabeças das linhas e printa seus elementos*/
	aux0 = a->Head->Down;
	for(i=0; i<a->Rows; i++)
	{
		aux1 = aux0->Right;
		for(j=0; j<a->Cols; j++)
		{
			/*Se o nó no index a[i,j] existir seu valor é impresso
			 * Caso contrário 0 */
			fprintf(stream, "%*.*lf%s",
					(int)PAD,
					(int)DEC, 
					(j==aux1->j) ? (aux1->Data) : (0.0),
					(j<a->Cols-1)?(" "):("\n"));
			/*Avança o ponteiro se um nó correspondente foi encontrado*/
			if(aux1->j == j)
				aux1 = aux1->Right;
		}
		aux0 = aux0->Down;
	}
	return;
}

void scanMatrix(Matrix *a)
{
	int rows = 0, cols =0;
	int i=0,j=0,k=0;
	double Data=0.0;

	printf("Número de linhas: ");
	scanf("%d", &rows);
	printf("Número de colunas: ");
	scanf("%d", &cols);

	initMatrix(a, rows, cols);

	printf("\nLê triplas \"i j Dado\" até (linha*colunas) elementos serem lidos ou os todos os campos serem '-1'\n");
	for(k=0; k<(rows*cols) && (scanf("%d %d %lf", &i, &j, &Data) && (i>-1 && j>-1)); k++)
		insertMatrix(a, i, j, Data);
}

Matrix sumMatrix(Matrix *a, Matrix *b)
{
	Matrix ret;
	Cell *auxA0 = NULL, *auxA1 = NULL, *auxB0 = NULL, *auxB1 = NULL;
	int i,j;

	/*As dimensões das duas matrizes devem ser iguais, 
	 * assim a dimensão da matriz soma será a mesma */
	initMatrix(&ret, a->Rows, a->Cols);
	
	/* Visita todas as linhas de ambas matrizes */
	auxA0 = a->Head->Down;
	auxB0 = b->Head->Down;
	for(i=0; i<a->Rows; i++)
	{
		auxA1 = auxA0->Right;
		auxB1 = auxB0->Right;
		for(j=0; j<a->Cols; j++)
		{
			/*Ambos os elementos são não nulos, insere a soma e avança ambas as colunas*/
			if(auxA1->j == j && auxB1->j ==j)
			{
				insertMatrix(&ret, i, j, auxA1->Data + auxB1->Data);
				if(auxA1 != auxA0) auxA1 = auxA1->Right;
				if(auxB1 != auxB0) auxB1 = auxB1->Right;
			}

			/*Somente o elemento da primeira matriz é não nulo, somente adiciona ele*/
			else if(auxA1->j == j)
			{
				insertMatrix(&ret, i, j, auxA1->Data);
				if(auxA1 != auxA0) auxA1 = auxA1->Right;
			}
			/*Somente o elemento da segunda matriz é não nulo, somente adiciona ele*/
			else if(auxB1->j == j)
			{
				insertMatrix(&ret, i, j, auxB1->Data);
				if(auxB1 != auxB0) auxB1 = auxB1->Right;
			}
			/*Ambos são nulos, insere o 0 para depois remover (?)*/
			else
				insertMatrix(&ret, i, j, 0);
			
			/*Avança somente se não chegou ao final */
			//if(auxA1 != auxA0) auxA1 = auxA1->Right;
			//if(auxB1 != auxB0) auxB1 = auxB1->Right;
		}
		auxA0 = auxA0->Down;
		auxB0 = auxB0->Down;
	}
	return ret;

}

Matrix prodMatrix(Matrix *a, Matrix *b)
{
	Matrix ret;
	Cell *auxA0 = NULL, *auxA1 = NULL, *auxB0 = NULL, *auxB1 = NULL;
	int i, j, k;
	double sum =0;

	/*As dimensões da nova matriz será o número de colunas da primiera matriz
	 * e o numero de linhas da segunda matriz */
	initMatrix(&ret, a->Rows, b->Cols);
	
	/* Basicamente o algoritmo simples de multiplicação de matrizes,
	 * mas com toda a insanidade dos ponteiros*/
	auxA0 = a->Head->Down;
	for(i=0; i<a->Rows; i++)
	{
		auxB0 = b->Head->Right;
		for(j=0; j<b->Cols; j++)
		{
			auxB1 = auxB0->Down;
			auxA1 = auxA0->Right;
			sum = 0;
			for(k=0; k<a->Cols; k++)
			{
				if(auxA1->j == k && auxB1->i == k)
				{
					sum+= (auxA1->Data * auxB1->Data);
					if(auxA1 != auxA0) auxA1 = auxA1->Right;
					if(auxB1 != auxB0) auxB1 = auxB1->Down;
				}
				else if(auxA1->j == k)
				{
					if(auxA1 != auxA0) auxA1 = auxA1->Right;
				}
				else if(auxB1->i == k)
				{
					if(auxB1 != auxB0) auxB1 = auxB1->Down;
				}
			}
			auxB0 = auxB0->Right;
			insertMatrix(&ret, i, j, sum);	
		}
		auxA0 = auxA0->Down;
	}
	return ret;
}

void insertMatrix(Matrix *a, int i, int j, double Data)
{
	int k;
	Cell *auxColHead = NULL, *auxRowHead = NULL, *auxCol = NULL, *auxRow = NULL;
	Cell *elemento = cellPtr(a,i,j);

	/*Tratamento do caso do elemento já existir, remove para inserir novamente */
	if(elemento) removeMatrix(a, i, j); 
	
	/*Aloca a nova célula */
	Cell *newCell = (Cell *)malloc(sizeof(Cell));
	newCell->i = i;
	newCell->j = j;
	newCell->Data = Data;

	/*Move auxRowHead para a cabeça da linha do novo elemento*/
	auxRowHead = a->Head->Down;
	for(k=0; k<i; k++)
		auxRowHead = auxRowHead->Down;
	
	/*Move auxColHead para a cabeça da coluna do novo elemento*/
	auxColHead = a->Head->Right;
	for(k=0; k<j; k++)
		auxColHead = auxColHead->Right;
	

	/*Move auxRow para o elemento anterior ao novo em sua linha*/
	auxRow = auxRowHead;
	while(1)
	{
		if((auxRow->Right != auxRowHead) && (auxRow->Right->j < j))
			auxRow = auxRow->Right;
		else
			break;
	}
	
	/*Move auxCol para o elemento anterior ao novo em sua coluna*/
	auxCol = auxColHead;
	while(1)
	{
		if((auxCol->Down != auxColHead) && (auxCol->Down->i < i))
			auxCol = auxCol->Down;
		else
			break;
	}


	/*Muda os ponteiros */
	newCell->Right = auxRow->Right;
	newCell->Down = auxCol->Down;
	auxRow->Right = newCell;
	auxCol->Down = newCell;
	
	
	/*Incrementa o número de elementos não zeros da matriz */
	a->nonZero++;

	/*Um valor 0 não precisaria ser armazenado na matriz, mas
	 * de acordo com enunciado da questão ele deve ser inserido e depois removido.(?) */
	if(!Data) removeMatrix(a, i, j);
	
	return;
}



void initMatrix(Matrix *a, int Rows, int Cols)
{
	int i;
	Cell *aux = NULL;

	a->Rows = Rows;
	a->Cols = Cols;
	a->nonZero = 0;

	/*Cabeção central*/
	a->Head = (Cell *)malloc(sizeof(Cell));
	a->Head->i = a->Head->j = -1;
	a->Head->Data = 42;
	
	/*Aloca 'Rows' Cabeças para as listas das linhas*/
	aux = a->Head;
	for(i=0; i<Rows; i++)
	{
		aux->Down = (Cell *)malloc(sizeof(Cell));
		aux->Down->Right = aux->Down;
		
		aux->Down->Data = 0;
		aux->Down->i = aux->Down->j = -1;
		
		aux = aux->Down;
	}
	aux->Down = a->Head;
	
	/*Aloca 'Cols' Cabeças para as listas das colunas */
	aux = a->Head;
	for(i=0;i<Cols; i++)
	{
		aux->Right = (Cell *)malloc(sizeof(Cell));
		aux->Right->Down = aux->Right;
		
		aux->Right->Data = 0;
		aux->Right->i = aux->Right->j = -1;
		
		aux = aux->Right;
	}
	aux->Right = a->Head;
	
	return;
}


void removeMatrix(Matrix *a, int i, int j)
{
	int k;
	
	/*Ponteiro para o elemento a ser removido */
	Cell *elemento = cellPtr(a,i,j);

	/*Ponteiros auxiliares*/
	Cell *aux0 = NULL, *aux1 = NULL; 


	/*Se o elemento não existe ele não pode ser removido,
	 * nesse caso a função não erguerá nenhuma exceção, somente
	 * irá retornar normalmente */
	if(!elemento) return;
	
	/*O elemento existe, remove ele da lista da sua respectiva linha */
	aux0 = a->Head->Down;
	for(k=0; k<i; k++)
		aux0 = aux0->Down;
	aux1 = aux0->Right;
	while((aux1->Right != elemento))
		aux1 = aux1->Right;
	aux1->Right = elemento->Right;
	
	/*Remove o elemento da lista da sua respectiva coluna*/
	aux0 = a->Head->Right;
	for(k=0; k<j; k++)
		aux0 = aux0->Right;
	aux1 = aux0->Down;
	while(aux1->Down != elemento)
		aux1 = aux1->Down;
	aux1->Down = elemento->Down;

	/*Desaloca o elemento */
	a->nonZero--;
	free(elemento);

	return;
}

Cell *cellPtr(Matrix *a, int i, int j)
{
	int k;
	Cell *aux0 = NULL, *aux1 = NULL;
	
	/*Tenta Caminhar até o elemento a[i,j] */
	aux0 = a->Head->Down;
	for(k=0; k<i; k++)
		aux0 = aux0->Down;
	aux1 = aux0->Right;
	for(k=0; k<j; k++)
	{
		/*O elemento foi encontrado */
		if(aux1->j == j && aux1->i == i) 
			return aux1;
		aux1 = aux1->Right;
	}

	/*O elemento foi encontrado */
	if(aux1->j == j && aux1->i == i) 
		return aux1;
	/*O elemento não foi encontrado*/
	return NULL;
}

void freeMatrix(Matrix *a)
{
	Cell *aux0, *aux1, *aux2;
	int i;

	/*Desaloca as células linha a linha*/
	aux0=a->Head;
	for(i=0; i<a->Rows; i++)
	{
		while(aux0->Right != aux0)
		{
			aux2 = aux0->Right;
			aux0->Right = aux2->Right;
			free(aux2);
		}
		aux1 = aux0;
		aux0 = aux0->Down;
		free(aux1);
	}
	a->Rows = a->Cols = a->nonZero = 0;
}
