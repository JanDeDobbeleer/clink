/* Readline.h -- the names of functions callable from within readline. */

/* Copyright (C) 1987-2020 Free Software Foundation, Inc.

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

#if !defined (_READLINE_H_)
#define _READLINE_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined (READLINE_LIBRARY)
#  include "rlstdc.h"
#  include "rltypedefs.h"
#  include "keymaps.h"
#  include "tilde.h"
#else
#  include <readline/rlstdc.h>
#  include <readline/rltypedefs.h>
#  include <readline/keymaps.h>
#  include <readline/tilde.h>
#endif

/* Hex-encoded Readline version number. */
#define RL_READLINE_VERSION	0x0801		/* Readline 8.0 */
#define RL_VERSION_MAJOR	8
#define RL_VERSION_MINOR	1

/* begin_clink_change */
#if defined(_MSC_VER)
#  undef RL_LIBRARY_VERSION
#  define VER_AS_STR(x)       #x
#  define MAKE_LIBRARY_VERSION(maj, min)  VER_AS_STR(maj) ## "." ## VER_AS_STR(min)
#  define RL_LIBRARY_VERSION  MAKE_LIBRARY_VERSION(RL_VERSION_MAJOR, RL_VERSION_MINOR)
#endif
/* end_clink_change */


/* Readline data structures. */

/* Maintaining the state of undo.  We remember individual deletes and inserts
   on a chain of things to do. */

/* The actions that undo knows how to undo.  Notice that UNDO_DELETE means
   to insert some text, and UNDO_INSERT means to delete some text.   I.e.,
   the code tells undo what to undo, not how to undo it. */
enum undo_code { UNDO_DELETE, UNDO_INSERT, UNDO_BEGIN, UNDO_END };

/* What an element of THE_UNDO_LIST looks like. */
typedef struct undo_list {
  struct undo_list *next;
  int start, end;		/* Where the change took place. */
  char *text;			/* The text to insert, if undoing a delete. */
  enum undo_code what;		/* Delete, Insert, Begin, End. */
} UNDO_LIST;

/* The current undo list for RL_LINE_BUFFER. */
extern UNDO_LIST *rl_undo_list;

/* The data structure for mapping textual names to code addresses. */
typedef struct _funmap {
  const char *name;
  rl_command_func_t *function;
} FUNMAP;

extern FUNMAP **funmap;

/* **************************************************************** */
/*								    */
/*	     Functions available to bind to key sequences	    */
/*								    */
/* **************************************************************** */

/* Bindable commands for numeric arguments. */
extern int rl_digit_argument PARAMS((int, int));
extern int rl_universal_argument PARAMS((int, int));

/* Bindable commands for moving the cursor. */
extern int rl_forward_byte PARAMS((int, int));
extern int rl_forward_char PARAMS((int, int));
extern int rl_forward PARAMS((int, int));
extern int rl_backward_byte PARAMS((int, int));
extern int rl_backward_char PARAMS((int, int));
extern int rl_backward PARAMS((int, int));
extern int rl_beg_of_line PARAMS((int, int));
extern int rl_end_of_line PARAMS((int, int));
extern int rl_forward_word PARAMS((int, int));
extern int rl_backward_word PARAMS((int, int));
extern int rl_refresh_line PARAMS((int, int));
extern int rl_clear_screen PARAMS((int, int));
extern int rl_clear_display PARAMS((int, int));
extern int rl_skip_csi_sequence PARAMS((int, int));
extern int rl_arrow_keys PARAMS((int, int));

extern int rl_previous_screen_line PARAMS((int, int));
extern int rl_next_screen_line PARAMS((int, int));

/* Bindable commands for inserting and deleting text. */
extern int rl_insert PARAMS((int, int));
extern int rl_quoted_insert PARAMS((int, int));
extern int rl_tab_insert PARAMS((int, int));
extern int rl_newline PARAMS((int, int));
extern int rl_do_lowercase_version PARAMS((int, int));
extern int rl_rubout PARAMS((int, int));
extern int rl_delete PARAMS((int, int));
extern int rl_rubout_or_delete PARAMS((int, int));
extern int rl_delete_horizontal_space PARAMS((int, int));
extern int rl_delete_or_show_completions PARAMS((int, int));
extern int rl_insert_comment PARAMS((int, int));

/* Bindable commands for changing case. */
extern int rl_upcase_word PARAMS((int, int));
extern int rl_downcase_word PARAMS((int, int));
extern int rl_capitalize_word PARAMS((int, int));

/* Bindable commands for transposing characters and words. */
extern int rl_transpose_words PARAMS((int, int));
extern int rl_transpose_chars PARAMS((int, int));

/* Bindable commands for searching within a line. */
extern int rl_char_search PARAMS((int, int));
extern int rl_backward_char_search PARAMS((int, int));

/* Bindable commands for readline's interface to the command history. */
/* begin_clink_change */
extern int rl_add_history PARAMS((int, int));
/* end_clink_change */
extern int rl_beginning_of_history PARAMS((int, int));
extern int rl_end_of_history PARAMS((int, int));
extern int rl_get_next_history PARAMS((int, int));
extern int rl_get_previous_history PARAMS((int, int));
extern int rl_operate_and_get_next PARAMS((int, int));
/* begin_clink_change */
extern int rl_remove_history PARAMS((int, int));
/* end_clink_change */

/* Bindable commands for managing the mark and region. */
extern int rl_set_mark PARAMS((int, int));
extern int rl_exchange_point_and_mark PARAMS((int, int));

/* Bindable commands to set the editing mode (emacs or vi). */
extern int rl_vi_editing_mode PARAMS((int, int));
extern int rl_emacs_editing_mode PARAMS((int, int));

/* Bindable commands to change the insert mode (insert or overwrite) */
extern int rl_overwrite_mode PARAMS((int, int));

/* Bindable commands for managing key bindings. */
extern int rl_re_read_init_file PARAMS((int, int));
extern int rl_dump_functions PARAMS((int, int));
extern int rl_dump_macros PARAMS((int, int));
extern int rl_dump_variables PARAMS((int, int));

