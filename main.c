#include <linux/init.h>
#include <linux/module.h>

#include <linux/fs.h>

MODULE_LICENSE("Dual BSD/GPL");

int major_number = 0;

static int hello_init( void )
{
    int code;

    printk( KERN_ALERT "Hello, world\n" );

	// устройство будет видно в /proc/devices
	// если передать 0, то новый major number возвратится и 
	// устройство будет зарегистрировано под этим номером
    code = register_blkdev( major_number, "wikipedia" );
    printk( KERN_ALERT "register_blkdev(0,\"wikipedia\") => %d", code );
    if( code < 0 ) // ошибка
    {
    	// TODO:
    }
    else if( code > 0 )
    {
    	major_number = code;
    }
    else // if( code == 0 )
    {
    	// TODO:
    }
    
    return 0;
}

static void hello_exit( void )
{
	unregister_blkdev( major_number, "wikipedia" );
	printk( KERN_ALERT "unregister_blkdev( %d, \"wikipedia\" )", major_number );
    printk( KERN_ALERT "Goodbye, cruel world\n" );
}

module_init( hello_init );
module_exit( hello_exit );
