#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <errno.h>    

#define _ASS2_Test_ 549
#define SIZE 10
struct array_stats_s{
    long min;
    long max;
    long sum;
};
int main(int argc, char * argv[])
{
    int index=0;
    struct array_stats_s* stats;
    long *array = (long*)malloc(10*sizeof(long));
    for (int i = 0;i < SIZE;i++)
    	array[i] = i*2;
    printf(" Total %d numbers are : \n",SIZE);
    for (int i = 0;i< SIZE; i++)
    	printf("%ld \t",array[i]);
    printf ("\n Changing into the kernel level \n\n");
    int result = syscall(_ASS2_Test_,stats,array,SIZE);
    int errorCode = errno;
    if (result<0){
    	printf("\n the user level result =%d (error #%d)\n",result, errorCode);
    	printf(" Wrong!!!! Please check all input again! \n");
    }
    else {
    	printf("\n the user level result = %d  (error #%d)\n",result,errorCode);
    	printf(" The minimum value is = %ld\n", stats->min);
    	printf(" The maximum value is = %ld\n", stats->max);
    	printf(" Tum Sum Value is = %ld\n", stats->sum);
    }
    return 0;
}
