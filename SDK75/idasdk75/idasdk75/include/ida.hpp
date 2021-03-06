/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2020 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _IDA_HPP
#define _IDA_HPP

#include <pro.h>
#include <range.hpp>

class plugin_t;
struct plugmod_t;

/*! \file ida.hpp

  \brief Contains the ::inf structure definition and some
         functions common to the whole IDA project.

  The ::inf structure is saved in the database and contains information
  specific to the current program being disassembled.
  Initially it is filled with values from ida.cfg.

  Although it is not a good idea to change values in ::inf
  structure (because you will overwrite values taken from ida.cfg),
  you are allowed to do it if you feel it necessary.

*/

//--------------------------------------------------------------------------
/// Known input file formats (kept in \inf{filetype}):
enum filetype_t
{
  f_EXE_old,            ///< MS DOS EXE File
  f_COM_old,            ///< MS DOS COM File
  f_BIN,                ///< Binary File
  f_DRV,                ///< MS DOS Driver
  f_WIN,                ///< New Executable (NE)
  f_HEX,                ///< Intel Hex Object File
  f_MEX,                ///< MOS Technology Hex Object File
  f_LX,                 ///< Linear Executable (LX)
  f_LE,                 ///< Linear Executable (LE)
  f_NLM,                ///< Netware Loadable Module (NLM)
  f_COFF,               ///< Common Object File Format (COFF)
  f_PE,                 ///< Portable Executable (PE)
  f_OMF,                ///< Object Module Format
  f_SREC,               ///< R-records
  f_ZIP,                ///< ZIP file (this file is never loaded to IDA database)
  f_OMFLIB,             ///< Library of OMF Modules
  f_AR,                 ///< ar library
  f_LOADER,             ///< file is loaded using LOADER DLL
  f_ELF,                ///< Executable and Linkable Format (ELF)
  f_W32RUN,             ///< Watcom DOS32 Extender (W32RUN)
  f_AOUT,               ///< Linux a.out (AOUT)
  f_PRC,                ///< PalmPilot program file
  f_EXE,                ///< MS DOS EXE File
  f_COM,                ///< MS DOS COM File
  f_AIXAR,              ///< AIX ar library
  f_MACHO,              ///< Mac OS X
  f_PSXOBJ,             ///< Sony Playstation PSX object file
};

/// Is unstructured input file?
inline bool is_filetype_like_binary(filetype_t ft)
{ return ft == f_BIN || ft == f_HEX || ft == f_MEX || ft == f_SREC; }

//--------------------------------------------------------------------------

typedef uchar comp_t;   ///< target compiler id
typedef uchar cm_t;     ///< calling convention and memory model

/// Information about the target compiler
struct compiler_info_t
{
  comp_t id;            ///< compiler id (see \ref COMP_)
  cm_t cm;              ///< memory model and calling convention (see \ref CM_)
  uchar size_i;         ///< sizeof(int)
  uchar size_b;         ///< sizeof(bool)
  uchar size_e;         ///< sizeof(enum)
  uchar defalign;       ///< default alignment for structures
  uchar size_s;         ///< short
  uchar size_l;         ///< long
  uchar size_ll;        ///< longlong
  uchar size_ldbl;      ///< longdouble (if different from \ph{tbyte_size})
};

//--------------------------------------------------------------------------
/// Storage types for flag bits
enum storage_type_t
{
  STT_CUR = -1, ///< use current storage type (may be used only as a function argument)
  STT_VA  = 0,  ///< regular storage: virtual arrays, an explicit flag for each byte
  STT_MM  = 1,  ///< memory map: sparse storage. useful for huge objects
  STT_DBG = 2,  ///< memory map: temporary debugger storage. used internally
};

/// \def{EA64_ALIGN, Maintain 64-bit alignments in 64-bit mode}
#ifdef __EA64__
#define EA64_ALIGN(n) uint32 n;
#else
#define EA64_ALIGN(n)
#endif

//--------------------------------------------------------------------------
/// The database parameters.
/// This structure is kept in the ida database.
/// It contains the essential parameters for the current program
#define IDAINFO_TAG_SIZE 3
#define IDAINFO_PROCNAME_SIZE 16
#define IDAINFO_STRLIT_PREF_SIZE 16
struct idainfo
{
  char tag[IDAINFO_TAG_SIZE];           ///< 'IDA'
  char zero;                            ///< this field is not present in the database
  ushort version;                       ///< Version of database
  char procname[IDAINFO_PROCNAME_SIZE]; ///< Name of the current processor (with \0)

  ushort s_genflags;                    ///< \ref INFFL_
/// \defgroup INFFL_ General idainfo flags
/// Used by idainfo::s_genflags
//@{
#define INFFL_AUTO       0x01           ///< Autoanalysis is enabled?
#define INFFL_ALLASM     0x02           ///< may use constructs not supported by
                                        ///< the target assembler
#define INFFL_LOADIDC    0x04           ///< loading an idc file that contains database info
#define INFFL_NOUSER     0x08           ///< do not store user info in the database
#define INFFL_READONLY   0x10           ///< (internal) temporary interdiction to modify the database
#define INFFL_CHKOPS     0x20           ///< check manual operands? (unused)
#define INFFL_NMOPS      0x40           ///< allow non-matched operands? (unused)
#define INFFL_GRAPH_VIEW 0x80           ///< currently using graph options (\dto{graph})
//@}

  uint32 lflags;                        ///< \ref LFLG_
/// \defgroup LFLG_ Misc. database flags
/// used by idainfo::lflags
//@{
#define LFLG_PC_FPP     0x00000001      ///< decode floating point processor instructions?
#define LFLG_PC_FLAT    0x00000002      ///< 32-bit program?
#define LFLG_64BIT      0x00000004      ///< 64-bit program?
#define LFLG_IS_DLL     0x00000008      ///< Is dynamic library?
#define LFLG_FLAT_OFF32 0x00000010      ///< treat ::REF_OFF32 as 32-bit offset for 16bit segments (otherwise try SEG16:OFF16)
#define LFLG_MSF        0x00000020      ///< Byte order: is MSB first?
#define LFLG_WIDE_HBF   0x00000040      ///< Bit order of wide bytes: high byte first?
                                        ///< (wide bytes: \ph{dnbits} > 8)
#define LFLG_DBG_NOPATH 0x00000080      ///< do not store input full path in debugger process options
#define LFLG_SNAPSHOT   0x00000100      ///< memory snapshot was taken?
#define LFLG_PACK       0x00000200      ///< pack the database?
#define LFLG_COMPRESS   0x00000400      ///< compress the database?
#define LFLG_KERNMODE   0x00000800      ///< is kernel mode binary?
//@}

/// \defgroup IDB_ Line prefix options
/// Used by idainfo::get_pack_mode
//@{
#define IDB_UNPACKED   0                ///< leave database components unpacked
#define IDB_PACKED     1                ///< pack database components into .idb
#define IDB_COMPRESSED 2                ///< compress & pack database components
//@}

  uint32 database_change_count;         ///< incremented after each byte and regular
                                        ///< segment modifications

  ushort filetype;                      ///< The input file type

  ushort ostype;                        ///< OS type the program is for
                                        ///< bit definitions in libfuncs.hpp

  ushort apptype;                       ///< Application type
                                        ///< bit definitions in libfuncs.hpp

  uchar asmtype;                        ///< target assembler number

  uchar specsegs;                       ///< What format do special segments use? 0-unspecified, 4-entries are 4 bytes, 8- entries are 8 bytes.

  uint32 af;                            ///< \ref AF_
/// \defgroup AF_ Analysis flags
/// used by idainfo::af
//@{
#define AF_CODE         0x00000001      ///< Trace execution flow
#define AF_MARKCODE     0x00000002      ///< Mark typical code sequences as code
#define AF_JUMPTBL      0x00000004      ///< Locate and create jump tables
#define AF_PURDAT       0x00000008      ///< Control flow to data segment is ignored
#define AF_USED         0x00000010      ///< Analyze and create all xrefs
#define AF_UNK          0x00000020      ///< Delete instructions with no xrefs

#define AF_PROCPTR      0x00000040      ///< Create function if data xref data->code32 exists
#define AF_PROC         0x00000080      ///< Create functions if call is present
#define AF_FTAIL        0x00000100      ///< Create function tails
#define AF_LVAR         0x00000200      ///< Create stack variables
#define AF_STKARG       0x00000400      ///< Propagate stack argument information
#define AF_REGARG       0x00000800      ///< Propagate register argument information
#define AF_TRACE        0x00001000      ///< Trace stack pointer
#define AF_VERSP        0x00002000      ///< Perform full SP-analysis. (\ph{verify_sp})
#define AF_ANORET       0x00004000      ///< Perform 'no-return' analysis
#define AF_MEMFUNC      0x00008000      ///< Try to guess member function types
#define AF_TRFUNC       0x00010000      ///< Truncate functions upon code deletion

#define AF_STRLIT       0x00020000      ///< Create string literal if data xref exists
#define AF_CHKUNI       0x00040000      ///< Check for unicode strings
#define AF_FIXUP        0x00080000      ///< Create offsets and segments using fixup info
#define AF_DREFOFF      0x00100000      ///< Create offset if data xref to seg32 exists
#define AF_IMMOFF       0x00200000      ///< Convert 32bit instruction operand to offset
#define AF_DATOFF       0x00400000      ///< Automatically convert data to offsets

#define AF_FLIRT        0x00800000      ///< Use flirt signatures
#define AF_SIGCMT       0x01000000      ///< Append a signature name comment for recognized anonymous library functions
#define AF_SIGMLT       0x02000000      ///< Allow recognition of several copies of the same function
#define AF_HFLIRT       0x04000000      ///< Automatically hide library functions

#define AF_JFUNC        0x08000000      ///< Rename jump functions as j_...
#define AF_NULLSUB      0x10000000      ///< Rename empty functions as nullsub_...

#define AF_DODATA       0x20000000      ///< Coagulate data segs at the final pass
#define AF_DOCODE       0x40000000      ///< Coagulate code segs at the final pass
#define AF_FINAL        0x80000000      ///< Final pass of analysis
//@}
  uint32 af2;                           ///< \ref AF2_
/// \defgroup AF2_ Analysis flags 2
/// Used by idainfo::af2
//@{
#define AF2_DOEH        0x00000001      ///< Handle EH information
#define AF2_DORTTI      0x00000002      ///< Handle RTTI information
#define AF2_MACRO       0x00000004      ///< Try to combine several instructions
                                        ///< into a macro instruction
/// remaining 29 bits are reserved
//@}
  uval_t baseaddr;                      ///< base address of the program (paragraphs)
  sel_t start_ss;                       ///< selector of the initial stack segment
  sel_t start_cs;                       ///< selector of the segment with the main entry point
  ea_t start_ip;                        ///< IP register value at the start of
                                        ///< program execution
  ea_t start_ea;                        ///< Linear address of program entry point
  ea_t start_sp;                        ///< SP register value at the start of
                                        ///< program execution
  ea_t main;                            ///< address of main()
  ea_t min_ea;                          ///< current limits of program
  ea_t max_ea;                          ///< maxEA is excluded
  ea_t omin_ea;                         ///< original minEA (is set after loading the input file)
  ea_t omax_ea;                         ///< original maxEA (is set after loading the input file)

