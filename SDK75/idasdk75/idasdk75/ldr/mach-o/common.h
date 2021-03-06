#ifndef MACHO_COMMON_H
#define MACHO_COMMON_H
#include <netnode.hpp>
#include <diskio.hpp>
#include <set>
#define IDA_SEG_DATA    SEG_DATA
#define __inline__ inline
#ifndef __MAC__
#  define __DARWIN_UNIX03 1
#endif
#if !__DARWIN_UNIX03
#define __eax            eax
#define __ebx            ebx
#define __ecx            ecx
#define __edx            edx
#define __edi            edi
#define __esi            esi
#define __ebp            ebp
#define __esp            esp
#define __ss             ss
#define __eflags         eflags
#define __eip            eip
#define __cs             cs
#define __ds             ds
#define __es             es
#define __fs             fs
#define __gs             gs
#define __rax            rax
#define __rbx            rbx
#define __rcx            rcx
#define __rdx            rdx
#define __rdi            rdi
#define __rsi            rsi
#define __rbp            rbp
#define __rsp            rsp
#define __rflags         rflags
#define __rip            rip
#define __r8             r8
#define __r9             r9
#define __r10            r10
#define __r11            r11
#define __r12            r12
#define __r13            r13
#define __r14            r14
#define __r15            r15
#define __r              r
#define __sp             sp
#define __lr             lr
#define __pc             pc
#define __cpsr           cpsr
#define __fpu_reserved   fpu_reserved
#define __fpu_fcw        fpu_fcw
#define __fpu_fsw        fpu_fsw
#define __fpu_ftw        fpu_ftw
#define __fpu_fop        fpu_fop
#define __fpu_ip         fpu_ip
#define __fpu_cs         fpu_cs
#define __fpu_rsrv1      fpu_rsrv1
#define __fpu_rsrv2      fpu_rsrv2
#define __fpu_rsrv3      fpu_rsrv3
#define __fpu_rsrv4      fpu_rsrv4
#define __fpu_stmm0      fpu_stmm0
#define __fpu_stmm1      fpu_stmm1
#define __fpu_stmm2      fpu_stmm2
#define __fpu_stmm3      fpu_stmm3
#define __fpu_stmm4      fpu_stmm4
#define __fpu_stmm5      fpu_stmm5
#define __fpu_stmm6      fpu_stmm6
#define __fpu_stmm7      fpu_stmm7
#define __fpu_xmm0       fpu_xmm0
#define __fpu_xmm1       fpu_xmm1
#define __fpu_xmm2       fpu_xmm2
#define __fpu_xmm3       fpu_xmm3
#define __fpu_xmm4       fpu_xmm4
#define __fpu_xmm5       fpu_xmm5
#define __fpu_xmm6       fpu_xmm6
#define __fpu_xmm7       fpu_xmm7
#define __fpu_dp         fpu_dp
#define __fpu_ds         fpu_ds
#define __fpu_mxcsr      fpu_mxcsr
#define __fpu_mxcsrmask  fpu_mxcsrmask
#define __fpu_reserved   fpu_reserved
#define __fpu_reserved1  fpu_reserved1
#define __rc             rc
#define __precis         precis
#define __undfl          undfl
#define __ovrfl          ovrfl
#define __zdiv           zdiv
#define __denorm         denorm
#define __invalid        invalid
#define __busy           busy
#define __c3             c3
#define __tos            tos
#define __c2             c2
#define __c1             c1
#define __c0             c0
#define __errsumm        errsumm
#define __stkflt         stkflt
#define __trapno         trapno
#define __err            err
#define __faultvaddr     faultvaddr
#define __darwin_mmst_reg mmst_reg
#define __darwin_xmm_reg  xmm_reg
#endif
#if __MF__
#ifndef __BIG_ENDIAN__
#define __BIG_ENDIAN__ 1
#endif
#else
#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__ 1
#endif
#endif
#ifdef __NT__
#ifndef __ppc__
#define __i386__ 1
#endif
#define __attribute__(x)
#endif
#ifdef __LINUX__
#define _DARWIN_C_SOURCE
#endif
#include <mach-o/fat.h>
#include <mach-o/loader.h>
#include <mach-o/reloc.h>
#include <mach-o/i860/reloc.h>
#include <mach-o/m88k/reloc.h>
#include <mach-o/ppc/reloc.h>
#include <mach-o/hppa/reloc.h>
#include <mach-o/sparc/reloc.h>
#include <mach-o/arm/reloc.h>
#include <mach-o/arm64/reloc.h>
#include <mach-o/x86_64/reloc.h>
#include <mach-o/nlist.h>
#include <mach-o/stab.h>
// these definitions are processor specific but are redefined in header files
// we undefine and never use them
#undef THREAD_STATE_NONE
#undef VALID_THREAD_STATE_FLAVOR
#undef MACHINE_THREAD_STATE
#undef MACHINE_THREAD_STATE_COUNT
#undef THREAD_STATE_MAX
#include <mach/i386/thread_status.h>
#undef THREAD_STATE_NONE
#undef VALID_THREAD_STATE_FLAVOR
#undef MACHINE_THREAD_STATE
#undef MACHINE_THREAD_STATE_COUNT
#undef THREAD_STATE_MAX
#define __arm__ 1
#include <mach/arm/thread_status.h>
#undef __arm__
#undef THREAD_STATE_NONE
#undef VALID_THREAD_STATE_FLAVOR
#undef MACHINE_THREAD_STATE
#undef MACHINE_THREAD_STATE_COUNT
#undef THREAD_STATE_MAX
CASSERT(sizeof(uint64_t) == 8);
#define SWAP_SHORT     swap16
#define SWAP_LONG      swap32
#define SWAP_LONG_LONG swap64
inline double SWAP_DOUBLE(double d)
{
  CASSERT(sizeof(uint64) == sizeof(double));
  uint64 x = swap64(*(uint64*)&d);
  return *(double *)&x;
}
#ifdef __LINUX__
inline uint64_t swap64(uint64_t x)
{
  // This is supposed to call pro.a's llong.cpp's swap64(ulonglong),
  // and thus _not_ endlessly recurse on itself --and maybe it does
  // in IDA-- but in some other tools, this is causing a problem.
  // For example, in EFD, we'd get segfaults because of
  // stack overflows.
  return make_ulonglong(swap32(high((ulonglong) x)), swap32(low((ulonglong) x)));
  /* return swap64((ulonglong)x); */
}
#endif
#define LC_ROUTINES_VALUE LC_ROUTINES_64
#define LC_ROUTINES_NAME "LC_ROUTINES_64"
#include "macho_node.h"
#define MAX_DEPTH 1024
// copy memory with range checking and auto sizing
template<class T> bool safecopy(const char *&begin, const char *end, T *dest)
{
  if ( end <= begin || (end - begin) < sizeof(T) )
  {
    memset(dest, 0, sizeof(T));
    begin = end;
    return false;
  }
  memcpy((char*)dest, begin, sizeof(T));
  begin += sizeof(T);
  return true;
}
// advance the pointer with range and overflow checking
inline bool safeskip(const char *&begin, const char *end, size_t amount)
{
  if ( end <= begin || (end - begin) < amount )
  {
    begin = end;
    return false;
  }
  begin += amount;
  return true;
}
void use_ppc_thread_state(const char *begin, const char *end, int mf);
void print_ppc_thread_state(const char *begin, const char *end, int mf);
DECLARE_TYPE_AS_MOVABLE(section_64);
DECLARE_TYPE_AS_MOVABLE(segment_command_64);
DECLARE_TYPE_AS_MOVABLE(nlist_64);
DECLARE_TYPE_AS_MOVABLE(relocation_info);
DECLARE_TYPE_AS_MOVABLE(dylib_module_64);
DECLARE_TYPE_AS_MOVABLE(dylib_table_of_contents);
DECLARE_TYPE_AS_MOVABLE(dylib_reference);
typedef qvector<struct section_64> secvec_t;
typedef qvector<struct segment_command_64> segcmdvec_t;
typedef qvector<struct nlist_64> nlistvec_t;
typedef qvector<struct relocation_info> relocvec_t;
typedef qvector<struct dylib_module_64> mod_table_t;
typedef qvector<struct dylib_table_of_contents> tocvec_t;
typedef qvector<struct dylib_reference> refvec_t;
typedef qstrvec_t dyliblist_t;
#define VISIT_COMMAND(name)   virtual int visit_##name       (const struct name##_command *, const char *, const char *)    { return 0; }
#define VISIT_COMMAND64(name) virtual int visit_##name##_64  (const struct name##_command_64 *, const char *, const char *) { return 0; }
// callbacks for visit_load_commands()
struct macho_lc_visitor_t
{
  // called for all load commands
  // lc: swapped load_command
  // begin, end: pointers to raw (unswapped) command
  // returns
  //  0: call specific callback
  //  1: stop enumeration
  //  2: don't call specific callback and continue
  virtual int visit_any_load_command(const struct load_command *, const char *, const char *) { return 0; }
  // unknown load command
  // return nonzero to stop enumeration
  // NB: lc is swapped
  virtual int visit_unknown_load_command(const struct load_command *, const char *, const char *) { return 0; }
  // the following functions get cmd already in native byte order (at least headers)
  // begin and end are pointers to raw, unswapped data
  // virtual int visit_XX  (const struct XX_command *cmd, char *begin, char *end) { return 0; }
  // LC_SYMTAB
  VISIT_COMMAND(symtab);
  // LC_SYMSEG
  VISIT_COMMAND(symseg);
  // LC_THREAD, LC_UNIXTHREAD
  VISIT_COMMAND(thread);
  // LC_IDFVMLIB, LC_LOADFVMLIB
  VISIT_COMMAND(fvmlib);
  // LC_IDENT
  VISIT_COMMAND(ident);
  // LC_FVMFILE
  VISIT_COMMAND(fvmfile);
  // LC_DYSYMTAB
  VISIT_COMMAND(dysymtab);
  // LC_LOAD_DYLIB, LC_LOAD_WEAK_DYLIB, LC_ID_DYLIB, LC_REEXPORT_DYLIB, LC_LAZY_LOAD_DYLIB
  VISIT_COMMAND(dylib);
  // LC_ID_DYLINKER, LC_LOAD_DYLINKER
  VISIT_COMMAND(dylinker);
  // LC_PREBOUND_DYLIB
  VISIT_COMMAND(prebound_dylib);
  // LC_ROUTINES
  VISIT_COMMAND(routines);
  // LC_SUB_FRAMEWORK
  VISIT_COMMAND(sub_framework);
  // LC_SUB_UMBRELLA
  VISIT_COMMAND(sub_umbrella);
  // LC_SUB_CLIENT
  VISIT_COMMAND(sub_client);
  // LC_SUB_LIBRARY
  VISIT_COMMAND(sub_library);
  // LC_TWOLEVEL_HINTS
  VISIT_COMMAND(twolevel_hints);
  // LC_PREBIND_CKSUM
  VISIT_COMMAND(prebind_cksum);
  // LC_ROUTINES_64
  VISIT_COMMAND64(routines);
  // LC_UUID
  VISIT_COMMAND(uuid);
  // LC_RPATH
  VISIT_COMMAND(rpath);
  // LC_CODE_SIGNATURE, LC_SEGMENT_SPLIT_INFO, LC_FUNCTION_STARTS, LC_DATA_IN_CODE
  // LC_DYLIB_CODE_SIGN_DRS
  VISIT_COMMAND(linkedit_data);
  // LC_ENCRYPTION_INFO
  VISIT_COMMAND(encryption_info);
  // LC_ENCRYPTION_INFO64
  virtual int visit_encryption_info_64(const struct encryption_info_command_64 *, const char *, const char *) { return 0; }
  // LC_DYLD_INFO, LC_DYLD_INFO_ONLY
  VISIT_COMMAND(dyld_info);
  // LC_VERSION_MIN_MACOSX, LC_VERSION_MIN_IPHONEOS, LC_VERSION_MIN_WATCHOS
  VISIT_COMMAND(version_min);
  // LC_BUILD_VERSION
  VISIT_COMMAND(build_version);
  virtual int visit_build_tool_version(const build_tool_version *, const char *, const char *) { return 0; }
  // LC_SOURCE_VERSION
  VISIT_COMMAND(source_version);
  // LC_MAIN
  VISIT_COMMAND(entry_point);
  virtual int visit_segment(const segment_command *,    const char *, const char *) { return 0; }
  virtual int visit_segment(const segment_command_64 *, const char *, const char *) { return 0; }
  // even though they're not strictly load commands, we also parse sections for convenience
  virtual int visit_section(const struct section *,    const char *, const char *) { return 0; }
  virtual int visit_section(const struct section_64 *, const char *, const char *) { return 0; }
  virtual ~macho_lc_visitor_t() {};
};
struct macho_reloc_visitor_t
{
  // magic values for section number
  enum
  {
    mach_reloc_external = -2,
    mach_reloc_local = -1,
  };
  // callback for visit_relocs()
  virtual void visit_relocs(uint64 baseea, const relocvec_t &relocs, int section_no) = 0;
};
struct dyld_info_visitor_t
{
  // visit a rebase location
  // type: type of rebasing (REBASE_TYPE_XXX)
  virtual void visit_rebase(uint64_t /*address*/, uchar /*type*/) {}
  enum bind_kind_t
  {
    bind_kind_normal = 0,
    bind_kind_weak   = 1,
    bind_kind_lazy   = 2,
  };
  // visit a bind location
  // bind_kind: which of bind directories are we walking
  // type: bind type (BIND_TYPE_XXX)
  // flags: BIND_SYMBOL_FLAGS_xxx
  // addend: value added to symbol's address
  // name: symbol name
  virtual int visit_bind(
        bind_kind_t /*bind_kind*/,
        uint64_t /*address*/,
        char /*type*/,
        uchar /*flags*/,
        int64_t /*libOrdinal*/,
        int64_t /*addend*/,
        const char * /*name*/)
  {
    return 0;
  }
  // visit an exported name
  // flags: EXPORT_SYMBOL_FLAGS_XXX
  virtual int visit_export(
        uint64_t /*address*/,
        uint32 /*flags*/,
        const char * /*name*/)
  {
    return 0;
  }
};
struct function_starts_visitor_t
{
  // visit a function start
  virtual int visit_start(uint64_t /*address*/) { return 0; }
  // callback for when info could not be found/loaded
  virtual void handle_error();
};
struct shared_region_visitor_t
{
  // kind: DYLD_CACHE_ADJ_V1_xx
  virtual int visit_region(uint8 /* kind */, uint64_t /*address*/) { return 0; }
  // kind: DYLD_CACHE_ADJ_V2_xx
  virtual int visit_regionv2(uint8 /*kind*/, uint64_t /*from*/, uint64_t /*to*/) { return 0; }
};

