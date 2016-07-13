#include <linux/module.h>

int init_module(void)
{
  printk("HelloWorld - NEXELL~\n");
  return 0;
}

void cleanup_module(void)
{
  printk("Goodbye~\n");
}

MODULE_LICENSE("GPL");
