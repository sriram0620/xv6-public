// Multiprocessor support
// Search memory for MP description structures.
// http://developer.intel.com/design/pentium/datashts/24201606.pdf

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mp.h"
#include "x86.h"
#include "mmu.h"
#include "proc.h"

struct cpu cpus[NCPU];
int ncpu;
uchar ioapicid;

static uchar sum(uchar *addr, int len)
{
  int i, sum;

  sum = 0;
  for(i = 0; i < len; i++)
    sum += addr[i];
  return sum;
}

// Look for an MP structure in the len bytes at addr.
static struct mp* mpsearch1(uint a, int len)
{
  uchar *e, *p, *addr;

  addr = P2V(a);
  e = addr + len;
  
  // Ensure addr is within the range
  if (addr == 0 || (uint)addr >= 0x100000) 
      return 0;

  for(p = addr; p < e; p += sizeof(struct mp))
    if(memcmp(p, "_MP_", 4) == 0 && sum(p, sizeof(struct mp)) == 0)
      return (struct mp*)p;
  return 0;
}

// Search for the MP Floating Pointer Structure, which according to the
// spec is in one of the following three locations:
// 1) in the first KB of the EBDA;
// 2) in the last KB of system base memory;
// 3) in the BIOS ROM between 0xE0000 and 0xFFFFF.
static struct mp* mpsearch(void)
{
  uchar *bda;
  uint p;
  struct mp *mp;

  bda = (uchar *)P2V(0x400);
  if((p = ((bda[0x0F] << 8) | bda[0x0E]) << 4)){
    if((mp = mpsearch1(p, 1024)))
      return mp;
  } else {
    p = ((bda[0x14] << 8) | bda[0x13]) * 1024;
    if((mp = mpsearch1(p - 1024, 1024)))
      return mp;
  }
  return mpsearch1(0xF0000, 0x10000);
}

// Search for an MP configuration table with improved safety checks.
static struct mpconf* mpconfig(struct mp **pmp) {
  struct mpconf *conf;
  struct mp *mp;

  // Search for the MP floating pointer structure
  if ((mp = mpsearch()) == 0 || mp->physaddr == 0)
    return 0;

  // Convert physical address to virtual address
  conf = (struct mpconf*)P2V((uint)mp->physaddr);

  // Ensure conf is valid and the signature matches "PCMP"
  if (conf == 0 || memcmp(conf, "PCMP", 4) != 0)
    return 0;

  // Check if version is 1 or 4
  if (conf->version != 1 && conf->version != 4)
    return 0;

  // Add a boundary check for conf->length to ensure it’s within a reasonable range
  if (conf->length < sizeof(struct mpconf) || conf->length > 1024)
    return 0;

  // Calculate the checksum to verify integrity
  if (sum((uchar*)conf, conf->length) != 0)
    return 0;

  *pmp = mp;
  return conf;
}

void mpinit(void) {
  uchar *p, *e;
  int ismp = 1;
  struct mp *mp;
  struct mpconf *conf;
  struct mpproc *proc;
  struct mpioapic *ioapic;

  // Load the MP configuration table
  if ((conf = mpconfig(&mp)) == 0)
    panic("Expect to run on an SMP");

  lapic = (uint*)P2V((uint)conf->lapicaddr);
  if (lapic == 0)
    panic("Invalid LAPIC address");

  // Set up pointers to parse configuration entries
  p = (uchar*)(conf + 1);
  e = (uchar*)conf + conf->length;

  // Iterate through the entries
  while (p < e) {
    switch (*p) {
      case MPPROC:
        proc = (struct mpproc*)p;
        if (ncpu < NCPU) {
          cpus[ncpu].apicid = proc->apicid;  // Store the APIC ID
          ncpu++;
        }
        p += sizeof(struct mpproc);
        break;

      case MPIOAPIC:
        ioapic = (struct mpioapic*)p;
        ioapicid = ioapic->apicno;
        p += sizeof(struct mpioapic);
        break;

      case MPBUS:
      case MPIOINTR:
      case MPLINTR:
        p += 8;
        break;

      default:
        ismp = 0;
        break;
    }
  }

  // Ensure the configuration is valid
  if (!ismp)
    panic("Didn't find a suitable machine");

  // Mask external interrupts if IMCR is present
  if (mp->imcrp) {
    outb(0x22, 0x70);   // Select AIMCR
    outb(0x23, inb(0x23) | 1);  // Mask external interrupts.
  }
}
