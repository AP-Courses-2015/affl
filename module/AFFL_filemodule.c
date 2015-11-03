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
#include <linux/seq_file.h>
#include "AFFL_filemodule.h"
#define PROCFS_NAME 		"blist"
#define BlackList "/home/mizantrop/C/apriorit_project/module/AFFL_blacklist"
#define MYDIRPROC "Firewall"
#define MYPROC "/proc/Firewall/blist"

#define DEBUG

/**
 * This structure hold information about the /proc file
 *
 */
static struct proc_dir_entry *Our_Proc_Dir;
static struct proc_dir_entry *Our_Proc_File;
static struct file *fileproc;
/**
 * The buffer used to store character for this module
 *
 */
static char* procfs_buffer;
static loff_t last_pos;
/**
 * The size of the buffer
 *
 */
char *buf;
/** 
 * This function is called then the /proc file is read
 *
 */
 
int read_callback(struct seq_file *m, void *v)
{
        seq_printf(m,"%s\n",procfs_buffer);
        return 0;
}


/**
 * This function is called with the /proc file is written
 *
 */

size_t write_callback(struct file *file, const char * buffer, size_t count, int* pos) 
{	
	int res;
	if(copy_from_user(&procfs_buffer[*pos], buffer, count))
	{
                return -EFAULT;
	}
	
	if(*pos!=0)
	{
		res=(int) (count+(*pos));
		procfs_buffer[res]='\n';
		procfs_buffer[res+1]='\0';
		last_pos=res+1;
	}
        return count;
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
	struct file *fileread;
	unsigned long count;
	mm_segment_t fs;
	loff_t pos;
	size_t buf_len=4096;
	fileread=filp_open(BlackList,O_RDWR|O_CREAT,0644);
	if (IS_ERR(fileread))
	{
   		return -EBADF;
	}
#ifdef DEBUG
	else
	  printk(KERN_NOTICE "AFFL_filemodule notice: blacklist opened success\n");
#endif
	buf = (char*) kmalloc(buf_len, GFP_KERNEL);
	if (buf == NULL)
	{
 	   filp_close(fileread, 0);
  	   return -ENOMEM;
	} 
#ifdef DEBUG
	else
	  printk(KERN_NOTICE "AFFL_filemodule notice: buffer alocated success\n");
#endif
	Our_Proc_Dir = proc_mkdir(MYDIRPROC,NULL);
	Our_Proc_File = proc_create(PROCFS_NAME, 0644, Our_Proc_Dir,&proc_file_fops);
	
	if (Our_Proc_File == NULL) {
		remove_proc_entry(PROCFS_NAME, NULL);
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",
			PROCFS_NAME);
		return -ENOMEM;
	}
#ifdef DEBUG
	else
	  printk(KERN_NOTICE "AFFL_filemodule notice: procfs file created success\n");
#endif
	fs = get_fs();
	set_fs(KERNEL_DS);
	pos=0;
	count=vfs_read(fileread,buf,buf_len,&pos);
	buf[pos]='\0';
	last_pos=pos;
	
	fileproc=filp_open(MYPROC,O_RDWR,0);
	if (IS_ERR(fileproc))
	{
		printk("File don't open in /proc\n");
   		return -EBADF;
	}
#ifdef DEBUG
	else
	  printk(KERN_NOTICE "AFFL_filemodule notice: procfs file opened success\n");
#endif
	procfs_buffer = (char*) kmalloc(buf_len, GFP_KERNEL);
	if (procfs_buffer == NULL)
	{
 	   filp_close(fileproc, 0);
  	   return -ENOMEM;
	} 
#ifdef DEBUG
	else
	  printk(KERN_NOTICE "AFFL_filemodule notice: procfs buffer allocated success\n");
#endif
	pos=0;
	vfs_write(fileproc,buf,buf_len,&pos);
	kfree(buf);
	filp_close(fileread,0);
	set_fs(fs);	
	
	return 0;
}

int findProcInBlackList(const char*name)
{
	int pos;
	int start;
	int res;
	pos=0;
	start=0;
#ifdef DEBUG
	printk(KERN_NOTICE "AFFL_filemodule notice: try to find proc named %s\n", name);
#endif
	while(procfs_buffer[pos]!='\0')
	{
		if(procfs_buffer[pos]=='\n')
		{
			res=strncmp(name,&procfs_buffer[start],pos-start);
			if(res==0)
			{
			   printk("found:%i",start);
#ifdef DEBUG
			   printk(KERN_NOTICE "AFFL_filemodule notice: process found!\n");
#endif
			   return start;
			}
			start=pos+1;
		}
		pos++;
	}
#ifdef DEBUG
	printk(KERN_NOTICE "AFFL_filemodule notice: process not found!\n");
#endif
	return -1;
}


int releaseBlackList(void)
{
	filp_close(fileproc,0);
	remove_proc_entry(PROCFS_NAME, Our_Proc_Dir);
	remove_proc_entry(MYDIRPROC,NULL);
	kfree(procfs_buffer);
	
#ifdef DEBUG
	printk(KERN_NOTICE "AFFL_filemodule notice: blacklist released\n");
#endif
	return 1;
}
EXPORT_SYMBOL(initBlackList);
EXPORT_SYMBOL(findProcInBlackList);
EXPORT_SYMBOL(releaseBlackList);
MODULE_LICENSE("GPL");
