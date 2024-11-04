#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "break_code_c1.h"
#include "break_code_c2.h"




/// français
	float statThFr[26] = {9.42, 1.02, 2.64, 3.39, 15.87, 0.95, 1.04, 0.77, 8.41, 0.89,
		0.00, 5.34, 3.24, 7.15,  5.14, 2.86, 1.06, 6.46, 7.90, 7.26,
		6.24, 2.15, 0.00, 0.30, 0.24, 0.32};

//anglais de a à z
  float statThEn[26] = {8.167, 1.492, 2.782,4.253,12.702,2.228,2.015,6.094,6.966,0.153,0.772,4.025,
            2.406,6.749,7.507,1.929,0.095,5.987,6.327,9.056,2.758,
            0.978,2.36,0.15,1.974,0.074};




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




void calculerFrequence(const unsigned char *msg,size_t longMsg,float *frequence){
    int nbLettres[26] = {0};
    int totLettre =0;
    for(size_t i=0;i<longMsg;i++){
        if(isalpha(msg[i])){
            char c =tolower(msg[i]); 
            nbLettres[c-'a']++;
            totLettre++;
        }
    }
    for(int i=0;i<26;i++){
        if(totLettre!=0){
            frequence[i] = (nbLettres[i] * 100.0) / totLettre;
        }else{
            frequence[i] = 0;
        }
    }
}

float calculerDistance(const float *freqTh,const float *freqCalculee){
    float distance =0;
    for(int i=0;i<26;i++){
        float diff =freqTh[i]-freqCalculee[i];
        distance +=diff*diff;
    }
    return distance;
}


void creeClef(char *msg,char clefPossible[LONGUEUR_MAX_CLEF][MAX_CARACTERE_VALIDE],char *clef,int pos,int longeurClef,float *statTh){
    int longMsg = strlen(msg);
    if(pos==longeurClef){
        unsigned char msgTemporaire[longMsg + 1];
        strcpy((char *)msgTemporaire, msg);
        xor_cipher(msgTemporaire,(unsigned char *)clef,longMsg,longeurClef);
        int valide = 1;
        for(int i=0;i<longMsg;i++){
            if(!estASCIIValide(msgTemporaire[i])){
                valide = 0;
                break;
            }
        }

        if(valide){
            float resFrequence[26] = {0};
            calculerFrequence(msgTemporaire,longMsg,resFrequence);
            float distance = calculerDistance(statTh,resFrequence);

            printf("Clef: %s avec distance = %.2f\n",clef,distance);
        }
    }else{
        int nbCandidats = strlen(clefPossible[pos]);
        for(int i=0;i<nbCandidats;i++) {
            clef[pos] = clefPossible[pos][i];
            creeClef(msg,clefPossible,clef,pos+1,longeurClef,statTh);
        }
    }
}


void break_code_c2(const char *msg,int longClefMax,float *statTh){
    char clefPossible[LONGUEUR_MAX_CLEF][MAX_CARACTERE_VALIDE];
    char clef[LONGUEUR_MAX_CLEF + 1] = {0}; 
    for (int longClef =1;longClef<=longClefMax;longClef++){
        caractereValide(clefPossible,longClef);
        printf("Test pour une longueur de clef = %d:\n",longClef);
        creeClef((char *)msg,clefPossible,clef,0,longClef,statTh);
    }
}

int main(void){
    const char *msg = "s(/1& !"; 
    int longueurClefMax = 2;
    break_code_c2(msg,longueurClefMax,statThFr);
    return 0;
}