  ea_t lowoff;                          ///< Low  limit for offsets
                                        ///< (used in calculation of 'void' operands)
  ea_t highoff;                         ///< High limit for offsets
                                        ///< (used in calculation of 'void' operands)

  uval_t maxref;                        ///< Max tail for references

  range_t privrange;                    ///< Range of addresses reserved for internal use.
                                        ///< Initially (MAXADDR, MAXADDR+0x800000)
  sval_t netdelta;                      ///< Delta value to be added to all adresses for mapping to netnodes.
                                        ///< Initially 0

  /// CROSS REFERENCES
  uchar xrefnum;                        ///< Number of references to generate
                                        ///< in the disassembly listing
                                        ///< 0 - xrefs won't be generated at all
  uchar type_xrefnum;                   ///< Number of references to generate
                                        ///< in the struct & enum windows
                                        ///< 0 - xrefs won't be generated at all
  uchar refcmtnum;                      ///< Number of comment lines to
                                        ///< generate for refs to string literals
                                        ///< or demangled names
                                        ///< 0 - such comments won't be
                                        ///< generated at all
  uchar s_xrefflag;                     ///< \ref SW_X
/// \defgroup SW_X Xref options
/// Used by idainfo::s_xrefflag
//@{
#define SW_SEGXRF       0x01            ///< show segments in xrefs?
#define SW_XRFMRK       0x02            ///< show xref type marks?
#define SW_XRFFNC       0x04            ///< show function offsets?
#define SW_XRFVAL       0x08            ///< show xref values? (otherwise-"...")
//@}

  /// NAMES
  ushort max_autoname_len;              ///< max autogenerated name length (without zero byte)
  char nametype;                        ///< \ref NM_
/// \defgroup NM_ Dummy names representation types
/// Used by idainfo::nametype
//@{
#define NM_REL_OFF      0
#define NM_PTR_OFF      1
#define NM_NAM_OFF      2
#define NM_REL_EA       3
#define NM_PTR_EA       4
#define NM_NAM_EA       5
#define NM_EA           6
#define NM_EA4          7
#define NM_EA8          8
#define NM_SHORT        9
#define NM_SERIAL       10
//@}

  uint32 short_demnames;                ///< short form of demangled names
  uint32 long_demnames;                 ///< long form of demangled names
                                        ///< see demangle.h for definitions
  uchar demnames;                       ///< \ref DEMNAM_
/// \defgroup DEMNAM_ Demangled name flags
/// used by idainfo::demnames
//@{
#define DEMNAM_MASK  3                  ///< mask for name form
#define DEMNAM_CMNT  0                  ///< display demangled names as comments
#define DEMNAM_NAME  1                  ///< display demangled names as regular names
#define DEMNAM_NONE  2                  ///< don't display demangled names
#define DEMNAM_GCC3  4                  ///< assume gcc3 names (valid for gnu compiler)
#define DEMNAM_FIRST 8                  ///< override type info
//@}

  uchar listnames;                      ///< \ref LN_
/// \defgroup LN_ Name list options
/// Used by idainfo::listnames
//@{
#define LN_NORMAL       0x01            ///< include normal names
#define LN_PUBLIC       0x02            ///< include public names
#define LN_AUTO         0x04            ///< include autogenerated names
#define LN_WEAK         0x08            ///< include weak names
//@}

  /// DISASSEMBLY LISTING DETAILS
  uchar indent;                         ///< Indentation for instructions
  uchar comment;                        ///< Indentation for comments
  ushort margin;                        ///< max length of data lines
  ushort lenxref;                       ///< max length of line with xrefs
  uint32 outflags;                      ///< \ref OFLG_
/// \defgroup OFLG_ output flags
/// used by idainfo::outflags
//@{
#define OFLG_SHOW_VOID    0x002         ///< Display void marks?
#define OFLG_SHOW_AUTO    0x004         ///< Display autoanalysis indicator?
#define OFLG_GEN_NULL     0x010         ///< Generate empty lines?
#define OFLG_SHOW_PREF    0x020         ///< Show line prefixes?
#define OFLG_PREF_SEG     0x040         ///< line prefixes with segment name?
#define OFLG_LZERO        0x080         ///< generate leading zeroes in numbers
#define OFLG_GEN_ORG      0x100         ///< Generate 'org' directives?
#define OFLG_GEN_ASSUME   0x200         ///< Generate 'assume' directives?
#define OFLG_GEN_TRYBLKS  0x400         ///< Generate try/catch directives?
//@}

  uchar s_cmtflg;                       ///< \ref SCF_
/// \defgroup SCF_ Comment options
/// Used by idainfo::s_cmtflg
//@{
#define SCF_RPTCMT       0x01            ///< show repeatable comments?
#define SCF_ALLCMT       0x02            ///< comment all lines?
#define SCF_NOCMT        0x04            ///< no comments at all
#define SCF_LINNUM       0x08            ///< show source line numbers
#define SCF_TESTMODE     0x10            ///< testida.idc is running
#define SCF_SHHID_ITEM   0x20            ///< show hidden instructions
#define SCF_SHHID_FUNC   0x40            ///< show hidden functions
#define SCF_SHHID_SEGM   0x80            ///< show hidden segments
//@}

  uchar s_limiter;                      ///< \ref LMT_
/// \defgroup LMT_ Delimiter options
/// Used by idainfo::s_limiter
//@{
#define LMT_THIN        0x01            ///< thin borders
#define LMT_THICK       0x02            ///< thick borders
#define LMT_EMPTY       0x04            ///< empty lines at the end of basic blocks
//@}

  short bin_prefix_size;                ///< # of instruction bytes (opcodes) to show in line prefix
  uchar s_prefflag;                     ///< \ref PREF_
/// \defgroup PREF_ Line prefix options
/// Used by idainfo::s_prefflag
//@{
#define PREF_SEGADR     0x01            ///< show segment addresses?
#define PREF_FNCOFF     0x02            ///< show function offsets?
#define PREF_STACK      0x04            ///< show stack pointer?
#define PREF_PFXTRUNC   0x08            ///< truncate instruction bytes if they would need more than 1 line
//@}

  /// STRING LITERALS
  uchar strlit_flags;                   ///< \ref STRF_
/// \defgroup STRF_ string literal flags
/// Used by idainfo::strlit_flags
//@{
#define STRF_GEN        0x01            ///< generate names?
#define STRF_AUTO       0x02            ///< names have 'autogenerated' bit?
#define STRF_SERIAL     0x04            ///< generate serial names?
#define STRF_UNICODE    0x08            ///< unicode strings are present?
#define STRF_COMMENT    0x10            ///< generate auto comment for string references?
#define STRF_SAVECASE   0x20            ///< preserve case of strings for identifiers
//@}
  uchar strlit_break;                   ///< string literal line break symbol
  char strlit_zeroes;                   ///< leading zeroes
  int32 strtype;                        ///< current ascii string type
                                        ///< see nalt.hpp for string types
  char strlit_pref[IDAINFO_STRLIT_PREF_SIZE]; ///< prefix for string literal names
  uval_t strlit_sernum;                 ///< serial number

  // DATA ITEMS
  uval_t datatypes;                     ///< data types allowed in data carousel

