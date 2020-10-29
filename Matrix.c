#include <stdlib.h>
#include <stdio.h>
#include <omp.h>


# ifndef MIN
# define MIN(x,y) ((x)<(y)?(x):(y))
# endif

# define DATA_N (1024*1024*64*3)

static double  MEM_CHUNK[DATA_N];

void Matrix_Init_col(int N, double *a, double *b, double *c)
{
  int j,k;

  for (k=0; k<N; k++)
    for (j=0; j<N; j++) {
      a[j+k*N] = 2.0*(j+k);
      b[j+k*N] = 3.2*(j-k);
      c[j+k*N] = 1.0;      
    }
}

void test(int N, double *a, double *b, double *c)
{
  int j,k;

printf("A: \n");
  for (k=0; k<N; k++){
    for (j=0; j<N; j++) {
      a[j+k*N] = j+k;
      b[j+k*N] = j+k;
      c[j+k*N] = 0.0;
      printf("%f ", a[j+k*N]);
    }
  printf("\n");
  }
  printf("B:\n");
  for (k=0; k<N; k++){
    for (j=0; j<N; j++) {
      a[j+k*N] = j+k;
      b[j+k*N] = j+k;
      c[j+k*N] = 0.0;
      printf("%f ", b[j+k*N]);
    }
  printf("\n");
  }
}

void print_matrix(int N, double *a, double *b, double *c)
{
  int j,k;

printf("C: \n");
  for (k=0; k<N; k++){
    for (j=0; j<N; j++) {
      printf("%f;", c[j+k*N]);
    }
  }
}

int main (int argc, char **argv)
{
  int N; 

  if(argc <= 2){
    printf("client sizeMatrix nThreads\n");
    return -1;
  }

  N = (int)atof(argv[1]);
  argc--;
  argv++;

  if (N > 1024*8)
  {
    printf("Unvalid MatrixSize\n");
    return -1;
  }

  argsInit(argc, argv);

#pragma omp parallel
{

  int     i, j, k, SZ = N;
  double  *a, *b, *c;
  a = MEM_CHUNK;
  b = a + SZ*SZ;
  c = b + SZ*SZ;
  int num_thread;

  num_thread = mpParallel();
 
  #pragma omp master
    Matrix_Init_col(SZ, a, b, c);    

 
  mpStart(num_thread);

 
#pragma omp for
  for (i=0; i<SZ; i++){
    for (j=0; j<SZ; j++) {
      double *pA, *pB, S;
      S=0.0;
      pA = a+(i*SZ); pB = b+j;
      for (k=SZ; k>0; k--, pA++, pB+=SZ)
        S += (*pA * *pB);
      c[i*SZ+j]= S;
    }
  }
 
  mpStop(num_thread);
  
}
  mpEnd();
  return 0;
}