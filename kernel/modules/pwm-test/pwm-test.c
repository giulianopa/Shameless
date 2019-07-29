#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pwm.h>
#include <linux/sysfs.h>
#include <uapi/linux/stat.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Giuliano Pasqualotto <giuliano.pasqualotto@gmail.com>");
MODULE_DESCRIPTION("A simple PWM test module");
MODULE_VERSION("0.01");

// Parameters, passed with: `insmod pwm-test.ko id=5 name=pwm5 period=32000000 dutycycle=15000000`
static int id = 5;
static char *name = "pwm5";
static int period = 10000000;
static int dutycycle = 1000000;
module_param(id, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
module_param(name, charp, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
module_param(period, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
module_param(dutycycle, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(id, "PWM id (e.g., 5)");
MODULE_PARM_DESC(name, "PWM name (e.g., pwm5)");
MODULE_PARM_DESC(period, "PWM period in ns");
MODULE_PARM_DESC(dutycycle, "PWM duty cycle in ns");

struct pwm_device *pwm_dev = NULL;

enum { FOO_SIZE_MAX = 4 };
static int foo_size;
static char foo_tmp[FOO_SIZE_MAX];

static ssize_t pwm_test_show(struct kobject *kobj, struct kobj_attribute *attr,
        char *buff)
{
    strncpy(buff, foo_tmp, foo_size);
    return foo_size;
}

static ssize_t pwm_test_store(struct  kobject *kobj, struct kobj_attribute *attr,
        const char *buff, size_t count)
{
    foo_size = min(count, (size_t)FOO_SIZE_MAX);
    strncpy(foo_tmp, buff, foo_size);
    return count;
}

static struct kobj_attribute pwm_test_attribute =
    __ATTR(pwm_test, S_IRUGO | S_IWUSR, pwm_test_show, pwm_test_store);

static struct attribute *attrs[] = {
    &pwm_test_attribute.attr,
    NULL,
};

static struct attribute_group attr_group = {
    .attrs = attrs,
};

static struct kobject *kobj;

static int __init pwm_test_init(void) {
	int ret;
	pwm_dev = pwm_request(id, name);
	printk("%s: Requested %s, returned 0x%x\n", __func__, name, pwm_dev);
	if (pwm_dev) {
		pwm_set_period(pwm_dev, period);
		ret = pwm_config(pwm_dev, dutycycle, period);
		printk("%s: pwm_config returned: %d\n", __func__, ret);
		ret = pwm_enable(pwm_dev);
		printk("%s: pwm_enable returned: %d\n", __func__, ret);
		if (ret == 0)
			printk("%s: %s enabled with period: %d ns, and duty cycle: %d ns\n", __func__, name, period, dutycycle);
	}

	// Create sysfs interface
	kobj = kobject_create_and_add("pwm_test-cmu_pwm5clk", kernel_kobj);
  if (!kobj)
    return -ENOMEM;
  ret = sysfs_create_group(kobj, &attr_group);
	printk("sysfs_create_group returned %d\n", ret);
  if (ret)
    kobject_put(kobj);
	return 0;
}

static void __exit pwm_test_exit(void) {
	if (pwm_dev) {
		printk("%s: Disable PWM\n", __func__);
		pwm_disable(pwm_dev);
		pwm_free(pwm_dev);
	}
	if (kobj)
		kobject_put(kobj);
}

module_init(pwm_test_init);
module_exit(pwm_test_exit);