  /// COMPILER
  compiler_info_t cc;                   ///< Target compiler
  uint32 abibits;                       ///< ABI features. Depends on info returned by get_abi_name()
                                        ///< Processor modules may modify them in set_compiler

/// \defgroup ABI_ abi options
/// Used by idainfo::abibits
//@{
#define ABI_8ALIGN4       0x00000001    ///< 4 byte alignment for 8byte scalars (__int64/double) inside structures?
#define ABI_PACK_STKARGS  0x00000002    ///< do not align stack arguments to stack slots
#define ABI_BIGARG_ALIGN  0x00000004    ///< use natural type alignment for argument if the alignment exceeds native word size
                                        ///< (e.g. __int64 argument should be 8byte aligned on some 32bit platforms)
#define ABI_STACK_LDBL    0x00000008    ///< long double arguments are passed on stack
#define ABI_STACK_VARARGS 0x00000010    ///< varargs are always passed on stack (even when there are free registers)
#define ABI_HARD_FLOAT    0x00000020    ///< use the floating-point register set
#define ABI_SET_BY_USER   0x00000040    ///< compiler/abi were set by user flag and require SETCOMP_BY_USER flag to be changed
#define ABI_GCC_LAYOUT    0x00000080    ///< use gcc layout for udts (used for mingw)
#define ABI_MAP_STKARGS   0x00000100    ///< register arguments are mapped to stack area (and consume stack slots)
#define ABI_HUGEARG_ALIGN  0x00000200   ///< use natural type alignment for an argument
                                        ///< even if its alignment exceeds double native word size
                                        ///< (the default is to use double word max).
                                        ///< e.g. if this bit is set, __int128 has 16-byte alignment
//@}

  uint32 appcall_options;               ///< appcall options, see idd.hpp
  EA64_ALIGN(padding);
                                        ///< total size for 32bit: 216 bytes
                                        ///<            for 64bit: 296 bytes

};
#ifdef __EA64__
CASSERT(sizeof(idainfo) == 280);
#else
CASSERT(sizeof(idainfo) == 200);
#endif

#if !defined(NO_OBSOLETE_FUNCS)
#define SW_RPTCMT       SCF_RPTCMT
#define SW_ALLCMT       SCF_ALLCMT
#define SW_NOCMT        SCF_NOCMT
#define SW_LINNUM       SCF_LINNUM
#define SW_TESTMODE     SCF_TESTMODE
#define SW_SHHID_ITEM   SCF_SHHID_ITEM
#define SW_SHHID_FUNC   SCF_SHHID_FUNC
#define SW_SHHID_SEGM   SCF_SHHID_SEGM
#endif

// we have to continue to export INF for old plugins
#if !defined(NO_OBSOLETE_FUNCS) || defined(__DEFINE_INF__)
idaman idainfo ida_export_data inf;     ///< program specific information
#endif

enum inftag_t
{
  INF_VERSION               =  0,
  INF_PROCNAME              =  1,
  INF_GENFLAGS              =  2,
  INF_LFLAGS                =  3,
  INF_DATABASE_CHANGE_COUNT =  4,
  INF_FILETYPE              =  5,
  INF_OSTYPE                =  6,
  INF_APPTYPE               =  7,
  INF_ASMTYPE               =  8,
  INF_SPECSEGS              =  9,
  INF_AF                    = 10,
  INF_AF2                   = 11,
  INF_BASEADDR              = 12,
  INF_START_SS              = 13,
  INF_START_CS              = 14,
  INF_START_IP              = 15,
  INF_START_EA              = 16,
  INF_START_SP              = 17,
  INF_MAIN                  = 18,
  INF_MIN_EA                = 19,
  INF_MAX_EA                = 20,
  INF_OMIN_EA               = 21,
  INF_OMAX_EA               = 22,
  INF_LOWOFF                = 23,
  INF_HIGHOFF               = 24,
  INF_MAXREF                = 25,
  INF_PRIVRANGE             = 26,
  INF_PRIVRANGE_START_EA    = 27,
  INF_PRIVRANGE_END_EA      = 28,
  INF_NETDELTA              = 29,
  INF_XREFNUM               = 30,
  INF_TYPE_XREFNUM          = 31,
  INF_REFCMTNUM             = 32,
  INF_XREFFLAG              = 33,
  INF_MAX_AUTONAME_LEN      = 34,
  INF_NAMETYPE              = 35,
  INF_SHORT_DEMNAMES        = 36,
  INF_LONG_DEMNAMES         = 37,
  INF_DEMNAMES              = 38,
  INF_LISTNAMES             = 39,
  INF_INDENT                = 40,
  INF_COMMENT               = 41,
  INF_MARGIN                = 42,
  INF_LENXREF               = 43,
  INF_OUTFLAGS              = 44,
  INF_CMTFLG                = 45,
  INF_LIMITER               = 46,
  INF_BIN_PREFIX_SIZE       = 47,
  INF_PREFFLAG              = 48,
  INF_STRLIT_FLAGS          = 49,
  INF_STRLIT_BREAK          = 50,
  INF_STRLIT_ZEROES         = 51,
  INF_STRTYPE               = 52,
  INF_STRLIT_PREF           = 53,
  INF_STRLIT_SERNUM         = 54,
  INF_DATATYPES             = 55,
  INF_CC                    = 56,
  INF_CC_ID                 = 57,
  INF_CC_CM                 = 58,
  INF_CC_SIZE_I             = 59,
  INF_CC_SIZE_B             = 60,
  INF_CC_SIZE_E             = 61,
  INF_CC_DEFALIGN           = 62,
  INF_CC_SIZE_S             = 63,
  INF_CC_SIZE_L             = 64,
  INF_CC_SIZE_LL            = 65,
  INF_CC_SIZE_LDBL          = 66,
  INF_ABIBITS               = 67,
  INF_APPCALL_OPTIONS       = 68,
  INF_LAST
};

/// get program specific information (a scalar value)
/// \param tag one of inftag_t constants
/// \return the requested info. if wrong tag is specified, return 0
idaman size_t ida_export getinf(inftag_t tag);

/// get program specific information (a non-scalar value)
/// \param tag one of inftag_t constants
/// \param buf output buffer
/// \param bufsize size of the output buffer
/// \return number of bytes stored in the buffer
idaman size_t ida_export getinf_buf(inftag_t tag, void *buf, size_t bufsize);

/// get a flag value
/// \param tag one of inftag_t constants
/// \param flag a flag in the field
/// \return whether the flag is set
idaman bool ida_export getinf_flag(inftag_t tag, uint32 flag);

/// set program specific information
/// \param tag one of inftag_t constants
/// \return success
idaman bool ida_export setinf(inftag_t tag, ssize_t value);

/// set program specific information
/// \param tag one of inftag_t constants
/// \param buf ptr to new value
/// \param bufsize size of the new value
/// \return success
idaman bool ida_export setinf_buf(inftag_t tag, const void *buf, size_t bufsize);

/// set a flag value
/// \param tag one of inftag_t constants
/// \param flag a flag in the field
/// \param value whether to set, or clear the flag
/// \return the previous value
idaman bool ida_export setinf_flag(inftag_t tag, uint32 flag, bool value=true);


// <INF_ACCESSORS>
inline ushort inf_get_version() { return ushort(getinf(INF_VERSION)); }
inline bool inf_set_version(ushort _v) { return setinf(INF_VERSION, ssize_t(_v)); }

inline ushort inf_get_genflags() { return ushort(getinf(INF_GENFLAGS)); }
inline bool inf_set_genflags(ushort _v) { return setinf(INF_GENFLAGS, ssize_t(_v)); }
inline bool inf_is_auto_enabled(void) { return getinf_flag(INF_GENFLAGS, INFFL_AUTO); }
inline bool inf_set_auto_enabled(bool _v=true) { return setinf_flag(INF_GENFLAGS, INFFL_AUTO, _v); }
inline bool inf_use_allasm(void) { return getinf_flag(INF_GENFLAGS, INFFL_ALLASM); }
inline bool inf_set_use_allasm(bool _v=true) { return setinf_flag(INF_GENFLAGS, INFFL_ALLASM, _v); }
inline bool inf_loading_idc(void) { return getinf_flag(INF_GENFLAGS, INFFL_LOADIDC); }
inline bool inf_set_loading_idc(bool _v=true) { return setinf_flag(INF_GENFLAGS, INFFL_LOADIDC, _v); }
inline bool inf_no_store_user_info(void) { return getinf_flag(INF_GENFLAGS, INFFL_NOUSER); }
inline bool inf_set_no_store_user_info(bool _v=true) { return setinf_flag(INF_GENFLAGS, INFFL_NOUSER, _v); }
inline bool inf_readonly_idb(void) { return getinf_flag(INF_GENFLAGS, INFFL_READONLY); }
inline bool inf_set_readonly_idb(bool _v=true) { return setinf_flag(INF_GENFLAGS, INFFL_READONLY, _v); }
inline bool inf_check_manual_ops(void) { return getinf_flag(INF_GENFLAGS, INFFL_CHKOPS); }
inline bool inf_set_check_manual_ops(bool _v=true) { return setinf_flag(INF_GENFLAGS, INFFL_CHKOPS, _v); }
inline bool inf_allow_non_matched_ops(void) { return getinf_flag(INF_GENFLAGS, INFFL_NMOPS); }
inline bool inf_set_allow_non_matched_ops(bool _v=true) { return setinf_flag(INF_GENFLAGS, INFFL_NMOPS, _v); }
inline bool inf_is_graph_view(void) { return getinf_flag(INF_GENFLAGS, INFFL_GRAPH_VIEW); }
inline bool inf_set_graph_view(bool _v=true) { return setinf_flag(INF_GENFLAGS, INFFL_GRAPH_VIEW, _v); }

