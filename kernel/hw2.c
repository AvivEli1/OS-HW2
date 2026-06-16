#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/errno.h>
#include <linux/sched/signal.h>
#include <linux/pid.h>

#define BAN_GETPID  0x1
#define BAN_PIPE    0x2
#define BAN_KILL    0x4
#define BAN_SIGNAL  0x8


asmlinkage long sys_hello(void) {
 printk("Hello, World!\n");
 return 0;
}


static unsigned char ban_char_to_mask(char ban) {
    switch (ban) {
        case 'g': return BAN_GETPID;
        case 'p': return BAN_PIPE;
        case 'k': return BAN_KILL;
        case 's': return BAN_SIGNAL;
        default:  return 0;
    }
}

//arguments: tast_struct* task = a pointer to some process's PCB, char ban = a char representing the syscall we want to check if is banned
//output: true if the syscall represented by that letter is banned, false if not
static int is_banned(struct task_struct* task, char ban) {
    unsigned char mask = ban_char_to_mask(ban);
    if (!mask)
        return -EINVAL;
    return (task->syscall_bans & mask) ? 1 : 0;
}


asmlinkage long sys_set_ban(int ban_getpid, int ban_pipe, int ban_kill, int ban_sig) {
    unsigned char bans = 0;
    if (ban_getpid < 0 || ban_pipe < 0 || ban_kill < 0 || ban_sig < 0)
        return -EINVAL;
    if (!uid_eq(current_euid(), GLOBAL_ROOT_UID))
        return -EPERM;

    if (ban_getpid > 0)
        bans |= BAN_GETPID;
    if (ban_pipe > 0)
        bans |= BAN_PIPE;
    if (ban_kill > 0)
        bans |= BAN_KILL;
    if (ban_sig > 0)
        bans |= BAN_SIGNAL;
    current->syscall_bans = bans;
    return 0;
}

asmlinkage long sys_get_ban(char ban) {
	if (ban != 'g' && ban != 'p' && ban != 'k' && ban != 's')
		return -EINVAL;
	
	unsigned char bans = current->syscall_bans;
	if ((bans & ban_char_to_mask(ban)) != 0)
		return true;
	else
		return false;
}


asmlinkage long sys_check_ban(pid_t pid, char ban)  {
	unsigned char mask = ban_char_to_mask(ban);
    struct task_struct *task;

    if (!mask)
        return -EINVAL;

    task = find_task_by_vpid(pid);
    if (task == NULL)
        return -ESRCH;

    if (current->syscall_bans & mask)
        return -EPERM;

    return (task->syscall_bans & mask) ? 1 : 0;
}

asmlinkage long sys_flip_ban_branch(int height, char ban)  {
	unsigned char mask = ban_char_to_mask(ban);
    struct task_struct *task;
    int count = 0;

    if (height <= 0 || !mask)
        return -EINVAL;

    if (current->syscall_bans & mask)
        return -EPERM;

    task = current->parent;

    while (height > 0 && task != NULL) {
        task->syscall_bans ^= mask;
        
        if (task->syscall_bans & mask)
            count++;
        task = task->parent;
        height--;
    }
    return count;
}