/* Bindable commands for word completion. */
extern int rl_complete PARAMS((int, int));
extern int rl_possible_completions PARAMS((int, int));
extern int rl_insert_completions PARAMS((int, int));
extern int rl_old_menu_complete PARAMS((int, int));
/* begin_clink_change */
extern int rl_backward_old_menu_complete PARAMS((int, int));
/* end_clink_change */
extern int rl_menu_complete PARAMS((int, int));
extern int rl_backward_menu_complete PARAMS((int, int));
/* begin_clink_change */
extern char** rl_get_completions PARAMS((int, int*, char**, int*, int*, int*, char*));
extern void rl_insert_match PARAMS((char*, char*, int, int, char));
/* end_clink_change */

/* Bindable commands for killing and yanking text, and managing the kill ring. */
extern int rl_kill_word PARAMS((int, int));
extern int rl_backward_kill_word PARAMS((int, int));
extern int rl_kill_line PARAMS((int, int));
extern int rl_backward_kill_line PARAMS((int, int));
extern int rl_kill_full_line PARAMS((int, int));
extern int rl_unix_word_rubout PARAMS((int, int));
extern int rl_unix_filename_rubout PARAMS((int, int));
extern int rl_unix_line_discard PARAMS((int, int));
extern int rl_copy_region_to_kill PARAMS((int, int));
extern int rl_kill_region PARAMS((int, int));
extern int rl_copy_forward_word PARAMS((int, int));
extern int rl_copy_backward_word PARAMS((int, int));
extern int rl_yank PARAMS((int, int));
extern int rl_yank_pop PARAMS((int, int));
extern int rl_yank_nth_arg PARAMS((int, int));
extern int rl_yank_last_arg PARAMS((int, int));
extern int rl_bracketed_paste_begin PARAMS((int, int));
/* Not available unless _WIN32 is defined. */
#if defined (_WIN32)
extern int rl_paste_from_clipboard PARAMS((int, int));
#endif

/* Bindable commands for incremental searching. */
extern int rl_reverse_search_history PARAMS((int, int));
extern int rl_forward_search_history PARAMS((int, int));

/* Bindable keyboard macro commands. */
extern int rl_start_kbd_macro PARAMS((int, int));
extern int rl_end_kbd_macro PARAMS((int, int));
extern int rl_call_last_kbd_macro PARAMS((int, int));
extern int rl_print_last_kbd_macro PARAMS((int, int));

/* Bindable undo commands. */
extern int rl_revert_line PARAMS((int, int));
extern int rl_undo_command PARAMS((int, int));

/* Bindable tilde expansion commands. */
extern int rl_tilde_expand PARAMS((int, int));

/* Bindable terminal control commands. */
extern int rl_restart_output PARAMS((int, int));
extern int rl_stop_output PARAMS((int, int));

/* Miscellaneous bindable commands. */
extern int rl_abort PARAMS((int, int));
extern int rl_tty_status PARAMS((int, int));

/* Bindable commands for incremental and non-incremental history searching. */
extern int rl_history_search_forward PARAMS((int, int));
extern int rl_history_search_backward PARAMS((int, int));
extern int rl_history_substr_search_forward PARAMS((int, int));
extern int rl_history_substr_search_backward PARAMS((int, int));
extern int rl_noninc_forward_search PARAMS((int, int));
extern int rl_noninc_reverse_search PARAMS((int, int));
extern int rl_noninc_forward_search_again PARAMS((int, int));
extern int rl_noninc_reverse_search_again PARAMS((int, int));

/* Bindable command used when inserting a matching close character. */
extern int rl_insert_close PARAMS((int, int));

/* Not available unless READLINE_CALLBACKS is defined. */
extern void rl_callback_handler_install PARAMS((const char *, rl_vcpfunc_t *));
extern void rl_callback_read_char PARAMS((void));
extern void rl_callback_handler_remove PARAMS((void));
extern void rl_callback_sigcleanup PARAMS((void));
/* begin_clink_change */
extern int rl_is_insert_next_callback_pending PARAMS((void));
/* end_clink_change */

/* Things for vi mode. Not available unless readline is compiled -DVI_MODE. */
/* VI-mode bindable commands. */
extern int rl_vi_redo PARAMS((int, int));
extern int rl_vi_undo PARAMS((int, int));
extern int rl_vi_yank_arg PARAMS((int, int));
extern int rl_vi_fetch_history PARAMS((int, int));
extern int rl_vi_search_again PARAMS((int, int));
extern int rl_vi_search PARAMS((int, int));
extern int rl_vi_complete PARAMS((int, int));
extern int rl_vi_tilde_expand PARAMS((int, int));
extern int rl_vi_prev_word PARAMS((int, int));
extern int rl_vi_next_word PARAMS((int, int));
extern int rl_vi_end_word PARAMS((int, int));
extern int rl_vi_insert_beg PARAMS((int, int));
extern int rl_vi_append_mode PARAMS((int, int));
extern int rl_vi_append_eol PARAMS((int, int));
extern int rl_vi_eof_maybe PARAMS((int, int));
extern int rl_vi_insertion_mode PARAMS((int, int));
extern int rl_vi_insert_mode PARAMS((int, int));
extern int rl_vi_movement_mode PARAMS((int, int));
extern int rl_vi_arg_digit PARAMS((int, int));
extern int rl_vi_change_case PARAMS((int, int));
extern int rl_vi_put PARAMS((int, int));
extern int rl_vi_column PARAMS((int, int));
extern int rl_vi_delete_to PARAMS((int, int));
extern int rl_vi_change_to PARAMS((int, int));
extern int rl_vi_yank_to PARAMS((int, int));
extern int rl_vi_yank_pop PARAMS((int, int));
extern int rl_vi_rubout PARAMS((int, int));
extern int rl_vi_delete PARAMS((int, int));
extern int rl_vi_back_to_indent PARAMS((int, int));
extern int rl_vi_unix_word_rubout PARAMS((int, int));
extern int rl_vi_first_print PARAMS((int, int));
extern int rl_vi_char_search PARAMS((int, int));
extern int rl_vi_match PARAMS((int, int));
extern int rl_vi_change_char PARAMS((int, int));
extern int rl_vi_subst PARAMS((int, int));
extern int rl_vi_overstrike PARAMS((int, int));
extern int rl_vi_overstrike_delete PARAMS((int, int));
extern int rl_vi_replace PARAMS((int, int));
extern int rl_vi_set_mark PARAMS((int, int));
extern int rl_vi_goto_mark PARAMS((int, int));