bool has_contiguous_segments(const segcmdvec_t &segcmds);

//--------------------------------------------------------------------------
// from mach_loader.c
/*
* The first APPLE_UNPROTECTED_HEADER_SIZE bytes (from offset 0 of
* this part of a Universal binary) are not protected...
* The rest needs to be "transformed".
*/
#define APPLE_UNPROTECTED_HEADER_SIZE  (3 * PAGE_SIZE)
#define APPLE_PROTECTED_MAGIC_AES      0xc2286295
#define APPLE_PROTECTED_MAGIC_BLOWFISH 0x2e69cf40
#define IS_PROTECTED(sg) ((sg.flags & SG_PROTECTED_VERSION_1) != 0 \
                        && sg.vmsize > 0 \
                        && (sg.fileoff > APPLE_UNPROTECTED_HEADER_SIZE \
                         || sg.fileoff + sg.filesize > APPLE_UNPROTECTED_HEADER_SIZE))

inline bool is_protected(const segment_command &sg) { return IS_PROTECTED(sg); }
inline bool is_protected(const segment_command_64 &sg) { return IS_PROTECTED(sg); }

//--------------------------------------------------------------------------
#define IS_PRELINK_INFO(s) (s.size > 0 && (strneq(s.sectname, "__info", 6) && strneq(s.segname, "__PRELINK", 9)))

