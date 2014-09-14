#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//-------------Estrutura
typedef struct No{
	bool exists;//Sinaliza se existe a palavra at�� esse ponto
	struct No* sheet[26];
} No;
//----------------------

//-----------Constante
int alphabet_length = 26;
//---------------------
//No trabalho de estrutura fazer com que o texto busque na gramatica e não a gramatica buscar no texto

char* append(char* token, char c) {
	//-------------------------------------------------------------
	//Retorno:
	//			char* : Retorna a string concatenada;
	//
	//Argumentos:
	//			char* token: Recebe a string como base;
	//			char c: A letra que deseja-se concatenar
	//
	//Descrição da função:
	//			Adiciona uma letra ao final da string, Ex.: cas + a = casa
	//-------------------------------------------------------------

	char * new_str;
	if(token==NULL){// Se for nulo, aloca um espaço de memoria pra armazenar a letra e o '\0'
		token = (char*)malloc(sizeof(char)*2);
		token[0] = c;
		token[1] = '\0';
	}
	else{
		int i;
		new_str = token;// salva em um ponteiro temporario
		token = (char*)malloc(sizeof(char)*(strlen(new_str)+2));//Aloca um novo espaço do tamanho da nova palavra
		for(i=0; i< strlen(new_str); i++)
			token[i] = new_str[i];
		token[i] = c;
		i++;
		token[i] = '\0';
		new_str = NULL;
		free(new_str);
	}
	return token;
}

void insert_word(No** root, char* word){
	//-------------------------------------------------------------
	//Retorno:
	//			void;
	//
	//Argumentos:
	//			No** root: Nó da arvore que será inserido a letra da palavra(Passagem por parametro);
	//			char* word: String que se deseja inserir;
	//
	//Descrição da função:
	//			Inserção na arvore trie de maneira recursiva;
	//-------------------------------------------------------------

	int i;

	if (*root == NULL){//Caso o nó é nulo (caso base)

		*root = malloc(sizeof(No));

		for (i = 0;i<alphabet_length;i++)
			(*root)->sheet[i]=NULL;//Retirada dos possiveis lixos de memoria

		if(word[0]!='\0'){//Se n for final de palavra
			insert_word(&(*root)->sheet[word[0]-'a'],word+1);//obs: a = 97, logo a-a = 0, posição 0(zero)
			(*root)->exists=false;
		}else{
			(*root)->exists=true;//Final de palavra
		}

	}else{

		if(word[0] != '/0'){//Se a palavra não chegou ao fim
			insert_word(&(*root)->sheet[word[0]-'a'],word+1);
		}else{
			(*root)->exists=true;
		}

	}
}

bool exist_word(No* root, char* word){
	//-------------------------------------------------------------
	//Retorno:
	//			bool : true = palavra achada
	//					false = palavra não achada
	//
	//Argumentos:
	//			No* root: nó da arvore
	//			char* word: palavra que se deseja achar
	//
	//Descrição da função:
	//			Busca a palavra na arvore e retorna a se existe ou não
	//-------------------------------------------------------------


	if(root == NULL)
		return false;
	else if(word[0]=='\0')
		return root->exists;
	else
		return exist_word(root->sheet[word[0]-'a'],word+1);

}

bool is_letter(char c){
	//-------------------------------------------------------------
	//Retorno:
	//			bool : true = é um char entre 'a' e 'z'(minusculo)
	//					false = é algo que não é entre 'a' e 'z' (minusculo)
	//
	//Argumentos:
	//			char c: letra que se deseja comparar
	//
	//Descrição da função:
	//			verificar se o 'char c' é uma letra(a-z)
	//-------------------------------------------------------------

	if(c>='a' && c<='z')
		return true;
	else
		return false;
}

char lower(char c){
	//-------------------------------------------------------------
	//Retorno:
	//			char : a letra minuscula
	//
	//Argumentos:
	//			char c: letra que se deseja torna minuscula
	//
	//Descrição da função:
	//			Tornar uma letra maiuscula em minuscula
	//-------------------------------------------------------------

	if(c>='A' && c<='Z')
		return c+32;
	else
		return c;
}


int main(int argc, char **argv) {


	if(argc >= 1){//Mudar dps <<<<<<<<<<<<<<<<<<<<<<<<<

		FILE *f;
		f = fopen("Debug/texto.txt","r");
		//f = fopen(argv[1],"r");
		if(f!=NULL){
			char c;
			char* str;
			long int file_size;
			No* root;//n�� raiz

			root = NULL;

			fseek(f, 0L, SEEK_END);//deslocar o curso para o fim para poder pegar seu tamanho maximo
			file_size = ftell(f);//pegar o tamanho do arquivo
			fseek(f,0,SEEK_SET);//setar o cursor do arquivo para o ��nicio


			c='a';//preenche com qualquer coisa para entao entrar no while

//TODO "Come" a ultima letra do arquivo, solução rapida: inserir " " no final do arquivo
			while(c!=EOF && ftell(f) < file_size){//Loop para pegar o texto no arquivo
				str=NULL;
				fscanf(f,"%c",&c);
//TODO Criar uma função para identificar separadores(' '; '-' ; ';' )
				while (c!= ' ' && c!='\n' && ftell(f) < file_size){
					str = append(str,c);
					fscanf(f,"%c",&c);
				}
				if(str!=NULL)
					insert_word(&root,str);

			}
			str=NULL;
//TODO O resto do trabalho! =)
			if(exist_word(root,"teste")){
				printf("aew");
			}else{
				printf("Not aew =/");
			}

			free(str);
			fclose(f);
		}else{
			printf("File (%s) not found!",argv[1]);
		}


	}else{
		printf("Bad arguments");
	}
	return 0;
}
