#ifndef __MACBASE_HPP__
#define __MACBASE_HPP__

#include "debmod.h"

#include "pc_debmod.h"
#define BASE_DEBUGGER_MODULE pc_debmod_t

#ifndef __LINUX__       // linux gcc cannot compile macho-o headers
#include "symmacho.hpp"
#endif

// avoid conflicts with audit.h:
#define token_t __mac_token_t
#include <sys/sysctl.h>
#include <mach-o/fat.h>
#undef token_t

class macbase_debmod_t: public BASE_DEBUGGER_MODULE
{
  typedef BASE_DEBUGGER_MODULE inherited;

protected:
  // return number of processes, -1 - not implemented
  virtual int idaapi get_process_list(procvec_t *proclist, qstring *errbuf) override;
  // return the file name assciated with pid
  virtual bool idaapi get_exec_fname(int pid, char *buf, size_t bufsize) newapi;
  // get process bitness: 32bit - 4, 64bit - 8, 0 - unknown
  virtual int idaapi get_process_bitness(int pid) newapi;

  cpu_type_t get_process_cpu(pid_t pid) const;
  inline int idaapi get_cpu_bitness(cpu_type_t cpu);
};

//--------------------------------------------------------------------------
inline int idaapi macbase_debmod_t::get_cpu_bitness(cpu_type_t cpu)
{
  return (cpu & CPU_ARCH_ABI64) != 0 ? 8 : 4;
}

#endif