inline bool is_prelink_info(const section &s) { return IS_PRELINK_INFO(s); }
inline bool is_prelink_info(const section_64 &s) { return IS_PRELINK_INFO(s); }

//--------------------------------------------------------------------------
struct kmod_params
{
  uint64 kinfo_ea;  // address of the kmod_info structure
  uint64 kstart;    // base vm address of this kext's __TEXT segment
  uint64 ksize;     // size of the kext
  uint64 loadaddr;  // vm address of the kext after it is initially loaded as part of the __PRELINK_TEXT segment
                    // (not always the same as kstart, since it might be relocated)
  uint64 off;       // start offset within the kernelcache file
  qstring name;     // name of the kext
  uchar uuid[16];   // UUID of the kext

  kmod_params(void) : kinfo_ea(0), kstart(0), ksize(0), loadaddr(0), off(0)
  {
    memset(uuid, 0, sizeof(uuid));
  }
};
DECLARE_TYPE_AS_MOVABLE(kmod_params);
typedef qvector<kmod_params> kmod_params_vec_t;

//--------------------------------------------------------------------------
struct kcache_pointer_visitor_t
{
  virtual int visit_pointer(uint64 ea, uint64 raw_value, uint64 untagged) = 0;
};

//--------------------------------------------------------------------------
struct import_info_t
{
  uint64 impea; // addrress of the import pointer
  qstring name; //  name of the imported symbol
  uint32 dylib; // index of the dylib providing the symbol (1-based)
};
DECLARE_TYPE_AS_MOVABLE(import_info_t);
typedef qvector<import_info_t> impvec_t;
typedef std::set<int> dyldlib_set_t;

