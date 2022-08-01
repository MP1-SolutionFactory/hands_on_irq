# Makefile for simple external out-of-tree Linux kernel module example

# Object file(s) to be built
obj-m := openstlinux-hands-on-irq-driver.o

# Path to the directory that contains the Linux kernel source code
# and the configuration file (.config)
KERNEL_DIR ?= $(KERNEL_SRC_PATH)
DESTDIR ?= kernel_install_dir

# Path to the directory that contains the source file(s) to compile
PWD := $(shell pwd) 
  
default:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

install:
	mkdir -p $(KERNEL_DIR)/$(DESTDIR)
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) INSTALL_MOD_PATH=$(DESTDIR) modules_install

clean:
	rm -rf $(KERNEL_DIR)/$(DESTDIR)
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