inline uint32 inf_get_lflags() { return uint32(getinf(INF_LFLAGS)); }
inline bool inf_set_lflags(uint32 _v) { return setinf(INF_LFLAGS, ssize_t(_v)); }
inline bool inf_decode_fpp(void) { return getinf_flag(INF_LFLAGS, LFLG_PC_FPP); }
inline bool inf_set_decode_fpp(bool _v=true) { return setinf_flag(INF_LFLAGS, LFLG_PC_FPP, _v); }
inline bool inf_is_32bit(void) { return getinf_flag(INF_LFLAGS, LFLG_PC_FLAT); }
inline bool inf_set_32bit(bool _v=true) { return setinf_flag(INF_LFLAGS, LFLG_PC_FLAT, _v); }
inline bool inf_is_64bit(void) { return getinf_flag(INF_LFLAGS, LFLG_64BIT); }
inline bool inf_set_64bit(bool _v=true) { return setinf_flag(INF_LFLAGS, LFLG_64BIT, _v); }
inline bool inf_is_dll(void) { return getinf_flag(INF_LFLAGS, LFLG_IS_DLL); }
inline bool inf_set_dll(bool _v=true) { return setinf_flag(INF_LFLAGS, LFLG_IS_DLL, _v); }
inline bool inf_is_flat_off32(void) { return getinf_flag(INF_LFLAGS, LFLG_FLAT_OFF32); }
inline bool inf_set_flat_off32(bool _v=true) { return setinf_flag(INF_LFLAGS, LFLG_FLAT_OFF32, _v); }
inline bool inf_is_be(void) { return getinf_flag(INF_LFLAGS, LFLG_MSF); }
inline bool inf_set_be(bool _v=true) { return setinf_flag(INF_LFLAGS, LFLG_MSF, _v); }
inline bool inf_is_wide_high_byte_first(void) { return getinf_flag(INF_LFLAGS, LFLG_WIDE_HBF); }
inline bool inf_set_wide_high_byte_first(bool _v=true) { return setinf_flag(INF_LFLAGS, LFLG_WIDE_HBF, _v); }
inline bool inf_dbg_no_store_path(void) { return getinf_flag(INF_LFLAGS, LFLG_DBG_NOPATH); }
inline bool inf_set_dbg_no_store_path(bool _v=true) { return setinf_flag(INF_LFLAGS, LFLG_DBG_NOPATH, _v); }
inline bool inf_is_snapshot(void) { return getinf_flag(INF_LFLAGS, LFLG_SNAPSHOT); }
inline bool inf_set_snapshot(bool _v=true) { return setinf_flag(INF_LFLAGS, LFLG_SNAPSHOT, _v); }
inline bool inf_pack_idb(void) { return getinf_flag(INF_LFLAGS, LFLG_PACK); }
inline bool inf_set_pack_idb(bool _v=true) { return setinf_flag(INF_LFLAGS, LFLG_PACK, _v); }
inline bool inf_compress_idb(void) { return getinf_flag(INF_LFLAGS, LFLG_COMPRESS); }
inline bool inf_set_compress_idb(bool _v=true) { return setinf_flag(INF_LFLAGS, LFLG_COMPRESS, _v); }
inline bool inf_is_kernel_mode(void) { return getinf_flag(INF_LFLAGS, LFLG_KERNMODE); }
inline bool inf_set_kernel_mode(bool _v=true) { return setinf_flag(INF_LFLAGS, LFLG_KERNMODE, _v); }

inline uint32 inf_get_database_change_count() { return uint32(getinf(INF_DATABASE_CHANGE_COUNT)); }
inline bool inf_set_database_change_count(uint32 _v) { return setinf(INF_DATABASE_CHANGE_COUNT, ssize_t(_v)); }

inline filetype_t inf_get_filetype() { return filetype_t(getinf(INF_FILETYPE)); }
inline bool inf_set_filetype(filetype_t _v) { return setinf(INF_FILETYPE, ssize_t(_v)); }

inline ushort inf_get_ostype() { return ushort(getinf(INF_OSTYPE)); }
inline bool inf_set_ostype(ushort _v) { return setinf(INF_OSTYPE, ssize_t(_v)); }

inline ushort inf_get_apptype() { return ushort(getinf(INF_APPTYPE)); }
inline bool inf_set_apptype(ushort _v) { return setinf(INF_APPTYPE, ssize_t(_v)); }

inline uchar inf_get_asmtype() { return uchar(getinf(INF_ASMTYPE)); }
inline bool inf_set_asmtype(uchar _v) { return setinf(INF_ASMTYPE, ssize_t(_v)); }

inline uchar inf_get_specsegs() { return uchar(getinf(INF_SPECSEGS)); }
inline bool inf_set_specsegs(uchar _v) { return setinf(INF_SPECSEGS, ssize_t(_v)); }

inline uint32 inf_get_af() { return uint32(getinf(INF_AF)); }
inline bool inf_set_af(uint32 _v) { return setinf(INF_AF, ssize_t(_v)); }
inline bool inf_trace_flow(void) { return getinf_flag(INF_AF, AF_CODE); }
inline bool inf_set_trace_flow(bool _v=true) { return setinf_flag(INF_AF, AF_CODE, _v); }
inline bool inf_mark_code(void) { return getinf_flag(INF_AF, AF_MARKCODE); }
inline bool inf_set_mark_code(bool _v=true) { return setinf_flag(INF_AF, AF_MARKCODE, _v); }
inline bool inf_create_jump_tables(void) { return getinf_flag(INF_AF, AF_JUMPTBL); }
inline bool inf_set_create_jump_tables(bool _v=true) { return setinf_flag(INF_AF, AF_JUMPTBL, _v); }
inline bool inf_noflow_to_data(void) { return getinf_flag(INF_AF, AF_PURDAT); }
inline bool inf_set_noflow_to_data(bool _v=true) { return setinf_flag(INF_AF, AF_PURDAT, _v); }
inline bool inf_create_all_xrefs(void) { return getinf_flag(INF_AF, AF_USED); }
inline bool inf_set_create_all_xrefs(bool _v=true) { return setinf_flag(INF_AF, AF_USED, _v); }
inline bool inf_del_no_xref_insns(void) { return getinf_flag(INF_AF, AF_UNK); }
inline bool inf_set_del_no_xref_insns(bool _v=true) { return setinf_flag(INF_AF, AF_UNK, _v); }
inline bool inf_create_func_from_ptr(void) { return getinf_flag(INF_AF, AF_PROCPTR); }
inline bool inf_set_create_func_from_ptr(bool _v=true) { return setinf_flag(INF_AF, AF_PROCPTR, _v); }
inline bool inf_create_func_from_call(void) { return getinf_flag(INF_AF, AF_PROC); }
inline bool inf_set_create_func_from_call(bool _v=true) { return setinf_flag(INF_AF, AF_PROC, _v); }
inline bool inf_create_func_tails(void) { return getinf_flag(INF_AF, AF_FTAIL); }
inline bool inf_set_create_func_tails(bool _v=true) { return setinf_flag(INF_AF, AF_FTAIL, _v); }
inline bool inf_should_create_stkvars(void) { return getinf_flag(INF_AF, AF_LVAR); }
inline bool inf_set_should_create_stkvars(bool _v=true) { return setinf_flag(INF_AF, AF_LVAR, _v); }
inline bool inf_propagate_stkargs(void) { return getinf_flag(INF_AF, AF_STKARG); }
inline bool inf_set_propagate_stkargs(bool _v=true) { return setinf_flag(INF_AF, AF_STKARG, _v); }
inline bool inf_propagate_regargs(void) { return getinf_flag(INF_AF, AF_REGARG); }
inline bool inf_set_propagate_regargs(bool _v=true) { return setinf_flag(INF_AF, AF_REGARG, _v); }
inline bool inf_should_trace_sp(void) { return getinf_flag(INF_AF, AF_TRACE); }
inline bool inf_set_should_trace_sp(bool _v=true) { return setinf_flag(INF_AF, AF_TRACE, _v); }
inline bool inf_full_sp_ana(void) { return getinf_flag(INF_AF, AF_VERSP); }
inline bool inf_set_full_sp_ana(bool _v=true) { return setinf_flag(INF_AF, AF_VERSP, _v); }
inline bool inf_noret_ana(void) { return getinf_flag(INF_AF, AF_ANORET); }
inline bool inf_set_noret_ana(bool _v=true) { return setinf_flag(INF_AF, AF_ANORET, _v); }
inline bool inf_guess_func_type(void) { return getinf_flag(INF_AF, AF_MEMFUNC); }
inline bool inf_set_guess_func_type(bool _v=true) { return setinf_flag(INF_AF, AF_MEMFUNC, _v); }
inline bool inf_truncate_on_del(void) { return getinf_flag(INF_AF, AF_TRFUNC); }
inline bool inf_set_truncate_on_del(bool _v=true) { return setinf_flag(INF_AF, AF_TRFUNC, _v); }
inline bool inf_create_strlit_on_xref(void) { return getinf_flag(INF_AF, AF_STRLIT); }
inline bool inf_set_create_strlit_on_xref(bool _v=true) { return setinf_flag(INF_AF, AF_STRLIT, _v); }
inline bool inf_check_unicode_strlits(void) { return getinf_flag(INF_AF, AF_CHKUNI); }
inline bool inf_set_check_unicode_strlits(bool _v=true) { return setinf_flag(INF_AF, AF_CHKUNI, _v); }
inline bool inf_create_off_using_fixup(void) { return getinf_flag(INF_AF, AF_FIXUP); }
inline bool inf_set_create_off_using_fixup(bool _v=true) { return setinf_flag(INF_AF, AF_FIXUP, _v); }
inline bool inf_create_off_on_dref(void) { return getinf_flag(INF_AF, AF_DREFOFF); }
inline bool inf_set_create_off_on_dref(bool _v=true) { return setinf_flag(INF_AF, AF_DREFOFF, _v); }
inline bool inf_op_offset(void) { return getinf_flag(INF_AF, AF_IMMOFF); }
inline bool inf_set_op_offset(bool _v=true) { return setinf_flag(INF_AF, AF_IMMOFF, _v); }
inline bool inf_data_offset(void) { return getinf_flag(INF_AF, AF_DATOFF); }
inline bool inf_set_data_offset(bool _v=true) { return setinf_flag(INF_AF, AF_DATOFF, _v); }
inline bool inf_use_flirt(void) { return getinf_flag(INF_AF, AF_FLIRT); }
inline bool inf_set_use_flirt(bool _v=true) { return setinf_flag(INF_AF, AF_FLIRT, _v); }
inline bool inf_append_sigcmt(void) { return getinf_flag(INF_AF, AF_SIGCMT); }
inline bool inf_set_append_sigcmt(bool _v=true) { return setinf_flag(INF_AF, AF_SIGCMT, _v); }
inline bool inf_allow_sigmulti(void) { return getinf_flag(INF_AF, AF_SIGMLT); }
inline bool inf_set_allow_sigmulti(bool _v=true) { return setinf_flag(INF_AF, AF_SIGMLT, _v); }
inline bool inf_hide_libfuncs(void) { return getinf_flag(INF_AF, AF_HFLIRT); }
inline bool inf_set_hide_libfuncs(bool _v=true) { return setinf_flag(INF_AF, AF_HFLIRT, _v); }
inline bool inf_rename_jumpfunc(void) { return getinf_flag(INF_AF, AF_JFUNC); }
inline bool inf_set_rename_jumpfunc(bool _v=true) { return setinf_flag(INF_AF, AF_JFUNC, _v); }
inline bool inf_rename_nullsub(void) { return getinf_flag(INF_AF, AF_NULLSUB); }
inline bool inf_set_rename_nullsub(bool _v=true) { return setinf_flag(INF_AF, AF_NULLSUB, _v); }
inline bool inf_coagulate_data(void) { return getinf_flag(INF_AF, AF_DODATA); }
inline bool inf_set_coagulate_data(bool _v=true) { return setinf_flag(INF_AF, AF_DODATA, _v); }
inline bool inf_coagulate_code(void) { return getinf_flag(INF_AF, AF_DOCODE); }
inline bool inf_set_coagulate_code(bool _v=true) { return setinf_flag(INF_AF, AF_DOCODE, _v); }
inline bool inf_final_pass(void) { return getinf_flag(INF_AF, AF_FINAL); }
inline bool inf_set_final_pass(bool _v=true) { return setinf_flag(INF_AF, AF_FINAL, _v); }