class dyld_cache_t;

//--------------------------------------------------------------------------
// extract a module from an archive when loading a macho file into the database
struct macho_module_extractor_t
{
  virtual bool extract(
        char *filename,
        size_t bufsize,
        char **temp_file_ptr,
        bool is_remote) const = 0;
};

// for some idiotic reason these are macros in linux system headers
#undef major
#undef minor

//--------------------------------------------------------------------------
struct macho_platform_version_t
{
  uint32 plfm;  // platform id: one of PLATFORM_... in mach-o/h/loader.h
  uint32 major; // major version
  uint32 minor; // minor version
  uint32 micro; // revision

  macho_platform_version_t(void)
    : plfm(0), major(0), minor(0), micro(0) {}
};

//--------------------------------------------------------------------------
// information about the target platform for a given macho binary.
//
// version info is provided in the LC_VERSION_MIN_* and LC_BUILD_VERSION
// load commands. one, all, or none of these load commands could appear
// in any given macho file.
//
// each version can be useful in different situations.
struct macho_platform_version_info_t
{
  macho_platform_version_t build_minos; // build_version_command.minos
  macho_platform_version_t build_sdk;   // build_version_command.sdk
  macho_platform_version_t min_version; // version_min_command.version
  macho_platform_version_t min_sdk;     // version_min_command.sdk
};

