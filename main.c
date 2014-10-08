#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//-------------Estrutura
typedef struct No{
	bool exists;//Sinaliza se existe a palavra até esse ponto
	struct No* sheet[26];
	int* line;
} No;

typedef struct Sugestion{
	char* word_text;//chave
	char* word_sugestion;
	struct Sugestion* prox;
} Sugestion;
//----------------------

//-----------Constante
#define c_alphabet_length 26
#define c_hash 100
#define c_custo 2 //Constante para controlar o custo de edição
//---------------------
//No trabalho de estrutura fazer com que o texto busque na gramatica e não a gramatica buscar no texto

//---------------Variaveis Globais
No* gno_root_dictionary;//n�� raiz  ; gno =global No
Sugestion* gno_sugestion[c_hash];// Estrutura para salvar as palavras 'erradas'
char* gs_last_word;//pegar na a palavra corrigida
bool is_correction = false;//armengue
int gi_custo = 0;

//--------------------------------

char type_correction(char t){
	if(gi_custo<c_custo)
		return t;
	else
		return 'F';
}

char* append(char* a_str, char a_c) {
	//-------------------------------------------------------------
	//Retorno:
	//			char* : Retorna a string concatenada;
	//
	//Argumentos:
	//			char* a_str: Recebe a string como base;
	//			char c: A letra que deseja-se concatenar
	//
	//Descrição da função:
	//			Adiciona uma letra ao final da string, Ex.: cas + a = casa
	//-------------------------------------------------------------

	char * ls_new_str;
	if(a_str=='\0')
		a_str = NULL;

	if(a_str==NULL){// Se for nulo, aloca um espaço de memoria pra armazenar a letra e o '\0'
		a_str = (char*)malloc(sizeof(char)*2);
		a_str[0] = a_c;
		a_str[1] = '\0';
	}
	else{
		int i;
		ls_new_str = a_str;// salva em um ponteiro temporario
		a_str = (char*)malloc(sizeof(char)*(strlen(ls_new_str)+2));//Aloca um novo espaço do tamanho da nova palavra
		for(i=0; i< strlen(ls_new_str); i++)
			a_str[i] = ls_new_str[i];
		a_str[i] = a_c;
		i++;
		a_str[i] = '\0';
		ls_new_str = NULL;
		free(ls_new_str);
	}
	return a_str;
}

char* append_pos(char* a_str, char a_c, int pos){
	//-------------------------------------------------------------
	//Retorno:
	//			char* : Retorna a string concatenada;
	//
	//Argumentos:
	//			char* ls_str1: Recebe a string como base;
	//			char a_c: Recebe o char para concatenar
	//			int pos: a posição para concatenar
	//Descrição da função:
	//			Concatena um char a uma string em uma determinada posição
	//-------------------------------------------------------------


	if(a_str==NULL)
		return append(NULL,a_c);

	else{
		int len = strlen(a_str);
		if(len-1>=pos){
			char * aux;
			aux = append(a_str,NULL); // allocar o 1 espaço a mais
			memmove(aux+pos+1, aux+pos, len-pos+1);
			aux[pos]=a_c;
			return aux;
		}
		else
			return append(a_str,a_c);;

	}
}

char* append_string(char* a_str1, char* a_str2) {
	//-------------------------------------------------------------
	//Retorno:
	//			char* : Retorna a string concatenada;
	//
	//Argumentos:
	//			char* ls_str1: Recebe a string como base;
	//			char* ls_str2: Recebe a segunda string
	//
	//Descrição da função:
	//			Concatena Strings
	//-------------------------------------------------------------

	char * ls_new_str = NULL;
	int tamanho=0;
	if(a_str1 != NULL){
		tamanho +=strlen(a_str1);
	}
	if(a_str2 != NULL){
		tamanho += strlen(a_str2);
	}
	ls_new_str = malloc(sizeof(char)*tamanho);
	if(a_str1 != NULL){
		strcpy(ls_new_str, a_str1);
	}
	if(a_str2 != NULL){
		strcat(ls_new_str,a_str2);
	}
	return ls_new_str;
}

int hash(char* a_str, int a_vhash) {
   int i, h = a_str[0];
   for (i = 1; a_str[i] != '\0'; i++)
      h = (h * 256 + a_str[i]) % a_vhash;
   return h % a_vhash;
}




