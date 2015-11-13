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
#define MAX_STR_LEN 255
#define PROCFS_NAME 		"blist"
#define MYDIRPROC "affl"
#define MYPROC "/proc/affl/blist"

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

TBlackList mlist;


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
int findProcAndDel(long pos)
{
	TBlackList *elem;
	long counter=0;
	list_for_each_entry(elem, &mlist.list, list)
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
        seq_printf(m,"%s\n",procfs_buffer);
        return 0;
}


/**
 * This function is called with the /proc file is written
 *
 */

size_t write_callback(struct file* file, const char __user* buffer, size_t count, int* pos)
{
	TBlackList *elem;
	long res;
	int i=0;
	char mystr[10];
	buf=(char*) kmalloc(sizeof(char)*MAX_STR_LEN,GFP_USER);
	
	if(buffer[0]!='k')
	{
		buf[count]='\0';
		memcpy(buf,buffer,count);
		elem=(TBlackList*)kmalloc(sizeof(TBlackList),GFP_KERNEL);
		elem->inf=buf;
		//INIT_LIST_HEAD(&elem->list);
		list_add_tail(&(elem->list),&(mlist.list));
		printk(KERN_INFO "add: %s", elem->inf);
	}
	else
	{	while(i<=count)
		{
			mystr[i] = buffer[i+1];
			i++;
		}
		mystr[count-1]='\0';
		kstrtol(mystr,10,&res);
		if(findProcAndDel(res)==1)
			printk(KERN_INFO "Element deleted");
	}
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
	INIT_LIST_HEAD(&mlist.list); 

	return 0;
}



int findProcInBlackList(const char*name)
{
	TBlackList *elem;
	list_for_each_entry(elem, &mlist.list, list)
    	{
		if(strcmp(elem->inf,name) == 0)
		{
			printk(KERN_INFO "found string %s",name);
			return 0;
		}
		
	}
	return -1;
}

int releaseBlackList(void)
{
	TBlackList *elem, *tmp;
	list_for_each_entry_safe(elem, tmp, &mlist.list, list)
    	{
         	printk(KERN_INFO "freeing node %s\n", elem->inf);
        	 list_del(&elem->list);
        	 kfree(elem);
  	 }
	remove_proc_entry(PROCFS_NAME, Our_Proc_Dir);
	remove_proc_entry(MYDIRPROC,NULL);
	kfree(procfs_buffer);
	kfree(buf);
	return 1;
}

EXPORT_SYMBOL(initBlackList);
EXPORT_SYMBOL(findProcInBlackList);
EXPORT_SYMBOL(releaseBlackList);
MODULE_LICENSE("GPL");
