#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <syscall.h>
#include <inttypes.h>

#define CPUINFO_VERSION "v1.2"

#undef __NR_sched_setaffinity
#undef __NR_sched_getaffinity

#ifdef __i386__
#define __NR_sched_setaffinity  241
#define __NR_sched_getaffinity  242
#endif
#ifdef __x86_64__
#define __NR_sched_setaffinity 203
#define __NR_sched_getaffinity 204
#endif

#ifdef __NR_sched_getaffinity
#define sched_setaffinity(p,l,m) syscall(__NR_sched_setaffinity,p,l,m)
#define sched_getaffinity(p,l,m) syscall(__NR_sched_getaffinity,p,l,m)
#else
#define sched_setaffinity(p,l,m)
#define sched_getaffinity(p,l,m)
#endif

int verbose=0;

#define CPU_INTEL 0x01
#define CPU_AMD   0x02

struct cacheinfo
{
  int  type;
  int  level;
  int  threads;
  int  size;
  int  l,p,w,s;
};

struct cpuinfo
{
  int   cpu;
  int   type;
  int   cpuid;
  int   nlogical;
  int   apicid;
  int   ncores;
  int   core_id;
  int   node_id;
  int   virt_bits;
  int   phys_bits;
  char  name[64];
  char  extname[64];
  int   x86;
  int   x86_model;
  int   x86_mask;
  
  int   ncache;
  struct cacheinfo cache[10];
};

const char *file;

static void cpuid(int iax, int icx, int *eax, int *ebx, int *ecx, int *edx)
{
  FILE *fp;
  char  line[128];
  int   tmp;

  if (file) {
    fp = fopen(file, "r");
    while (fgets(line, sizeof(line), fp) != NULL) {
      sscanf(line, " %x: %x %x %x %x", &tmp, eax, ebx, ecx, edx);
      if (tmp == iax) {
	break;
      }
    }
    fclose(fp);
  }
  else {
    __asm__("cpuid"
	    : "=a" (*eax),
	    "=b" (*ebx),
	    "=c" (*ecx),
	    "=d" (*edx)
	    : "0" (iax), "c" (icx));
  }
  if (verbose && ebx != ecx) {
    printf(" %.08x: %.08x %.08x %.08x %.08x\n", iax, *eax, *ebx, *ecx, *edx);
  }
}

unsigned int cachesize(struct cacheinfo *ci)
{
  return (ci->l * ci->w * ci->p * ci->s) >> 10;
}

void parsecpu(unsigned int start, struct cpuinfo *ci)
{
  unsigned int a,b,c,d,end;

  cpuid(start,0,&end,&a,&a,&a);
  while(start <= end) {
    cpuid(start,0,&a,&b,&c,&d);
    switch(start) {
    case 0x00:
      *(int *)(ci->name)   = b;
      *(int *)(ci->name+4) = d;
      *(int *)(ci->name+8) = c;
      if (!strcmp(ci->name, "GenuineIntel")) {
	ci->type = CPU_INTEL;
      }
      else if (!strcmp(ci->name, "AuthenticAMD")) {
	ci->type = CPU_AMD;
      }
      break;
    case 0x01:
      ci->apicid   = (b >> 24);
      ci->nlogical = (b >> 16) & 0xFF;
      ci->cpuid    = a & 0xFFF;
      ci->x86 = (a >> 8) & 0xF;
      ci->x86_model = (a >> 4) & 0xF;
      ci->x86_mask = (a & 0xF);
      if (ci->x86 == 0xF) {
	ci->x86 += (a >> 20) & 0xFF;
      }
      if (ci->x86 >= 6) {
	ci->x86_model += ((a >> 12) & 0xF0);
      }
      break;
    case 0x04:
      ci->ncores = 1+(a >> 26);
      /* Save Cache information */
      do {
	ci->cache[ci->ncache].type = (a & 0x1F);
	ci->cache[ci->ncache].level = (a >> 5) & 0x7;
	ci->cache[ci->ncache].threads = 1+((a >> 14) & 0xFFF);
	ci->cache[ci->ncache].l = 1+(b & 0xFFF);
	ci->cache[ci->ncache].p = 1+((b >> 12) & 0x3FF);
	ci->cache[ci->ncache].w = 1+(b >> 22);
	ci->cache[ci->ncache].s = 1+c;
	ci->cache[ci->ncache].size = cachesize(&ci->cache[ci->ncache]);
	cpuid(start,++ci->ncache,&a,&b,&c,&d);
      } while(c);
      break;
    case 0x80000002:
      *(int *)(ci->extname+0x0) = a;
      *(int *)(ci->extname+0x4) = b;
      *(int *)(ci->extname+0x8) = c;
      *(int *)(ci->extname+0xc) = d;
      break;
    case 0x80000003:
      *(int *)(ci->extname+0x10) = a;
      *(int *)(ci->extname+0x14) = b;
      *(int *)(ci->extname+0x18) = c;
      *(int *)(ci->extname+0x1c) = d;
      break;
    case 0x80000004:
      *(int *)(ci->extname+0x20) = a;
      *(int *)(ci->extname+0x24) = b;
      *(int *)(ci->extname+0x28) = c;
      *(int *)(ci->extname+0x2C) = d;
      break;
    case 0x80000008:
      d = (c >> 12) & 0xF;
      if (d != 0) {
	ci->ncores = 1 << d;
      }
      else {
	ci->ncores = (c & 0xFF)+1;
      }
      ci->virt_bits = (a >> 8) & 0xFF;
      ci->phys_bits = (a & 0xFF);
      break;
    case 0x80000005:
      if (ci->type == CPU_AMD) {
	ci->cache[ci->ncache].type = 1; // data
	ci->cache[ci->ncache].size = c >> 24;
	ci->cache[ci->ncache].level = 1;
	ci->cache[ci->ncache+1].type = 2; // inst
	ci->cache[ci->ncache+1].size = d >> 24;
	ci->cache[ci->ncache+1].level = 1;
	ci->ncache += 2;
      }
      break;
    case 0x80000006:
      if (ci->type == CPU_AMD) {
	ci->cache[ci->ncache].type = 2; // unif
	ci->cache[ci->ncache].size = c >> 24;
	ci->cache[ci->ncache].level = 2;
	ci->cache[ci->ncache+1].type = 2; // unif
	ci->cache[ci->ncache+1].size = (d >> 24) * 512;
	ci->cache[ci->ncache+1].level = 3;
	ci->ncache += 2;
      }
      break;
    }
    start++;
  }
}

