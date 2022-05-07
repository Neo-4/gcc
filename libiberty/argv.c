/* Create and destroy argument vectors (argv's)
   Copyright (C) 1992-2017 Free Software Foundation, Inc.
   Written by Fred Fish @ Cygnus Support

This file is part of the libiberty library.
Libiberty is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

Libiberty is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with libiberty; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
Boston, MA 02110-1301, USA.  */


/*  Create and destroy argument vectors.  An argument vector is simply an
    array of string pointers, terminated by a NULL pointer. */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "ansidecl.h"
#include "libiberty.h"
#include "safe-ctype.h"

/*  Routines imported from standard C runtime libraries. */

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef EOS
#define EOS '\0'
#endif

#define INITIAL_MAXARGC 8	/* Number of args + NULL in initial argv */


/*

@deftypefn Extension char** dupargv (char * const *@var{vector})

Duplicate an argument vector.  Simply scans through @var{vector},
duplicating each argument until the terminating @code{NULL} is found.
Returns a pointer to the argument vector if successful.  Returns
@code{NULL} if there is insufficient memory to complete building the
argument vector.

@end deftypefn

*/

char **
dupargv (char * const *argv)
{
  int argc;
  char **copy;

  if (argv == NULL)
    return NULL;

  /* the vector */
  for (argc = 0; argv[argc] != NULL; argc++);
  copy = (char **) xmalloc ((argc + 1) * sizeof (char *));

  /* the strings */
  for (argc = 0; argv[argc] != NULL; argc++)
    copy[argc] = xstrdup (argv[argc]);
  copy[argc] = NULL;
  return copy;
}

/*

@deftypefn Extension void freeargv (char **@var{vector})

Free an argument vector that was built using @code{buildargv}.  Simply
scans through @var{vector}, freeing the memory for each argument until
the terminating @code{NULL} is found, and then frees @var{vector}
itself.

@end deftypefn

*/

void freeargv (char **vector)
{
  register char **scan;

  if (vector != NULL)
    {
      for (scan = vector; *scan != NULL; scan++)
	{
	  free (*scan);
	}
      free (vector);
    }
}

static void
consume_whitespace (const char **input)
{
  while (ISSPACE (**input))
    {
      (*input)++;
    }
}

static int
only_whitespace (const char* input)
{
  while (*input != EOS && ISSPACE (*input))
    input++;

  return (*input == EOS);
}