//--------------------------------------------------------------------------
//-V:macho_file_t:730 not all members of a class are initialized inside the constructor
class macho_file_t
{
protected:
  // input file reference
  linput_t *li;
  bool should_close_linput;
  // fat header (for fat files)
  fat_header fheader;
  // infos about available architecures
  qvector<fat_arch> fat_archs;
  // offset of main file in the stream
  size_t start_offset;
  // offset of the selected subfile
  size_t mach_offset;
  // size of the selected subfile
  size_t mach_size;
  // mach header + load commands
  // NB: non-swapped
  bytevec_t mach_header_data;
  // do we need to swap endianness?
  bool mf;
  // is file 64-bit?
  bool m64;
  // header of currently selecte file (swapped)
  mach_header_64 mh;
  // list of load commands in the header
  // pointers point into mach_header_data
  qvector<const load_command *> load_commands;
  // list of segment command (swapped)
  segcmdvec_t mach_segcmds;
  // list of section infos (swapped)
  secvec_t    mach_sections;
  // seg2section[i] = index of the first section of segment i in the mach_sections array
  intvec_t    seg2section;
  // list of prelinked kexts
  kmod_params_vec_t kpv;
  // list of dylibs (swapped)
  dyliblist_t mach_dylibs;
  // module table (swapped)
  mod_table_t mach_modtable;
  // toc (swapped)
  tocvec_t mach_toc;
  // reference table
  refvec_t mach_reftable;
  // are mach_sections and mach_segcmds valid?
  bool parsed_section_info;
  // are prelinked kext infos valid?
  bool parsed_kmod_info;
  // expected base address (vmaddr of the segment that includes the mach header)
  uint64 base_addr;
  // helper for extracting an archived mach-o file
  macho_module_extractor_t *extractor;
  // hints about this macho file
  uint32 hints;
#define MACHO_HINT_MEM_IMAGE        0x0001 // macho file represents an image in memory
#define MACHO_HINT_SHARED_CACHE_LIB 0x0002 // macho file is a lib from dyld_shared_cache
#define MACHO_HINT_BRANCH_ISLAND    0x0004 // macho file is a dyldcache branch island
  //load array of relocs from file with range checking and endianness swapping
  bool load_relocs(uint32 reloff, uint32 nreloc, relocvec_t *relocs, const char *descr);
  bool parse_fat_header();
  bool parse_load_commands(bool silent=false);
  void parse_section_info();
  bool getSegInfo(uint64_t *segStartAddr, uint64_t *segSize, int segIndex);
  void parse_kmod_info(void);
  bool parse_kmod_starts(void);
  bool parse_prelink_xml(void);
  uint64 find_next_magic(uint64 off, uint64 end);
  void scan_for_kexts(void);
  bool visit_rebase_opcodes(const bytevec_t &data, dyld_info_visitor_t &v);
  bool visit_bind_opcodes(
        dyld_info_visitor_t::bind_kind_t bind_kind,
        const bytevec_t &data,
        dyld_info_visitor_t &v);
  bool processExportNode(
        const uchar *start,
        const uchar *p,
        const uchar *end,
        char *symname,
        int symnameoff,
        size_t symnamelen,
        dyld_info_visitor_t &v,
        int level=0);
  bool visit_export_info(const bytevec_t &data, dyld_info_visitor_t &v);
  uint64 find_exported_symbol_dyld(
        const char *symname,
        bool verbose,
        const dyld_cache_t *dcache = NULL);
  uint64 va2off(uint64 addr);
  friend class dyld_cache_t;
public:
  macho_file_t(linput_t *_li, size_t _start_offset = 0, uint32 _hints = 0, macho_module_extractor_t *_extractor = NULL)
    : li(_li),
      should_close_linput(false),
      start_offset(_start_offset),
      mach_offset(-1),
      mach_size(0),
      mf(false),
      m64(false),
      parsed_section_info(false),
      parsed_kmod_info(false),
      base_addr(BADADDR),
      extractor(_extractor),
      hints(_hints) {}
  virtual ~macho_file_t(void)
  {
    if ( should_close_linput )
      close_linput(li);
  }
  // check if file begins either with fat header or mach header
  // must be called first
  bool parse_header();
  // get fat header
  // returns false if it's not a fat file
  bool get_fat_header(fat_header *fh);
  // get number of subfiles in a fat file
  // 0 means it's a not fat file
  size_t get_fat_subfiles() { return fat_archs.size(); }
  // get fat_arch structure for subfile n
  bool get_fat_arch(uint n, fat_arch *fa);
  // set subfile for the following functions
  // 0 works for non-fat files
  // filesize: limit reads to this size; if 0, use linput's size
  bool set_subfile(uint n, size_t filesize = 0, bool silent=false);
  // select subfile of a specific cpu type (and subtype)
  // cpusubtype of 0 matches any subtype
  bool select_subfile(cpu_type_t cputype, cpu_subtype_t cpusubtype = (cpu_subtype_t)0);
  // if the current submodule is an ar library, select a module from it and switch to it
  bool select_ar_module(size_t offset, size_t size);
  // get mach header for the current subfile
  const mach_header_64 &get_mach_header();
  void get_mach_header_data(bytevec_t *out) const { *out = mach_header_data; }
  bool is64() const { return m64; }
  bool ismf() const { return mf; }
  size_t get_subfile_offset() const { return mach_offset; }
  size_t get_subfile_size()   const { return mach_size; }
  enum subfile_type_t
  {
    SUBFILE_UNKNOWN,
    SUBFILE_MACH,
    SUBFILE_MACH_64,
    SUBFILE_AR
  };
  subfile_type_t get_subfile_type(uint n, size_t filesize=0);
  // Move the linput_t to the beginning of the n-th subfile.
  //
  // If this is not a FAT file, this will
  // rewind the linput_t to the beginning of the file
  // (plus a potential start_offset).
  //
  // returns true if successful, false otherwise.
  bool seek_to_subfile(uint n, size_t filesize = 0);
  // enumerate load commands and call visitor on each
  // return true if visitor returned a non-zero
  bool visit_load_commands(macho_lc_visitor_t &v);
  // get segments and sections info
  const segcmdvec_t &get_segcmds();
  const secvec_t &get_sections();
  size_t get_seg2section(size_t segidx) const { return seg2section[segidx]; }
  // get section by 1-based index (0 for header pseudo-section)
  bool get_section_or_hdr(section_64 *psect, size_t sectIndex);
  // get segment by index
  bool get_segment(size_t segIndex, segment_command_64 *pseg);
  // get section by segment index and virtual address inside section
  bool get_section(size_t segIndex, uint64_t vaddr, section_64 *psect);
  // find segment by name
  bool get_segment(const char *segname, segment_command_64 *pseg = NULL);
  // get section by segment name and section name
  bool get_section(const char *segname, const char *sectname, section_64 *psect = NULL);
  // get section contents by segment name and section name
  bool get_section(const char *segname, const char *sectname, bytevec_t *data);
  // get list of dylibs (LC_LOAD_DYLIB)
  // kind: which kind of dylibs to enumerate (LC_LOAD_DYLIB, LC_LOAD_WEAK_DYLIB, LC_REEXPORT_DYLIB, LC_LOAD_UPWARD_DYLIB or LC_LAZY_LOAD_DYLIB)
  // 0 = get all of them
  const dyliblist_t get_dylib_list(int kind = 0);
  // get dylib module table
  const mod_table_t &get_module_table();
  // get dylib table of contents
  const tocvec_t &get_toc();
  // get reference table
  const refvec_t &get_ref_table();
  // get thread state (LC_THREAD/LC_UNIXTHREAD)
  void get_thread_state(const char *&begin, const char *&end);
  // get entrypoint (either from LC_MAIN, or from the thread state
  uint64 get_entry_address();
  // check if file is encrypted
  // returns: 0-no, 1-apple protected, 2-iOS encrypted
  int is_encrypted();
  // load a chunk of data from the linkedit section
  // size: number of bytes to load
  // it is updated to the actual number of bytes loaded
  bool load_linkedit_data(uint32 offset, size_t *size, void *buffer);
  // get symbol table load command
  bool get_symtab_command(struct symtab_command *st);
  // load symbol table and string table
  void get_symbol_table_info(nlistvec_t *symbols, qstring *strings);
  void get_symbol_table(const struct symtab_command &st, nlistvec_t *symbols);
  void get_string_table(const struct symtab_command &st, qstring *strings);
  // gather all symbols imported from other dylibs together with the import pointers' addresses
  size_t get_import_info(impvec_t *imports, dyliblist_t *dylibs, rangeset_t *ranges, bool verbose);
  // get 16-bit uuid for this file
  bool get_uuid(uint8 uuid[16]);
  // get information about the target operating system
  bool get_platform_version_info(macho_platform_version_info_t *mpvi);
  // compare this macho file's uuid against the given bytes
  bool match_uuid(const bytevec_t &bytes);
  // gets the dysymtab_command from load commands
  // return false if not found
  bool get_dyst(struct dysymtab_command *dyst);
  // load indirect symbols table
  void get_indirect_symbol_table_info(qvector<uint32> *indirect_symbols);
  // enumerate relocations and call visitor on each
  void visit_relocs(macho_reloc_visitor_t &v);
  // enumerate dyld_info structures
  void visit_dyld_info(dyld_info_visitor_t &v);
  // enumerate LC_FUNCTION_STARTS data
  void visit_function_starts(function_starts_visitor_t &v);
  // enumerate LC_SEGMENT_SPLIT_INFO data
  void visit_shared_regions(shared_region_visitor_t &v);
  // get preferrable base address
  uint64 get_base()
  {
    if ( !parsed_section_info )
      parse_section_info();
    // default to 0 if we couldn't find it
    return base_addr != BADADDR64 ? base_addr : 0;
  }
  // get the address of an exported symbol 'symname'
  // will first try LC_DYLD_INFO_ONLY (if present), otherwise LC_SYMTAB
  // returns BADADDR64 on error/not found
  uint64 find_exported_symbol(
        const char *symname,
        bool verbose=false,
        const dyld_cache_t *dcache=NULL);
  // get the name of a symbol from its address
  bool get_symbol_name(
        qstring *name,
        uint64 *offset,
        uint64 address,
        int sectionidx,
        bool verbose = false,
        const dyld_cache_t *dcache = NULL);
  // return dylib ID, if present
  bool get_id_dylib(qstring *id);
  // get the linput pointer
  linput_t *get_linput(void) const { return li; }
  // test hint flags
  bool is_mem_image()        const { return (hints & MACHO_HINT_MEM_IMAGE) != 0; }
  bool is_shared_cache_lib() const { return (hints & MACHO_HINT_SHARED_CACHE_LIB) != 0; }
  bool is_branch_island()    const { return (hints & MACHO_HINT_BRANCH_ISLAND) != 0; }
  // is the address valid?
  bool is_valid_addr(uint64 addr) const;
  // does the address have an initialized byte?
  bool is_loaded_addr(uint64 addr);
  // read values by virtual address
  uint64 read_dword_at_va(uint64 addr);
  uint64 read_qword_at_va(uint64 addr);
  uint64 read_addr_at_va(uint64 addr);
  // read string by virtual address
  char *read_string_at_va(uint64 addr, char *buf, size_t bufsize);
  // is the linput a mach kernel executable?
  bool is_kernel();
  // is the linput a prelinked kernelcache?
  bool is_kcache();
  // is the linput a kernel extention?
  bool is_kext() const;
  // detect all prelinked KEXTs
  const kmod_params_vec_t &get_kmod_info(void);
  // get the index of the kmod_info for the kext with the given name (-1 if not found)
  int get_kmod_idx(const qstring &name) const;
  // get the section containing the prelink xml description
  bool get_prelink_info(section_64 *prelink_info);
  // get the data section common to all kexts
  bool get_prelink_data(section_64 *prelink_data);
  // get the section that contains the prelinked kexts
  bool get_prelink_text(section_64 *prelink_text);
  // create an linput for a given KEXT
  linput_t *create_single_kmod_input(qoff64_t start_off);
  // untag a threaded pointer
  uint64 untag(uint64 value) const;
  // visit all threaded pointers
  int visit_threaded_pointers(kcache_pointer_visitor_t &pv);
  // read fields from a kmod_info structure
  uint64 get_kmod_ver(uint64 kinfo_ea);
  uint64 get_kmod_start(uint64 kinfo_ea);
  uint64 get_kmod_size(uint64 kinfo_ea);
  uint64 get_kmod_hdr_size(uint64 kinfo_ea);
  uint64 get_kmod_start_func(uint64 kinfo_ea);
  qstring get_kmod_name(uint64 kinfo_ea);
};

