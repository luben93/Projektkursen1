#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "stringx.h"

void reverse(char *s) {
 char tmp; 
 int i, length = strlen(s);
 	for(i=0;i<length/2;i++) {
 	tmp=s[i]; 
 	s[i]=s[length-1-i]; 
 	s[length-1-i]=tmp;
	}
}

void sort(char *s)
{
 char tmp; 
 int i,j, length = strlen(s);
 for(i=0;i<length;i++)
 for(j=i+1;j<length;j++)
 if(s[i]>s[j]){
 	tmp=s[i];
 	s[i]=s[j];
 	s[j]=tmp;
	}
}

void mix(char *s)
{
 char tmp; 
 int i,j, length = strlen(s);
 srand(time(0));
 for(i=0;i<length;i++){
 	for(j=i+1;j<length;j++){
 		if(rand()%2==0){
 		tmp=s[i];
 		s[i]=s[j];
 		s[j]=tmp;
 		}
	}
 }
}