/* VI-mode utility functions. */
extern int rl_vi_check PARAMS((void));
extern int rl_vi_domove PARAMS((int, int *));
extern int rl_vi_bracktype PARAMS((int));

extern void rl_vi_start_inserting PARAMS((int, int, int));

/* VI-mode pseudo-bindable commands, used as utility functions. */
extern int rl_vi_fWord PARAMS((int, int));
extern int rl_vi_bWord PARAMS((int, int));
extern int rl_vi_eWord PARAMS((int, int));
extern int rl_vi_fword PARAMS((int, int));
extern int rl_vi_bword PARAMS((int, int));
extern int rl_vi_eword PARAMS((int, int));

/* **************************************************************** */
/*								    */
/*			Well Published Functions		    */
/*								    */
/* **************************************************************** */

/* Readline functions. */
/* Read a line of input.  Prompt with PROMPT.  A NULL PROMPT means none. */
extern char *readline PARAMS((const char *));

extern int rl_set_prompt PARAMS((const char *));
extern int rl_expand_prompt PARAMS((char *));
/* begin_clink_change */
extern const char *rl_get_local_prompt PARAMS((void));
extern const char *rl_get_local_prompt_prefix PARAMS((void));
extern int rl_set_rprompt PARAMS((const char *));
/* end_clink_change */

extern int rl_initialize PARAMS((void));

/* Undocumented; unused by readline */
extern int rl_discard_argument PARAMS((void));

/* Utility functions to bind keys to readline commands. */
extern int rl_add_defun PARAMS((const char *, rl_command_func_t *, int));
extern int rl_bind_key PARAMS((int, rl_command_func_t *));
extern int rl_bind_key_in_map PARAMS((int, rl_command_func_t *, Keymap));
extern int rl_unbind_key PARAMS((int));
extern int rl_unbind_key_in_map PARAMS((int, Keymap));
extern int rl_bind_key_if_unbound PARAMS((int, rl_command_func_t *));
extern int rl_bind_key_if_unbound_in_map PARAMS((int, rl_command_func_t *, Keymap));
extern int rl_unbind_function_in_map PARAMS((rl_command_func_t *, Keymap));
extern int rl_unbind_command_in_map PARAMS((const char *, Keymap));
extern int rl_bind_keyseq PARAMS((const char *, rl_command_func_t *));
extern int rl_bind_keyseq_in_map PARAMS((const char *, rl_command_func_t *, Keymap));
extern int rl_bind_keyseq_if_unbound PARAMS((const char *, rl_command_func_t *));
extern int rl_bind_keyseq_if_unbound_in_map PARAMS((const char *, rl_command_func_t *, Keymap));
extern int rl_generic_bind PARAMS((int, const char *, char *, Keymap));

extern char *rl_variable_value PARAMS((const char *));
extern int rl_variable_bind PARAMS((const char *, const char *));

/* Backwards compatibility, use rl_bind_keyseq_in_map instead. */
extern int rl_set_key PARAMS((const char *, rl_command_func_t *, Keymap));

/* Backwards compatibility, use rl_generic_bind instead. */
extern int rl_macro_bind PARAMS((const char *, const char *, Keymap));

/* Undocumented in the texinfo manual; not really useful to programs. */
extern int rl_translate_keyseq PARAMS((const char *, char *, int *));
extern char *rl_untranslate_keyseq PARAMS((int));

extern rl_command_func_t *rl_named_function PARAMS((const char *));
extern rl_command_func_t *rl_function_of_keyseq PARAMS((const char *, Keymap, int *));
extern rl_command_func_t *rl_function_of_keyseq_len PARAMS((const char *, size_t, Keymap, int *));

extern void rl_list_funmap_names PARAMS((void));
extern char **rl_invoking_keyseqs_in_map PARAMS((rl_command_func_t *, Keymap));
extern char **rl_invoking_keyseqs PARAMS((rl_command_func_t *));
 
extern void rl_function_dumper PARAMS((int));
extern void rl_macro_dumper PARAMS((int));
extern void rl_variable_dumper PARAMS((int));

extern int rl_read_init_file PARAMS((const char *));
extern int rl_parse_and_bind PARAMS((char *));

/* Functions for manipulating keymaps. */
extern Keymap rl_make_bare_keymap PARAMS((void));
extern int rl_empty_keymap PARAMS((Keymap));
extern Keymap rl_copy_keymap PARAMS((Keymap));
extern Keymap rl_make_keymap PARAMS((void));
extern void rl_discard_keymap PARAMS((Keymap));
extern void rl_free_keymap PARAMS((Keymap));

extern Keymap rl_get_keymap_by_name PARAMS((const char *));
extern char *rl_get_keymap_name PARAMS((Keymap));
extern void rl_set_keymap PARAMS((Keymap));
extern Keymap rl_get_keymap PARAMS((void));

extern int rl_set_keymap_name PARAMS((const char *, Keymap));

/* Undocumented; used internally only. */
extern void rl_set_keymap_from_edit_mode PARAMS((void));
extern char *rl_get_keymap_name_from_edit_mode PARAMS((void));

/* Functions for manipulating the funmap, which maps command names to functions. */
extern int rl_add_funmap_entry PARAMS((const char *, rl_command_func_t *));
extern const char **rl_funmap_names PARAMS((void));
/* Undocumented, only used internally -- there is only one funmap, and this
   function may be called only once. */
extern void rl_initialize_funmap PARAMS((void));

/* Utility functions for managing keyboard macros. */
extern void rl_push_macro_input PARAMS((char *));

