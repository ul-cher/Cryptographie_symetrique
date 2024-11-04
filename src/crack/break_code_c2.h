#ifndef __BREAK_CODE_C2_H__
#define __BREAK_CODE_C2_H__



#define LONGUEUR_MAX_CLEF 10   
#define MAX_CARACTERE_VALIDE 62   




int estASCIIValide(char c);



void caractereValide(char clef[LONGUEUR_MAX_CLEF][MAX_CARACTERE_VALIDE],int longClef);



void xor_cipher(unsigned char *msg, unsigned char *key, size_t msg_len, size_t key_len);


void calculerFrequence(const unsigned char *msg,size_t longMsg,float *frequence);


float calculerDistance(const float *freqTh,const float *freqCalculee);


void creeClef(char *msg,char clefPossible[LONGUEUR_MAX_CLEF][MAX_CARACTERE_VALIDE],char *clef,int pos,int longeurClef,float *statTh);


void break_code_c2(const char *msg,int longClefMax,float *statTh);


#endif


