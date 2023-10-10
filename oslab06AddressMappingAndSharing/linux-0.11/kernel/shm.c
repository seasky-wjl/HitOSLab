#define __LIBRARY__
#include <unistd.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <errno.h>
#include <linux/shm.h>

static shm_ds shm_list[SHM_SIZE] = {{0,0,0}};   /*整个数组的全部元素都初始化为0*/

int sys_shmget(unsigned int key, size_t size)
{
    int i;
    void  *page;
    if(size > PAGE_SIZE || key == 0)
        return -EINVAL;
    for(i = 0; i < SHM_SIZE; i++)   /* 如果key存在，直接返回共享内存的俄id */
    {   
        if(shm_list[i].key == key)
        {
            printk("Find previous shm key:%u\n", shm_list[i].key);
            return i;
        }
    }
    page = get_free_page(); /*get_free_page中会将mem_map相应位置u置为1*/
    /*需要将mem_map相应位置清零，因为在sys_shmat才会将申请的物理也和虚拟地址关联增加饮用次数*/
    decrease_mem_map(page);

    if(!page)
        return -ENOMEM;
    printk("Shmget get memory's address is 0x%08x\n",page);
    for(i = 0;i < SHM_SIZE; i++)    /*找到空闲的共享内存*/
    {
        if(shm_list[i].key == 0)
        {
            shm_list[i].page = page;
            shm_list[i].key = key;
            shm_list[i].size = size;
            printk("Generate a new shm key:%u\n", shm_list[i].key);
            return i;
        }
        return -1;
    }

}

void *sys_shmat(int shmid)
{
    if(shmid < 0 || SHM_SIZE <= shmid || shm_list[shmid].page == 0 || shm_list[shmid].key == 0)
        return (void *)-EINVAL;
    
    /*建立物理地址和线性地址的映射（前20位）*/
    /* current->brk 和 current->start_code都是4kb对齐的*/
    printk("current->brk：0x%08x,current->start_code:0x%08x\n",current->brk,current->start_code);
    put_page(shm_list[shmid].page,current->brk + current->start_code);

    /* 需要增加一次共享物理页的引用次数，否则会在free_page中panic死机*/
    increase_mem_map(shm_list[shmid].page);
    current->brk += PAGE_SIZE;
    return (void *)(current->brk - PAGE_SIZE);
}

long sys_get_jiffies()
{
    return jiffies;
}