#include <stdio.h>
#include <time.h>

int fib(int n){
  if(n < 3){
    return 1;
  }

  return fib(n-1) + fib(n-2);
}

int main(){
  clock_t start_time;
  double elapsed_time;
  
  start_time = clock();
  fib(20);
  elapsed_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
  printf("fib 20 took %f seconds\n", elapsed_time);

  start_time = clock();
  fib(21);
  elapsed_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
  printf("fib 21 took %f seconds\n", elapsed_time);

  start_time = clock();
  fib(22);
  elapsed_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
  printf("fib 22 took %f seconds\n", elapsed_time);

  start_time = clock();
  fib(23);
  elapsed_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
  printf("fib 23 took %f seconds\n", elapsed_time);

  start_time = clock();
  fib(24);
  elapsed_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
  printf("fib 24 took %f seconds\n", elapsed_time);

  start_time = clock();
  fib(25);
  elapsed_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
  printf("fib 25 took %f seconds\n", elapsed_time);

  start_time = clock();
  fib(26);
  elapsed_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
  printf("fib 26 took %f seconds\n", elapsed_time);

  start_time = clock();
  fib(27);
  elapsed_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
  printf("fib 27 took %f seconds\n", elapsed_time);

  start_time = clock();
  fib(28);
  elapsed_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
  printf("fib 28 took %f seconds\n", elapsed_time);

  start_time = clock();
  int fib_list[7] = {21, 22, 23, 24, 25, 26, 27};
  for(int i = 0; i < 7; i++){
    fib(fib_list[i]);
  }
  elapsed_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
  printf("fib 21-27 took %f seconds\n", elapsed_time);
  
}
