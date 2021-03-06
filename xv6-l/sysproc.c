#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_split(void)
{
  int op;

  if(argint(0, &op) < 0)
    return -1;
  splitw(op);
  return  0;
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_trace(void)
{
  return myproc()->timepiece;
}

int
sys_releasesharem(void)
{
  int idx;
  if(argint(0,&idx)<0)
    return -1;
  releaseshared(idx);
  return 0;
}

int
sys_getsharem(void)
{
  int idx;
  if(argint(0,&idx)<0)
    return -1;
  if(idx==1)
    getshared(0);
  return (int)getshared(idx);
}

int
sys_att(void)
{
  int cmd;
  if(argint(0,&cmd)<0)
    return -1;
  cli();
  cpus[0].consflag=cmd;
  cpus[1].consflag=cmd;
  return cpus[0].rtime+cpus[1].rtime;
}