inline uint32 inf_get_af2() { return uint32(getinf(INF_AF2)); }
inline bool inf_set_af2(uint32 _v) { return setinf(INF_AF2, ssize_t(_v)); }
inline bool inf_handle_eh(void) { return getinf_flag(INF_AF2, AF2_DOEH); }
inline bool inf_set_handle_eh(bool _v=true) { return setinf_flag(INF_AF2, AF2_DOEH, _v); }
inline bool inf_handle_rtti(void) { return getinf_flag(INF_AF2, AF2_DORTTI); }
inline bool inf_set_handle_rtti(bool _v=true) { return setinf_flag(INF_AF2, AF2_DORTTI, _v); }
inline bool inf_macros_enabled(void) { return getinf_flag(INF_AF2, AF2_MACRO); }
inline bool inf_set_macros_enabled(bool _v=true) { return setinf_flag(INF_AF2, AF2_MACRO, _v); }

inline uval_t inf_get_baseaddr() { return uval_t(getinf(INF_BASEADDR)); }
inline bool inf_set_baseaddr(uval_t _v) { return setinf(INF_BASEADDR, ssize_t(_v)); }

inline sel_t inf_get_start_ss() { return sel_t(getinf(INF_START_SS)); }
inline bool inf_set_start_ss(sel_t _v) { return setinf(INF_START_SS, ssize_t(_v)); }

inline sel_t inf_get_start_cs() { return sel_t(getinf(INF_START_CS)); }
inline bool inf_set_start_cs(sel_t _v) { return setinf(INF_START_CS, ssize_t(_v)); }

inline ea_t inf_get_start_ip() { return ea_t(getinf(INF_START_IP)); }
inline bool inf_set_start_ip(ea_t _v) { return setinf(INF_START_IP, ssize_t(_v)); }

inline ea_t inf_get_start_ea() { return ea_t(getinf(INF_START_EA)); }
inline bool inf_set_start_ea(ea_t _v) { return setinf(INF_START_EA, ssize_t(_v)); }

inline ea_t inf_get_start_sp() { return ea_t(getinf(INF_START_SP)); }
inline bool inf_set_start_sp(ea_t _v) { return setinf(INF_START_SP, ssize_t(_v)); }

inline ea_t inf_get_main() { return ea_t(getinf(INF_MAIN)); }
inline bool inf_set_main(ea_t _v) { return setinf(INF_MAIN, ssize_t(_v)); }

inline ea_t inf_get_min_ea() { return ea_t(getinf(INF_MIN_EA)); }
inline bool inf_set_min_ea(ea_t _v) { return setinf(INF_MIN_EA, ssize_t(_v)); }

inline ea_t inf_get_max_ea() { return ea_t(getinf(INF_MAX_EA)); }
inline bool inf_set_max_ea(ea_t _v) { return setinf(INF_MAX_EA, ssize_t(_v)); }

inline ea_t inf_get_omin_ea() { return ea_t(getinf(INF_OMIN_EA)); }
inline bool inf_set_omin_ea(ea_t _v) { return setinf(INF_OMIN_EA, ssize_t(_v)); }

inline ea_t inf_get_omax_ea() { return ea_t(getinf(INF_OMAX_EA)); }
inline bool inf_set_omax_ea(ea_t _v) { return setinf(INF_OMAX_EA, ssize_t(_v)); }

inline ea_t inf_get_lowoff() { return ea_t(getinf(INF_LOWOFF)); }
inline bool inf_set_lowoff(ea_t _v) { return setinf(INF_LOWOFF, ssize_t(_v)); }

inline ea_t inf_get_highoff() { return ea_t(getinf(INF_HIGHOFF)); }
inline bool inf_set_highoff(ea_t _v) { return setinf(INF_HIGHOFF, ssize_t(_v)); }

inline uval_t inf_get_maxref() { return uval_t(getinf(INF_MAXREF)); }
inline bool inf_set_maxref(uval_t _v) { return setinf(INF_MAXREF, ssize_t(_v)); }

inline sval_t inf_get_netdelta() { return sval_t(getinf(INF_NETDELTA)); }
inline bool inf_set_netdelta(sval_t _v) { return setinf(INF_NETDELTA, ssize_t(_v)); }

inline uchar inf_get_xrefnum() { return uchar(getinf(INF_XREFNUM)); }
inline bool inf_set_xrefnum(uchar _v) { return setinf(INF_XREFNUM, ssize_t(_v)); }

inline uchar inf_get_type_xrefnum() { return uchar(getinf(INF_TYPE_XREFNUM)); }
inline bool inf_set_type_xrefnum(uchar _v) { return setinf(INF_TYPE_XREFNUM, ssize_t(_v)); }

inline uchar inf_get_refcmtnum() { return uchar(getinf(INF_REFCMTNUM)); }
inline bool inf_set_refcmtnum(uchar _v) { return setinf(INF_REFCMTNUM, ssize_t(_v)); }

inline uchar inf_get_xrefflag() { return uchar(getinf(INF_XREFFLAG)); }
inline bool inf_set_xrefflag(uchar _v) { return setinf(INF_XREFFLAG, ssize_t(_v)); }
inline bool inf_show_xref_seg(void) { return getinf_flag(INF_XREFFLAG, SW_SEGXRF); }
inline bool inf_set_show_xref_seg(bool _v=true) { return setinf_flag(INF_XREFFLAG, SW_SEGXRF, _v); }
inline bool inf_show_xref_tmarks(void) { return getinf_flag(INF_XREFFLAG, SW_XRFMRK); }
inline bool inf_set_show_xref_tmarks(bool _v=true) { return setinf_flag(INF_XREFFLAG, SW_XRFMRK, _v); }
inline bool inf_show_xref_fncoff(void) { return getinf_flag(INF_XREFFLAG, SW_XRFFNC); }
inline bool inf_set_show_xref_fncoff(bool _v=true) { return setinf_flag(INF_XREFFLAG, SW_XRFFNC, _v); }
inline bool inf_show_xref_val(void) { return getinf_flag(INF_XREFFLAG, SW_XRFVAL); }
inline bool inf_set_show_xref_val(bool _v=true) { return setinf_flag(INF_XREFFLAG, SW_XRFVAL, _v); }

inline ushort inf_get_max_autoname_len() { return ushort(getinf(INF_MAX_AUTONAME_LEN)); }
inline bool inf_set_max_autoname_len(ushort _v) { return setinf(INF_MAX_AUTONAME_LEN, ssize_t(_v)); }

inline char inf_get_nametype() { return char(getinf(INF_NAMETYPE)); }
inline bool inf_set_nametype(char _v) { return setinf(INF_NAMETYPE, ssize_t(_v)); }

inline uint32 inf_get_short_demnames() { return uint32(getinf(INF_SHORT_DEMNAMES)); }
inline bool inf_set_short_demnames(uint32 _v) { return setinf(INF_SHORT_DEMNAMES, ssize_t(_v)); }

inline uint32 inf_get_long_demnames() { return uint32(getinf(INF_LONG_DEMNAMES)); }
inline bool inf_set_long_demnames(uint32 _v) { return setinf(INF_LONG_DEMNAMES, ssize_t(_v)); }

