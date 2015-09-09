/* Abstract AVL Tree Generic C Package.
** Interface generation header file.
**
** This code is in the public domain.  See cavl_tree.html for interface
** documentation.
**
** Version: 1.3  Author: Walt Karas
*/

#if 0
Code was downloaded from...

  http://www.geocities.com/wkaras/gen_c/cavl_tree.html

... and modifications were made to add walk() function
and fix minor compilation warnings.

Code has been used with permission of its author (see
email below)

From: Walt Karas <wkaras@aaahawk.com>
To: Dominique Pelle <DPelle@navman.com>
Subject: Re: AVL tree library

Yes, that was my intention in declaring the code
to be public domain.  I want it to be used by others
for any purpose, commercial or non-commercial, without
any obligations to me.  If your company's lawyers don't
feel this is sufficient for you to use this code without legal
concerns, please have them contact me.  I will try to do what
is necessary to satisfy them.

----- Original Message ----- 
From: Dominique Pelle <DPelle@navman.com>
To: <wkaras@aaahawk.com>
Sent: Sunday, March 16, 2003 7:53 PM
Subject: AVL tree library


> Hi,
> 
> I stumbled upon your well written AVL tree library
> available at:
> 
>    http://www.geocities.com/wkaras/gen_c/cavl_tree.html
> 
> The above link states that the source code it describes,
> is in the public domain but there is no other detail.
> 
> I would like to use this AVL tree library and link it
> with an embedded car navigation product (proprietary
> software).  However, before doing so, I want to be
> 100% sure that it is OK to reuse the AVL tree library
> in this context.  I will use the library only if I am
> sure that I do not violate any license agreement.
> 
> Could you confirm that it is ok to use this AVL tree
> for proprietary software without any legal concern?
> 
> Thank you very much in advance.
> 
> -- 
> Dominique Pelle
#endif

/* This header contains the definition of CHAR_BIT (number of bits in a
** char). */
#include <limits.h>

#undef L__
#undef L__EST_LONG_BIT
#undef L__SIZE
#undef L__SC
#undef L__LONG_BIT
#undef L__BIT_ARR_DEFN

#ifndef AVL_SEARCH_TYPE_DEFINED_
#define AVL_SEARCH_TYPE_DEFINED_

typedef enum
  {
    AVL_EQUAL = 1,
    AVL_LESS = 2,
    AVL_GREATER = 4,
    AVL_LESS_EQUAL = AVL_EQUAL | AVL_LESS,
    AVL_GREATER_EQUAL = AVL_EQUAL | AVL_GREATER
  }
avl_search_type;

#endif

#ifdef AVL_UNIQUE

#define L__ AVL_UNIQUE

#else

#define L__(X) X

#endif

/* Determine storage class for function prototypes. */
#ifdef AVL_PRIVATE

#define L__SC   static
#define L__SC_M static

#else

#define L__SC extern

#ifndef AVL_PRIVATE_MODIFIERS
#define L__SC_M extern
#else
#define L__SC_M static
#endif  // #ifndef AVL_PRIVATE_MODIFIERS

#endif  // #ifdef AVL_PRIVATE


#ifdef AVL_SIZE

#define L__SIZE AVL_SIZE

#else

#define L__SIZE unsigned long

#endif

typedef struct L__(tree)
  {
    #ifdef AVL_INSIDE_STRUCT

    AVL_INSIDE_STRUCT

    #endif

    AVL_HANDLE root;
  }
L__(tree);

/* Function prototypes. */

#ifndef AVL_PRIVATE

#ifndef AVL_PRIVATE_MODIFIERS

L__SC_M void L__(init)(L__(tree) *tree);

L__SC_M AVL_HANDLE L__(insert)(L__(tree) *tree, AVL_HANDLE h);

L__SC_M AVL_HANDLE L__(remove)(L__(tree) *tree, AVL_KEY k);

L__SC_M AVL_HANDLE L__(subst)(L__(tree) *tree, AVL_HANDLE new_node);

#endif  // #ifndef AVL_PRIVATE_MODIFIERS

L__SC int L__(is_empty)(L__(tree) *tree);

L__SC AVL_HANDLE L__(search)(L__(tree) *tree, AVL_KEY k, avl_search_type st);

L__SC AVL_HANDLE L__(search_equal)(L__(tree) *tree, AVL_KEY k);

L__SC AVL_HANDLE L__(search_least)(L__(tree) *tree);

L__SC AVL_HANDLE L__(search_greatest)(L__(tree) *tree);