/* Functions for undoing, from undo.c */
extern void rl_add_undo PARAMS((enum undo_code, int, int, char *));
extern void rl_free_undo_list PARAMS((void));
extern int rl_do_undo PARAMS((void));
extern int rl_begin_undo_group PARAMS((void));
extern int rl_end_undo_group PARAMS((void));
extern int rl_modifying PARAMS((int, int));

/* Functions for redisplay. */
extern void rl_redisplay PARAMS((void));
extern int rl_on_new_line PARAMS((void));
extern int rl_on_new_line_with_prompt PARAMS((void));
extern int rl_forced_update_display PARAMS((void));
extern int rl_clear_visible_line PARAMS((void));
extern int rl_clear_message PARAMS((void));
extern int rl_reset_line_state PARAMS((void));
extern int rl_crlf PARAMS((void));

/* Functions to manage the mark and region, especially the notion of an
   active mark and an active region. */
extern void rl_keep_mark_active PARAMS((void));

extern void rl_activate_mark PARAMS((void));
extern void rl_deactivate_mark PARAMS((void));
extern int rl_mark_active_p PARAMS((void));

#if defined (USE_VARARGS) && defined (PREFER_STDARG)
extern int rl_message (const char *, ...)  __attribute__((__format__ (printf, 1, 2)));
#else
extern int rl_message ();
#endif

extern int rl_show_char PARAMS((int));

/* Undocumented in texinfo manual. */
extern int rl_character_len PARAMS((int, int));
extern void rl_redraw_prompt_last_line PARAMS((void));

/* Save and restore internal prompt redisplay information. */
extern void rl_save_prompt PARAMS((void));
extern void rl_restore_prompt PARAMS((void));

/* Modifying text. */
extern void rl_replace_line PARAMS((const char *, int));
extern int rl_insert_text PARAMS((const char *));
extern int rl_delete_text PARAMS((int, int));
extern int rl_kill_text PARAMS((int, int));
extern char *rl_copy_text PARAMS((int, int));

/* Terminal and tty mode management. */
extern void rl_prep_terminal PARAMS((int));
extern void rl_deprep_terminal PARAMS((void));
extern void rl_tty_set_default_bindings PARAMS((Keymap));
extern void rl_tty_unset_default_bindings PARAMS((Keymap));

extern int rl_tty_set_echoing PARAMS((int));
extern int rl_reset_terminal PARAMS((const char *));
extern void rl_resize_terminal PARAMS((void));
extern void rl_set_screen_size PARAMS((int, int));
extern void rl_get_screen_size PARAMS((int *, int *));
extern void rl_reset_screen_size PARAMS((void));

extern char *rl_get_termcap PARAMS((const char *));

/* Functions for character input. */
extern int rl_stuff_char PARAMS((int));
extern int rl_execute_next PARAMS((int));
extern int rl_clear_pending_input PARAMS((void));
extern int rl_read_key PARAMS((void));
extern int rl_getc PARAMS((FILE *));
extern int rl_set_keyboard_input_timeout PARAMS((int));

/* `Public' utility functions . */
extern void rl_extend_line_buffer PARAMS((int));
extern int rl_ding PARAMS((void));
extern int rl_alphabetic PARAMS((int));
extern void rl_free PARAMS((void *));

/* Readline signal handling, from signals.c */
extern int rl_set_signals PARAMS((void));
extern int rl_clear_signals PARAMS((void));
extern void rl_cleanup_after_signal PARAMS((void));
extern void rl_reset_after_signal PARAMS((void));
extern void rl_free_line_state PARAMS((void));

extern int rl_pending_signal PARAMS((void));
extern void rl_check_signals PARAMS((void));

extern void rl_echo_signal_char PARAMS((int)); 

extern int rl_set_paren_blink_timeout PARAMS((int));

/* History management functions. */

extern void rl_clear_history PARAMS((void));

/* begin_clink_change */
/* Returns true if rl_operate_and_get_next has set a saved history
   offset, so that the host knows not to rearrange history entries yet. */
extern int rl_has_saved_history PARAMS((void));
/* end_clink_change */

/* Undocumented. */
extern int rl_maybe_save_line PARAMS((void));
extern int rl_maybe_unsave_line PARAMS((void));
extern int rl_maybe_replace_line PARAMS((void));

/* Completion functions. */
extern int rl_complete_internal PARAMS((int));
extern void rl_display_match_list PARAMS((char **, int, int));

extern char **rl_completion_matches PARAMS((const char *, rl_compentry_func_t *));
extern char *rl_username_completion_function PARAMS((const char *, int));
extern char *rl_filename_completion_function PARAMS((const char *, int));

extern int rl_completion_mode PARAMS((rl_command_func_t *));

/* begin_clink_change */
/* Path separators. */
extern int rl_is_path_separator PARAMS((char c));
extern char *rl_last_path_separator PARAMS((const char *string));
/* end_clink_change */

#if 0
/* Backwards compatibility (compat.c).  These will go away sometime. */
extern void free_undo_list PARAMS((void));
extern int maybe_save_line PARAMS((void));
extern int maybe_unsave_line PARAMS((void));
extern int maybe_replace_line PARAMS((void));

extern int ding PARAMS((void));
extern int alphabetic PARAMS((int));
extern int crlf PARAMS((void));

extern char **completion_matches PARAMS((char *, rl_compentry_func_t *));
extern char *username_completion_function PARAMS((const char *, int));
extern char *filename_completion_function PARAMS((const char *, int));
#endif

/* **************************************************************** */
/*								    */
/*			Well Published Variables		    */
/*								    */
/* **************************************************************** */

/* The version of this incarnation of the readline library. */
extern const char *rl_library_version;		/* e.g., "4.2" */
extern int rl_readline_version;			/* e.g., 0x0402 */

/* True if this is real GNU readline. */
extern int rl_gnu_readline_p;

/* Flags word encapsulating the current readline state. */
extern unsigned long rl_readline_state;

/* Says which editing mode readline is currently using.  1 means emacs mode;
   0 means vi mode. */
extern int rl_editing_mode;

/* Insert or overwrite mode for emacs mode.  1 means insert mode; 0 means
   overwrite mode.  Reset to insert mode on each input line. */
extern int rl_insert_mode;

