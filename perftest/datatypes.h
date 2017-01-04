#pragma once

#include <math.h>
#include <stdint.h>

const float PI = 3.1415926535897932384626433832795f;

typedef uint32_t uint;

struct uint2
{
	uint x;
	uint y;

	uint2() : x(0), y(0) {}
	uint2(int x, int y) : x(x), y(y) {}

	uint2 operator+(const uint2 &other) const { return uint2(x + other.x, y + other.y); }
	uint2 operator-(const uint2 &other) const { return uint2(x - other.x, y - other.y); }
	uint2 operator*(const uint2 &other) const { return uint2(x * other.x, y * other.y); }
	uint2 operator/(const uint2 &other) const { return uint2(x / other.x, y / other.y); }
	uint2 operator*(int val) const { return uint2(x * val, y * val); }
	uint2 operator/(int val) const { return uint2(x / val, y / val); }
};

struct uint3
{
	uint x;
	uint y;
	uint z;

	uint3() : x(0), y(0), z(0) {}
	uint3(int x, int y, int z) : x(x), y(y), z(z) {}
	uint3(uint2 xy, int z) : x(xy.x), y(xy.y), z(z) {}

	uint3 operator+(const uint3 &other) const { return uint3(x + other.x, y + other.y, z + other.z); }
	uint3 operator-(const uint3 &other) const { return uint3(x - other.x, y - other.y, z - other.z); }
	uint3 operator*(const uint3 &other) const { return uint3(x * other.x, y * other.y, z * other.z); }
	uint3 operator/(const uint3 &other) const { return uint3(x / other.x, y / other.y, z / other.z); }
	uint3 operator*(int val) const { return uint3(x * val, y * val, z * val); }
	uint3 operator/(int val) const { return uint3(x / val, y / val, z / val); }
};

struct uint4
{
	uint x;
	uint y;
	uint z;
	uint w;

	uint4() : x(0), y(0), z(0), w(0) {}
	uint4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}
	uint4(uint2 xy, int z, int w) : x(xy.x), y(xy.y), z(z), w(w) {}

	uint4 operator+(const uint4 &other) const { return uint4(x + other.x, y + other.y, z + other.z, w + other.w); }
	uint4 operator-(const uint4 &other) const { return uint4(x - other.x, y - other.y, z - other.z, w - other.w); }
	uint4 operator*(const uint4 &other) const { return uint4(x * other.x, y * other.y, z * other.z, w * other.w); }
	uint4 operator/(const uint4 &other) const { return uint4(x / other.x, y / other.y, z / other.z, w / other.w); }
	uint4 operator*(int val) const { return uint4(x * val, y * val, z * val, w * val); }
	uint4 operator/(int val) const { return uint4(x / val, y / val, z / val, w / val); }
};

struct float2
{
	float x;
	float y;

	float2() : x(0), y(0) {}
	float2(float x, float y) : x(x), y(y) {}

	float2 operator+(const float2 &other) const { return float2(x + other.x, y + other.y); }
	float2 operator-(const float2 &other) const { return float2(x - other.x, y - other.y); }
	float2 operator*(const float2 &other) const { return float2(x * other.x, y * other.y); }
	float2 operator/(const float2 &other) const { return float2(x / other.x, y / other.y); }
	float2 operator*(float val) const { return float2(x * val, y * val); }
	float2 operator/(float val) const { return float2(x / val, y / val); }
};

inline float dot(float2 val1, float2 val2)
{
	return
		val1.x * val2.x +
		val1.y * val2.y;
}

struct float3
{
	float x;
	float y;
	float z;

	float3() : x(0), y(0), z(0) {}
	float3(float x, float y, float z) : x(x), y(y), z(z) {}

	float3 operator+(const float3 &other) const { return float3(x + other.x, y + other.y, z + other.z); }
	float3 operator-(const float3 &other) const { return float3(x - other.x, y - other.y, z - other.z); }
	float3 operator*(const float3 &other) const { return float3(x * other.x, y * other.y, z * other.z); }
	float3 operator/(const float3 &other) const { return float3(x / other.x, y / other.y, z / other.z); }
	float3 operator*(float val) const { return float3(x * val, y * val, z * val); }
	float3 operator/(float val) const { return float3(x / val, y / val, z / val); }