/*

@deftypefn Extension char** buildargv (char *@var{sp})

Given a pointer to a string, parse the string extracting fields
separated by whitespace and optionally enclosed within either single
or double quotes (which are stripped off), and build a vector of
pointers to copies of the string for each field.  The input string
remains unchanged.  The last element of the vector is followed by a
@code{NULL} element.

All of the memory for the pointer array and copies of the string
is obtained from @code{xmalloc}.  All of the memory can be returned to the
system with the single function call @code{freeargv}, which takes the
returned result of @code{buildargv}, as it's argument.

Returns a pointer to the argument vector if successful.  Returns
@code{NULL} if @var{sp} is @code{NULL} or if there is insufficient
memory to complete building the argument vector.

If the input is a null string (as opposed to a @code{NULL} pointer),
then buildarg returns an argument vector that has one arg, a null
string.

@end deftypefn

The memory for the argv array is dynamically expanded as necessary.

In order to provide a working buffer for extracting arguments into,
with appropriate stripping of quotes and translation of backslash
sequences, we allocate a working buffer at least as long as the input
string.  This ensures that we always have enough space in which to
work, since the extracted arg is never larger than the input string.

The argument vector is always kept terminated with a @code{NULL} arg
pointer, so it can be passed to @code{freeargv} at any time, or
returned, as appropriate.

*/
// 将文件中的内容，全部转成参数，存放到申请的argv[argc]中。
char **buildargv (const char *input)
{
  char *arg;
  char *copybuf;
  int squote = 0;
  int dquote = 0;
  int bsquote = 0;
  int argc = 0;
  int maxargc = 0;
  char **argv = NULL;
  char **nargv;

  if (input != NULL)
  {
      copybuf = (char *) xmalloc(strlen(input) + 1);

      /* Is a do{}while to always execute the loop once.  Always return an
	 argv, even for null strings.  See NOTES above, test case below. */
      do
	  {
	    /* Pick off argv[argc] */
        // 将空格符丢掉，input指针将指向第一个非空格符的字符。
	    consume_whitespace (&input);

        // 申请用于存放参数字符串地址的buffer。每申请一次，能存放8个char *地址。
	    if ((maxargc == 0) || (argc >= (maxargc - 1)))
	    {
	      /* argv needs initialization, or expansion */
	      if (argv == NULL)
		  {
            // 首次申请
		    maxargc = INITIAL_MAXARGC;
		    nargv = (char **) xmalloc (maxargc * sizeof (char *));
		  }
	      else
		  {
            // 8个参数地址空间已经用完，在原来的基础上再扩展8个
		    maxargc *= 2;
		    nargv = (char **) xrealloc (argv, maxargc * sizeof (char *));
		  }

	      argv = nargv;
	      argv[argc] = NULL;
	    }

	    /* Begin scanning arg */
	    arg = copybuf;
	    while (*input != EOS)
	    {
	      if (ISSPACE (*input) && !squote && !dquote && !bsquote)
		  {
		    break; // 一个完整的参数已经扫描完毕，存放到copybuf中。
		  }
	      else
		  {
		    if (bsquote)
		    {
		      bsquote = 0;
		      *arg++ = *input; // 将'\'符号后面的第一个字符存放到copybuf中
		    }
		    else if (*input == '\\')
		    {
		      bsquote = 1; // 识别到'\'符号，丢掉
		    }
		    else if (squote)
		    {
		      if (*input == '\'')
			  {
			    squote = 0;
			  }
		      else
			  {
			    *arg++ = *input; // 将''之间的内容，全部放入copybuf中
			  }
		    }
		    else if (dquote)
		    {
		      if (*input == '"')
			  {
			    dquote = 0;
			  }
		      else
			  {
			    *arg++ = *input; // 将“”之间的内容，全部放入copybuf中。
			  }
		    }
		    else
		    {
		      if (*input == '\'')
			  {
			    squote = 1; // 识别到'''字符，丢掉
			  }
		      else if (*input == '"')
			  {
			    dquote = 1; // 识别到'"'字符，丢掉
			  }
		      else
			  {
			    *arg++ = *input;
			  }
		    }

		    input++;
          }
	    }

	    *arg = EOS;

        // 申请一段内存，把参数拷贝到申请的内存中。并将指针存放到argv数组中。
	    argv[argc] = xstrdup (copybuf);
	    argc++;
	    argv[argc] = NULL;

        // 将下一个参数之前的空格符丢掉，input指针将指向第一个非空格符的字符。
	    consume_whitespace (&input);
	}while (*input != EOS);

    free (copybuf);
  }

  return (argv);
}

/*

@deftypefn Extension int writeargv (char * const *@var{argv}, FILE *@var{file})

Write each member of ARGV, handling all necessary quoting, to the file
named by FILE, separated by whitespace.  Return 0 on success, non-zero
if an error occurred while writing to FILE.

@end deftypefn

*/

int
writeargv (char * const *argv, FILE *f)
{
  int status = 0;

  if (f == NULL)
    return 1;

  while (*argv != NULL)
    {
      const char *arg = *argv;

      while (*arg != EOS)
        {
          char c = *arg;

          if (ISSPACE(c) || c == '\\' || c == '\'' || c == '"')
            if (EOF == fputc ('\\', f))
              {
                status = 1;
                goto done;
              }

          if (EOF == fputc (c, f))
            {
              status = 1;
              goto done;
            }
          arg++;
        }

      if (EOF == fputc ('\n', f))
        {
          status = 1;
          goto done;
        }
      argv++;
    }

 done:
  return status;
}