#ifdef AVL_BUILD_ITER_TYPE

L__SC int L__(build)(
  L__(tree) *tree, AVL_BUILD_ITER_TYPE p, L__SIZE num_nodes);

#endif

#endif

/* ANSI C/ISO C++ require that a long have at least 32 bits.  Set
** L__EST_LONG_BIT to be the greatest multiple of 8 in the range
** 32 - 64 (inclusive) that is less than or equal to the number of
** bits in a long.
*/

#if (((LONG_MAX >> 31) >> 7) == 0)

#define L__EST_LONG_BIT 32

#elif (((LONG_MAX >> 31) >> 15) == 0)

#define L__EST_LONG_BIT 40

#elif (((LONG_MAX >> 31) >> 23) == 0)

#define L__EST_LONG_BIT 48

#elif (((LONG_MAX >> 31) >> 31) == 0)

#define L__EST_LONG_BIT 56

#else

#define L__EST_LONG_BIT 64

#endif

/* Number of bits in a long. */
#define L__LONG_BIT (sizeof(long) * CHAR_BIT)

/* The macro L__BIT_ARR_DEFN defines a bit array whose index is a (0-based)
** node depth.  The definition depends on whether the maximum depth is more
** or less than the number of bits in a single long.
*/

#if ((AVL_MAX_DEPTH) > L__EST_LONG_BIT)

/* Maximum depth may be more than number of bits in a long. */

#define L__BIT_ARR_DEFN(NAME) \
  unsigned long NAME[((AVL_MAX_DEPTH) + L__LONG_BIT - 1) / L__LONG_BIT];

#else

/* Maximum depth is definitely less than number of bits in a long. */

#define L__BIT_ARR_DEFN(NAME) unsigned long NAME;

#endif

/* Iterator structure. */
typedef struct L__(iter)
  {
    /* Tree being iterated over. */
    L__(tree) *tree_;

    /* Records a path into the tree.  If bit n is true, indicates
    ** take greater branch from the nth node in the path, otherwise
    ** take the less branch.  bit 0 gives branch from root, and
    ** so on. */
    L__BIT_ARR_DEFN(branch)

    /* Zero-based depth of path into tree. */
    unsigned depth;

    /* Handles of nodes in path from root to current node (returned by *). */
    AVL_HANDLE path_h[(AVL_MAX_DEPTH) - 1];
  }
L__(iter);

/* Iterator function prototypes. */

typedef int (*L__(TWalkWorker))(AVL_HANDLE handle);

#ifndef AVL_PRIVATE
L__SC void L__(start_iter)(
  L__(tree) *tree, L__(iter) *iter, AVL_KEY k, avl_search_type st);

L__SC void L__(start_iter_least)(L__(tree) *tree, L__(iter) *iter);

L__SC void L__(start_iter_greatest)(L__(tree) *tree, L__(iter) *iter);

L__SC AVL_HANDLE L__(get_iter)(L__(iter) *iter);

L__SC void L__(incr_iter)(L__(iter) *iter);

L__SC void L__(decr_iter)(L__(iter) *iter);

L__SC void L__(walk)(L__(tree) *tree, L__(TWalkWorker) worker);
#endif

#define AVL_IMPL_INIT                    1
#define AVL_IMPL_IS_EMPTY               (1 << 1)
#define AVL_IMPL_INSERT                 (1 << 2)
#define AVL_IMPL_SEARCH                 (1 << 3)
#define AVL_IMPL_SEARCH_EQUAL           (1 << 4)
#define AVL_IMPL_SEARCH_LEAST           (1 << 5)
#define AVL_IMPL_SEARCH_GREATES         (1 << 6)
#define AVL_IMPL_REMOVE                 (1 << 7)
#define AVL_IMPL_BUILD                  (1 << 8)
#define AVL_IMPL_START_ITER             (1 << 9)
#define AVL_IMPL_START_ITER_LEAST       (1 << 10)
#define AVL_IMPL_START_ITER_GREATEST    (1 << 11)
#define AVL_IMPL_GET_ITER               (1 << 12)
#define AVL_IMPL_INCR_ITER              (1 << 13)
#define AVL_IMPL_DECR_ITER              (1 << 14)
#define AVL_IMPL_SUBST                  (1 << 15)
#define AVL_IMPL_WALK                   (1 << 16)

#define AVL_IMPL_ALL                    (~0)

#undef L__
#undef L__EST_LONG_BIT
#undef L__SIZE
#undef L__SC
#undef L__LONG_BIT
#undef L__BIT_ARR_DEFN