const char *stripch(const char *c)
{
  while(*c == ' ') {
    c++;
  }
  return c;
}

const char *cachetype(int type)
{
  switch(type) {
  case 1: return "data";
  case 2: return "inst";
  case 3: return "unif";
  }
  return "";
}

void docpuinfo(int cpu)
{
  struct cpuinfo ci;
  int maxi,x;

  memset(&ci, 0, sizeof(ci));
  ci.cpu = cpu;
  ci.ncores = 1;

  printf("---- (CPU %d) ----\n", ci.cpu);
  parsecpu(0x0000000UL,&ci);
  parsecpu(0x80000000L,&ci);

  /* Display CPU information */
  maxi=ci.nlogical;
  for(x=0; maxi>>=1; x++)
    ;
  ci.node_id = ci.apicid>>x;
  maxi=ci.nlogical/ci.ncores;
  for(x=0; maxi>>=1; x++)
    ;
  printf("  name    : %s\n", ci.name);
  printf("  extname : %s\n", stripch(ci.extname));
  printf("  cpuid   : %3x (%x,%x,%x)\n", ci.cpuid, ci.x86, ci.x86_model, ci.x86_mask);
  printf("  apic id : %.2x [node=%.2x]\n", ci.apicid, ci.node_id);
  printf("  #logical: %d\n", ci.nlogical);
  printf("  #cores  : %d\n", ci.ncores);
  printf("  virtbits: %d\n", ci.virt_bits);
  printf("  physbits: %d\n", ci.phys_bits);
  printf("  -- cache info --\n");
  for(x=0; x<ci.ncache; x++) {
    printf("  type=%d[%s] level=L%d nthread=%2d size=%uK\n",
	   ci.cache[x].type, cachetype(ci.cache[x].type),
	   ci.cache[x].level,
	   ci.cache[x].threads,
	   ci.cache[x].size);
    if (verbose) {
      printf("    L=%.4x P=%.4x W=%.4x S=%.4x\n",
	     ci.cache[x].l,
	     ci.cache[x].p,
	     ci.cache[x].w,
	     ci.cache[x].s);
    }
  }
}

int main(int argc, char *argv[])
{
  int ncpu, idx;
  uint64_t nm,om;
  pid_t p;

  printf("cpuinfo version: %s\n\n", CPUINFO_VERSION);
  p = getpid();
  if (argc > 1) {
    verbose++;
  }
  if (argc > 2) {
    file = argv[2];
  }
  ncpu = sysconf(_SC_NPROCESSORS_CONF);
  printf("%d cpus configured\n", ncpu);

  sched_getaffinity(p, sizeof(om), &om);
  for(idx=0; idx<ncpu; idx++) {
    nm=(1L << idx);
    sched_setaffinity(p, sizeof(nm), &nm);
    docpuinfo(idx);
  }
  sched_setaffinity(p, sizeof(om), &om);
  return 0;
}