inline uchar inf_get_demnames() { return uchar(getinf(INF_DEMNAMES)); }
inline bool inf_set_demnames(uchar _v) { return setinf(INF_DEMNAMES, ssize_t(_v)); }

inline uchar inf_get_listnames() { return uchar(getinf(INF_LISTNAMES)); }
inline bool inf_set_listnames(uchar _v) { return setinf(INF_LISTNAMES, ssize_t(_v)); }

inline uchar inf_get_indent() { return uchar(getinf(INF_INDENT)); }
inline bool inf_set_indent(uchar _v) { return setinf(INF_INDENT, ssize_t(_v)); }

inline uchar inf_get_comment() { return uchar(getinf(INF_COMMENT)); }
inline bool inf_set_comment(uchar _v) { return setinf(INF_COMMENT, ssize_t(_v)); }

inline ushort inf_get_margin() { return ushort(getinf(INF_MARGIN)); }
inline bool inf_set_margin(ushort _v) { return setinf(INF_MARGIN, ssize_t(_v)); }

inline ushort inf_get_lenxref() { return ushort(getinf(INF_LENXREF)); }
inline bool inf_set_lenxref(ushort _v) { return setinf(INF_LENXREF, ssize_t(_v)); }

inline uint32 inf_get_outflags() { return uint32(getinf(INF_OUTFLAGS)); }
inline bool inf_set_outflags(uint32 _v) { return setinf(INF_OUTFLAGS, ssize_t(_v)); }
inline bool inf_show_void(void) { return getinf_flag(INF_OUTFLAGS, OFLG_SHOW_VOID); }
inline bool inf_set_show_void(bool _v=true) { return setinf_flag(INF_OUTFLAGS, OFLG_SHOW_VOID, _v); }
inline bool inf_show_auto(void) { return getinf_flag(INF_OUTFLAGS, OFLG_SHOW_AUTO); }
inline bool inf_set_show_auto(bool _v=true) { return setinf_flag(INF_OUTFLAGS, OFLG_SHOW_AUTO, _v); }
inline bool inf_gen_null(void) { return getinf_flag(INF_OUTFLAGS, OFLG_GEN_NULL); }
inline bool inf_set_gen_null(bool _v=true) { return setinf_flag(INF_OUTFLAGS, OFLG_GEN_NULL, _v); }
inline bool inf_show_line_pref(void) { return getinf_flag(INF_OUTFLAGS, OFLG_SHOW_PREF); }
inline bool inf_set_show_line_pref(bool _v=true) { return setinf_flag(INF_OUTFLAGS, OFLG_SHOW_PREF, _v); }
inline bool inf_line_pref_with_seg(void) { return getinf_flag(INF_OUTFLAGS, OFLG_PREF_SEG); }
inline bool inf_set_line_pref_with_seg(bool _v=true) { return setinf_flag(INF_OUTFLAGS, OFLG_PREF_SEG, _v); }
inline bool inf_gen_lzero(void) { return getinf_flag(INF_OUTFLAGS, OFLG_LZERO); }
inline bool inf_set_gen_lzero(bool _v=true) { return setinf_flag(INF_OUTFLAGS, OFLG_LZERO, _v); }
inline bool inf_gen_org(void) { return getinf_flag(INF_OUTFLAGS, OFLG_GEN_ORG); }
inline bool inf_set_gen_org(bool _v=true) { return setinf_flag(INF_OUTFLAGS, OFLG_GEN_ORG, _v); }
inline bool inf_gen_assume(void) { return getinf_flag(INF_OUTFLAGS, OFLG_GEN_ASSUME); }
inline bool inf_set_gen_assume(bool _v=true) { return setinf_flag(INF_OUTFLAGS, OFLG_GEN_ASSUME, _v); }
inline bool inf_gen_tryblks(void) { return getinf_flag(INF_OUTFLAGS, OFLG_GEN_TRYBLKS); }
inline bool inf_set_gen_tryblks(bool _v=true) { return setinf_flag(INF_OUTFLAGS, OFLG_GEN_TRYBLKS, _v); }

inline uchar inf_get_cmtflg() { return uchar(getinf(INF_CMTFLG)); }
inline bool inf_set_cmtflg(uchar _v) { return setinf(INF_CMTFLG, ssize_t(_v)); }
inline bool inf_show_repeatables(void) { return getinf_flag(INF_CMTFLG, SCF_RPTCMT); }
inline bool inf_set_show_repeatables(bool _v=true) { return setinf_flag(INF_CMTFLG, SCF_RPTCMT, _v); }
inline bool inf_show_all_comments(void) { return getinf_flag(INF_CMTFLG, SCF_ALLCMT); }
inline bool inf_set_show_all_comments(bool _v=true) { return setinf_flag(INF_CMTFLG, SCF_ALLCMT, _v); }
inline bool inf_hide_comments(void) { return getinf_flag(INF_CMTFLG, SCF_NOCMT); }
inline bool inf_set_hide_comments(bool _v=true) { return setinf_flag(INF_CMTFLG, SCF_NOCMT, _v); }
inline bool inf_show_src_linnum(void) { return getinf_flag(INF_CMTFLG, SCF_LINNUM); }
inline bool inf_set_show_src_linnum(bool _v=true) { return setinf_flag(INF_CMTFLG, SCF_LINNUM, _v); }
inline bool inf_test_mode(void) { return getinf_flag(INF_CMTFLG, SCF_TESTMODE); }
inline bool inf_show_hidden_insns(void) { return getinf_flag(INF_CMTFLG, SCF_SHHID_ITEM); }
inline bool inf_set_show_hidden_insns(bool _v=true) { return setinf_flag(INF_CMTFLG, SCF_SHHID_ITEM, _v); }
inline bool inf_show_hidden_funcs(void) { return getinf_flag(INF_CMTFLG, SCF_SHHID_FUNC); }
inline bool inf_set_show_hidden_funcs(bool _v=true) { return setinf_flag(INF_CMTFLG, SCF_SHHID_FUNC, _v); }
inline bool inf_show_hidden_segms(void) { return getinf_flag(INF_CMTFLG, SCF_SHHID_SEGM); }
inline bool inf_set_show_hidden_segms(bool _v=true) { return setinf_flag(INF_CMTFLG, SCF_SHHID_SEGM, _v); }

inline uchar inf_get_limiter() { return uchar(getinf(INF_LIMITER)); }
inline bool inf_set_limiter(uchar _v) { return setinf(INF_LIMITER, ssize_t(_v)); }
inline bool inf_is_limiter_thin(void) { return getinf_flag(INF_LIMITER, LMT_THIN); }
inline bool inf_set_limiter_thin(bool _v=true) { return setinf_flag(INF_LIMITER, LMT_THIN, _v); }
inline bool inf_is_limiter_thick(void) { return getinf_flag(INF_LIMITER, LMT_THICK); }
inline bool inf_set_limiter_thick(bool _v=true) { return setinf_flag(INF_LIMITER, LMT_THICK, _v); }
inline bool inf_is_limiter_empty(void) { return getinf_flag(INF_LIMITER, LMT_EMPTY); }
inline bool inf_set_limiter_empty(bool _v=true) { return setinf_flag(INF_LIMITER, LMT_EMPTY, _v); }

inline short inf_get_bin_prefix_size() { return short(getinf(INF_BIN_PREFIX_SIZE)); }
inline bool inf_set_bin_prefix_size(short _v) { return setinf(INF_BIN_PREFIX_SIZE, ssize_t(_v)); }

inline uchar inf_get_prefflag() { return uchar(getinf(INF_PREFFLAG)); }
inline bool inf_set_prefflag(uchar _v) { return setinf(INF_PREFFLAG, ssize_t(_v)); }
inline bool inf_prefix_show_segaddr(void) { return getinf_flag(INF_PREFFLAG, PREF_SEGADR); }
inline bool inf_set_prefix_show_segaddr(bool _v=true) { return setinf_flag(INF_PREFFLAG, PREF_SEGADR, _v); }
inline bool inf_prefix_show_funcoff(void) { return getinf_flag(INF_PREFFLAG, PREF_FNCOFF); }
inline bool inf_set_prefix_show_funcoff(bool _v=true) { return setinf_flag(INF_PREFFLAG, PREF_FNCOFF, _v); }
inline bool inf_prefix_show_stack(void) { return getinf_flag(INF_PREFFLAG, PREF_STACK); }
inline bool inf_set_prefix_show_stack(bool _v=true) { return setinf_flag(INF_PREFFLAG, PREF_STACK, _v); }
inline bool inf_prefix_truncate_opcode_bytes(void) { return getinf_flag(INF_PREFFLAG, PREF_PFXTRUNC); }
inline bool inf_set_prefix_truncate_opcode_bytes(bool _v=true) { return setinf_flag(INF_PREFFLAG, PREF_PFXTRUNC, _v); }

