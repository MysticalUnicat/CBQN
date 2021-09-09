#pragma once
#include "h.h"
#include "core/stuff.h"
#include "core/heap.h"

#if MM==0
  #include "opt/mm_malloc.h"
#elif MM==1
  #include "opt/mm_buddy.h"
#elif MM==2
  #include "opt/mm_2buddy.h"
#else
  #error "bad MM value"
#endif

#include "core/gstack.h"
#include "core/harr.h"

#define I8Atom i8
#define C8Atom u8
#define I16Atom i16
#define C16Atom u16
#define I32Atom i32
#define C32Atom u32
#define F64Atom f64
#define TYARR_SZ(T,IA) fsizeof(T##Arr, a, T##Atom, IA)
#define TYARR_SZ2(T,IA) TYARR_SZ(T,IA)

#define TU I8
#define TP(W,X) W##i8##X
#include "core/tyarrTemplate.h"
#define TU I16
#define TP(W,X) W##i16##X
#include "core/tyarrTemplate.h"
#define TU I32
#define TP(W,X) W##i32##X
#include "core/tyarrTemplate.h"
#define TU C8
#define TP(W,X) W##c8##X
#include "core/tyarrTemplate.h"
#define TU C16
#define TP(W,X) W##c16##X
#include "core/tyarrTemplate.h"
#define TU C32
#define TP(W,X) W##c32##X
#include "core/tyarrTemplate.h"
#define TU F64
#define TP(W,X) W##f64##X
#include "core/tyarrTemplate.h"

#include "core/i32arr.h"
#include "core/c32arr.h"
#include "core/f64arr.h"

#include "core/fillarr.h"
#include "core/derv.h"

#ifdef RT_VERIFY
  extern B r1Objs[rtLen];
#endif

static B* arr_bptr(B x) { assert(isArr(x));
  if (v(x)->type==t_harr) return harr_ptr(x);
  if (v(x)->type==t_hslice) return c(HSlice,x)->a;
  if (v(x)->type==t_fillarr) return fillarr_ptr(a(x));
  if (v(x)->type==t_fillslice) return c(FillSlice,x)->a;
  return NULL;
}


typedef struct BFn {
  struct Fun;
  B ident;
   BBB2B uc1;
  BBBB2B ucw;
} BFn;
typedef struct BMd2 {
  struct Md2;
   BBBBB2B uc1;
  BBBBBB2B ucw;
} BMd2;

static i64 isum(B x) { // doesn't consume; may error; TODO error on overflow
  assert(isArr(x));
  i64 r = 0;
  usz xia = a(x)->ia;
  u8 xe = TI(x,elType);
  if      (xe==el_i8 ) { i8*  p = i8any_ptr (x); for (usz i = 0; i < xia; i++) r+= p[i]; }
  else if (xe==el_i16) { i16* p = i16any_ptr(x); for (usz i = 0; i < xia; i++) r+= p[i]; }
  else if (xe==el_i32) { i32* p = i32any_ptr(x); for (usz i = 0; i < xia; i++) r+= p[i]; }
  else if (xe==el_f64) {
    f64* p = f64any_ptr(x);
    for (usz i = 0; i < xia; i++) { if(p[i]!=(i64)p[i]) thrM("Expected integer"); r+= p[i]; }
  } else {
    SGetU(x)
    for (usz i = 0; i < xia; i++) r+= o2i64(GetU(x,i));
  }
  return r;
}