Sugestion* search_insert_hash(char* a_str,Sugestion* no){
	//int key = 0;//hash(a_str,c_hash);

	if(no== NULL){
		(no) = malloc(sizeof(Sugestion));
		return (no);
	}else{
		if(strcmp((no)->word_text,a_str)==0)//Não é o ideal para comparação
			return NULL;

		return search_insert_hash(a_str,no->prox);
	}

	//return NULL;
}

void insert_hash(char* a_str,char* a_str1){
	int key = 0;
		Sugestion* aux;
		aux = search_insert_hash(a_str,gno_sugestion[key]);
		if(aux!=NULL){
			aux->word_text = a_str;
			aux->word_sugestion = a_str1;
			aux->prox = NULL;
		}

}

bool search_hash(char* a_str){
	int key = hash(a_str,c_hash);
	if(gno_sugestion[key]==NULL){
		return false;
	}else{
		//while(gno_sugestion[key])
	}
	return false;
}

void insert_word(No** a_root, char* a_word){
	//-------------------------------------------------------------
	//Retorno:
	//			void;
	//
	//Argumentos:
	//			No** a_root: Nó da arvore que será inserido a letra da palavra(Passagem por parametro);
	//			char* a_word: String que se deseja inserir;
	//
	//Descrição da função:
	//			Inserção na arvore trie de maneira recursiva;
	//-------------------------------------------------------------

	int i;

	if (*a_root == NULL){//Caso o nó é nulo (caso base)

		*a_root = malloc(sizeof(No));

		for (i = 0;i<c_alphabet_length;i++){
			(*a_root)->sheet[i]=NULL;//Retirada dos possiveis lixos de memoria
			(*a_root)->line=NULL;
		}

		if(a_word[0]!='\0'){//Se n for final de palavra
			insert_word(&(*a_root)->sheet[a_word[0]-'a'],a_word+1);//obs: a = 97, logo a-a = 0, posição 0(zero)
			(*a_root)->exists=false;
		}else{
			(*a_root)->exists=true;//Final de palavra
		}

	}else{

		if(a_word[0] !='\0'){//Se a palavra não chegou ao fim
			insert_word(&(*a_root)->sheet[a_word[0]-'a'],a_word+1);
		}else{
			(*a_root)->exists=true;
		}

	}
}

void exist_word(No* a_root, char* a_word){
	//-------------------------------------------------------------
	//Retorno:
	//			bool : true = palavra achada
	//					false = palavra não achada
	//
	//Argumentos:
	//			No* a_root: nó da arvore
	//			char* a_word: auxiliar de string
	//
	//Descrição da função:
	//			Varre toda a arvore Trie, imprimindo as palavras encontradas
	//-------------------------------------------------------------


	if(a_root == NULL)//Caso a arvore esteja nula, só ocorre na inicialização
		return;
	else {

		int i;
		if(a_root->exists)
			if(a_root->line != NULL)
				printf("%s %s\n",a_word,a_root->line);//Se a palavra existe e ocorreu no texto, mostre!


		for(i = 0 ; i<c_alphabet_length; i++)//Continue procurando em cada filho
			if(a_root->sheet[i]!= NULL)
				exist_word(a_root->sheet[i],append(a_word,'a'+i));
	}

	return;

}
/*
char* exist_word(No* a_root, char* a_word){

	if(a_root == NULL)
		return NULL;
	else if(a_word[0]=='\0')
		return a_root->line;
	else
		return exist_word(a_root->sheet[a_word[0]-'a'],a_word+1);

}*/



