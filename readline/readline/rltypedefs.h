/* rltypedefs.h -- Type declarations for readline functions. */

/* Copyright (C) 2000-2011 Free Software Foundation, Inc.

   This file is part of the GNU Readline Library (Readline), a library
   for reading lines of text with interactive input and history editing.      

   Readline is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Readline is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Readline.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _RL_TYPEDEFS_H_
#define _RL_TYPEDEFS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Old-style, attempt to mark as deprecated in some way people will notice. */

#if !defined (_FUNCTION_DEF)
#  define _FUNCTION_DEF

#if defined(__GNUC__) || defined(__clang__)
typedef int Function () __attribute__ ((deprecated));
typedef void VFunction () __attribute__ ((deprecated));
typedef char *CPFunction () __attribute__ ((deprecated));
typedef char **CPPFunction () __attribute__ ((deprecated));
#else
typedef int Function ();
typedef void VFunction ();
typedef char *CPFunction ();
typedef char **CPPFunction ();
#endif

#endif /* _FUNCTION_DEF */

/* New style. */

#if !defined (_RL_FUNCTION_TYPEDEF)
#  define _RL_FUNCTION_TYPEDEF

/* Bindable functions */
typedef int rl_command_func_t PARAMS((int, int));

/* Typedefs for the completion system */
typedef char *rl_compentry_func_t PARAMS((const char *, int));
typedef char **rl_completion_func_t PARAMS((const char *, int, int));

typedef char *rl_quote_func_t PARAMS((char *, int, char *));
typedef char *rl_dequote_func_t PARAMS((char *, int));

typedef int rl_compignore_func_t PARAMS((char **));

typedef void rl_compdisp_func_t PARAMS((char **, int, int));

/* Type for input and pre-read hook functions like rl_event_hook */
typedef int rl_hook_func_t PARAMS((void));

/* begin_clink_change */
/* Type for add/remove history hook function */
typedef int rl_history_hook_func_t PARAMS((int rl_history_index, const char* line));
/* Type for readkey input in modal situations like the pager */
typedef int rl_read_key_hook_func_t PARAMS((void));
/* Type for sort match list function */
typedef void rl_qsort_match_list_func_t PARAMS((char**, int len));
/* Type for adjusting completion word hook function */
typedef char rl_adjcmpwrd_func_t PARAMS((char qc, int *fp, int *dp));
/* Type for comparing lcd hook function */
typedef int rl_compare_lcd_func_t PARAMS((const char *, const char *));
/* Type for postprocessing the lcd hook function */
typedef void rl_postprocess_lcd_func_t PARAMS((char *, const char *));
/* Type for function to get face for char in input buffer */
typedef char rl_get_face_func_t PARAMS((int in, int active_begin, int active_end));
/* Type for function to print string with face */
typedef void rl_puts_face_func_t PARAMS((const char* s, const char* face, int n));
/* Type for function to process macros */
typedef int rl_macro_hook_func_t PARAMS((const char* macro));
/* end_clink_change */

/* Input function type */
typedef int rl_getc_func_t PARAMS((FILE *));

/* Generic function that takes a character buffer (which could be the readline
   line buffer) and an index into it (which could be rl_point) and returns
   an int. */
typedef int rl_linebuf_func_t PARAMS((char *, int));

/* `Generic' function pointer typedefs */
typedef int rl_intfunc_t PARAMS((int));
#define rl_ivoidfunc_t rl_hook_func_t
typedef int rl_icpfunc_t PARAMS((char *));
typedef int rl_icppfunc_t PARAMS((char **));

typedef void rl_voidfunc_t PARAMS((void));
typedef void rl_vintfunc_t PARAMS((int));
typedef void rl_vcpfunc_t PARAMS((char *));
typedef void rl_vcppfunc_t PARAMS((char **));

typedef char *rl_cpvfunc_t PARAMS((void));
typedef char *rl_cpifunc_t PARAMS((int));
typedef char *rl_cpcpfunc_t PARAMS((char  *));
typedef char *rl_cpcppfunc_t PARAMS((char  **));

/* begin_clink_change */
typedef int rl_iccpfunc_t PARAMS((const char*));
/* end_clink_change */

#endif /* _RL_FUNCTION_TYPEDEF */

#ifdef __cplusplus
}
#endif

#endif /* _RL_TYPEDEFS_H_ */
