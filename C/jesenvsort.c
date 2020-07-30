/* jesenvsort.c by Kevin Dodd */
#include "jesenvsort.h"

typedef char* str;
typedef unsigned char uch;
typedef const uch* ustr;

int jesEnvCmp(const void* x, const void* y) {
	ustr a=x, b=y;
#if 0
	if (!a) { return b ? 1 : 0; }
	if (!b) { return -1; }
#endif
	for (; *a == *b; ++a, ++b) {
		if (*a == (uch)'=' || !*a) { return 0; }
	}
	int r = *a == (uch)'=' ? -1 : *b == (uch)'=' ? 1 : (int)*a - (int)*b;
	JES_ASSUME(r);
	return r;
}

str* jesEnvGet(const char* restrict v, str* restrict a, size_t n) {
	while (n) {
		size_t i = n/2;
		int c = jesEnvCmp(v, a[i]);
		if (c < 0) {
			n = i;
		} else if (c) {
			a += ++i;
			n -= i;
		} else {
			return a+i;
		}
	}
	return NULL;
}

JesEnvRangeT jesEnvSort(str* restrict begin, str* restrict temp, str** restrict ps) {
	str** p = ps;
	str* dst = *p = temp;
	*dst = *begin;
	str* src = begin + 1;
	int d = 0;
	do {
		int c = jesEnvCmp(*src, *dst);
		if (!c) {
			continue;
		}
		*++dst = *src;
		if (!d) {
			d = c;
		} else if ((c^d) < 0) {
			if (d < 0) {
				str* a = *p;
				str* b = dst;
				while (a < --b) {
					str t = *b;
					*b = *a;
					*a++ = t;
				}
			}
			*++p = dst;
			d = 0;
		}
	} while (*++src);
	p[1] = ++dst;
	if (d < 0) {
		str* a = *p;
		str* b = dst;
		while (a < --b) {
			str t = *b;
			*b = *a;
			*a++ = t;
		}
	}
	dst = begin;
	size_t n = p - ps;
	while (n) {
		str* restrict q = dst;
		dst = *ps;
		size_t i = 0;
		do {
			str* a = ps[i];
			*(str**)(i*(sizeof a/2)+(str)ps) = q;
			i += 2;
			str* b = ps[i-1];
			while (1) {
				int c = jesEnvCmp(*b, *a);
				if (c < 0) {
					*q++ = *b++;
					if (b < ps[i]) {
						continue;
					}
					goto FinishA;
				}
				if (!c && ++b >= ps[i]) {
FinishA:
					do {
						*q++ = *a++;
					} while (a < ps[i-1]);
					break;
				}
				*q++ = *a++;
				if (a >= ps[i-1]) {
					do {
						*q++ = *b++;
					} while (b < ps[i]);
					break;
				}
			}
		} while (i < n);
		if (!(n & 1)) {
			str* a = ps[n];
			*(str**)(n*(sizeof a/2)+(str)ps) = q;
			do {
				*q++ = *a++;
			} while (a < ps[n+1]);
		}
		*q = NULL;
		n /= 2;
		ps[n+1] = q;
	}
	JesEnvRangeT r;
	r.a = *ps;
	r.n = ps[1]-r.a;
	return r;
}
