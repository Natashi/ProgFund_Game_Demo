#pragma once

#include "../../pch.h"

class Vectorize {
public:
	//Loads vector from D3DXVECTOR4, alignment ignored
	static __forceinline __m128 Load(const D3DXVECTOR4& vec);
	//Loads vector from float pointer, alignment ignored
	static __forceinline __m128 Load(float* const ptr);
	//Loads double vector from double pointer, alignment ignored
	static __forceinline __m128d Load(double* const ptr);
	//Stores the data of vector "dst" into float array "ptr" (size=4)
	static __forceinline void Store(float* const ptr, const __m128& dst);

	//Creates vector (a, b, c, d)
	static __forceinline __m128 Set(float a, float b, float c, float d);
	//Creates double vector (a, b)
	static __forceinline __m128d Set(double a, double b);
	//Creates int vector (a, b, c, d)
	static __forceinline __m128i Set(int a, int b, int c, int d);

	//Set wrappers----------------------------------------------------------------
	static __forceinline __m128 SetF(float a, float b, float c, float d) { return Set(a, b, c, d); }
	static __forceinline __m128d SetD(double a, double b) { return Set(a, b); }
	static __forceinline __m128i SetI(int a, int b, int c, int d) { return Set(a, b, c, d); }
	//----------------------------------------------------------------------------

	//Generates vector (x, x, x, x)
	static __forceinline __m128 Replicate(float x);
	//Generates double vector (x, x)
	static __forceinline __m128d Replicate(double x);
	//Generates int vector (x, x, x, x)
	static __forceinline __m128i Replicate(int x);

	//From vector (a, b, c, d), generate a new vector (a, a, c, c)
	static __forceinline __m128 DuplicateEven(const __m128& x);
	//From vector (a, b, c, d), generate a new vector (b, b, d, d)
	static __forceinline __m128 DuplicateOdd(const __m128& x);

	//[add] vector a and b
	static __forceinline __m128 Add(const __m128& a, const __m128& b);
	//[subtract] vector a and b
	static __forceinline __m128 Sub(const __m128& a, const __m128& b);
	//[multiply] vector a and b
	static __forceinline __m128 Mul(const __m128& a, const __m128& b);
	//[divide] vector a and b
	static __forceinline __m128 Div(const __m128& a, const __m128& b);

	//[add] double vector a and b
	static __forceinline __m128d Add(const __m128d& a, const __m128d& b);
	//[subtract] double vector a and b
	static __forceinline __m128d Sub(const __m128d& a, const __m128d& b);
	//[multiply] double vector a and b
	static __forceinline __m128d Mul(const __m128d& a, const __m128d& b);
	//[divide] double vector a and b
	static __forceinline __m128d Div(const __m128d& a, const __m128d& b);

	//Alternating [add] and [subtract]-> (-, +, -, +)
	static __forceinline __m128 AddSub(const __m128& a, const __m128& b);
	//Fused [multiply]+[add]
	static __forceinline __m128 MulAdd(const __m128& a, const __m128& b, const __m128& c);
	//Fused [multiply]+[add] for double vector
	static __forceinline __m128d MulAdd(const __m128d& a, const __m128d& b, const __m128d& c);

	//Performs [max] on vector a and b
	static __forceinline __m128 MaxPacked(const __m128& a, const __m128& b);
	//Performs [min] on vector a and b
	static __forceinline __m128 MinPacked(const __m128& a, const __m128& b);
	//Performs [clamp] on vector a and b (Fused [min]+[max])
	static __forceinline __m128 ClampPacked(const __m128& a, const __m128& min, const __m128& max);

	//Performs [max] on double vector a and b
	static __forceinline __m128d MaxPacked(const __m128d& a, const __m128d& b);
	//Performs [min] on double vector a and b
	static __forceinline __m128d MinPacked(const __m128d& a, const __m128d& b);
	//Performs [clamp] on double vector a and b (Fused [min]+[max])
	static __forceinline __m128d ClampPacked(const __m128d& a, const __m128d& min, const __m128d& max);