struct dyld_cache_header
{
  char     magic[16];              // e.g. "dyld_v0     ppc"
  uint32_t mappingOffset;          // file offset to first dyld_cache_mapping_info
  uint32_t mappingCount;           // number of dyld_cache_mapping_info entries
  uint32_t imagesOffset;           // file offset to first dyld_cache_image_info
  uint32_t imagesCount;            // number of dyld_cache_image_info entries
  uint64_t dyldBaseAddress;        // base address of dyld when cache was built
  uint64_t codeSignatureOffset;    // file offset in of code signature blob
  uint64_t codeSignatureSize;      // size of code signature blob (zero means to end of file)
  uint64_t slideInfoOffset;        // file offset of kernel slide info
  uint64_t slideInfoSize;          // size of kernel slide info
  uint64_t localSymbolsOffset;     // file offset of where local symbols are stored
  uint64_t localSymbolsSize;       // size of local symbols information
  uint8_t  uuid[16];               // unique value for each shared cache file
  uint64_t cacheType;              // 1 for development, 0 for optimized
  uint32_t branchPoolsOffset;      // file offset to table of uint64_t pool addresses
  uint32_t branchPoolsCount;       // number of uint64_t entries
  uint64_t accelerateInfoAddr;     // (unslid) address of optimization info
  uint64_t accelerateInfoSize;     // size of optimization info
  uint64_t imagesTextOffset;       // file offset to first dyld_cache_image_text_info
  uint64_t imagesTextCount;        // number of dyld_cache_image_text_info entries
  uint64_t dylibsImageGroupAddr;   // (unslid) address of ImageGroup for dylibs in this cache
  uint64_t dylibsImageGroupSize;   // size of ImageGroup for dylibs in this cache
  uint64_t otherImageGroupAddr;    // (unslid) address of ImageGroup for other OS dylibs
  uint64_t otherImageGroupSize;    // size of oImageGroup for other OS dylibs
  uint64_t progClosuresAddr;       // (unslid) address of list of program launch closures
  uint64_t progClosuresSize;       // size of list of program launch closures
  uint64_t progClosuresTrieAddr;   // (unslid) address of trie of indexes into program launch closures
  uint64_t progClosuresTrieSize;   // size of trie of indexes into program launch closures
  uint32_t platform;               // platform number (macOS=1, etc)
  uint32_t flags;                  // formatVersion:8; dylibsExpectedOnDisk:1, simulator:1
  uint64_t sharedRegionStart;      // base load address of cache if not slid
  uint64_t sharedRegionSize;       // overall size of region cache can be mapped into
  uint64_t maxSlide;               // runtime slide of cache can be between zero and this value
};

