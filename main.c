#include <linux/init.h>
#include <linux/module.h>

#include <linux/fs.h>

MODULE_LICENSE("Dual BSD/GPL");

static int hello_init( void )
{
    int code;

    printk( KERN_ALERT "Hello, world\n" );

	// устройство будет видно в /proc/devices
    code = register_blkdev( 137, "wikipedia" );
    printk( KERN_ALERT "register_blkdev(137,\"wikipedia\") => %d", code );
    if( code < 0 ) // ошибка
    {
    	// TODO:
    }
    
    return 0;
}

static void hello_exit( void )
{
    printk( KERN_ALERT "Goodbye, cruel world\n" );
}

module_init( hello_init );
module_exit( hello_exit );