inline uchar inf_get_strlit_flags() { return uchar(getinf(INF_STRLIT_FLAGS)); }
inline bool inf_set_strlit_flags(uchar _v) { return setinf(INF_STRLIT_FLAGS, ssize_t(_v)); }
inline bool inf_strlit_names(void) { return getinf_flag(INF_STRLIT_FLAGS, STRF_GEN); }
inline bool inf_set_strlit_names(bool _v=true) { return setinf_flag(INF_STRLIT_FLAGS, STRF_GEN, _v); }
inline bool inf_strlit_name_bit(void) { return getinf_flag(INF_STRLIT_FLAGS, STRF_AUTO); }
inline bool inf_set_strlit_name_bit(bool _v=true) { return setinf_flag(INF_STRLIT_FLAGS, STRF_AUTO, _v); }
inline bool inf_strlit_serial_names(void) { return getinf_flag(INF_STRLIT_FLAGS, STRF_SERIAL); }
inline bool inf_set_strlit_serial_names(bool _v=true) { return setinf_flag(INF_STRLIT_FLAGS, STRF_SERIAL, _v); }
inline bool inf_unicode_strlits(void) { return getinf_flag(INF_STRLIT_FLAGS, STRF_UNICODE); }
inline bool inf_set_unicode_strlits(bool _v=true) { return setinf_flag(INF_STRLIT_FLAGS, STRF_UNICODE, _v); }
inline bool inf_strlit_autocmt(void) { return getinf_flag(INF_STRLIT_FLAGS, STRF_COMMENT); }
inline bool inf_set_strlit_autocmt(bool _v=true) { return setinf_flag(INF_STRLIT_FLAGS, STRF_COMMENT, _v); }
inline bool inf_strlit_savecase(void) { return getinf_flag(INF_STRLIT_FLAGS, STRF_SAVECASE); }
inline bool inf_set_strlit_savecase(bool _v=true) { return setinf_flag(INF_STRLIT_FLAGS, STRF_SAVECASE, _v); }

inline uchar inf_get_strlit_break() { return uchar(getinf(INF_STRLIT_BREAK)); }
inline bool inf_set_strlit_break(uchar _v) { return setinf(INF_STRLIT_BREAK, ssize_t(_v)); }

inline char inf_get_strlit_zeroes() { return char(getinf(INF_STRLIT_ZEROES)); }
inline bool inf_set_strlit_zeroes(char _v) { return setinf(INF_STRLIT_ZEROES, ssize_t(_v)); }

inline int32 inf_get_strtype() { return int32(getinf(INF_STRTYPE)); }
inline bool inf_set_strtype(int32 _v) { return setinf(INF_STRTYPE, ssize_t(_v)); }

inline uval_t inf_get_strlit_sernum() { return uval_t(getinf(INF_STRLIT_SERNUM)); }
inline bool inf_set_strlit_sernum(uval_t _v) { return setinf(INF_STRLIT_SERNUM, ssize_t(_v)); }

inline uval_t inf_get_datatypes() { return uval_t(getinf(INF_DATATYPES)); }
inline bool inf_set_datatypes(uval_t _v) { return setinf(INF_DATATYPES, ssize_t(_v)); }

inline uint32 inf_get_abibits() { return uint32(getinf(INF_ABIBITS)); }
inline bool inf_set_abibits(uint32 _v) { return setinf(INF_ABIBITS, ssize_t(_v)); }
inline bool inf_is_mem_aligned4(void) { return getinf_flag(INF_ABIBITS, ABI_8ALIGN4); }
inline bool inf_set_mem_aligned4(bool _v=true) { return setinf_flag(INF_ABIBITS, ABI_8ALIGN4, _v); }
inline bool inf_pack_stkargs(void) { return getinf_flag(INF_ABIBITS, ABI_PACK_STKARGS); }
inline bool inf_set_pack_stkargs(bool _v=true) { return setinf_flag(INF_ABIBITS, ABI_PACK_STKARGS, _v); }
inline bool inf_big_arg_align(void) { return getinf_flag(INF_ABIBITS, ABI_BIGARG_ALIGN); }
inline bool inf_set_big_arg_align(bool _v=true) { return setinf_flag(INF_ABIBITS, ABI_BIGARG_ALIGN, _v); }
inline bool inf_huge_arg_align(void) { return getinf_flag(INF_ABIBITS, ABI_HUGEARG_ALIGN); }
inline bool inf_set_huge_arg_align(bool _v=true) { return setinf_flag(INF_ABIBITS, ABI_HUGEARG_ALIGN, _v); }
inline bool inf_stack_ldbl(void) { return getinf_flag(INF_ABIBITS, ABI_STACK_LDBL); }
inline bool inf_set_stack_ldbl(bool _v=true) { return setinf_flag(INF_ABIBITS, ABI_STACK_LDBL, _v); }
inline bool inf_stack_varargs(void) { return getinf_flag(INF_ABIBITS, ABI_STACK_VARARGS); }
inline bool inf_set_stack_varargs(bool _v=true) { return setinf_flag(INF_ABIBITS, ABI_STACK_VARARGS, _v); }
inline bool inf_is_hard_float(void) { return getinf_flag(INF_ABIBITS, ABI_HARD_FLOAT); }
inline bool inf_set_hard_float(bool _v=true) { return setinf_flag(INF_ABIBITS, ABI_HARD_FLOAT, _v); }
inline bool inf_abi_set_by_user(void) { return getinf_flag(INF_ABIBITS, ABI_SET_BY_USER); }
inline bool inf_set_abi_set_by_user(bool _v=true) { return setinf_flag(INF_ABIBITS, ABI_SET_BY_USER, _v); }
inline bool inf_use_gcc_layout(void) { return getinf_flag(INF_ABIBITS, ABI_GCC_LAYOUT); }
inline bool inf_set_use_gcc_layout(bool _v=true) { return setinf_flag(INF_ABIBITS, ABI_GCC_LAYOUT, _v); }
inline bool inf_map_stkargs(void) { return getinf_flag(INF_ABIBITS, ABI_MAP_STKARGS); }
inline bool inf_set_map_stkargs(bool _v=true) { return setinf_flag(INF_ABIBITS, ABI_MAP_STKARGS, _v); }

inline uint32 inf_get_appcall_options() { return uint32(getinf(INF_APPCALL_OPTIONS)); }
inline bool inf_set_appcall_options(uint32 _v) { return setinf(INF_APPCALL_OPTIONS, ssize_t(_v)); }

inline ea_t inf_get_privrange_start_ea() { return ea_t(getinf(INF_PRIVRANGE_START_EA)); }
inline bool inf_set_privrange_start_ea(ea_t _v) { return setinf(INF_PRIVRANGE_START_EA, ssize_t(_v)); }

inline ea_t inf_get_privrange_end_ea() { return ea_t(getinf(INF_PRIVRANGE_END_EA)); }
inline bool inf_set_privrange_end_ea(ea_t _v) { return setinf(INF_PRIVRANGE_END_EA, ssize_t(_v)); }

inline comp_t inf_get_cc_id() { return comp_t(getinf(INF_CC_ID)); }
inline bool inf_set_cc_id(comp_t _v) { return setinf(INF_CC_ID, ssize_t(_v)); }

inline cm_t inf_get_cc_cm() { return cm_t(getinf(INF_CC_CM)); }
inline bool inf_set_cc_cm(cm_t _v) { return setinf(INF_CC_CM, ssize_t(_v)); }

inline uchar inf_get_cc_size_i() { return uchar(getinf(INF_CC_SIZE_I)); }
inline bool inf_set_cc_size_i(uchar _v) { return setinf(INF_CC_SIZE_I, ssize_t(_v)); }

inline uchar inf_get_cc_size_b() { return uchar(getinf(INF_CC_SIZE_B)); }
inline bool inf_set_cc_size_b(uchar _v) { return setinf(INF_CC_SIZE_B, ssize_t(_v)); }

inline uchar inf_get_cc_size_e() { return uchar(getinf(INF_CC_SIZE_E)); }
inline bool inf_set_cc_size_e(uchar _v) { return setinf(INF_CC_SIZE_E, ssize_t(_v)); }

inline uchar inf_get_cc_defalign() { return uchar(getinf(INF_CC_DEFALIGN)); }
inline bool inf_set_cc_defalign(uchar _v) { return setinf(INF_CC_DEFALIGN, ssize_t(_v)); }

inline uchar inf_get_cc_size_s() { return uchar(getinf(INF_CC_SIZE_S)); }
inline bool inf_set_cc_size_s(uchar _v) { return setinf(INF_CC_SIZE_S, ssize_t(_v)); }

inline uchar inf_get_cc_size_l() { return uchar(getinf(INF_CC_SIZE_L)); }
inline bool inf_set_cc_size_l(uchar _v) { return setinf(INF_CC_SIZE_L, ssize_t(_v)); }

inline uchar inf_get_cc_size_ll() { return uchar(getinf(INF_CC_SIZE_LL)); }
inline bool inf_set_cc_size_ll(uchar _v) { return setinf(INF_CC_SIZE_LL, ssize_t(_v)); }

inline uchar inf_get_cc_size_ldbl() { return uchar(getinf(INF_CC_SIZE_LDBL)); }
inline bool inf_set_cc_size_ldbl(uchar _v) { return setinf(INF_CC_SIZE_LDBL, ssize_t(_v)); }

inline bool inf_get_procname(char *buf, size_t bufsize=IDAINFO_PROCNAME_SIZE)
{
  return (bool)getinf_buf(INF_PROCNAME, buf, bufsize);
}
inline qstring inf_get_procname()
{
  char buf[IDAINFO_PROCNAME_SIZE];
  if ( !getinf_buf(INF_PROCNAME, buf, sizeof(buf)) )
    buf[0] = '\0';
  return qstring(buf);
}
inline bool inf_set_procname(const char *_v, size_t len=size_t(-1))
{
  if ( _v == NULL )
    return false;
  if ( len == size_t(-1) )
    len = ::qstrlen(_v);
  return setinf_buf(INF_PROCNAME, _v, qmin(len, IDAINFO_PROCNAME_SIZE));
}

