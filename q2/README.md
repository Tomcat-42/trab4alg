# Considerações sobre a Questão 2

## **Organização**

	O programa foi dividido em módulos e a interface principal. 
	Os módulos de usuários, livros e empréstimos estão nos diretórios de mesmos nomes e a interface principal está no arquivo *main.c*.
	Além disso, a pasta *tools/* contém funções genéricas que são usadas no programa inteiro, tais como busca, ordenação, etc.

## Relatórios

	Os relatórios de cada módulo, gerados a partir do menu, são armazenados no diretório *relatorios/* com extensão *.txt e com nome gerado a partir da data atual.

## Arquivos
	Os arquivos binários de cada módulo são armazenados no diretório de seu respectivo módulo.
	A pasta *bk/* contém arquivos binários com dados já inseridos de livros e usuários para teste.

## Outros
	* O script *compile.sh* gera o executável **biblioteca**, facilita na hora de compilar o programa e seus módulos.
	* O script *compile.bat* é uma versão do *compile.sh* para sistemas Windows.
	* O script *reb.sh* Apaga os arquivos binários e os substitui com aqueles no diretório *bk/*,útil para testar o
	programa e não precisar copiar e apagar manualmente os arquivos a cada teste. 
	* O programa foi testado e compilado com o **GCC 9.2.0** em sistemas GNU/Linux, e **GCC 8.2.0** Em sistemas windows.
