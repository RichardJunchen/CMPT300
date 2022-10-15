#include <linux/kernel.h>
#include <linux/syscalls.h>
struct array_stats_s { 
    long min;
    long max;
    long sum; 
};
SYSCALL_DEFINE3(array_stats,struct array_stats_s*,stats,long*,data,long,size)
{
      // your code here...
    printk("Change into the kernel level \n");
    struct array_stats_s local_stats;
    struct array_stats_s* temp_stats;
    long check_number;
    int temp_size;
    int i=0;
    temp_stats = &local_stats;
    if (size<=0){				// if size not correct then return -EINVAL
    	printk(" The size is a invalid digit \n");
        return -EINVAL;
    }
    if (copy_from_user(&temp_stats->min,&stats->min,sizeof(long)))
        return -EFAULT;
    if (copy_from_user(&temp_stats->max,&stats->max,sizeof(long)))
        return -EFAULT;
    if (copy_from_user(&temp_stats->sum,&stats->sum,sizeof(long)))
        return -EFAULT; 
    
    temp_size = size;
    if (temp_stats==NULL)		      // if struct invaild, return -EFAULT
        return -EFAULT;
    if (copy_from_user(&check_number , &data[0], sizeof(long)))
    	return -EFAULT;
    temp_stats->min=check_number;	      // initialize the struct
    temp_stats->max=check_number;
    temp_stats->sum=0;
    
    for (i=0;i<size;i++){			// start doing comparsion and addition
    	if (copy_from_user(&check_number , &data[i] , sizeof(long)))
    		return -EFAULT;
        if (check_number<=temp_stats->min)
            temp_stats->min=check_number;
        if (check_number>=temp_stats->max)
            temp_stats->max=check_number;
        temp_stats->sum+=check_number;
    }
    printk(" Finish operation... And the max value in the array is %ld \n",temp_stats->max);
    printk(" The min value in the array is %ld \n",temp_stats->min);
    printk(" The total sum of the array is %ld \n",temp_stats->sum);
    
    if (copy_to_user(&stats->min,&temp_stats->min,sizeof(long)))
        return -EFAULT;
    if (copy_to_user(&stats->max,&temp_stats->max,sizeof(long)))
        return -EFAULT;
    if (copy_to_user(&stats->sum,&temp_stats->sum,sizeof(long)))
        return -EFAULT;
    printk("\n Finish everything and return to the user level \n");
    return 0;
}