inline bool inf_get_strlit_pref(char *buf, size_t bufsize=IDAINFO_STRLIT_PREF_SIZE)
{
  return (bool)getinf_buf(INF_STRLIT_PREF, buf, bufsize);
}
inline qstring inf_get_strlit_pref()
{
  char buf[IDAINFO_STRLIT_PREF_SIZE];
  if ( !getinf_buf(INF_STRLIT_PREF, buf, sizeof(buf)) )
    buf[0] = '\0';
  return qstring(buf);
}
inline bool inf_set_strlit_pref(const char *_v, size_t len=size_t(-1))
{
  if ( _v == NULL )
    return false;
  if ( len == size_t(-1) )
    len = ::qstrlen(_v);
  return setinf_buf(INF_STRLIT_PREF, _v, qmin(len, IDAINFO_STRLIT_PREF_SIZE));
}

inline bool inf_get_cc(compiler_info_t *out) { return (bool)getinf_buf(INF_CC, out, sizeof(*out)); }
inline bool inf_set_cc(const compiler_info_t &_v) { return setinf_buf(INF_CC, &_v, sizeof(_v)); }

inline bool inf_get_privrange(range_t *out) { return (bool)getinf_buf(INF_PRIVRANGE, out, sizeof(*out)); }
inline bool inf_set_privrange(const range_t &_v) { return setinf_buf(INF_PRIVRANGE, &_v, sizeof(_v)); }

// </INF_ACCESSORS>

inline range_t inf_get_privrange() { range_t r; inf_get_privrange(&r); return r; }

/// Get/set low/high 16bit halves of inf.af
inline ushort inf_get_af_low() { return inf_get_af() & 0xffff; }
inline void inf_set_af_low(ushort saf) { uint32 af = (inf_get_af() & 0xffff0000) | saf; inf_set_af(af); }
inline ushort inf_get_af_high() { return (inf_get_af() >> 16) & 0xffff; }
inline void inf_set_af_high(ushort saf2) { uint32 af = (inf_get_af() & 0xffff) | (saf2 << 16); inf_set_af(af); }
/// Get/set low 16bit half of inf.af2
inline ushort inf_get_af2_low() { return inf_get_af2() & 0xffff; }
inline void inf_set_af2_low(ushort saf) { uint32 af2 = (inf_get_af2() & 0xffff0000) | saf; inf_set_af2(af2); }

inline int inf_get_pack_mode()
{
  uint32 lflags = inf_get_lflags();
  return (lflags & LFLG_COMPRESS) != 0 ? IDB_COMPRESSED
       : (lflags & LFLG_PACK) != 0     ? IDB_PACKED
       :                                 IDB_UNPACKED;
}

inline int inf_set_pack_mode(int pack_mode)
{
  int old = inf_get_pack_mode();
  uint32 lflags = inf_get_lflags();
  setflag(lflags, LFLG_COMPRESS, pack_mode == IDB_COMPRESSED);
  setflag(lflags, LFLG_PACK,     pack_mode == IDB_PACKED);
  inf_set_lflags(lflags);
  return old;
}

inline void inf_inc_database_change_count(int cnt=1)
{ inf_set_database_change_count(inf_get_database_change_count()+cnt); }

/// Get #DEMNAM_MASK bits of #demnames
inline uchar inf_get_demname_form(void) { return uchar(inf_get_demnames() & DEMNAM_MASK); }

inline uval_t inf_postinc_strlit_sernum(uval_t cnt=1)
{
  uval_t was = inf_get_strlit_sernum();
  inf_set_strlit_sernum(was + cnt);
  return was;
}

inline bool inf_like_binary(void)
{ return is_filetype_like_binary(inf_get_filetype()); }



//-------------------------------------------------------------------------
/// max number of operands allowed for an instruction

#define UA_MAXOP        8


/// Get default disassembly line options

inline int calc_default_idaplace_flags(void)
{
  return 0;
}

//-------------------------------------------------------------------------
/// \def{MAXADDR, Start of the private netnode range}
/// In the past this was the maximal address of the disassembly listing.
#ifdef __EA64__
#ifdef __GNUC__
#define MAXADDR         0xFF00000000000000ll
#else
#define MAXADDR         0xFF00000000000000ui64
#endif
#else
#define MAXADDR         0xFF000000L
#endif


//-------------------------------------------------------------------------
/// Convert (sel,off) value to a linear address

inline ea_t idaapi to_ea(sel_t reg_cs, uval_t reg_ip)
{
  return (reg_cs << 4) + reg_ip;
}

/// \def{IDB_EXT32, Database file extension for 32-bit programs}
/// \def{IDB_EXT64, Database file extension for 64-bit programs}
/// \def{IDB_EXT,   Database file extension}
#define IDB_EXT32 "idb"
#define IDB_EXT64 "i64"
#ifdef __EA64__
#define IDB_EXT IDB_EXT64
#else
#define IDB_EXT IDB_EXT32
#endif

//--------------------------------------------------------------------------
/// Callback provided to hook_to_notification_point().
/// A plugin can hook to a notification point and receive notifications
/// of all major events in IDA. The callback function will be called
/// for each event.
/// \param user_data          data supplied in call to hook_to_notification_point()
/// \param notification_code  processor_t::event_t or ::ui_notification_t or
///                           debugger_t::event_t, depending on the hook type
/// \param va                 additional parameters supplied with the notification.
///                           see the event descriptions for information
/// \retval 0    ok, the event should be processed further
/// \retval !=0  the event is blocked and should be discarded.
///              in the case of processor modules, the returned value is used
///              as the return value of processor_t::notify()

typedef ssize_t idaapi hook_cb_t(void *user_data, int notification_code, va_list va);

/// Types of events that be hooked to with hook_to_notification_point()
enum hook_type_t
{
  HT_IDP,         ///< Hook to the processor module.
                  ///< The callback will receive all processor_t::event_t events.
  HT_UI,          ///< Hook to the user interface.
                  ///< The callback will receive all ::ui_notification_t events.
  HT_DBG,         ///< Hook to the debugger.
                  ///< The callback will receive all ::dbg_notification_t events.
  HT_IDB,         ///< Hook to the database events.
                  ///< These events are separated from the ::HT_IDP group
                  ///< to speed things up (there are too many plugins and
                  ///< modules hooking to the ::HT_IDP). Some essential events
                  ///< are still generated in th ::HT_IDP group:
                  ///< make_code, make_data
                  ///< This list is not exhaustive.
                  ///< A common trait of all events in this group: the kernel
                  ///< does not expect any reaction to the event and does not
                  ///< check the return code. For event names, see ::idb_event.
  HT_DEV,         ///< Internal debugger events.
                  ///< Not stable and undocumented for the moment
  HT_VIEW,        ///< Custom/IDA views notifications.
                  ///< Refer to ::view_notification_t
                  ///< for notification codes
  HT_OUTPUT,      ///< Output window notifications.
                  ///< Refer to ::msg_notification_t
                  ///< (::view_notification_t)
  HT_GRAPH,       ///< Handling graph operations
                  ///< (::graph_notification_t)
  HT_IDD,         ///< Hook to the debugger plugin.
                  ///< The callback will receive all debugger_t::event_t events.
  HT_LAST
};


/// Register a callback for a class of events in IDA

idaman bool ida_export hook_to_notification_point(
        hook_type_t hook_type,
        hook_cb_t *cb,
        void *user_data = NULL);


/// Unregister a callback (also see hook_to_notification_point()).
/// A plugin should unhook before being unloaded
/// (preferably in its termination function).
/// If different callbacks have the same callback function pointer
/// and user_data is not NULL, only the callback whose associated
/// user defined data matches will be removed.
/// \return number of unhooked functions.

idaman int ida_export unhook_from_notification_point(
        hook_type_t hook_type,
        hook_cb_t *cb,
        void *user_data = NULL);


/// Generate event notification.
/// \param hook_type          hook type
/// \param notification_code  event code
/// \param va                 additional arguments
/// \return !=0: event processed

idaman ssize_t ida_export invoke_callbacks(hook_type_t hook_type, int notification_code, va_list va);


/// Implements action to take when the event was processed by the notification callback
/// (i.e. when the notification callback returns non-zero code).
/// All the registered (\ref register_post_event_visitor()) post-event visitors will be
/// called in turn after the event was processed by one of the notification callbacks.

struct post_event_visitor_t
{
  /// Action for post-event management.
  /// May change the result of the processed event or data carried by the event.
  /// \param code               the notification callback or other post-event action
  ///                           return code (result of the processed event)
  /// \param notification_code  event code
  /// \param va                 additional arguments, the same as for processed event
  /// \return result of the event processed

  virtual ssize_t idaapi handle_post_event(
        ssize_t code,
        int notification_code,
        va_list va) = 0;

  DEFINE_VIRTUAL_DTOR(post_event_visitor_t)
};


/// Register the post-event visitor.
/// The kernel will not take ownership, nor delete the VISITOR instance.
/// Therefore, it's up to the plugin to handle it.
/// In addition, the post_event_visitor_t will be automatically unregistered
/// when the owner plugin is unloaded from memory.
/// \param hook_type  hook type
/// \param visitor    post-event visitor
/// \param owner      the owner plugin of the post_event_visitor_t type
/// \return success

idaman bool ida_export register_post_event_visitor(
        hook_type_t hook_type,
        post_event_visitor_t *visitor,
        const plugmod_t *owner);


/// Unregister the post-event visitor.
/// \param hook_type  hook type
/// \param visitor    post-event visitor
/// \return success

idaman bool ida_export unregister_post_event_visitor(
        hook_type_t hook_type,
        post_event_visitor_t *visitor);



#endif // _IDA_HPP
