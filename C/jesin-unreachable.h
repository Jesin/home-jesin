/* jesin-unreachable.h by Kevin Dodd */
#ifndef jesin_unreachable_h_INCLUDED
#define jesin_unreachable_h_INCLUDED


#ifndef __has_builtin
#define __has_builtin(x) 0
#endif


#if __has_builtin(__builtin_unreachable) || __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)

#define JESIN_UNREACHABLE() __builtin_unreachable()
#define JESIN_ASSUME(x) do { if (!(x)) { __builtin_unreachable(); } } while (0)

#elif defined(_MSC_VER)

#define JESIN_UNREACHABLE() __assume(0)
#define JESIN_ASSUME(x) __assume(x)

#else

#define JESIN_UNREACHABLE() do { } while (0)
#define JESIN_ASSUME(x) do { } while (0)

#endif


#endif //jesin_unreachable_h_INCLUDED
