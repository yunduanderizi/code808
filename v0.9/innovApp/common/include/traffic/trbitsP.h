#ifndef TRBITSP_H
#define TRBITSP_H

/*
 * The following are some bit-manipulation macros that I thought might
 * be useful elsewhere at some point.
 *
 * The expansions of these macros are pretty hideous, but as long as the
 * arguments are constants the compiler should do almost all of the work
 * at compile-time.
 */

/// @brief The number that contains ones only in bits \p i through \p i -
///        \p n.
///
#define MASK(n, i) (((1 << n) - 1) << (i - n))

/// @brief The amount an \p i bit number should be shifted left so that
///        it's most significant bit occupies bit \p n.
///
#define SHIFT(n, i) (i - n)

/// @brief Masks out bits other than \p i through \p i - \p n.
///
#define MASK_FIELD(value, n, i) (value & MASK(n, i))

/// @brief Sets bits \p i through \p i - \p n from the corresponding
///        bits from \p byte.
#define DEPOSIT_FIELD(value, n, i, byte) \
  ((value & ~MASK(n, i)) | (byte & MASK(n, i)))

#define LDB(value, n, i) (MASK_FIELD(value, n, i) >> SHIFT(n, i))
#define DPB(value, n, i, byte) DEPOSIT_FIELD(value, n, i, (byte << SHIFT(n, i)))

#endif
