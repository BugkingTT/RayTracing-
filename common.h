/*************************************************
  Copyright(C)2011
  项目描述：光线跟踪
  文件名：common.h
  作者：wy
  文件描述：公用声明，向量类
  函数列表：
**************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

#include "math.h"
#include "stdlib.h"

/*视窗像素*/
#define VIEW_PIXEL_WIDTH	1000
#define VIEW_PIXEL_HEIGHT	1000

/*视窗尺寸*/
#define VIEW_WIDTH 6.0f
#define VIEW_HEIGHT 6.0f

#define TOTALDEPTH 4 /*光线递归深度*/ // change 2 to 4;
#define INFINITY 1000000.0f /*无穷大*/
#define SMALL 0.0001f/*无穷小*/

#define MAX(a, b) (((a) > (b)) ? (a) : (b)) /*取最大值*/
#define DOT(A, B) (A.x*B.x+A.y*B.y+A.z*B.z) /*向量点乘*/

/*相交类型，INTERSECTED_IN相交在内部，MISS未相交，INTERSECTED相交在外部*/
enum INTERSECTION_TYPE {INTERSECTED_IN = -1,MISS = 0,INTERSECTED = 1};

/*向量类*/
class Vector3
{
public:
	/*构造函数*/
	Vector3();
	Vector3(float a_x,float a_y,float a_z);
	Vector3(const Vector3& a_v);
		
	void set(float a_x,float a_y,float a_z);/*修改向量成员变量值*/
	void set(Vector3& a_v);
	void normalize();/*向量单位化*/
	float dot(Vector3 a_v);/*两向量点积*/
	Vector3 cross(Vector3 a_v);/*两向量差积*/

	/*重载操作符*/
	Vector3 operator- () const;/*重载操作符-，求反*/
	
	/*友元函数*/
	friend Vector3 operator+ (const Vector3& v1, const Vector3& v2);/*两向量加法*/
	friend Vector3 operator- (const Vector3& v1, const Vector3& v2);/*两向量减法*/
	friend Vector3 operator* (const Vector3& v, float f);/*向量乘浮点数，向量的各分量分别乘浮点数*/
	friend Vector3 operator* (float f, const Vector3& v);/*浮点数乘向量，向量的各分量分别乘浮点数*/
	friend Vector3 operator* (const Vector3& v1, Vector3& v2);/*两个向量乘法，向量的各分量对应相乘*/
	
	/*成员变量*/
	float x, y, z;
};

/*颜色*/
typedef Vector3 Color;

/*像素*/
typedef unsigned int Pixel;

#endif/*_COMMON_H_*/