/*

@deftypefn Extension void expandargv (int *@var{argcp}, char ***@var{argvp})

The @var{argcp} and @code{argvp} arguments are pointers to the usual
@code{argc} and @code{argv} arguments to @code{main}.  This function
looks for arguments that begin with the character @samp{@@}.  Any such
arguments are interpreted as ``response files''.  The contents of the
response file are interpreted as additional command line options.  In
particular, the file is separated into whitespace-separated strings;
each such string is taken as a command-line option.  The new options
are inserted in place of the option naming the response file, and
@code{*argcp} and @code{*argvp} will be updated.  If the value of
@code{*argvp} is modified by this function, then the new value has
been dynamically allocated and can be deallocated by the caller with
@code{freeargv}.  However, most callers will simply call
@code{expandargv} near the beginning of @code{main} and allow the
operating system to free the memory when the program exits.

@end deftypefn

*/

void
expandargv (int *argcp, char ***argvp)
{
  /* The argument we are currently processing.  */
  int i = 0;
  /* Non-zero if ***argvp has been dynamically allocated.  */
  int argv_dynamic = 0;
  /* Limit the number of response files that we parse in order
     to prevent infinite recursion.  */
  unsigned int iteration_limit = 2000;
  /* Loop over the arguments, handling response files.  We always skip
     ARGVP[0], as that is the name of the program being run.  */
    while (++i < *argcp) // 从第二个参数开始，因为第一个参数是程序名'./gcc'。
    {
      /* The name of the response file.  */
      const char *filename;
      /* The response file.  */
      FILE *f;
      /* An upper bound on the number of characters in the response
	 file.  */
      long pos;
      /* The number of characters in the response file, when actually
	 read.  */
      size_t len;
      /* A dynamically allocated buffer used to hold options read from a
	 response file.  */
      char *buffer;
      /* Dynamically allocated storage for the options read from the
	 response file.  */
      char **file_argv;
      /* The number of options read from the response file, if any.  */
      size_t file_argc;
#ifdef S_ISDIR
      struct stat sb;
#endif
      /* We are only interested in options of the form "@file".  */
      // 获取第i个参数字符串的起始地址，比如'./gcc main.c'，i=1时filename='main.c'。
      filename = (*argvp)[i];

      // 函数只对'@file'类型的参数进行处理。
      if (filename[0] != '@')
        continue;

      /* If we have iterated too many times then stop.  */
      // 最多支持处理2000个'@file'类型的参数。
      if (-- iteration_limit == 0)
	  {
	    fprintf (stderr, "%s: error: too many @-files encountered\n", (*argvp)[0]);
	    xexit (1);
	  }
#ifdef S_ISDIR
      if (stat (filename+1, &sb) < 0)
	continue;
      if (S_ISDIR(sb.st_mode))
	{
	  fprintf (stderr, "%s: error: @-file refers to a directory\n", (*argvp)[0]);
	  xexit (1);
	}
#endif
      /* Read the contents of the file.  */
      // 只读的方式打开文件
      f = fopen (++filename, "r");

      // 打开文件失败
      if (!f)
	    continue;

      // 将文件指针跳转到文件的末尾
      if (fseek (f, 0L, SEEK_END) == -1)
	    goto error;

      // 获取文件指针相对于文件起始的偏移位置
      pos = ftell (f);

      if (pos == -1)
	    goto error;

      // 将文件指针跳转到文件的起始
      if (fseek (f, 0L, SEEK_SET) == -1)
	    goto error;

      // 将文件内容全部读取出来，存放到buffer里面。
      buffer = (char *) xmalloc (pos * sizeof (char) + 1);
      len = fread (buffer, sizeof (char), pos, f);

      // 文件内容读取失败
      if (len != (size_t) pos
	  /* On Windows, fread may return a value smaller than POS,
	     due to CR/LF->CR translation when reading text files.
	     That does not in-and-of itself indicate failure.  */
	  && ferror (f))
	    goto error;

      /* Add a NUL terminator.  */
      buffer[len] = '\0';

      /* If the file is empty or contains only whitespace, buildargv would
	 return a single empty argument.  In this context we want no arguments,
	 instead.  */
	 // 判断文件的内容是不是空格符或者换行符
      if (only_whitespace (buffer))
	  {
	    file_argv = (char **) xmalloc (sizeof (char *));
	    file_argv[0] = NULL;
	  }
      else
      {
	    /* Parse the string.  */
	    file_argv = buildargv (buffer); // 解析文件内容，转成参数，存放起来。
      }

      /* If *ARGVP is not already dynamically allocated, copy it.  */
      if (!argv_dynamic)
      {
        // 申请一段内存，将命令行传入的参数全部拷贝到新内存中。并改写argvp地址。
	    *argvp = dupargv (*argvp);

//        FILE *file;
//        file = fopen("out.txt","a");
//        fprintf(file, "argc=%x, file=%s\n", *argvp, filename);
//        fclose(file);
      }

      /* Count the number of arguments.  */
      file_argc = 0;

      // 统计文件内容转成的参数个数。
      while (file_argv[file_argc])
	    ++file_argc;

      /* Now, insert FILE_ARGV into ARGV.  The "+1" below handles the
	 NULL terminator at the end of ARGV.  */
	 // 扩展申请的内存大小，用来存放file_argv和argvp的所有参数。
      *argvp = ((char **)xrealloc(*argvp, (*argcp + file_argc + 1) * sizeof (char *)));

      // 将参数往后移file_argc - 1个参数位置。然后将file_argv的参数替换掉@file参数。
      memmove(*argvp + i + file_argc, *argvp + i + 1, (*argcp - i) * sizeof (char *));
      memcpy(*argvp + i, file_argv, file_argc * sizeof (char *));

      /* The original option has been replaced by all the new options.  */
      *argcp += file_argc - 1;

      /* Free up memory allocated to process the response file.  We do
	 not use freeargv because the individual options in FILE_ARGV
	 are now in the main ARGV.  */
      free (file_argv);
      free (buffer);

      /* Rescan all of the arguments just read to support response
	 files that include other response files.  */
      --i;

    error:
      /* We're all done with the file now.  */
      fclose (f);
    }
}

