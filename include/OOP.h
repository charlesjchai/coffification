#ifndef OOP_H
#define OOP_H
// Downcasts ptr to type by extracting variable member
#define OOP_DEFSELF(ptr, type, member)                                         \
    type *self = ((type *)((char *)(ptr) - offsetof(type, member)))
#define OOP_DEFCONSTSELF(ptr, type, member)                                    \
    const type *self =                                                         \
        ((const type *)((const char *)(ptr) - offsetof(type, member)))

// Safe restrict macro
#if defined(__cplusplus) || (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L))
#if defined(_MSC_VER)
#define restrict __restrict
#elif defined(__GNUC__) || defined(__clang__)
#define restrict __restrict__
#else
#define restrict // Remove if not supported
#endif
#endif
#endif