	float distanceSq(const float3 &rhs)
	{
		float dx = x - rhs.x;
		float dy = y - rhs.y;
		float dz = z - rhs.z;
		return dx*dx + dy*dy + dz*dz;
	}

	float distance(const float3 &rhs)
	{
		return sqrtf(distanceSq(rhs));
	}
};

inline float dot(float3 val1, float3 val2)
{
	return
		val1.x * val2.x +
		val1.y * val2.y +
		val1.z * val2.z;
}

struct float4
{
	float x;
	float y;
	float z;
	float w;

	float4() : x(0), y(0), z(0), w(0) {}
	float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	float4 operator+(const float4 &other) const { return float4(x + other.x, y + other.y, z + other.z, w + other.w); }
	float4 operator-(const float4 &other) const { return float4(x - other.x, y - other.y, z - other.z, w - other.w); }
	float4 operator*(const float4 &other) const { return float4(x * other.x, y * other.y, z * other.z, w * other.w); }
	float4 operator/(const float4 &other) const { return float4(x / other.x, y / other.y, z / other.z, w / other.w); }
	float4 operator*(float val) const { return float4(x * val, y * val, z * val, w * val); }
	float4 operator/(float val) const { return float4(x / val, y / val, z / val, w / val); }
};

inline float dot(float4 val1, float4 val2)
{
	return
		val1.x * val2.x +
		val1.y * val2.y +
		val1.z * val2.z +
		val1.w * val2.w;
}

struct quat
{
	float x;
	float y;
	float z;
	float w;

	quat() : x(0), y(0), z(0), w(1) {}
	quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	quat(float3 axis, float angle)
	{
		float hangle = angle * 0.5f;
		float sinz = sinf(hangle);
		x = axis.x * sinz;
		y = axis.y * sinz;
		z = axis.z * sinz;
		w = cosf(hangle);
	}

	quat operator*(const quat &other) const
	{
		return quat(
			x*other.w + y*other.z - z*other.y + w*other.x,
			-x*other.z + y*other.w + z*other.x + w*other.y,
			x*other.y - y*other.x + z*other.w + w*other.z,
			-x*other.x - y*other.y - z*other.z + w*other.w);
	}

	quat operator*(float val) const { return quat(x * val, y * val, z * val, w * val); }
	quat operator/(float val) const { return quat(x / val, y / val, z / val, w / val); }

	quat conjugate() const { return quat(-x, -y, -z, w); }

	float3 transform(const float3 &vec) const
	{
		quat res = *this * quat(vec.x, vec.y, vec.z, 0) * this->conjugate();
		//quat res = this->conjugate() * (quat(vec.x, vec.y, vec.z, 0) * *this);
		return float3(res.x, res.y, res.z);
	}
};

inline float dot(quat val1, quat val2)
{
	return
		val1.x * val2.x +
		val1.y * val2.y +
		val1.z * val2.z +
		val1.w * val2.w;
}

template <typename T>
float length(T val)
{
	return sqrt(dot(val, val));
}

template <typename T>
T normalize(T val)
{
	return val / length(val);
}

inline float2 uint2toFloat2(uint2 v) { return float2(static_cast<float>(v.x), static_cast<float>(v.y)); }
inline uint2 float2touint2(float2 v) { return uint2(static_cast<int>(v.x), static_cast<int>(v.y)); }

inline float3 uint3toFloat3(uint3 v) { return float3(static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z)); }
inline uint3 float3touint3(float3 v) { return uint3(static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z)); }

inline int divRoundUp(uint value, uint divisor)
{
	return (value + divisor - 1) / divisor;
}

inline uint2 divRoundUp(uint2 value, uint2 divisor)
{
	return (value + divisor - uint2(1, 1)) / divisor;
}

inline uint3 divRoundUp(uint3 value, uint3 divisor)
{
	return (value + divisor - uint3(1, 1, 1)) / divisor;
}