struct dyld_cache_mapping_info
{
  uint64_t address;
  uint64_t size;
  uint64_t fileOffset;
  uint32_t maxProt;
  uint32_t initProt;
};

struct dyld_cache_image_info
{
  uint64_t address;
  uint64_t modTime;
  uint64_t inode;
  uint32_t pathFileOffset;
  uint32_t pad;
};

struct dyld_cache_slide_info
{
  uint32_t version;        // currently 1
  uint32_t toc_offset;
  uint32_t toc_count;
  uint32_t entries_offset;
  uint32_t entries_count;
  uint32_t entries_size;  // currently 128
  // uint16_t toc[toc_count];
  // entrybitmap entries[entries_count];
};

struct dyld_cache_slide_info2
{
  uint32_t version;    // currently 2
  uint32_t page_size;  // currently 4096 (may also be 16384)
  uint32_t page_starts_offset;
  uint32_t page_starts_count;
  uint32_t page_extras_offset;
  uint32_t page_extras_count;
  uint64_t delta_mask; // which (contiguous) set of bits contains the delta to the next rebase location
  uint64_t value_add;
  //uint16_t page_starts[page_starts_count];
  //uint16_t page_extras[page_extras_count];
};

#define DYLD_CACHE_SLIDE_PAGE_ATTRS           0xC000  // high bits of uint16_t are flags
#define DYLD_CACHE_SLIDE_PAGE_ATTR_EXTRA      0x8000  // index is into extras array (not starts array)
#define DYLD_CACHE_SLIDE_PAGE_ATTR_NO_REBASE  0x4000  // page has no rebasing
#define DYLD_CACHE_SLIDE_PAGE_ATTR_END        0x8000  // last chain entry for page

#define DYLD_CACHE_SLIDE_V3_PAGE_ATTR_NO_REBASE 0xFFFF // page has no rebasing

struct dyld_cache_slide_info3
{
  uint32_t version;    // currently 3
  uint32_t page_size;  // currently 4096
  uint32_t page_starts_count;
  uint32_t padding;
  uint64_t auth_value_add;
  //uint16_t page_starts[page_starts_count];
};

struct dyld_cache_local_symbols_info
{
  uint32_t nlistOffset;        // offset into this chunk of nlist entries
  uint32_t nlistCount;         // count of nlist entries
  uint32_t stringsOffset;      // offset into this chunk of string pool
  uint32_t stringsSize;        // byte count of string pool
  uint32_t entriesOffset;      // offset into this chunk of array of dyld_cache_local_symbols_entry
  uint32_t entriesCount;       // number of elements in dyld_cache_local_symbols_entry array
};

struct dyld_cache_local_symbols_entry
{
  uint32_t dylibOffset;        // offset in cache file of start of dylib
  uint32_t nlistStartIndex;    // start index of locals for this dylib
  uint32_t nlistCount;         // number of local symbols for this dylib
};

struct dyld_cache_image_text_info
{
  uint8_t uuid[16];
  uint64_t loadAddress;        // unslid address of start of __TEXT
  uint32_t textSegmentSize;
  uint32_t pathOffset;         // offset from start of cache file
};

// callback for visit_slid_pointers
struct dyld_cache_slide_visitor_t
{
  // dyld slide should be applied to the sanitized pointer at address 'addr'
  // return: 0- ok, 1- stop iterating
  virtual int visit_pointer(uint64 /*addr*/, uint64 /*untagged*/) { return 0; }

  DEFINE_VIRTUAL_DTOR(dyld_cache_slide_visitor_t);
};

//--------------------------------------------------------------------------
//-V:dyld_cache_t:730 not all members of a class are initialized inside the constructor: header
class dyld_cache_t
{
private:
  // input file reference
  linput_t *li;

  // header
  dyld_cache_header header;
  // mappings
  qvector<dyld_cache_mapping_info> mappings;
  // image infos
  qvector<dyld_cache_image_info> image_infos;
  // image text infos
  qvector<dyld_cache_image_text_info> text_infos;
  // image names
  qvector<qstring> image_names;

  // branch islands
  qvector<uint64> island_addrs;

  // slide info version
  uint32 slide_version;

  // slide info v1: TOC
  qvector<uint16> slide_toc;
  // slide info v1: entry size (should be 128: 1 bit per 4 bytes per page)
  uint32 slide_entries_size;
  // slide info v1: bitmap TOC
  bytevec_t slide_entries;

  // slide info v2: page starts
  qvector<uint16> slide_page_starts;
  // slide info v2: page extras
  qvector<uint16> slide_page_extras;
  // slide info v2: slide page size
  uint32 slide_page_size;
  // slide info v2: delta mask
  uint64 slide_delta_mask;
  // slide info v2: adjust pointer value
  uint64 slide_value_add;

  // local symbol info
  nlistvec_t localst_symbols;
  qstring localst_strings;
  qvector<dyld_cache_local_symbols_entry> localst_entries;

  // do we need to swap endianness?
  bool mf;
  // is file 64-bit?
  bool m64;

  // manage linput: see open()
  bool should_close_linput;

  void parse_local_symbols();

  uint64 find_exported_symbol_dyld(
        const char *dylib,
        const char *symname,
        bool verbose) const;

  int parse_slid_chain(dyld_cache_slide_visitor_t *v, uint64 start);

public:
  dyld_cache_t(linput_t *_li = NULL)
    : li(_li),
      slide_version(0),
      slide_entries_size(0),
      slide_page_size(0),
      slide_delta_mask(0),
      slide_value_add(0),
      mf(false),
      m64(false),
      should_close_linput(false) {}

  ~dyld_cache_t()
  {
    if ( li != NULL && should_close_linput )
      close_linput(li);
  }

