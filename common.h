/*************************************************
  Copyright(C)2011
  ��Ŀ���������߸���
  �ļ�����common.h
  ���ߣ�wy
  �ļ�����������������������
  �����б�
**************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

#include "math.h"
#include "stdlib.h"

/*�Ӵ�����*/
#define VIEW_PIXEL_WIDTH	1000
#define VIEW_PIXEL_HEIGHT	1000

/*�Ӵ��ߴ�*/
#define VIEW_WIDTH 6.0f
#define VIEW_HEIGHT 6.0f

#define TOTALDEPTH 4 /*���ߵݹ����*/ // change 2 to 4;
#define INFINITY 1000000.0f /*�����*/
#define SMALL 0.0001f/*����С*/

#define MAX(a, b) (((a) > (b)) ? (a) : (b)) /*ȡ���ֵ*/
#define DOT(A, B) (A.x*B.x+A.y*B.y+A.z*B.z) /*�������*/

/*�ཻ���ͣ�INTERSECTED_IN�ཻ���ڲ���MISSδ�ཻ��INTERSECTED�ཻ���ⲿ*/
enum INTERSECTION_TYPE {INTERSECTED_IN = -1,MISS = 0,INTERSECTED = 1};

/*������*/
class Vector3
{
public:
	/*���캯��*/
	Vector3();
	Vector3(float a_x,float a_y,float a_z);
	Vector3(const Vector3& a_v);
		
	void set(float a_x,float a_y,float a_z);/*�޸�������Ա����ֵ*/
	void set(Vector3& a_v);
	void normalize();/*������λ��*/
	float dot(Vector3 a_v);/*���������*/
	Vector3 cross(Vector3 a_v);/*���������*/

	/*���ز�����*/
	Vector3 operator- () const;/*���ز�����-����*/
	
	/*��Ԫ����*/
	friend Vector3 operator+ (const Vector3& v1, const Vector3& v2);/*�������ӷ�*/
	friend Vector3 operator- (const Vector3& v1, const Vector3& v2);/*����������*/
	friend Vector3 operator* (const Vector3& v, float f);/*�����˸������������ĸ������ֱ�˸�����*/
	friend Vector3 operator* (float f, const Vector3& v);/*�������������������ĸ������ֱ�˸�����*/
	friend Vector3 operator* (const Vector3& v1, Vector3& v2);/*���������˷��������ĸ�������Ӧ���*/
	
	/*��Ա����*/
	float x, y, z;
};

/*��ɫ*/
typedef Vector3 Color;

/*����*/
typedef unsigned int Pixel;

#endif/*_COMMON_H_*/