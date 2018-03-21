#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>



void davidson(int N,double **v,double a,double b,int nev,int first_ev);


#define NCHAR 1024

#define DELIM   " \n" 
#define MAXWORD 50
#define MAXLEN  512
typedef struct {
  char line[NCHAR];
  int ntokens;
  char words[MAXWORD][MAXLEN];
} Line;
Line *read_line(char *line){
  Line *sline;sline=malloc(sizeof(Line));   sprintf(sline->line,"%s",line);
  char *sep=NULL;  sep=strtok(line,DELIM);

  sline->ntokens=0;
  while (sep != NULL){
    strcpy(sline->words[sline->ntokens++], sep);
    sep = strtok(NULL, DELIM);
  }
  free(sep);
  return sline;
}


/* --------------------------------------------------------------------------------------------------------------
   Davidson algorithm. Variable names according Saad's book "Numerical Methods For Large Eigenvalue Problems", p203
   -------------------------------------------------------------------------------------------------------------- */
int main(int nargument,char **argument){

  /* INPUT
     - dimension N of the function/vector to compute and given by a the partial differential equation d^2Phi/dx^2 + gamma(x) dPhi/dx + V(x).Phi(x) = lambda.Phi(x)
    -  vectors V(x) (potential for example) and gamma(x) (damping for example)
    - initial guess vector
  */

  int N=200;
  double L=M_PI;
  double dx,dx2;
  dx=L/(N+1);
  dx2=dx*dx;
  double a,b;a=1.0/dx2;b=-.5/dx2;
  int nev;nev=4;
  int first_ev;first_ev=0;
  int nvecini;nvecini=first_ev+nev;

  int i,j,k,l;
  double **v;v=malloc(nvecini*sizeof(double*)); for(i=0;i<nvecini;i++) v[i]=malloc(N*sizeof(double));
  double norm;
  double *alpha;
  /* --------------------------------------------------------
     first, one initializes the first vectors of the subspace
     In the present version, They are set randomly.
     They are also normalized.
  ----------------------------------------------------------*/
  for(j=0;j<nvecini;j++){
    norm=0.0;
    for(i=0;i<N;i++) {
      v[j][i]=1.0*rand()/RAND_MAX;
      norm+=v[j][i]*v[j][i];
    }
    norm=pow(norm,.5);
    for(i=0;i<N;i++)     v[j][i]/=norm;
    norm=0.0;  for(i=0;i<N;i++)     norm+=v[j][i]*v[j][i];
    norm=pow(norm,.5);
  }
  if(nvecini>1){		/* Gram-Schmidt process if the iitial size of the subspace is larger than 1*/
    alpha=malloc((nvecini-1)*sizeof(double));
    for(j=1;j<nvecini;j++){
      for(k=0;k<j;k++) {
	alpha[k]=0.0;
	for(l=0;l<N;l++) alpha[k]+=v[k][l]*v[j][l];
      }
      for(k=0;k<j;k++) {
	for(l=0;l<N;l++) v[j][l]-=alpha[k]*v[k][l];
      }
      norm=0.0;
      for(l=0;l<N;l++) norm+=v[j][l]*v[j][l];
      norm=pow(norm,.5);
      for(l=0;l<N;l++) v[j][l]/=norm;
    }
  }
  free(alpha);


  davidson(N,v,a,b,nev,first_ev);

  
  printf("JOB DONE !\n");

  return 0;
}