#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//-----------Constante
int alphabet_length = 26;
//---------------------

char* alphabet;

char* initialize_alphabet(){
	char* string_aux = (char*)malloc(sizeof(char)*(alphabet_length+1));//aloca����o de espa��o para armazenar as letras do alfabeto
	char c;
	int i = 0;

		for(c = 'a';c<='z';c++){
			string_aux[i] = c;
			i++;
		}
	return string_aux;
}

int pos_letter(char c){
	int i;
	for(i=0;i<alphabet_length;i++){
		if(c==alphabet[i]){
			break;
		}
	}
	if(i>=0 && i<alphabet_length){
		return i;
	}else{
		return -1;
	}
}

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
	char* word;
	struct No** sheet;
} No;

No* newSheet(){
	No* main_no = (No*)malloc(sizeof(No));//Aloca����o do n�� principal
	No** aux = (No**)malloc(sizeof(No*)*alphabet_length);//aloca����o dos n��s que representam de [a-z]
	//char* string_aux = (char*)malloc(sizeof(char)*(alphabet_length+1));//aloca����o de espa��o para armazenar as letras do alfabeto

	//char c;
	//int i = 0;

	//for(c = 'a';c<='z';c++){
	//	string_aux[i] = c;
	//	i++;
	//}
	main_no->sheet = aux;
	main_no->exists = false;
	main_no->word = alphabet;

	return main_no;
}

int main(int argc, char **argv) {


	if(argc >= 1){

		FILE *f;
		f = fopen("Debug/texto.txt","r");
		//f = fopen(argv[1],"r");
		if(f!=NULL){
			char c;
			char* str;
			long int file_size;
			int i;
			No* tree;//n�� raiz

			alphabet = initialize_alphabet();
			tree = newSheet();//Inicializa����o da arvore

			fseek(f, 0L, SEEK_END);//deslocar o curso para o fim para poder pegar seu tamanho maximo
			file_size = ftell(f);//pegar o tamanho do arquivo
			fseek(f,0,SEEK_SET);//setar o cursor do arquivo para o ��nicio


			c='a';//preenche com qualquer coisa para entao entrar no while
			No* aux_tree;//auxiliar para preencher/pecorrer a arvore com as palavras

			while(c!=EOF && ftell(f) < file_size){//Loop para pegar o texto no arquivo
				str=NULL;
				aux_tree = tree;//aux_tree recebe o endere��o de tree(n�� raiz)
				fscanf(f,"%c",&c);

				while (c!= ' ' && c!='\n' && ftell(f) < file_size){
					//str = append(str,c);
					i = pos_letter(c);
					aux_tree = tree->sheet[i];

					if (aux_tree= NULL) {
						aux_tree = newSheet();
					}
					fscanf(f,"%c",&c);

				}
				aux_tree->exists = true;


			}
			str=NULL;
			aux_tree = NULL;

			//testes para varrer a arvore
			aux_tree = tree;
			while(aux_tree->exists){
				for(i=0;i<alphabet_length;i++){
					if(aux_tree->sheet[i] != NULL){
						aux_tree = aux_tree->sheet[i];
					}
				}
			}
			//
			free(aux_tree);
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