  bool open(const char *path);
  bool is64() const { return m64; }
  bool ismf() const { return mf; }

#define PHF_MAPPINGS 0x01
#define PHF_IMAGES   0x02
#define PHF_SYMBOLS  0x04
#define PHF_ISLANDS  0x08
#define PHF_SLIDE    0x10
#define PHF_TEXT     0x20
#define PHF_ALL      0xFF

  // check if file begins with a dyld cache header
  // must be called first
  bool parse_header(uint32 flags = PHF_ALL);

  const char *get_arch() const;

  const dyld_cache_header &get_header(void) const { return header; }

  const qstring &get_image_name(int n) const { return image_names[n]; }
  const dyld_cache_mapping_info &get_mapping_info(int n) const { return mappings[n]; }
  const dyld_cache_image_info &get_image_info(int n) const { return image_infos[n]; }

  uint64 get_island_addr(int n) const { return island_addrs[n]; }

  // return an linput which can be used for parsing a single Mach-O from the cache
  // call close_linput() to close and free it
  linput_t *create_single_macho_input(size_t imgindex) const;
  linput_t *create_single_island_input(size_t index) const;

  // get an image's index in the list of images by full path
  // returns -1 if not found
  ssize_t get_image_index(const qstring &name) const
  {
    const qvector <qstring>::const_iterator p = image_names.find(name);
    if ( p != image_names.end() )
      return p - image_names.begin();
    return -1;
  };

  int get_numfiles() const { return image_infos.size(); }
  int get_nummappings() const { return mappings.size(); }
  int get_numislands() const { return island_addrs.size(); }

  // load local symbol table and string table
  void get_symbol_table_info(nlistvec_t *symbols, qstring *strings) const
  {
    *symbols = localst_symbols;
    *strings = localst_strings;
  }
  // the the local symbols entry for the given image
  const dyld_cache_local_symbols_entry *get_symbols_entry(int n) const;

  // get the address of a symbol 'symname' exported by dylib 'dylib'
  // returns BADADDR64 on error/not found
  uint64 find_exported_symbol(
        const char *dylib,
        const char *symname,
        bool verbose=false) const;

  uint64 va2off(uint64 addr) const;
  uint64 read_addr_at_va(uint64 addr) const;
  char *read_string_at_va(uint64 addr, char *buf, size_t bufsize) const;

  linput_t *get_linput(void) const { return li; }

  bool has_slide_info() const { return !slide_toc.empty() || !slide_page_starts.empty(); }
  bool calc_aslr_slide(int64 *slide, int n, bool verbose = false, qstring *errbuf = NULL) const;
  int visit_slid_pointers(dyld_cache_slide_visitor_t *v);
  uint32 get_slide_info_version() const { return slide_version; }
  uint64 untag(uint64 v) const;

  uint64 get_shared_region_start() const { return header.sharedRegionStart; }
  bytevec_t get_uuid() const { return bytevec_t(header.uuid, sizeof(header.uuid)); }

  void get_dependencies(dyldlib_set_t *p, int n, int max = -1);

  const dyld_cache_mapping_info *get_text_mapping(void) const;
  bool get_header_range(uint64 *start, uint64 *end) const;
  bool is_header_address(uint64 address) const;

  uint32 get_max_text_segm(void) const;
  uint64 get_min_image_address(void) const;

  void get_modules(intvec_t *out) const;
  void get_islands(intvec_t *out) const;
};

#define LOW50(v) (v & 0x7FFFFFFFFFFFFLL)
#define UNTAG_51BIT_SE(v) ((LOW50(v) << 13) & 0xFF00000000000000LL) | (((int64)(LOW50(v) << 21) >> 21) & 0xFFFFFFFFFFFFFFLL)

// tagged pointer (ios12+) layout:
// bit 63: 1 if pointer is authenticated  (protected with PAC)
// bit 62: 1: import/bound pointer (0: just rebase)
// bits 61..51: skip count to the next pointer in chain (0 if last one)
// for PAC pointers:
//   50..49: key index (IA/IB/DA/DB)
//   48: is address?
//   47..32: diversity
//   31..0: offset from base
// for non-PAC pointers:
//  50..0: sign-extended pointer value
class tagged_pointer64
{
  uint64 raw_value;
  bool bit(int n) { return (raw_value & (1ull << n)) != 0; }
  uint64 bits(int high, int low)
  {
    return (raw_value >> low) & ((1ull << (high - low + 1)) - 1);
  }
 public:
  tagged_pointer64(uint64 v) : raw_value(v) {};
  bool is_auth() { return bit(63); }
  bool is_bind() { return bit(62); }
  bool is_rebase() { return !bit(62); }
  int skip_count() { return bits(61, 51); }
  int key_index() { return bits(50, 49); }
  bool is_addr() { return bit(48); }
  int diversity() { return bits(47, 32); }
  uint64 untag(uint64 base)
  {
    if ( is_auth() )
    {
      //signed pointer:  low 32 bits are offset from base
      return base + bits(31, 0);
    }
    else
    {
      //non-signed, sign-extend low 51 bits
      return UNTAG_51BIT_SE(raw_value);
    }
  }
};

struct string_table_waitbox_t
{
  bool displayed;
  string_table_waitbox_t(const macho_file_t &mfile) : displayed(false)
  {
    // all dyld_shared_cache libs in memory share a common string table.
    // since this table can be huge, be sure to show a wait dialog when parsing it.
    if ( mfile.is_shared_cache_lib() && mfile.is_mem_image() )
    {
      show_wait_box("Loading shared strings table");
      displayed = true;
    }
  }
  ~string_table_waitbox_t()
  {
    if ( displayed )
      hide_wait_box();
  }
  bool cancelled() { return user_cancelled(); }
};
#endif // MACHO_COMMON_H