/* The name of the calling program.  You should initialize this to
   whatever was in argv[0].  It is used when parsing conditionals. */
extern const char *rl_readline_name;

/* The prompt readline uses.  This is set from the argument to
   readline (), and should not be assigned to directly. */
extern char *rl_prompt;

/* The prompt string that is actually displayed by rl_redisplay.  Public so
   applications can more easily supply their own redisplay functions. */
extern char *rl_display_prompt;

/* begin_clink_change */
/* The right-justified prompt string, if any.  This is set by
   rl_set_rprompt (), and should not be assigned to directly. */
extern char *rl_rprompt;
extern int rl_visible_rprompt_length;
/* end_clink_change */

/* The line buffer that is in use. */
extern char *rl_line_buffer;

/* The location of point, and end. */
extern int rl_point;
extern int rl_end;

/* The mark, or saved cursor position. */
extern int rl_mark;

/* Flag to indicate that readline has finished with the current input
   line and should return it. */
extern int rl_done;

/* If set to a character value, that will be the next keystroke read. */
extern int rl_pending_input;

/* Non-zero if we called this function from _rl_dispatch().  It's present
   so functions can find out whether they were called from a key binding
   or directly from an application. */
extern int rl_dispatching;

/* Non-zero if the user typed a numeric argument before executing the
   current function. */
extern int rl_explicit_arg;

/* The current value of the numeric argument specified by the user. */
extern int rl_numeric_arg;

/* The address of the last command function Readline executed. */
extern rl_command_func_t *rl_last_func;

/* The name of the terminal to use. */
extern const char *rl_terminal_name;

/* The input and output streams. */
extern FILE *rl_instream;
extern FILE *rl_outstream;

/* If non-zero, Readline gives values of LINES and COLUMNS from the environment
   greater precedence than values fetched from the kernel when computing the
   screen dimensions. */
extern int rl_prefer_env_winsize;

/* If non-zero, then this is the address of a function to call just
   before readline_internal () prints the first prompt. */
extern rl_hook_func_t *rl_startup_hook;

/* If non-zero, this is the address of a function to call just before
   readline_internal_setup () returns and readline_internal starts
   reading input characters. */
extern rl_hook_func_t *rl_pre_input_hook;
      
/* The address of a function to call periodically while Readline is
   awaiting character input, or NULL, for no event handling. */
extern rl_hook_func_t *rl_event_hook;

/* The address of a function to call if a read is interrupted by a signal. */
extern rl_hook_func_t *rl_signal_event_hook;

/* The address of a function to call if Readline needs to know whether or not
   there is data available from the current input source. */
extern rl_hook_func_t *rl_input_available_hook;

/* begin_clink_change */
/* Called before the input buffer is changed. */
extern rl_voidfunc_t *rl_buffer_changing_hook;
/* Called when an event occurs that is relevant for a host that implements a
   text selection model where typing can replace the selection. The function is
   called with one argument, an enum indicating the type of selection event.
   The function can return zero to allow the command to continue, or return
   non-zero to stop further processing. */
enum selection_event { SEL_BEFORE_INSERTCHAR, SEL_AFTER_INSERTCHAR, SEL_BEFORE_DELETE };
extern rl_intfunc_t *rl_selection_event_hook;
/* end_clink_change */

/* begin_clink_change */
/* Called when rl_add_history adds a history line. */
extern rl_history_hook_func_t *rl_add_history_hook;
/* Called when rl_remove_history removes a history line. */
extern rl_history_hook_func_t *rl_remove_history_hook;

/* If non-zero, adds backslash as a path separator. */
extern int rl_backslash_path_sep;

/* Filename completion inserts this as the path separator character. */
extern char rl_preferred_path_separator;
/* end_clink_change */

/* begin_clink_change */
/* Function to call for post-processing of set_completion_defaults, before
   completion is performed. */
extern rl_voidfunc_t *rl_adjust_completion_defaults;
/* Function to call for post-processing of _rl_find_completion_word. */
extern rl_adjcmpwrd_func_t *rl_adjust_completion_word;
/* Function to call for comparing lcd. */
extern rl_compare_lcd_func_t *rl_compare_lcd_func;
/* Function to call for post-processing of lcd. */
extern rl_postprocess_lcd_func_t *rl_postprocess_lcd_func;
/* Completion functions can set this to signal that the first char of each
   match is bit flags about the match type. */
#define MATCH_TYPE_NONE			1
#define MATCH_TYPE_WORD			2
#define MATCH_TYPE_ARG			3
#define MATCH_TYPE_COMMAND		4
#define MATCH_TYPE_ALIAS		5
#define MATCH_TYPE_FILE			6
#define MATCH_TYPE_DIR			7
#define MATCH_TYPE_MASK			0x07
#define MATCH_TYPE_LINK			0x10
#define MATCH_TYPE_ORPHANED		0x20
#define MATCH_TYPE_HIDDEN		0x40
#define MATCH_TYPE_READONLY		0x80
#define IS_MATCH_TYPE_NONE(x)		(((x) & MATCH_TYPE_MASK) == MATCH_TYPE_NONE)
#define IS_MATCH_TYPE_WORD(x)		(((x) & MATCH_TYPE_MASK) == MATCH_TYPE_WORD)
#define IS_MATCH_TYPE_ARG(x)		(((x) & MATCH_TYPE_MASK) == MATCH_TYPE_ARG)
#define IS_MATCH_TYPE_COMMAND(x)	(((x) & MATCH_TYPE_MASK) == MATCH_TYPE_COMMAND)
#define IS_MATCH_TYPE_ALIAS(x)		(((x) & MATCH_TYPE_MASK) == MATCH_TYPE_ALIAS)
#define IS_MATCH_TYPE_FILE(x)		(((x) & MATCH_TYPE_MASK) == MATCH_TYPE_FILE)
#define IS_MATCH_TYPE_DIR(x)		(((x) & MATCH_TYPE_MASK) == MATCH_TYPE_DIR)
#define IS_MATCH_TYPE_LINK(x)		(((x) & MATCH_TYPE_LINK) == MATCH_TYPE_LINK)
#define IS_MATCH_TYPE_ORPHANED(x)	(((x) & MATCH_TYPE_ORPHANED) == MATCH_TYPE_ORPHANED)
#define IS_MATCH_TYPE_HIDDEN(x)		(((x) & MATCH_TYPE_HIDDEN) == MATCH_TYPE_HIDDEN)
#define IS_MATCH_TYPE_READONLY(x)	(((x) & MATCH_TYPE_READONLY) == MATCH_TYPE_READONLY)
#define IS_MATCH_TYPE_PATHISH(x)	(((x) & MATCH_TYPE_MASK) >= MATCH_TYPE_FILE && \
					 ((x) & MATCH_TYPE_MASK) <= MATCH_TYPE_LINK)
