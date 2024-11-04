#ifndef __BREAK_CODE_C1_H__
#define __BREAK_CODE_C1_H__



#define LONGUEUR_MAX_CLEF 10   
#define MAX_CARACTERE_VALIDE 62   




extern int estASCIIValide(char c);



extern void caractereValide(char clef[LONGUEUR_MAX_CLEF][MAX_CARACTERE_VALIDE],int longClef);


extern void xor_cipher(unsigned char * msg,unsigned char *clef,size_t longMsg,size_t longClef);


extern void recherchePossibleClef(char *msg,char clefPossible[LONGUEUR_MAX_CLEF][MAX_CARACTERE_VALIDE],char *clef,int pos,int longeurClef);



extern void break_code_c1(const char *msg, int longClefMax);




#endif