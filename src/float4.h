
#include <nmmintrin.h>

static const __m128 SIGNMASK = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));

class __attribute__ ((aligned(16))) float4 {
public:
	union {
		struct {
			float x, y, z, w;
		};
		__m128 mmvalue;
	};

	inline float4() :
			mmvalue(_mm_setzero_ps()) {
	}
	inline float4(float a) :
			mmvalue(_mm_set1_ps(a)) {
	}
	inline float4(float a, float b, float c) :
			mmvalue(_mm_setr_ps(0, a, b, c)) {
	}
	inline float4(float a, float b, float c, float d) :
			mmvalue(_mm_setr_ps(a, b, c, d)) {
	}
	inline float4(__m128 m) :
			mmvalue(m) {
	}

	inline float4 operator+(const float4& b) const {
		return _mm_add_ps(mmvalue, b.mmvalue);
	}
	inline float4 operator-(const float4& b) const {
		return _mm_sub_ps(mmvalue, b.mmvalue);
	}
	inline float4 operator*(const float4& b) const {
		return _mm_mul_ps(mmvalue, b.mmvalue);
	}
	inline float4 operator/(const float4& b) const {
		return _mm_div_ps(mmvalue, b.mmvalue);
	}
	inline float4 operator-() const {
		return _mm_xor_ps(mmvalue, SIGNMASK);
	}

	inline float4& operator+=(const float4& b) {
		*this = *this + b;
		return *this;
	}
	inline float4& operator-=(const float4& b) {
		*this = *this - b;
		return *this;
	}
	inline float4& operator*=(const float4& b) {
		*this = *this * b;
		return *this;
	}
	inline float4& operator/=(const float4& b) {
		*this = *this / b;
		return *this;
	}

	inline float4 operator+(float b) const {
		return _mm_add_ps(mmvalue, _mm_set1_ps(b));
	}
	inline float4 operator-(float b) const {
		return _mm_sub_ps(mmvalue, _mm_set1_ps(b));
	}
	inline float4 operator*(float b) const {
		return _mm_mul_ps(mmvalue, _mm_set1_ps(b));
	}
	inline float4 operator/(float b) const {
		return _mm_div_ps(mmvalue, _mm_set1_ps(b));
	}

	inline float4& operator+=(float b) {
		*this = *this + b;
		return *this;
	}
	inline float4& operator-=(float b) {
		*this = *this - b;
		return *this;
	}
	inline float4& operator*=(float b) {
		*this = *this * b;
		return *this;
	}
	inline float4& operator/=(float b) {
		*this = *this / b;
		return *this;
	}

	inline float length() const {
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(mmvalue, mmvalue, 0x71)));
	}
	inline float rlength() const {
		return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_dp_ps(mmvalue, mmvalue, 0x71)));
	}

	inline float4 cross(const float4 &b) const {
		return _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(mmvalue, mmvalue, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(b.mmvalue, b.mmvalue, _MM_SHUFFLE(3, 1, 0, 2))),
				_mm_mul_ps(_mm_shuffle_ps(mmvalue, mmvalue, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(b.mmvalue, b.mmvalue, _MM_SHUFFLE(3, 0, 2, 1))));
	}
};

inline float4 operator+(float a, const float4& b) {
	return b + a;
}
inline float4 operator-(float a, const float4& b) {
	return float4(a) - b;
}
inline float4 operator*(float a, const float4& b) {
	return b * a;
}
inline float4 operator/(float a, const float4& b) {
	return float4(a) / b;
}
inline float length(const float4& a) {
	return a.length();
}
inline float rlength(const float4& a) {
	return a.rlength();
}

inline float4 cross(const float4& a, const float4& b) {
	return a.cross(b);
}
