#include <linux/init.h>
#include <linux/module.h>

#include <linux/blkdev.h> // block_device_operations
#include <linux/genhd.h>  // gendisk
#include <linux/fs.h>     // register_blkdev, unregister_blkdev
#include <linux/hdreg.h>  // HDIO_GETGEO

MODULE_LICENSE("Dual BSD/GPL");

int major_number = 0;
module_param( major_number, int, 0 );

struct gendisk * gendisk = NULL;

spinlock_t hello_lock;// = SPIN_LOCK_UNLOCKED;

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
	struct hd_geometry geo;

	switch( cmd )
	{
		case HDIO_GETGEO:
			geo.cylinders=1;
			geo.heads=1;
			geo.sectors=8;
			geo.start=1;
			if( copy_to_user( (void __user *) arg, &geo, sizeof( geo ) ) )
				return -EFAULT;
			return 0;
	}

	return -ENOTTY;
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

int make_request( struct request_queue * queue, struct bio * bio )
{
	char * data;
	int i;
	printk( KERN_NOTICE "bi_sector: %ld, bi_size: %d", bio->bi_sector, bio->bi_size );
	data = bio_data( bio );
	for( i=0; i<bio->bi_size/2; i++ )
	{
		((u16*) data)[i] = bio->bi_sector * KERNEL_SECTOR_SIZE + i*2;
	}
	bio_endio( bio, 0 );
	return 0;
}

/*void request( struct request_queue * queue )
{
	struct request * req;
	bool code;
	
	while(1)
	{
		spin_lock( &hello_lock );
		req = blk_fetch_request( queue );
		if( req != NULL )
		{
			printk( KERN_NOTICE "new request" );
			//blk_abort_request( req );
			printk( KERN_NOTICE "bi_sector: %ld, bi_size: %d", req->bio->bi_sector, req->bio->bi_size );
//			blk_start_request( req );
			
			code = blk_end_request( req, 0, 4096 );
			printk( KERN_NOTICE "blk_end_request code: %d", code );
			spin_unlock( &hello_lock );
		}
		else
		{
			spin_unlock( &hello_lock );
			break;
		}
	}
}*/

static int hello_init( void )
{
    int code;

    printk( KERN_ALERT "Hello, world\n" );
    
    spin_lock_init( &hello_lock );

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
		blk_queue_physical_block_size( gendisk->queue, 4096 );
		// TODO: queue == NULL
		blk_queue_make_request( gendisk->queue, make_request );
		spin_lock_init( &hello_lock );
		//gendisk->queue = blk_init_queue( &request, &hello_lock );
		gendisk->flags = 0;
    	set_capacity( gendisk, 65536 / KERNEL_SECTOR_SIZE );
    	gendisk->private_data = NULL;
    }
    printk( KERN_NOTICE "add_disk( gendisk )..." );
    add_disk( gendisk );
    
    return 0;
  bad:
    return -ENOMEM;
}

//extern void unlink_gendisk(struct gendisk *gp);

static void hello_exit( void )
{
//	unlink_gendisk( gendisk );
	//blk_cleanup_queue( gendisk->queue );
	del_gendisk( gendisk );
	unregister_blkdev( major_number, "wikipedia" );
	printk( KERN_ALERT "unregister_blkdev( %d, \"wikipedia\" )", major_number );
    printk( KERN_ALERT "Goodbye, cruel world\n" );
}

module_init( hello_init );
module_exit( hello_exit );
