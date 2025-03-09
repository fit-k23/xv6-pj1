#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// trace the system calls that appears in mask 
uint64 
sys_trace(void) 
{
  int tracemask;
  argint(0, &tracemask);

  struct proc* cur_proc = myproc();
  cur_proc->requestmask = tracemask;
  return 0;
}

// Collect running system's information
uint64 
sys_info(void) 
{
  // u_mode_info is not surely initialized => use k_mode_info then copy to u_mode_info
  struct sysinfo k_mode_info;
  struct sysinfo *u_mode_info;
  argaddr(0, (uint64 *)&u_mode_info);
  k_mode_info.freemem = (uint64)(PGSIZE * (uint64)kcountfree());
  k_mode_info.nproc = count_unused_proc();
  k_mode_info.nopenfiles = count_open_file();
  return copyout(myproc()->pagetable, (uint64)u_mode_info, (char*)&k_mode_info, sizeof(k_mode_info));
}
