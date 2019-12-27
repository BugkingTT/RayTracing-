/*************************************************
  Copyright(C)2011
  项目描述：光线跟踪
  文件名：common.h
  作者：wy 
  文件描述：公用声明，向量类
**************************************************/

#include "common.h"
#include "math.h"

/*默认构造函数*/
Vector3::Vector3()
{
	x=0;
	y=0;
	z=0;
}

Vector3::Vector3(float a_x, float a_y, float a_z)
{
	x=a_x; 
	y=a_y; 
	z=a_z;
}

Vector3::Vector3(const Vector3 &a_v)
{
	x=a_v.x; 
	y=a_v.y; 
	z=a_v.z;
}

/*成员函数*/

/*修改向量成员变量值*/
void Vector3::set(float a_x, float a_y, float a_z)
{
	x=a_x;
	y=a_y;
	z=a_z;
}

void Vector3::set(Vector3 &a_v)
{
	x=a_v.x;
	y=a_v.y;
	z=a_v.z;
}

/*向量单位化*/
void Vector3::normalize()
{
	float l=1.0f/sqrt(x*x+y*y+z*z);
	x*=l;
	y*=l;
	z*=l;
}

/*两向量点积*/
float Vector3::dot(Vector3 a_v)
{
	return x*a_v.x+y*a_v.y+z*a_v.z;
}
/*两向量差积*/
Vector3 Vector3::cross(Vector3 a_v)
{
	return Vector3(y*a_v.z-z*a_v.y, z*a_v.x-x*a_v.z, x*a_v.y-y*a_v.x);
}

/*重载操作符*/

/*重载操作符-，求反*/
Vector3 Vector3::operator -() const
{
	return Vector3(-x, -y, -z);
}

/*友元函数*/

/*两向量加法*/
Vector3 operator + (const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

/*两向量减法*/
Vector3 operator- (const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

/*向量乘浮点数，向量的各分量分别乘浮点数*/
Vector3 operator* (const Vector3& v, float f)
{
	return Vector3(v.x * f, v.y * f, v.z * f);
}

/*浮点数乘向量，向量的各分量分别乘浮点数*/
Vector3 operator* (float f, const Vector3& v)
{
	return Vector3(v.x * f, v.y * f, v.z * f);
}

/*两个向量乘法，向量的各分量对应相乘*/
Vector3 operator* (const Vector3& v1, Vector3& v2)
{
	return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}
	