bool verify_word(No** a_root, char* a_word, int a_line, char a_tipo){//exist_word 2.0
	//-------------------------------------------------------------
	//Retorno:
	//			bool : true = palavra achada
	//					false = palavra não achada
	//
	//Argumentos:
	//			No** a_root: nó da arvore
	//			char* a_word: palavra que se deseja achar
	//			int a_line: A linha a palavra foi encontrada
	//
	//Descrição da função:
	//			Busca a palavra na arvore,marca a linha do texto caso exista
	//			e retorna a se existe ou não
	//-------------------------------------------------------------

	if((*a_root) == NULL)
		return false;
	else if(a_word[0]=='\0')

		if ((*a_root)->exists){//Se existe a palavra, marca a linha que foi encontrada
			if (a_line != -1){
				if((*a_root)->line == NULL){
					(*a_root)->line = malloc(sizeof(int)*2);
					(*a_root)->line[0] = a_line;
				}else{
					int* aux;
					int i=0;
					aux = (*a_root)->line;
					(*a_root)->line = malloc(sizeof((*a_root)->line)+sizeof(int)*2);

					while(aux[i]!='\0'){
						(*a_root)->line[i] = aux[i];
						i++;
					}
					if((*a_root)->line[i-1]!=(*a_root)->line[i])
						(*a_root)->line[i] = a_line; //TODO fazer a impressao
				}
			}
			return true;

		}else{
			return false;
		}

	else
		if (verify_word(&(*a_root)->sheet[a_word[0]-'a'],a_word+1,a_line,'I')){
			//printf("%c\n",a_word[0]);
			gs_last_word = append_pos(gs_last_word,a_word[0],0);
			return true;
		}
		else{
			int i;

			//INSERCAO
			if(a_tipo =='I'){
				gi_custo++;
				char* aux = NULL;
				for (i=0;i<c_alphabet_length;i++){
					aux = append_pos(a_word,'a'+i,0);
					if((*a_root)->sheet[aux[0]-'a']!=NULL) // evitar 'empilhamento' desnecessario
						if(verify_word(&(*a_root)->sheet[aux[0]-'a'],aux+1,-1,type_correction('I'))){ // TODO não insere na ultima posição
				//			printf("%c\n",aux[0]);
							gs_last_word = append_pos(gs_last_word,aux[0],0);
							is_correction = true;
							return true;
						}
				}
				a_tipo = 'T';
				gi_custo--;
			}

			//TROCA
			if(a_tipo == 'T'){
				gi_custo++;
				for (i=0;i<c_alphabet_length;i++){
					if((*a_root)->sheet[i]!=NULL)
						if(verify_word(&(*a_root)->sheet[i],a_word+2,-1,type_correction('T'))){
					//		printf("%c\n",'a'+i);
							gs_last_word = append_pos(gs_last_word,'a'+i,0);
							is_correction = true;
							return true;
						}
				}
				a_tipo = 'D';
				gi_custo--;
			}

			//DELETE
			if(a_tipo == 'D'){
				gi_custo++;
				if(a_word[1]!='\0')
					if(verify_word(&(*a_root)->sheet[a_word[1]-'a'],a_word+1,-1,type_correction('D'))){
						//printf("%c",a_word[1]);
						gs_last_word = append_pos(gs_last_word,a_word[1],0);
						is_correction = true;
						return true;
					}
				gi_custo--;
			}


			return false;
		}

}

bool is_letter(char a_c){
	//-------------------------------------------------------------
	//Retorno:
	//			bool : true = é um char entre 'a' e 'z'(minusculo)
	//					false = é algo que não é entre 'a' e 'z' (minusculo)
	//
	//Argumentos:
	//			char a_c: letra que se deseja comparar
	//
	//Descrição da função:
	//			verificar se o 'char c' é uma letra minuscula(a-z)
	//-------------------------------------------------------------

	if(a_c>='a' && a_c<='z')
		return true;
	else
		return false;
}

char lower(char a_c){
	//-------------------------------------------------------------
	//Retorno:
	//			char : a letra minuscula
	//
	//Argumentos:
	//			char a_c: letra que se deseja torna minuscula
	//
	//Descrição da função:
	//			Tornar uma letra maiuscula em minuscula
	//-------------------------------------------------------------

	if(a_c>='A' && a_c<='Z')
		return a_c+32;//Tabela ASCII
	else
		return a_c;
}