extern int rl_completion_matches_include_type;
/* end_clink_change */

/* The address of the function to call to fetch a character from the current
   Readline input stream */
extern rl_getc_func_t *rl_getc_function;

extern rl_voidfunc_t *rl_redisplay_function;

/* begin_clink_change */
/* The address of a function to call before displaying the input line. */
extern rl_voidfunc_t *rl_before_display_function;
/* end_clink_change */

/* begin_clink_change */
extern const char *_rl_display_modmark_color;
extern const char *_rl_display_horizscroll_color;
extern const char *_rl_display_message_color;
extern char _rl_face_modmark;
extern char _rl_face_horizscroll;
extern rl_get_face_func_t *rl_get_face_func;
extern rl_puts_face_func_t *rl_puts_face_func;
/* end_clink_change */

extern rl_vintfunc_t *rl_prep_term_function;
extern rl_voidfunc_t *rl_deprep_term_function;

/* Dispatch variables. */
extern Keymap rl_executing_keymap;
extern Keymap rl_binding_keymap;

extern int rl_executing_key;
extern char *rl_executing_keyseq;
extern int rl_key_sequence_length;

/* begin_clink_change */
extern rl_macro_hook_func_t *rl_macro_hook_func;
extern rl_voidfunc_t *rl_last_func_hook_func;
/* end_clink_change */

/* Display variables. */
/* If non-zero, readline will erase the entire line, including any prompt,
   if the only thing typed on an otherwise-blank line is something bound to
   rl_newline. */
extern int rl_erase_empty_line;

/* If non-zero, the application has already printed the prompt (rl_prompt)
   before calling readline, so readline should not output it the first time
   redisplay is done. */
extern int rl_already_prompted;

/* A non-zero value means to read only this many characters rather than
   up to a character bound to accept-line. */
extern int rl_num_chars_to_read;

/* The text of a currently-executing keyboard macro. */
extern char *rl_executing_macro;

/* Variables to control readline signal handling. */
/* If non-zero, readline will install its own signal handlers for
   SIGINT, SIGTERM, SIGQUIT, SIGALRM, SIGTSTP, SIGTTIN, and SIGTTOU. */
extern int rl_catch_signals;

/* If non-zero, readline will install a signal handler for SIGWINCH
   that also attempts to call any calling application's SIGWINCH signal
   handler.  Note that the terminal is not cleaned up before the
   application's signal handler is called; use rl_cleanup_after_signal()
   to do that. */
extern int rl_catch_sigwinch;

/* If non-zero, the readline SIGWINCH handler will modify LINES and
   COLUMNS in the environment. */
extern int rl_change_environment;

/* Completion variables. */
/* Pointer to the generator function for completion_matches ().
   NULL means to use rl_filename_completion_function (), the default
   filename completer. */
extern rl_compentry_func_t *rl_completion_entry_function;

/* Optional generator for menu completion.  Default is
   rl_completion_entry_function (rl_filename_completion_function). */
extern rl_compentry_func_t *rl_menu_completion_entry_function;

/* If rl_ignore_some_completions_function is non-NULL it is the address
   of a function to call after all of the possible matches have been
   generated, but before the actual completion is done to the input line.
   The function is called with one argument; a NULL terminated array
   of (char *).  If your function removes any of the elements, they
   must be free()'ed. */
extern rl_compignore_func_t *rl_ignore_some_completions_function;

/* Pointer to alternative function to create matches.
   Function is called with TEXT, START, and END.
   START and END are indices in RL_LINE_BUFFER saying what the boundaries
   of TEXT are.
   If this function exists and returns NULL then call the value of
   rl_completion_entry_function to try to match, otherwise use the
   array of strings returned. */
extern rl_completion_func_t *rl_attempted_completion_function;

/* The basic list of characters that signal a break between words for the
   completer routine.  The initial contents of this variable is what
   breaks words in the shell, i.e. "n\"\\'`@$>". */
extern const char *rl_basic_word_break_characters;
extern const char *rl_basic_word_break_characters_without_backslash;

/* The list of characters that signal a break between words for
   rl_complete_internal.  The default list is the contents of
   rl_basic_word_break_characters.  */
/* begin_clink_change */
//extern /*const*/ char *rl_completer_word_break_characters;
extern const char *rl_completer_word_break_characters;
/* end_clink_change */

/* Hook function to allow an application to set the completion word
   break characters before readline breaks up the line.  Allows
   position-dependent word break characters. */
extern rl_cpvfunc_t *rl_completion_word_break_hook;

/* List of characters which can be used to quote a substring of the line.
   Completion occurs on the entire substring, and within the substring   
   rl_completer_word_break_characters are treated as any other character,
   unless they also appear within this list. */
extern const char *rl_completer_quote_characters;

/* List of quote characters which cause a word break. */
extern const char *rl_basic_quote_characters;

/* List of characters that need to be quoted in filenames by the completer. */
extern const char *rl_filename_quote_characters;

/* List of characters that are word break characters, but should be left
   in TEXT when it is passed to the completion function.  The shell uses
   this to help determine what kind of completing to do. */
extern const char *rl_special_prefixes;

/* If non-zero, then this is the address of a function to call when
   completing on a directory name.  The function is called with
   the address of a string (the current directory name) as an arg.  It
   changes what is displayed when the possible completions are printed
   or inserted.  The directory completion hook should perform
   any necessary dequoting.  This function should return 1 if it modifies
   the directory name pointer passed as an argument.  If the directory
   completion hook returns 0, it should not modify the directory name
   pointer passed as an argument. */
