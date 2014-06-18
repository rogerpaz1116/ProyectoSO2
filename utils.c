
#include "utils.h"

int mod(int a, int b)
{
        while(a>=b)
                a=a-b;
        return a;
}     

int mystrlen(char *cadena){
    int i = 0;

    while(cadena[i] !=  '\0'){
        i++;
    }
    return i;
}

char * mistrcat(char * destino,char * fuente)
{
 int i=0, j=0, pos = 0, len = mystrlen(destino) + mystrlen(fuente);
 char * temp = malloc(len);
 
    while(destino[i])
    {
         temp[i] = destino[i];
         i++;
    }
  
 while(fuente[j])
 {
  temp[i]=fuente[j];
  i++;
  j++;
 }
 
  destino = malloc(len);
  for(pos;pos<len;pos++)
  destino[pos] = temp[pos];
 return destino;
}

char * strcpy( char *to,char* from)
{
    int i=0;
    while (to[i]!='\0')
    {
       from[i]=to[i];
       i++;
    }
  return from;
}

int strCmp (const char *s1, const char *s2)
{
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;

    while (*p1 != '\0') {
        if (*p2 == '\0') return  1;
        if (*p2 > *p1)   return -1;
        if (*p1 > *p2)   return  1;

        p1++;
        p2++;
    }

    if (*p2 != '\0') return -1;

    return 0;
}

int stlen(char * cadena, int start, char delim)
{
    int count = 0, pos = (cadena[start]==delim) ? start+1 : start;

    while(cadena[pos]!=delim && cadena[pos]!='\0'){
        count++;
        pos++;
    }

    return count;
}



int qtytoken(char * token, char delim)
{
    int totalTokens = 1;
    int pos = 0;

    while(token[pos]!='\0')
    {
        if(token[pos]==delim){
        totalTokens++;
        }
        pos++;
    }
    return totalTokens;
}

char * strtok(char * str, char delim, int pos)
{
    
    int totalTokens = qtytoken(str,delim), count = 1, posStr = 0;
    char * token = (char*)malloc(stlen(str, posStr, delim));
    int posToken = 0;

while(str[posStr]!='\0')
    {
        if(str[posStr]==delim)
        {
            int len = stlen(str, posStr, delim);
            count++;
            token =((count<pos)) ? (char*)malloc(stlen(str, posStr, delim)) : token;
        }
        else if(count==pos){
            token[posToken] = str[posStr];
            posToken++;
        }
        posStr++;
    }   
    return token;
}
