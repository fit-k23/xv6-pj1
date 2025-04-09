#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

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

// set flag to print pagetable
uint64
sys_flagpgtbl(void)
{
  myproc()->flagpgtbl = 1;
  return 0;
}

// detect accessed pages
uint64 
sys_pageaccess(void)
{
  uint64 start_va;
  int npages;
  uint64 bitmask;

  argaddr(0, &start_va);
  argint(1, &npages); 
  argaddr(2, &bitmask);

  if (npages > 32)
    return -1;

  uint64 res = 0;

  struct proc *cur_proc = myproc();

  // Traverse each base virtual address of pages (base_pva)
  int page_index = 0;
  for (uint64 base_pva = start_va; base_pva < start_va + PGSIZE * npages; base_pva += PGSIZE) {
    pte_t *pte = walk(cur_proc->pagetable, base_pva, 0);
    if ((*pte & PTE_V) && (*pte & PTE_A)) {
      res |= (1L << page_index);
      *pte ^= PTE_A; // turn off accessed bit 
    }
    ++page_index;
  }
  return copyout(cur_proc->pagetable, bitmask, (char *) &res, sizeof(res));
}