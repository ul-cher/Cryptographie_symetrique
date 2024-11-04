#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "break_code_c1.h"

 

int estASCIIValide(char c){
    return isprint(c);
}

void caractereValide(char clef[LONGUEUR_MAX_CLEF][MAX_CARACTERE_VALIDE],int longClef){
    for(int i = 0; i< longClef;i++){
        int j = 0;
        for(char c ='0';c<='9';c++) 
            clef[i][j++]=c;
        for(char c ='A';c<='Z';c++) 
            clef[i][j++]=c;
        for(char c= 'a';c<='z';c++) 
            clef[i][j++]=c;
        clef[i][j] = '\0'; 
    }
}



void xor_cipher(unsigned char *msg, unsigned char *key, size_t msg_len, size_t key_len) {
    for (size_t i = 0; i < msg_len; i++) {
        msg[i] ^= key[i % key_len];
    }
}



void recherchePossibleClef(char *msg,char clefPossible[LONGUEUR_MAX_CLEF][MAX_CARACTERE_VALIDE],char *clef,int pos,int longeurClef){
    int longMsg =strlen(msg);
    if(pos== longeurClef){
        unsigned char msgTemporaire[longMsg+1];
        strcpy((char *)msgTemporaire,msg);
        xor_cipher(msgTemporaire,(unsigned char *)clef,longMsg,longeurClef);

        int valide = 1;
        for(int i = 0; i < longMsg; i++){
            if(!estASCIIValide(msgTemporaire[i])){
                valide = 0;
                break;
            }
        }
        if(valide){
            printf("Cle valide trouvee : %s \n", clef);
        }else{
            printf("Cle : %s a produit un msg invalide \n", clef);
        }
    }
    int nbCandidats = strlen(clefPossible[pos]);
    for(int i = 0;i<nbCandidats;i++){
        clef[pos] =clefPossible[pos][i];
        recherchePossibleClef(msg,clefPossible, clef,pos+1,longeurClef);
    }
}


void break_code_c1(const char *msg,int longClefMax){
    char clefPossible[LONGUEUR_MAX_CLEF][MAX_CARACTERE_VALIDE];
    char clef[LONGUEUR_MAX_CLEF + 1] ={0}; 
    for(int longClef =1;longClef<=longClefMax;longClef++){
        caractereValide(clefPossible,longClef);
        printf("Test pour une longueur de clef = %d:\n",longClef);
        recherchePossibleClef((char *)msg,clefPossible,clef,0,longClef);
    }
}

int main(void){
    const char *msg ="s(/1& !"; 
    int  longueurClefMax = 2; 
    break_code_c1(msg,longueurClefMax);

    return 0;
}