	//Performs [max] on int vector a and b
	static __forceinline __m128i MaxPacked(const __m128i& a, const __m128i& b);
	//Performs [min] on int vector a and b
	static __forceinline __m128i MinPacked(const __m128i& a, const __m128i& b);
	//Performs [clamp] on int vector a and b (Fused [min]+[max])
	static __forceinline __m128i ClampPacked(const __m128i& a, const __m128i& min, const __m128i& max);
};

//---------------------------------------------------------------------
//Implementation
//---------------------------------------------------------------------

__m128 Vectorize::Load(const D3DXVECTOR4& vec) {
	return Load((float*)&vec);
}
__m128 Vectorize::Load(float* const ptr) {
	__m128 res;
#ifndef __L_MATH_VECTORIZE
	memcpy(res.m128_f32, ptr, sizeof(__m128));
#else
	//SSE
	res = _mm_loadu_ps(ptr);
#endif
	return res;
}
__m128d Vectorize::Load(double* const ptr) {
	__m128d res;
#ifndef __L_MATH_VECTORIZE
	memcpy(res.m128d_f64, ptr, sizeof(__m128d));
#else
	//SSE2
	res = _mm_loadu_pd(ptr);
#endif
	return res;
}
void Vectorize::Store(float* const ptr, const __m128& dst) {
#ifndef __L_MATH_VECTORIZE
	memcpy(ptr, &dst, sizeof(__m128));
#else
	//SSE
	_mm_storeu_ps(ptr, dst);
#endif
}

//---------------------------------------------------------------------

__m128 Vectorize::Set(float a, float b, float c, float d) {
	__m128 res;
#ifndef __L_MATH_VECTORIZE
	res.m128_f32[0] = a;
	res.m128_f32[1] = b;
	res.m128_f32[2] = c;
	res.m128_f32[3] = d;
#else
	//SSE
	res = _mm_set_ps(d, c, b, a);
#endif
	return res;
}
__m128d Vectorize::Set(double a, double b) {
	__m128d res;
#ifndef __L_MATH_VECTORIZE
	res.m128d_f64[0] = a;
	res.m128d_f64[1] = b;
#else
	//SSE
	res = _mm_set_pd(b, a);
#endif
	return res;
}
__m128i Vectorize::Set(int a, int b, int c, int d) {
	__m128i res;
#ifndef __L_MATH_VECTORIZE
	res.m128i_i32[0] = a;
	res.m128i_i32[1] = b;
	res.m128i_i32[2] = c;
	res.m128i_i32[3] = d;
#else
	//SSE2
	res = _mm_set_epi32(d, c, b, a);
#endif
	return res;
}

//---------------------------------------------------------------------

__m128 Vectorize::Replicate(float x) {
	__m128 res;
#ifndef __L_MATH_VECTORIZE
	res.m128_f32[0] = x;
	res.m128_f32[1] = x;
	res.m128_f32[2] = x;
	res.m128_f32[3] = x;
#else
	//SSE
	res = _mm_set_ps1(x);
#endif
	return res;
}
__m128d Vectorize::Replicate(double x) {
	__m128d res;
#ifndef __L_MATH_VECTORIZE
	res.m128d_f64[0] = x;
	res.m128d_f64[1] = x;
#else
	//SSE
	res = _mm_set1_pd(x);
#endif
	return res;
}
__m128i Vectorize::Replicate(int x) {
	__m128i res;
#ifndef __L_MATH_VECTORIZE
	res.m128i_i32[0] = x;
	res.m128i_i32[1] = x;
	res.m128i_i32[2] = x;
	res.m128i_i32[3] = x;
#else
	//SSE2
	res = _mm_set1_epi32(x);
#endif
	return res;
}

//---------------------------------------------------------------------