/*

@deftypefn Extension int countargv (char * const *@var{argv})

Return the number of elements in @var{argv}.
Returns zero if @var{argv} is NULL.

@end deftypefn

*/

int
countargv (char * const *argv)
{
  int argc;

  if (argv == NULL)
    return 0;
  for (argc = 0; argv[argc] != NULL; argc++)
    continue;
  return argc;
}

#ifdef MAIN

/* Simple little test driver. */

static const char *const tests[] =
{
  "a simple command line",
  "arg 'foo' is single quoted",
  "arg \"bar\" is double quoted",
  "arg \"foo bar\" has embedded whitespace",
  "arg 'Jack said \\'hi\\'' has single quotes",
  "arg 'Jack said \\\"hi\\\"' has double quotes",
  "a b c d e f g h i j k l m n o p q r s t u v w x y z 1 2 3 4 5 6 7 8 9",

  /* This should be expanded into only one argument.  */
  "trailing-whitespace ",

  "",
  NULL
};

int
main (void)
{
  char **argv;
  const char *const *test;
  char **targs;

  for (test = tests; *test != NULL; test++)
    {
      printf ("buildargv(\"%s\")\n", *test);
      if ((argv = buildargv (*test)) == NULL)
	{
	  printf ("failed!\n\n");
	}
      else
	{
	  for (targs = argv; *targs != NULL; targs++)
	    {
	      printf ("\t\"%s\"\n", *targs);
	    }
	  printf ("\n");
	}
      freeargv (argv);
    }

  return 0;
}

#endif	/* MAIN */
