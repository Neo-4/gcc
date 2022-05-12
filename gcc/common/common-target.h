/* Data structure definitions for common hooks.
   Copyright (C) 2010-2017 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 3, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!  */

#ifndef GCC_COMMON_TARGET_H
#define GCC_COMMON_TARGET_H


/* Sets of optimization levels at which an option may be enabled by
   default_options_optimization.  */
enum opt_levels
{
  OPT_LEVELS_NONE, /* No levels (mark end of array).  */
  OPT_LEVELS_ALL, /* All levels (used by targets to disable options
		     enabled in target-independent code).  */
  OPT_LEVELS_0_ONLY, /* -O0 only.  */
  OPT_LEVELS_1_PLUS, /* -O1 and above, including -Os and -Og.  */
  OPT_LEVELS_1_PLUS_SPEED_ONLY, /* -O1 and above, but not -Os or -Og.  */
  OPT_LEVELS_1_PLUS_NOT_DEBUG, /* -O1 and above, but not -Og.  */
  OPT_LEVELS_2_PLUS, /* -O2 and above, including -Os.  */
  OPT_LEVELS_2_PLUS_SPEED_ONLY, /* -O2 and above, but not -Os or -Og.  */
  OPT_LEVELS_3_PLUS, /* -O3 and above.  */
  OPT_LEVELS_3_PLUS_AND_SIZE, /* -O3 and above and -Os.  */
  OPT_LEVELS_SIZE, /* -Os only.  */
  OPT_LEVELS_FAST /* -Ofast only.  */
};

/* Description of options to enable by default at given levels.  */
struct default_options
{
  /* The levels at which to enable the option.  */
  enum opt_levels levels;

  /* The option index and argument or enabled/disabled sense of the
     option, as passed to handle_generated_option.  If ARG is NULL and
     the option allows a negative form, the option is considered to be
     passed in negative form when the optimization level is not one of
     those in LEVELS (in order to handle changes to the optimization
     level with the "optimize" attribute).  */
  size_t opt_index;
  const char *arg;
  int value;
};

#define DEFHOOKPOD(NAME, DOC, TYPE, INIT) TYPE NAME;
#define DEFHOOK(NAME, DOC, TYPE, PARAMS, INIT) TYPE (* NAME) PARAMS;
#define DEFHOOK_UNDOC DEFHOOK
#define HOOKSTRUCT(FRAGMENT) FRAGMENT

#include "common-target.def"

/*
此处根据target-hooks-macros.h和common-target.def展开得到：
struct gcc_targetm_common {
    bool (* handle_option) (struct gcc_options *opts, struct gcc_options *opts_set, const struct cl_decoded_option *decoded, location_t loc);
    void (* option_init_struct) (struct gcc_options *opts);
    const struct default_options * option_optimization_table;
    void (* option_default_params) (void);
    int default_target_flags;
    enum unwind_info_type (* except_unwind_info) (struct gcc_options *opts);
    bool (* supports_split_stack) (bool report, struct gcc_options *opts);
    bool unwind_tables_default;
    bool have_named_sections;
    bool always_strip_dotdot;
};
*/

extern struct gcc_targetm_common targetm_common;

#endif /* GCC_C_TARGET_H */
