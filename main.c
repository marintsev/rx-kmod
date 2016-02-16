#include <linux/init.h>
#include <linux/module.h>

#include <linux/blkdev.h> // block_device_operations
#include <linux/genhd.h>  // gendisk
#include <linux/fs.h>     // register_blkdev, unregister_blkdev

MODULE_LICENSE("Dual BSD/GPL");

int major_number = 0;
module_param( major_number, int, 0 );

struct gendisk * gendisk = NULL;

#define KERNEL_SECTOR_SIZE	512

int wiki_open( struct block_device * dev, fmode_t mode )
{
	return 0;
}

int wiki_release( struct gendisk * gd, fmode_t mode )
{
	return 0;
}

int wiki_ioctl( struct block_device * dev, fmode_t mode, unsigned int cmd, unsigned long arg )
{
	return 0;
}

int wiki_media_changed( struct gendisk * gd )
{
	return 0;
}

int wiki_revalidate_disk( struct gendisk * gd )
{
	return 0;
}

struct block_device_operations fops = 
{
	.open = wiki_open,
	.release = wiki_release,
	.ioctl = wiki_ioctl,
	.media_changed = wiki_media_changed,
	.revalidate_disk = wiki_revalidate_disk,
	.owner = THIS_MODULE
};

static int hello_init( void )
{
    int code;

    printk( KERN_ALERT "Hello, world\n" );

	// устройство будет видно в /proc/devices
	// если передать 0, то новый major number возвратится и 
	// устройство будет зарегистрировано под этим номером
    code = register_blkdev( major_number, "wikipedia" );
    printk( KERN_ALERT "register_blkdev(0,\"wikipedia\") => %d", code );
    if( code <= 0 ) // ошибка
    {
    	if( major_number == 0 )
    		goto bad;
    }
    else if( code > 0 )
    {
    	major_number = code;
    }
    
    gendisk = alloc_disk( 4 );
    if( gendisk == NULL )
    {
    	printk( KERN_ALERT "gendisk == NULL." );
    	goto bad;
    }
    else
    {
    	gendisk->major = major_number;
    	gendisk->first_minor = 0;
    	gendisk->minors = 4;
		snprintf( gendisk->disk_name, 32, "wiki_gc" );
		gendisk->fops = &fops;
		gendisk->queue = blk_alloc_queue( GFP_KERNEL );
		gendisk->flags = 0;
    	set_capacity( gendisk, 4096 / KERNEL_SECTOR_SIZE );
    	gendisk->private_data = NULL;
    }
    printk( KERN_NOTICE "add_disk( gendisk )..." );
    add_disk( gendisk );
    
    return 0;
  bad:
    return -ENOMEM;
}

static void hello_exit( void )
{
	unregister_blkdev( major_number, "wikipedia" );
	printk( KERN_ALERT "unregister_blkdev( %d, \"wikipedia\" )", major_number );
    printk( KERN_ALERT "Goodbye, cruel world\n" );
}

module_init( hello_init );
module_exit( hello_exit );