extern rl_icppfunc_t *rl_directory_completion_hook;

/* If non-zero, this is the address of a function to call when completing
   a directory name.  This function takes the address of the directory name
   to be modified as an argument.  Unlike rl_directory_completion_hook, it
   only modifies the directory name used in opendir(2), not what is displayed
   when the possible completions are printed or inserted.  If set, it takes
   precedence over rl_directory_completion_hook.  The directory rewrite
   hook should perform any necessary dequoting.  This function has the same
   return value properties as the directory_completion_hook.

   I'm not happy with how this works yet, so it's undocumented.  I'm trying
   it in bash to see how well it goes. */
extern rl_icppfunc_t *rl_directory_rewrite_hook;

/* If non-zero, this is the address of a function for the completer to call
   before deciding which character to append to a completed name.  It should
   modify the directory name passed as an argument if appropriate, and return
   non-zero if it modifies the name.  This should not worry about dequoting
   the filename; that has already happened by the time it gets here. */
extern rl_icppfunc_t *rl_filename_stat_hook;

/* If non-zero, this is the address of a function to call when reading
   directory entries from the filesystem for completion and comparing
   them to the partial word to be completed.  The function should
   either return its first argument (if no conversion takes place) or
   newly-allocated memory.  This can, for instance, convert filenames
   between character sets for comparison against what's typed at the
   keyboard.  The returned value is what is added to the list of
   matches.  The second argument is the length of the filename to be
   converted. */
extern rl_dequote_func_t *rl_filename_rewrite_hook;

/* Backwards compatibility with previous versions of readline. */
#define rl_symbolic_link_hook rl_directory_completion_hook

/* If non-zero, then this is the address of a function to call when
   completing a word would normally display the list of possible matches.
   This function is called instead of actually doing the display.
   It takes three arguments: (char **matches, int num_matches, int max_length)
   where MATCHES is the array of strings that matched, NUM_MATCHES is the
   number of strings in that array, and MAX_LENGTH is the length of the
   longest string in that array. */
extern rl_compdisp_func_t *rl_completion_display_matches_hook;

/* begin_clink_change */
/* If non-zero, then this is the address of a function to call that completely
   replaces the display_matches behavior (rl_completion_display_matches_hook
   only replaces listing multiple matches).
   It takes one argument: (char** matches) where MATCHES is the array of
   strings that matched. */
extern rl_vcppfunc_t *rl_completion_display_matches_func;
/* If non-zero, then this is the address of a function to call to sort the
   list of matches.  It can accommodate any special sorting behavior the host
   may require, such as ignoring trailing path separators. */
extern rl_qsort_match_list_func_t *rl_qsort_match_list_func;
/* If non-zero, then this is the address of a function to call that determines
   whether a file extension is executable. */
extern rl_iccpfunc_t *rl_is_exec_func;
/* end_clink_change */

/* Non-zero means that the results of the matches are to be treated
   as filenames.  This is ALWAYS zero on entry, and can only be changed
   within a completion entry finder function. */
extern int rl_filename_completion_desired;

/* begin_clink_change */
/* Similar to rl_filename_completion_desired, but only affects display.
   This is ALWAYS zero on entry, and can only be changed within a
   completion entry finder function. */
extern int rl_filename_display_desired;
/* end_clink_change */

/* Non-zero means that the results of the matches are to be quoted using
   double quotes (or an application-specific quoting mechanism) if the
   filename contains any characters in rl_word_break_chars.  This is
   ALWAYS non-zero on entry, and can only be changed within a completion
   entry finder function. */
extern int rl_filename_quoting_desired;

/* Set to a function to quote a filename in an application-specific fashion.
   Called with the text to quote, the type of match found (single or multiple)
   and a pointer to the quoting character to be used, which the function can
   reset if desired. */
extern rl_quote_func_t *rl_filename_quoting_function;

/* Function to call to remove quoting characters from a filename.  Called
   before completion is attempted, so the embedded quotes do not interfere
   with matching names in the file system. */
extern rl_dequote_func_t *rl_filename_dequoting_function;

/* Function to call to decide whether or not a word break character is
   quoted.  If a character is quoted, it does not break words for the
   completer. */
extern rl_linebuf_func_t *rl_char_is_quoted_p;

/* Non-zero means to suppress normal filename completion after the
   user-specified completion function has been called. */
extern int rl_attempted_completion_over;

/* Set to a character describing the type of completion being attempted by
   rl_complete_internal; available for use by application completion
   functions. */
extern int rl_completion_type;

/* Set to the last key used to invoke one of the completion functions */
extern int rl_completion_invoking_key;

/* Up to this many items will be displayed in response to a
   possible-completions call.  After that, we ask the user if she
   is sure she wants to see them all.  The default value is 100. */
extern int rl_completion_query_items;

/* begin_clink_change */
/* When set, overrides rl_completion_query_items for `complete`
   based on the screen height and number of columns of matches. */
extern int rl_completion_auto_query_items;
/* end_clink_change */

/* Character appended to completed words when at the end of the line.  The
   default is a space.  Nothing is added if this is '\0'. */
extern int rl_completion_append_character;

/* If set to non-zero by an application completion function,
   rl_completion_append_character will not be appended. */
extern int rl_completion_suppress_append;

/* Set to any quote character readline thinks it finds before any application
   completion function is called. */
extern int rl_completion_quote_character;

/* Set to a non-zero value if readline found quoting anywhere in the word to
   be completed; set before any application completion function is called. */
extern int rl_completion_found_quote;

/* If non-zero, the completion functions don't append any closing quote.
   This is set to 0 by rl_complete_internal and may be changed by an
   application-specific completion function. */
extern int rl_completion_suppress_quote;

/* If non-zero, readline will sort the completion matches.  On by default. */
extern int rl_sort_completion_matches;

/* If non-zero, a slash will be appended to completed filenames that are
   symbolic links to directory names, subject to the value of the
   mark-directories variable (which is user-settable).  This exists so
   that application completion functions can override the user's preference
   (set via the mark-symlinked-directories variable) if appropriate.
   It's set to the value of _rl_complete_mark_symlink_dirs in
   rl_complete_internal before any application-specific completion
   function is called, so without that function doing anything, the user's
   preferences are honored. */
