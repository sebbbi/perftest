#pragma once

#include <math.h>

const float PI = 3.1415926535897932384626433832795f;

struct int2
{
	int x;
	int y;

	int2() : x(0), y(0) {}
	int2(int x, int y) : x(x), y(y) {}

	int2 operator-() const { return int2(-x, -y); }
	int2 operator+(const int2 &other) const { return int2(x + other.x, y + other.y); }
	int2 operator-(const int2 &other) const { return int2(x - other.x, y - other.y); }
	int2 operator*(const int2 &other) const { return int2(x * other.x, y * other.y); }
	int2 operator/(const int2 &other) const { return int2(x / other.x, y / other.y); }
	int2 operator*(int val) const { return int2(x * val, y * val); }
	int2 operator/(int val) const { return int2(x / val, y / val); }
};

struct int3
{
	int x;
	int y;
	int z;

	int3() : x(0), y(0), z(0) {}
	int3(int x, int y, int z) : x(x), y(y), z(z) {}
	int3(int2 xy, int z) : x(xy.x), y(xy.y), z(z) {}

	int3 operator-() const { return int3(-x, -y, -z); }
	int3 operator+(const int3 &other) const { return int3(x + other.x, y + other.y, z + other.z); }
	int3 operator-(const int3 &other) const { return int3(x - other.x, y - other.y, z - other.z); }
	int3 operator*(const int3 &other) const { return int3(x * other.x, y * other.y, z * other.z); }
	int3 operator/(const int3 &other) const { return int3(x / other.x, y / other.y, z / other.z); }
	int3 operator*(int val) const { return int3(x * val, y * val, z * val); }
	int3 operator/(int val) const { return int3(x / val, y / val, z / val); }
};

struct int4
{
	int x;
	int y;
	int z;
	int w;

	int4() : x(0), y(0), z(0), w(0) {}
	int4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}
	int4(int2 xy, int z, int w) : x(xy.x), y(xy.y), z(z), w(w) {}

	int4 operator-() const { return int4(-x, -y, -z, -w); }
	int4 operator+(const int4 &other) const { return int4(x + other.x, y + other.y, z + other.z, w + other.w); }
	int4 operator-(const int4 &other) const { return int4(x - other.x, y - other.y, z - other.z, w - other.w); }
	int4 operator*(const int4 &other) const { return int4(x * other.x, y * other.y, z * other.z, w * other.w); }
	int4 operator/(const int4 &other) const { return int4(x / other.x, y / other.y, z / other.z, w / other.w); }
	int4 operator*(int val) const { return int4(x * val, y * val, z * val, w * val); }
	int4 operator/(int val) const { return int4(x / val, y / val, z / val, w / val); }
};

struct float2
{
	float x;
	float y;

	float2() : x(0), y(0) {}
	float2(float x, float y) : x(x), y(y) {}

	float2 operator-() const { return float2(-x, -y); }
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

	float3 operator-() const { return float3(-x, -y, -z); }
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

	float4 operator-() const { return float4(-x, -y, -z, -w); }
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

inline float2 int2toFloat2(int2 v) { return float2(static_cast<float>(v.x), static_cast<float>(v.y)); }
inline int2 float2toInt2(float2 v) { return int2(static_cast<int>(v.x), static_cast<int>(v.y)); }

inline float3 int3toFloat3(int3 v) { return float3(static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z)); }
inline int3 float3toInt3(float3 v) { return int3(static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z)); }

inline int divRoundUp(int value, int divisor)
{
	return (value + divisor - 1) / divisor;
}

inline int2 divRoundUp(int2 value, int2 divisor)
{
	return (value + divisor - int2(1, 1)) / divisor;
}

inline int3 divRoundUp(int3 value, int3 divisor)
{
	return (value + divisor - int3(1, 1, 1)) / divisor;
}
