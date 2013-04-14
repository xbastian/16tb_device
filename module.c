#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/bio.h>
#include <linux/blkdev.h>

MODULE_AUTHOR("ZiomboI");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

struct tb_disk {
	int major;
	struct gendisk *gd;
	struct request_queue *rq;
	spinlock_t lock;
};

static struct tb_disk tbd;

static int tb_make_request_fn(struct request_queue *rq, struct bio *bio) {
	bio_endio(bio,0);
	return 0;
}
struct block_device_operations tb_blk_ops = {
		.owner = THIS_MODULE,
};

static int __init tb_module_init(void) {

	tbd.major = register_blkdev(tbd.major, "big_disk");
	if (tbd.major <= 0) {
		printk("%s(): \n", __func__);
		return -EINVAL;
	}

	spin_lock_init(tbd.lock);

	tbd.rq = blk_alloc_queue(GFP_KERNEL);

	tbd.rq->queue_lock = &tbd.lock;
	blk_queue_make_request(tbd.rq, tb_make_request_fn);

	tbd.gd = alloc_disk(1);

	tbd.gd->major = tbd.major;
	tbd.gd->first_minor = 2;
	tbd.gd->fops = &tb_blk_ops;
	tbd.gd->queue = tbd.rq;
	snprintf (tbd.gd->disk_name, 32, "big_disk");
	set_capacity(tbd.gd, 0);
	add_disk(tbd.gd);
	return 0;
}
module_init(tb_module_init);

static void __exit tb_module_exit(void) {

	del_gendisk(tbd.gd);
	put_disk(tbd.gd);
	blk_cleanup_queue(tbd.rq);

	unregister_blkdev(tbd.major,"big_disk");
}
module_exit(tb_module_exit);