extern int rl_completion_mark_symlink_dirs;

/* If non-zero, then disallow duplicates in the matches. */
extern int rl_ignore_completion_duplicates;

/* If this is non-zero, completion is (temporarily) inhibited, and the
   completion character will be inserted as any other. */
extern int rl_inhibit_completion;

/* begin_clink_change */
/* This is a terminal sequence for the pager color.  The CSI and "m" are
   automatically added when writing it to the terminal. */
extern const char* _rl_pager_color;
/* This is a terminal sequence for the hidden file color.  The CSI and "m" are
   automatically added when writing it to the terminal. */
extern const char* _rl_hidden_color;
/* This is a terminal sequence for the readonly file color.  The CSI and "m" are
   automatically added when writing it to the terminal. */
extern const char* _rl_readonly_color;
/* This is a terminal sequence for the shell command color.  The CSI and "m" are
   automatically added when writing it to the terminal. */
extern const char* _rl_command_color;
/* This is a terminal sequence for the alias color.  The CSI and "m" are
   automatically added when writing it to the terminal. */
extern const char* _rl_alias_color;
/* This callback allows an application to provide keyboard input to readline
   even in modal input situations like the pager. */
extern rl_read_key_hook_func_t *rl_read_key_hook;
/* end_clink_change */

/* Applications can set this to non-zero to have readline's signal handlers
   installed during the entire duration of reading a complete line, as in
   readline-6.2.  This should be used with care, because it can result in
   readline receiving signals and not handling them until it's called again
   via rl_callback_read_char, thereby stealing them from the application.
   By default, signal handlers are only active while readline is active. */   
extern int rl_persistent_signal_handlers;

/* Input error; can be returned by (*rl_getc_function) if readline is reading
   a top-level command (RL_ISSTATE (RL_STATE_READCMD)). */
#define READERR			(-2)

/* Definitions available for use by readline clients. */
#define RL_PROMPT_START_IGNORE	'\001'
#define RL_PROMPT_END_IGNORE	'\002'

/* Possible values for do_replace argument to rl_filename_quoting_function,
   called by rl_complete_internal. */
#define NO_MATCH        0
#define SINGLE_MATCH    1
#define MULT_MATCH      2

/* Possible state values for rl_readline_state */
#define RL_STATE_NONE		0x000000		/* no state; before first call */

#define RL_STATE_INITIALIZING	0x0000001	/* initializing */
#define RL_STATE_INITIALIZED	0x0000002	/* initialization done */
#define RL_STATE_TERMPREPPED	0x0000004	/* terminal is prepped */
#define RL_STATE_READCMD	0x0000008	/* reading a command key */
#define RL_STATE_METANEXT	0x0000010	/* reading input after ESC */
#define RL_STATE_DISPATCHING	0x0000020	/* dispatching to a command */
#define RL_STATE_MOREINPUT	0x0000040	/* reading more input in a command function */
#define RL_STATE_ISEARCH	0x0000080	/* doing incremental search */
#define RL_STATE_NSEARCH	0x0000100	/* doing non-inc search */
#define RL_STATE_SEARCH		0x0000200	/* doing a history search */
#define RL_STATE_NUMERICARG	0x0000400	/* reading numeric argument */
#define RL_STATE_MACROINPUT	0x0000800	/* getting input from a macro */
#define RL_STATE_MACRODEF	0x0001000	/* defining keyboard macro */
#define RL_STATE_OVERWRITE	0x0002000	/* overwrite mode */
#define RL_STATE_COMPLETING	0x0004000	/* doing completion */
#define RL_STATE_SIGHANDLER	0x0008000	/* in readline sighandler */
#define RL_STATE_UNDOING	0x0010000	/* doing an undo */
#define RL_STATE_INPUTPENDING	0x0020000	/* rl_execute_next called */
#define RL_STATE_TTYCSAVED	0x0040000	/* tty special chars saved */
#define RL_STATE_CALLBACK	0x0080000	/* using the callback interface */
#define RL_STATE_VIMOTION	0x0100000	/* reading vi motion arg */
#define RL_STATE_MULTIKEY	0x0200000	/* reading multiple-key command */
#define RL_STATE_VICMDONCE	0x0400000	/* entered vi command mode at least once */
#define RL_STATE_CHARSEARCH	0x0800000	/* vi mode char search */
#define RL_STATE_REDISPLAYING	0x1000000	/* updating terminal display */

#define RL_STATE_DONE		0x2000000	/* done; accepted line */

#define RL_SETSTATE(x)		(rl_readline_state |= (x))
#define RL_UNSETSTATE(x)	(rl_readline_state &= ~(x))
#define RL_ISSTATE(x)		(rl_readline_state & (x))

struct readline_state {
  /* line state */
  int point;
  int end;
  int mark;
  int buflen;
  char *buffer;
  UNDO_LIST *ul;
  char *prompt;

  /* global state */
  int rlstate;
  int done;
  Keymap kmap;

  /* input state */
  rl_command_func_t *lastfunc;
  int insmode;
  int edmode;
  char *kseq;
  int kseqlen;

  int pendingin;
  FILE *inf;
  FILE *outf;
  char *macro;

  /* signal state */
  int catchsigs;
  int catchsigwinch;

  /* search state */

  /* completion state */
  rl_compentry_func_t *entryfunc;
  rl_compentry_func_t *menuentryfunc;
  rl_compignore_func_t *ignorefunc;
  rl_completion_func_t *attemptfunc;
/* begin_clink_change */
  //char *wordbreakchars;
  const char *wordbreakchars;
/* end_clink_change */

  /* options state */

  /* hook state */
  
  /* reserved for future expansion, so the struct size doesn't change */
  char reserved[64];
};

extern int rl_save_state PARAMS((struct readline_state *));
extern int rl_restore_state PARAMS((struct readline_state *));

#ifdef __cplusplus
}
#endif

#endif /* _READLINE_H_ */
