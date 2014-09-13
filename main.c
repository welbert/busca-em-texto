#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//-----------Constante
int alphabet_length = 26;
//---------------------
//No trabalho de estrutura fazer com que o texto busque na gramatica e não a gramatica buscar no texto

char* append(char* token, char c) {//Fun����o para concaternar um char a uma string
	char * new_str;
	if(token==NULL){
		token = (char*)malloc(sizeof(char)*2);
		token[0] = c;
		token[1] = '\0';
	}
	else{
		int i;
		new_str = token;
		token = (char*)malloc(sizeof(char)*(strlen(new_str)+2));
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

typedef struct No{
	bool exists;//Sinaliza se existe a palavra at�� esse ponto
	struct No* sheet[26];
} No;

void insert_word(No** root, char* word){
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
		if(word[0] != '/0'){
			insert_word(&(*root)->sheet[word[0]-'a'],word+1);
		}else{
			(*root)->exists=true;
		}
	}
}

bool exist_word(No* root, char* word){

	if(root == NULL)
		return false;
	else if(word[0]=='\0')
		return root->exists;
	else
		return exist_word(root->sheet[word[0]-'a'],word+1);

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