__m128 Vectorize::DuplicateEven(const __m128& x) {
	__m128 res;
#ifndef __L_MATH_VECTORIZE
	res.m128_f32[0] = x.m128_f32[0];
	res.m128_f32[1] = x.m128_f32[0];
	res.m128_f32[2] = x.m128_f32[2];
	res.m128_f32[3] = x.m128_f32[2];
#else
	//SSE3
	res = _mm_moveldup_ps(x);
#endif
	return res;
}
__m128 Vectorize::DuplicateOdd(const __m128& x) {
	__m128 res;
#ifndef __L_MATH_VECTORIZE
	res.m128_f32[0] = x.m128_f32[1];
	res.m128_f32[1] = x.m128_f32[1];
	res.m128_f32[2] = x.m128_f32[3];
	res.m128_f32[3] = x.m128_f32[3];
#else
	//SSE3
	res = _mm_movehdup_ps(x);
#endif
	return res;
}

//---------------------------------------------------------------------

__m128 Vectorize::Add(const __m128& a, const __m128& b) {
	__m128 res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 4; ++i)
		res.m128_f32[i] = a.m128_f32[i] + b.m128_f32[i];
#else
	//SSE
	res = _mm_add_ps(a, b);
#endif
	return res;
}
__m128 Vectorize::Sub(const __m128& a, const __m128& b) {
	__m128 res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 4; ++i)
		res.m128_f32[i] = a.m128_f32[i] - b.m128_f32[i];
#else
	//SSE
	res = _mm_sub_ps(a, b);
#endif
	return res;
}
__m128 Vectorize::Mul(const __m128& a, const __m128& b) {
	__m128 res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 4; ++i)
		res.m128_f32[i] = a.m128_f32[i] * b.m128_f32[i];
#else
	//SSE
	res = _mm_mul_ps(a, b);
#endif
	return res;
}
__m128 Vectorize::Div(const __m128& a, const __m128& b) {
	__m128 res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 4; ++i)
		res.m128_f32[i] = a.m128_f32[i] / b.m128_f32[i];
#else
	//SSE
	res = _mm_div_ps(a, b);
#endif
	return res;
}

//---------------------------------------------------------------------

__m128d Vectorize::Add(const __m128d& a, const __m128d& b) {
	__m128d res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 2; ++i)
		res.m128d_f64[i] = a.m128d_f64[i] + b.m128d_f64[i];
#else
	//SSE
	res = _mm_add_pd(a, b);
#endif
	return res;
}
__m128d Vectorize::Sub(const __m128d& a, const __m128d& b) {
	__m128d res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 2; ++i)
		res.m128d_f64[i] = a.m128d_f64[i] - b.m128d_f64[i];
#else
	//SSE
	res = _mm_sub_pd(a, b);
#endif
	return res;
}
__m128d Vectorize::Mul(const __m128d& a, const __m128d& b) {
	__m128d res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 2; ++i)
		res.m128d_f64[i] = a.m128d_f64[i] * b.m128d_f64[i];
#else
	//SSE
	res = _mm_mul_pd(a, b);
#endif
	return res;
}
__m128d Vectorize::Div(const __m128d& a, const __m128d& b) {
	__m128d res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 2; ++i)
		res.m128d_f64[i] = a.m128d_f64[i] / b.m128d_f64[i];
#else
	//SSE
	res = _mm_div_pd(a, b);
#endif
	return res;
}

//---------------------------------------------------------------------

__m128 Vectorize::AddSub(const __m128& a, const __m128& b) {
	__m128 res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 4; ++i) {
		if ((i & 0b1) == 0)
			res.m128_f32[i] = a.m128_f32[i] - b.m128_f32[i];
		else
			res.m128_f32[i] = a.m128_f32[i] + b.m128_f32[i];
	}
#else
	//SSE3
	res = _mm_addsub_ps(a, b);
#endif
	return res;
}
__m128 Vectorize::MulAdd(const __m128& a, const __m128& b, const __m128& c) {
	__m128 res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 4; ++i)
		res.m128_f32[i] = fma(a.m128_f32[i], b.m128_f32[i], c.m128_f32[i]);
