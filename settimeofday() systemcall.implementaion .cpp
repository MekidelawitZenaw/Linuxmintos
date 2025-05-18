/*
 * Implementation of a simplified version of settimeofday() syscall in Linux Mint
 * For academic purposes only.
 * This is not meant to be compiled directly into a live kernel.
 */

#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/time.h>
#include <linux/uaccess.h>
#include <linux/ktime.h>

// Define the syscall
SYSCALL_DEFINE1(my_settimeofday, const struct timeval __user *, tv)
{
    struct timeval user_tv;
    struct timespec64 ts;

    // Check for null pointer
    if (tv == NULL)
        return -EINVAL;

    // Copy data from userspace
    if (copy_from_user(&user_tv, tv, sizeof(struct timeval)))
        return -EFAULT;

    // Validate the time
    if (user_tv.tv_usec < 0 || user_tv.tv_usec >= 1000000)
        return -EINVAL;

    // Convert timeval to timespec64
    ts.tv_sec = user_tv.tv_sec;
    ts.tv_nsec = user_tv.tv_usec * 1000;

    // Set system time (admin only)
    if (!capable(CAP_SYS_TIME))
        return -EPERM;

    do_settimeofday64(&ts);

    return 0;
}

/*
To use this syscall:
1. Add the function in an appropriate kernel source file (e.g., kernel/time/time.c).
2. Register it in syscall table (e.g., arch/x86/entry/syscalls/syscall_64.tbl).
3. Recompile and install the kernel (academic/test environment only).

This syscall mimics settimeofday() but is named my_settimeofday for isolation.
*/
