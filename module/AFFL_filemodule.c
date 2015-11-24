/**
 *  procfs2.c -  create a "file" in /proc
 *
 */

#include <linux/module.h>	/* Specifically, a module */
#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/proc_fs.h>	/* Necessary because we use the proc fs */
#include <asm/uaccess.h>	/* for copy_from_user */
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/seq_file.h>
#include "md5.h"
#define MAX_STR_LEN 255
#define PROCFS_NAME 		"blist"
#define MYDIRPROC "affl"
#define MYPROC "/proc/affl/blist"
#define HASH_VALUE_SIZE 32

#define DEBUG

/**
 * This structure hold information about the /proc file
 *
 */



static struct proc_dir_entry *Our_Proc_Dir;
static struct proc_dir_entry *Our_Proc_File;

typedef struct TBlackList
{
	 char* inf; 
	 struct list_head list; /* kernel's list structure */
}TBlackList;

TBlackList mlist[2];


char *buf;

/**
 * The buffer used to store character for this module
 *
 */
static char* procfs_buffer;

/** 
 * This function is called then the /proc file is read
 *
 */
int findProcAndDel(long pos,TBlackList mlist)
{
	
	long counter=0;
	
	TBlackList *elem, *tmp;
	list_for_each_entry_safe(elem, tmp, &mlist.list, list)
    	{
    		
			if(pos==counter)
			{
			 printk(KERN_INFO "deleted %s", elem->inf);
			 list_del(&elem->list);
        		 kfree(elem);
			 return 1;
			}
		counter++;	
		}
	
	return 0;
} 


int read_callback(struct seq_file *m, void *v)
{
        TBlackList *elem;
	list_for_each_entry(elem, &mlist[0].list, list)
    	{
		
		
		seq_printf(m,"%s\n",elem->inf);

	}
        return 0;
}



/**
 * This function is called with the /proc file is written
 *
 */

size_t write_callback(struct file* file, const char __user* buffer, size_t count, int* pos)
{
	TBlackList *elem,*elem2;
	long res;
	
	int i=0;
	char hash_value[HASH_VALUE_SIZE + 1];
	char mystr[10];
	buf=(char*) kmalloc(sizeof(char)*MAX_STR_LEN,GFP_USER);
	buf[count]='\0';
	memcpy(buf,buffer,count);
	
	elem=(TBlackList*)kmalloc(sizeof(TBlackList),GFP_KERNEL);
	elem2=(TBlackList*)kmalloc(sizeof(TBlackList),GFP_KERNEL);
	
	switch(buffer[0])
	{
	case 'k':
		while(i<=count)
		{
			mystr[i] = buffer[i+1];
			i++;
		}
		mystr[count-1]='\0';
		kstrtol(mystr,10,&res);
		if(findProcAndDel(res,mlist[0])==1 && findProcAndDel(res,mlist[1])==1)
			printk(KERN_INFO "Element deleted");
	break;
	case '/':
		
		elem->inf=(char*) kmalloc(sizeof(char)*MAX_STR_LEN,GFP_KERNEL);
		memcpy(elem->inf,buf,count);
		
		if(makeHash(elem->inf, hash_value)!=(-1))
		{
			elem2->inf=(char*) kmalloc(sizeof(char)*HASH_VALUE_SIZE,GFP_KERNEL);
			if(elem2->inf == NULL)
			{
			kfree(elem2);
			return -1;
			}
		list_add_tail(&(elem->list),&(mlist[0].list));
		printk(KERN_INFO "add path: %s", elem->inf);
		hash_value[HASH_VALUE_SIZE] = '\0';
		memcpy(elem2->inf,hash_value,(HASH_VALUE_SIZE+1));
		printk(KERN_INFO "add hash: %s", elem2->inf);
		list_add_tail(&(elem2->list),&(mlist[1].list));
		}
		else
		{
			printk(KERN_INFO "Error in makeHash \n");
			return -1;
		}
	break;
	default:
	elem2->inf=(char*) kmalloc(HASH_VALUE_SIZE,GFP_KERNEL);
	if(elem2->inf == NULL)
	{
		kfree(elem2);
		return -1;
	}
	memcpy(hash_value,buf,(HASH_VALUE_SIZE));
	hash_value[HASH_VALUE_SIZE] = '\0';
	memcpy(elem2->inf,hash_value,(HASH_VALUE_SIZE+1));
	list_add_tail(&(elem2->list),&(mlist[1].list));
	printk(KERN_INFO "add hash: %s", elem2->inf);
	elem->inf=(char*) kmalloc(sizeof(char)*MAX_STR_LEN,GFP_KERNEL);
	memcpy(elem->inf,&buf[HASH_VALUE_SIZE],count - (HASH_VALUE_SIZE-1));
	printk(KERN_INFO "add path: %s", elem->inf);
	list_add_tail(&(elem->list),&(mlist[0].list));
	
	}
      kfree(buf);
     return count; // Return number of bytes passed by the user
}

int open_callback(struct inode *inode, struct file *file){
        return single_open(file, read_callback, NULL);
}
static const struct file_operations proc_file_fops = {
        .owner = THIS_MODULE,
        .read = seq_read,
        .write = write_callback,
	.open=open_callback,
};
/**
 *This function is called when the module is loaded
 *
 */


int initBlackList(void)
{
	size_t buf_len=4096;
	
	Our_Proc_Dir = proc_mkdir(MYDIRPROC,NULL);
	Our_Proc_File = proc_create(PROCFS_NAME, 0666, Our_Proc_Dir,&proc_file_fops);
	
	if (Our_Proc_File == NULL) 
	{
		remove_proc_entry(PROCFS_NAME, NULL);
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",
			PROCFS_NAME);
		return -ENOMEM;
	}

	procfs_buffer = (char*) kmalloc(buf_len, GFP_KERNEL);
	if (procfs_buffer == NULL)
	{
  	   return -ENOMEM;
	} 
	INIT_LIST_HEAD(&mlist[0].list); 
	INIT_LIST_HEAD(&mlist[1].list); 

	return 0;
}



int findProcInBlackList(const char*name)
{
	TBlackList *elem;
	char hash_value[HASH_VALUE_SIZE + 1];
	makeHash(name, hash_value);
	list_for_each_entry(elem, &mlist[1].list, list)
    	{
		
		if(strcmp(elem->inf,hash_value) == 0)
		{
			printk(KERN_INFO "found string %s",name);
			return 0;
		}
	}
	
	printk(KERN_INFO "don't found string %s",name);
	return -1;
}

int releaseBlackList(void)
{
	TBlackList *elem, *tmp,*elem1,*tmp1;
	list_for_each_entry_safe(elem, tmp, &mlist[0].list, list)
    	{
         	printk(KERN_INFO "freeing node %s\n", elem->inf);
        	 list_del(&elem->list);
        	 kfree(elem);
  	 }
	list_for_each_entry_safe(elem1, tmp1, &mlist[1].list, list)
    	{
         	printk(KERN_INFO "freeing node %s\n", elem1->inf);
        	 list_del(&elem1->list);
        	 kfree(elem1);
  	 }
	remove_proc_entry(PROCFS_NAME, Our_Proc_Dir);
	remove_proc_entry(MYDIRPROC,NULL);
	kfree(procfs_buffer);
	
		
	return 1;
}

EXPORT_SYMBOL(initBlackList);
EXPORT_SYMBOL(findProcInBlackList);
EXPORT_SYMBOL(releaseBlackList);
MODULE_LICENSE("GPL");