#else
	//SSE
	res = _mm_mul_ps(a, b);
	res = _mm_add_ps(res, c);
#endif
	return res;
}
__m128d Vectorize::MulAdd(const __m128d& a, const __m128d& b, const __m128d& c) {
	__m128d res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 2; ++i)
		res.m128d_f64[i] = fma(a.m128d_f64[i], b.m128d_f64[i], c.m128d_f64[i]);
#else
	//SSE2
	res = _mm_mul_pd(a, b);
	res = _mm_add_pd(res, c);
#endif
	return res;
}

//---------------------------------------------------------------------

__m128 Vectorize::MaxPacked(const __m128& a, const __m128& b) {
	__m128 res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 4; ++i)
		res.m128_f32[i] = std::max(a.m128_f32[i], b.m128_f32[i]);
#else
	//SSE
	res = _mm_min_ps(a, b);
#endif
	return res;
}
__m128 Vectorize::MinPacked(const __m128& a, const __m128& b) {
	__m128 res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 4; ++i)
		res.m128_f32[i] = std::min(a.m128_f32[i], b.m128_f32[i]);
#else
	//SSE
	res = _mm_max_ps(a, b);
#endif
	return res;
}
__m128 Vectorize::ClampPacked(const __m128& a, const __m128& min, const __m128& max) {
	__m128 res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 4; ++i)
		res.m128_f32[i] = std::clamp(a.m128_f32[i], min.m128_f32[i], max.m128_f32[i]);
#else
	//SSE
	res = _mm_max_ps(a, min);
	res = _mm_min_ps(res, max);
#endif
	return res;
}

//---------------------------------------------------------------------

__m128d Vectorize::MaxPacked(const __m128d& a, const __m128d& b) {
	__m128d res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 2; ++i)
		res.m128d_f64[i] = std::max(a.m128d_f64[i], b.m128d_f64[i]);
#else
	//SSE2
	res = _mm_min_pd(a, b);
#endif
	return res;
}
__m128d Vectorize::MinPacked(const __m128d& a, const __m128d& b) {
	__m128d res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 2; ++i)
		res.m128d_f64[i] = std::min(a.m128d_f64[i], b.m128d_f64[i]);
#else
	//SSE2
	res = _mm_max_pd(a, b);
#endif
	return res;
}
__m128d Vectorize::ClampPacked(const __m128d& a, const __m128d& min, const __m128d& max) {
	__m128d res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 2; ++i)
		res.m128d_f64[i] = std::clamp(a.m128d_f64[i], min.m128d_f64[i], max.m128d_f64[i]);
#else
	//SSE2
	res = _mm_max_pd(a, min);
	res = _mm_min_pd(res, max);
#endif
	return res;
}

//---------------------------------------------------------------------

__m128i Vectorize::MaxPacked(const __m128i& a, const __m128i& b) {
	__m128i res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 4; ++i)
		res.m128i_i32[i] = std::max(a.m128i_i32[i], b.m128i_i32[i]);
#else
	//SSE4.1
	res = _mm_min_epi32(a, b);
#endif
	return res;
}
__m128i Vectorize::MinPacked(const __m128i& a, const __m128i& b) {
	__m128i res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 4; ++i)
		res.m128i_i32[i] = std::min(a.m128i_i32[i], b.m128i_i32[i]);
#else
	//SSE4.1
	res = _mm_max_epi32(a, b);
#endif
	return res;
}
__m128i Vectorize::ClampPacked(const __m128i& a, const __m128i& min, const __m128i& max) {
	__m128i res;
#ifndef __L_MATH_VECTORIZE
	for (int i = 0; i < 4; ++i)
		res.m128i_i32[i] = std::clamp(a.m128i_i32[i], min.m128i_i32[i], max.m128i_i32[i]);
#else
	//SSE4.1
	res = _mm_max_epi32(a, min);
	res = _mm_min_epi32(res, max);
#endif
	return res;
}