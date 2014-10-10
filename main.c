#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//-------------Estrutura
typedef struct No{
	char* exists;//Sinaliza se existe a palavra até esse ponto
	struct No* sheet[26];
	int* line;
} No;

typedef struct Sugestion{
	char* word_text;//chave
	char* word_sugestion;
	int length;
	struct Sugestion* prox;
} Sugestion;
//----------------------

//-----------Constante
#define C_MAX_INT 32767
#define c_alphabet_length 26
#define c_hash 100
#define c_custo 2 //Constante para controlar o custo de edição
//---------------------
//No trabalho de estrutura fazer com que o texto busque na gramatica e não a gramatica buscar no texto

//---------------Variaveis Globais
No* gno_root_dictionary;//n�� raiz  ; gno =global No
Sugestion* gno_sugestion;
char* gs_palavra;//Salva a palavra exatamente como foi escrita
char* gs_sugestion_word;
int gi_custo,gi_custo_aux; //variavel pra 'podar' o lenvenstein distance

//--------------------------------


inline int MIN(int x,int y,int z){

	int result = x;
	if (result > y) result = y;
	if (result > z) result = z;

	return result;
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
			aux = append(a_str,'\0'); // allocar o 1 espaço a mais
			memmove(aux+pos+1, aux+pos, len-pos+1);
			aux[pos]=a_c;
			return aux;
		}
		else
			return append(a_str,a_c);

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
			(*a_root)->exists=gs_palavra;//Final de palavra
		}

	}else{

		if(a_word[0] !='\0'){//Se a palavra não chegou ao fim
			insert_word(&(*a_root)->sheet[a_word[0]-'a'],a_word+1);
		}else{
			(*a_root)->exists=gs_palavra;
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
		if(a_root->exists!=NULL)
			if(a_root->line != NULL){//Se a palavra existe e ocorreu no texto, mostre!
				printf("%s %d",a_root->exists,a_root->line[0]);
				for(i=1;a_root->line[i]!='\0';i++)
					printf(", %d",a_root->line[i]);

				printf("\n");
			}

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



bool verify_word(No** a_root, char* a_word, int a_line){//exist_word 2.0
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

		if ((*a_root)->exists!=NULL){//Se existe a palavra, marca a linha que foi encontrada

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
					if((*a_root)->line[i-1]!=a_line)
						(*a_root)->line[i] = a_line;
				}

			return true;
		}else{
			return false;
		}

	else
		return verify_word(&(*a_root)->sheet[a_word[0]-'a'],a_word+1,a_line);
}

int LevenshteinDistance(char* s, char* t)
{
    // degenerate cases
    if (strcmp(s,t)==0) return 0;
    //if (s.Length == 0) return t.Length;
    //if (t.Length == 0) return s.Length;

    int slen = strlen(s);
    int tlen = strlen(t);
    int i,j;

    // create two work vectors of integer distances
    int* v0 = malloc(sizeof(int)*(tlen+1));//new int[t.Length + 1];
    int* v1 = malloc(sizeof(int)*(tlen+1));

    // initialize v0 (the previous row of distances)
    // this row is A[0][i]: edit distance for an empty s
    // the distance is just the number of characters to delete from t
    for (i = 0; i < tlen+1; i++)
        v0[i] = i;

    for (i = 0; i < slen; i++)
    {
        // calculate v1 (current row distances) from the previous row v0

        // first element of v1 is A[i+1][0]
        //   edit distance is delete (i+1) chars from s to match empty t
        v1[0] = i + 1;

        // use formula to fill in the rest of the row
        for (j = 0; j < tlen; j++)
        {
            short int  cost = (s[i] == t[j]) ? 0 : 1;
            v1[j + 1] = MIN(v1[j] + 1, v0[j + 1] + 1, v0[j] + cost);

        }

        // copy v1 (current row) to v0 (previous row) for next iteration
        for (j = 0; j < tlen+1; j++)
            v0[j] = v1[j];
    }

    return v1[tlen];
}

char* correct_word(No* a_root, char* a_word){


	if(a_root == NULL)//Caso a arvore esteja nula, só ocorre na inicialização
		return NULL;
	else {

		int i;
		if(a_root->exists!=NULL){
			gi_custo_aux = LevenshteinDistance(a_word,gs_palavra);
			if(gi_custo_aux<gi_custo){
				gi_custo = gi_custo_aux;
				gs_sugestion_word = a_root->exists;
			}
			if(gi_custo==1)
				return gs_sugestion_word;
		}

		for(i = 0 ; i<c_alphabet_length; i++)//Continue procurando em cada filho
			if(a_root->sheet[i]!= NULL)
				if(correct_word(a_root->sheet[i],append(a_word,'a'+i))!=NULL)
					return gs_sugestion_word;
	}

	return NULL;

}

void insert_word_in_list(char* str,int len){
	Sugestion* aux = gno_sugestion;
	gs_palavra = str;
	gi_custo = C_MAX_INT;
	if(aux==NULL){
		gno_sugestion = (Sugestion *)malloc(sizeof(Sugestion));
		gno_sugestion->word_text = str;
		gno_sugestion->length = len;

		correct_word(gno_root_dictionary,NULL);
		gno_sugestion->word_sugestion = gs_sugestion_word;

	}else{
		while (aux->prox != NULL)
			aux = aux->prox;

		aux = (Sugestion *)malloc(sizeof(Sugestion));
		aux->word_text = str;
		aux->length = len;
		aux->word_sugestion = correct_word(gno_root_dictionary,NULL);

	}
	printf("%s %s\n",gs_palavra,gs_sugestion_word);
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

	if((a_c>='a' && a_c<='z')||(a_c>='A' && a_c<='Z'))
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
					gs_palavra=NULL;
					fscanf(lf_file,"%c",&lc_c);


					while (is_letter(lc_c) && ftell(lf_file) < li_file_size){
						gs_palavra = append(gs_palavra,lc_c);

						lc_c = lower(lc_c);
						ls_str = append(ls_str,lc_c);

						fscanf(lf_file,"%c",&lc_c);

					}
					if(ls_str!=NULL){
						if(!is_letter(lc_c)){
							insert_word(&gno_root_dictionary,ls_str);
						}else{//Necessário devido ao final de texto(código exclui a ultima letra por causa do while)
							gs_palavra = append(gs_palavra,lc_c);

							lc_c = lower(lc_c);
							ls_str = append(ls_str,lc_c);
							insert_word(&gno_root_dictionary,ls_str);
						}
					}
				}
				ls_str=NULL;
				gs_palavra = NULL;
				free(gs_palavra);
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
			int len;

			fseek(lf_file, 0L, SEEK_END);//deslocar o curso para o fim para poder pegar seu tamanho maximo
			li_file_size = ftell(lf_file);//pegar o tamanho do arquivo
			fseek(lf_file,0,SEEK_SET);//setar o cursor do arquivo para o ��nicio


			lc_c=' ';//preenche com qualquer coisa para entao entrar no while


			while(lc_c!=EOF && ftell(lf_file) < li_file_size){//Loop para pegar o texto no arquivo
				len = 0;
				gs_palavra = NULL;
				ls_str=NULL;
				fscanf(lf_file,"%c",&lc_c);

				while (is_letter(lc_c) && ftell(lf_file) < li_file_size){
					len++;
					gs_palavra = append(gs_palavra,lc_c);
					lc_c = lower(lc_c);
					ls_str = append(ls_str,lc_c);
					fscanf(lf_file,"%c",&lc_c);

				}
				if(ls_str!=NULL){
					if(!is_letter(lc_c)){
						if(!verify_word(&gno_root_dictionary,ls_str,li_line))
							insert_word_in_list(ls_str,len);
					}else{//Necessário devido ao final de texto(código exclui a ultima letra por causa do while)
						len++;
						gs_palavra = append(gs_palavra,lc_c);
						lc_c = lower(lc_c);
						ls_str = append(ls_str,lc_c);
						if(!verify_word(&gno_root_dictionary,ls_str,li_line))
							insert_word_in_list(ls_str,len);
					}
				}
				if(lc_c == '\n'){
					li_line++;
				}
			}
			ls_str=NULL;
			gs_palavra = NULL;
			free(gs_palavra);
			free(ls_str);
			fclose(lf_file);

		}else{
			printf("File (%s) not found!",a_name_file);
			exit(-1);
		}

}


int main(int argc, char **argv) {


	if(argc >= 1){//Mudar dps para 3<<<<<<<<<<<<<<<<<<<<<<<<<

		initialize_dictionary(argv[1]);
		initialize_text(argv[2]);


//TODO
		//1 - Criar estrutura para armazenar os "erros" e as correções
		//2 - Conserta o problema de inserção da correção
		//3 - Simplificar códigos


		exist_word(gno_root_dictionary,NULL);//Mostra a saída


	}else{
		printf("Bad arguments");
	}
	return 0;
}
