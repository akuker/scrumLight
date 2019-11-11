#pragma once

#define ELEMENTCNT(x) (sizeof(x)/sizeof(x[0]))

#define SET_BIT(p,n) (p |=  (1 << n))
#define CLR_BIT(p,n) (p &= ~(1 << n))
#define GET_BIT(p,n) ((p & (1 << n)) != 0)

#define GET_BYTE(n,val) ((val >> (8*n)) & 0xff)