void initialize_dictionary(char* a_name_file){
	//-------------------------------------------------------------
	//Retorno:
	//			void
	//
	//Argumentos:
	//			char* a_name_file: diretório/nome do arquivo que será aberto
	//
	//Descrição da função:
	//			Inicializa a arvore com as palavras do dicionário
	//-------------------------------------------------------------

	FILE *lf_file;
			lf_file = fopen("Debug/gramatica.txt","r");
			//lf_file = fopen(a_name_file,"r"); // Gramatica
			if(lf_file!=NULL){
				char lc_c;
				char* ls_str;
				long int li_file_size;


				gno_root_dictionary = NULL;

				fseek(lf_file, 0L, SEEK_END);//deslocar o curso para o fim para poder pegar seu tamanho maximo
				li_file_size = ftell(lf_file);//pegar o tamanho do arquivo
				fseek(lf_file,0,SEEK_SET);//setar o cursor do arquivo para o ��nicio


				lc_c=' ';//preenche com qualquer coisa para entao entrar no while


				while(lc_c!=EOF && ftell(lf_file) < li_file_size){//Loop para pegar o texto no arquivo
					ls_str=NULL;
					fscanf(lf_file,"%c",&lc_c);
					lc_c = lower(lc_c);

					while (is_letter(lc_c) && ftell(lf_file) < li_file_size){
						ls_str = append(ls_str,lc_c);
						fscanf(lf_file,"%c",&lc_c);
						lc_c = lower(lc_c);
					}
					if(ls_str!=NULL){
						if(!is_letter(lc_c)){
							insert_word(&gno_root_dictionary,ls_str);
						}else{//Necessário devido ao final de texto(código exclui a ultima letra por causa do while)
							ls_str = append(ls_str,lc_c);
							insert_word(&gno_root_dictionary,ls_str);
						}
					}
				}
				ls_str=NULL;
				free(ls_str);
				fclose(lf_file);
			}else{
				printf("File (%s) not found!",a_name_file);
				exit(-1);
			}

}

void initialize_text(char* a_name_file){
	//-------------------------------------------------------------
	//Retorno:
	//			void
	//
	//Argumentos:
	//			char* a_name_file: diretório/nome do arquivo que será aberto
	//
	//Descrição da função:
	//			Lê o texto, buscando as palavras que existem e marcando a linha(verify_word)
	//-------------------------------------------------------------

	FILE *lf_file;
		lf_file = fopen("Debug/texto.txt","r");
		//f = fopen(a_name_file,"r"); // Gramatica
		if(lf_file!=NULL){
			char lc_c;
			char* ls_str;
			long int li_file_size;
			int li_line = 1;

			fseek(lf_file, 0L, SEEK_END);//deslocar o curso para o fim para poder pegar seu tamanho maximo
			li_file_size = ftell(lf_file);//pegar o tamanho do arquivo
			fseek(lf_file,0,SEEK_SET);//setar o cursor do arquivo para o ��nicio


			lc_c=' ';//preenche com qualquer coisa para entao entrar no while


			while(lc_c!=EOF && ftell(lf_file) < li_file_size){//Loop para pegar o texto no arquivo
				ls_str=NULL;
				fscanf(lf_file,"%c",&lc_c);
				lc_c = lower(lc_c);

				while (is_letter(lc_c) && ftell(lf_file) < li_file_size){
					ls_str = append(ls_str,lc_c);
					fscanf(lf_file,"%c",&lc_c);
					lc_c = lower(lc_c);
				}
				if(ls_str!=NULL){
					if(!is_letter(lc_c)){
						gs_last_word = NULL;
						is_correction = false;
						verify_word(&gno_root_dictionary,ls_str,li_line,'I');
						if(is_correction)
							printf("%s %s\n",ls_str,gs_last_word);
					}else{//Necessário devido ao final de texto(código exclui a ultima letra por causa do while)
						ls_str = append(ls_str,lc_c);
						gs_last_word = NULL;
						is_correction = false;
						verify_word(&gno_root_dictionary,ls_str,li_line,'I');
						if(is_correction)
							printf("%s %s\n",ls_str,gs_last_word);
					}
				}
				if(lc_c == '\n'){
					li_line++;
				}
			}
			ls_str=NULL;
			free(ls_str);
			fclose(lf_file);

		}else{
			printf("File (%s) not found!",a_name_file);
			exit(-1);
		}

}


int main(int argc, char **argv) {


	if(argc >= 1){//Mudar dps para 3<<<<<<<<<<<<<<<<<<<<<<<<<

		//insert_hash("aew","ae");
		//insert_hash("vaca","abra");
		//insert_hash("aew","ae");
		//insert_hash("cada","casa");

		initialize_dictionary(argv[1]);
		initialize_text(argv[2]);


//TODO
		//1 - Criar estrutura para armazenar os "erros" e as correções
		//2 - Conserta o problema de inserção da correção
		//3 - Simplificar códigos


		exist_word(gno_root_dictionary,NULL);//Mostra a saída
		/*int i;
		insert_sugestion(gno_sugestion,"tesre",'T');
		insert_sugestion(gno_sugestion,"teste",'S');
		for(i=0;i<=gi_number_sugestion;i++){
			printf("%s %s\n",gno_sugestion[i]->word_text,gno_sugestion[i]->word_sugestion);
		}*/

	}else{
		printf("Bad arguments");
	}
	return 0;
}
