/*
 *      The Interactive Disassembler (IDA).
 *      Copyright (c) 1990-2020 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 */

#ifndef _STRLIST_HPP
#define _STRLIST_HPP

/*! \file strlist.hpp

  \brief Functions that deal with the string list

  While the kernel keeps the strings list, it does not update it.
  The string list is not used by the kernel because
  keeping it up-to-date would slow down IDA without any benefit.

  The users of this list should build_strlist() before accessing it.
*/

/// Structure to keep string list parameters
struct strwinsetup_t
{
  strwinsetup_t()
    : minlen(-1), display_only_existing_strings(0),
      only_7bit(1), ignore_heads(0) {}
  bytevec_t strtypes; // set of allowed string types
  sval_t minlen;
  uchar display_only_existing_strings;
  uchar only_7bit;
  uchar ignore_heads;

  inline bool is_initialized() const { return minlen > -1; }

};

/// Information about one string from the string list
struct string_info_t
{
  ea_t ea;
  int length; // in octets
  int type;
  string_info_t() : ea(BADADDR), length(0), type(0) {}
  string_info_t(ea_t _ea) : ea(_ea), length(0), type(0) {}
  bool operator<(const string_info_t &r) const { return ea < r.ea; }
};
DECLARE_TYPE_AS_MOVABLE(string_info_t);


/// Get access to the static string list options

idaman strwinsetup_t *ida_export get_strlist_options();


/// Build the string list.
/// You should initialize options before this call using the
/// restore_config() or setup_strings_window() methods.

idaman void ida_export build_strlist();


/// Clear the string list.

idaman void ida_export clear_strlist();


/// Get number of elements in the string list

idaman size_t ida_export get_strlist_qty(void);


/// Get nth element of the string list (n=0..get_strlist_qty()-1)

idaman bool ida_export get_strlist_item(string_info_t *si, size_t n);


#endif // _STRLIST_HPP
