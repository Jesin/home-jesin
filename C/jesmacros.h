/* jesmacros.h by Kevin Dodd */
#ifndef jesmacros_h_INCLUDED
#define jesmacros_h_INCLUDED 1
#include <stddef.h>

#if __STDC_VERSION__ < 199901L
#define restrict __restrict__
#endif

#define JES_ASSUME(x) do { if (!(x)) JES_UNREACHABLE; } while (0)

#if __has_builtin(__builtin_unreachable) || __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
#define JES_UNREACHABLE __builtin_unreachable()
#elif defined(_MSC_VER)
#define JES_ASSUME(x) __assume(x)
#define JES_UNREACHABLE __assume(0)
#else
#define JES_UNREACHABLE (*(char*)0)
#endif

#if __has_builtin(__builtin_expect_with_probability) || __GNUC__ > 8
#define JES_EXPECT_PROB(a,c,p) __builtin_expect_with_probability(a,c,p)
#elif __has_builtin(__builtin_expect) || __GNUC__ > 2
#define JES_EXPECT_PROB(a,c,p) __builtin_expect(a,c)
#else
#define JES_EXPECT_PROB(a,c,p) (a)
#endif

#endif //jesmacros_h_INCLUDED
