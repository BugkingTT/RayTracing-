/*************************************************
  Copyright(C)2011
  项目描述：光线跟踪
  文件名：scene.cpp
  作者：wy 日期：
  文件描述：场景类
  函数列表：
**************************************************/

#include "scene.h"
#include "common.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
/*射线类*/

/*构造函数*/
Ray::Ray()
{
	m_origin = Vector3(0,0,0);
	m_direction = Vector3(0,0,1);
}

Ray::Ray(Vector3 &org, Vector3 &drct)
{
	m_origin = org;
	m_direction = drct;
}

/*读取射线起点*/
const Vector3& Ray::get_origin()
{
	return m_origin;
}

/*读取射线方向*/
const Vector3& Ray::get_direction()
{
	return m_direction;
}

/*设置起点*/
void Ray::set_origin(Vector3& org)
{
	m_origin = org;
}

/*设置射线方向*/
void Ray::set_direction(Vector3& drct)
{
	m_direction = drct;
}

/*获取射线上离起点距离为d的某点*/
Vector3 Ray::get_point(float d)
{
	return m_origin+d*m_direction;
}

/*一般物体类*/

/*构造函数*/
Object::Object()
{
	m_color = Vector3(0,0,0);
	m_spec = 1.0f;
	m_refl = 1.0f;
}

Vector3 Object::get_color()
{
	return m_color;
}

float Object::get_spec()
{
	return m_spec;
}

float Object::get_refl()
{
	return m_refl;
}

float Object::get_diffuse()
{
	return 1.0f - m_spec;
}

/*重新设置成员变量值*/
void Object::reset(Vector3 color, float spec, float refl)
{
	m_color = color;
	m_spec = spec;
	m_refl = refl;
}

Vector3 Triangle::get_normal(Vector3 Point)
{
	return m_normal;
}

Vector3 Triangle::get_normal()
{
	return m_normal;
}

float Triangle::get_distance()
{
	return m_dist;
}

INTERSECTION_TYPE Triangle::is_intersected(Ray ray, float& dist)/*判断光线是否与物体相交*/
{
		Vector3 e1 = v1 - v0;
		Vector3 e2 = v2 - v0;
		Vector3 dir(ray.get_direction());
		Vector3 pvec =dir.cross(e2);
		float det = DOT(e1, pvec);

		if (det > -0.00001f && det < 0.00001f) {
			return MISS;
		}

		float invDet = 1 / det;

		Vector3 tvec = ray.get_origin() - v0;
		float u = DOT(tvec, pvec) * invDet;

		if (u < 0.0f || u > 1.0f) {
			return MISS;
		}

		Vector3 qvec = tvec.cross(e1);
		float v = DOT(ray.get_direction(), qvec) * invDet;

		if (v < 0.0f || (u + v) > 1.0f) {
			return MISS;
		}

		float t = DOT(e2, qvec) * invDet;

		if (t < 0.0f) {
			return MISS;
		}
		if (t > 0)
		{
				if (t < dist)
				{
					dist = t;
					return INTERSECTED;
				}
			
		}
	return MISS;

}

Vector3 Qurd::get_normal(Vector3 Point)
{
	//	cout << m_normal.x << " " << m_normal.y << " " << m_normal.z << endl;
	return m_normal;
}

Vector3 Qurd::get_normal()
{
	
	return m_normal;
}

float Qurd::get_distance()
{
	return m_dist;
}
INTERSECTION_TYPE Qurd::is_intersected(Ray ray, float& dist)
{
	if (t1->is_intersected(ray, dist) == MISS)
		return t2->is_intersected(ray, dist);
	return INTERSECTED;
	/*float dst1,dst2;
	INTERSECTION_TYPE p1 = t1->is_intersected(ray, dst1);
	INTERSECTION_TYPE p2 = t2->is_intersected(ray, dst2);
	if (p1 == INTERSECTED)
	{
		cout << "1" << endl;
		dist = dst1;
		return p1;
	}
	if (p2 == INTERSECTED)
	{
		dist = dst2;
		return p2;
	}
	return MISS;
	*/
}

/*球体类*/

Sphere::Sphere()
{
	m_center = Vector3(0,0,0);
	m_radius = 1.0f;
}

Sphere::Sphere(Vector3 cent, float radi)
{
	m_center = cent;
	m_radius = radi;
}

/*重新设置球心和半径*/
void Sphere::reset_c_r(Vector3 cent, float radi)
{
	m_center = cent;
	m_radius = radi;
}

Vector3 Sphere::get_center()
{
	return m_center;
}

float Sphere::get_radius()
{
	return m_radius;
}

/*获取球体表面一点的法线*/
Vector3 Sphere::get_normal(Vector3 point)
{
	return (point - m_center);
}

/*判断光线是否与球体相交，若有交点则取最近交点，计算其到眼睛的距离*/
INTERSECTION_TYPE Sphere::is_intersected(Ray ray, float& dst) 
{
	float a = 1.0f;
	Vector3 v = ray.get_origin() - m_center;
	float b = 2*DOT(ray.get_direction(), v);
	float c = DOT(v, v) - m_radius * m_radius;
	float det = b*b-4*a*c;
	INTERSECTION_TYPE retval = MISS;
	if(det>0)
	{
		det = sqrt(det);
		float t1 = (-b-det)/2*a;
		float t2 = (-b+det)/2*a;
		if(t2>0)
		{ 
			if(t1<0)
			{
				if(t2<dst)
				{
					dst = t2;
					retval = INTERSECTED_IN;
				}		
			}
			else
			{
				if(t1<dst)
				{
					dst = t1;
					retval = INTERSECTED;
				}
			}
		}
	}
	
	return retval;
}

Vector3 Triangular::get_normal(Vector3 Point)
{
	return m_normal;
}

INTERSECTION_TYPE Triangular::is_intersected(Ray ray, float& dist)
{
	INTERSECTION_TYPE i[5];
	float dst[5];
	int c;
	vector<float> it;
	for (c = 0; c < 2; c++)
	{
		dst[c] = INFINITY;
		i[c] = t[c]->is_intersected(ray, dst[c]);
		if (i[c] == INTERSECTED)
			it.push_back(dst[c]);
	}
	for (c = 0; c < 3; c++)
	{
		dst[c + 2] = INFINITY;
		i[c + 2] = q[c]->is_intersected(ray, dst[c + 2]);
		if (i[c + 2] == INTERSECTED)
			it.push_back(dst[c + 2]);
	}
	//计算与所有面的相交情况
	if (it.size() != 0)
	{
		sort(it.begin(), it.end());
		float tmp = *(it.begin());
		for (c = 0; c < 2; c++)
		{
			if (tmp == dst[c])
			{
				dist = tmp;
				m_normal = t[c]->get_normal();
			}
		}
		for (c = 0; c < 3; c++)
		{
			if (tmp == dst[c + 2])
			{
				dist = tmp;
				m_normal = q[c]->get_normal();
			}
			
		}
		return INTERSECTED;
	}
	else
		return MISS;
}

Cube::Cube()
{

}

float Cube::get_length()
{
	return m_length;
}

Vector3 Cube::get_normal(Vector3 Point)
{
	return m_normal;
}

INTERSECTION_TYPE Cube::is_intersected(Ray ray, float& dist)// 根据 面和观测点的距离确定 判定顺序  远的先判定
{
	INTERSECTION_TYPE i[6];
	float dst[6];
	int c;
	vector<float> it;
	for ( c = 0; c < 6; c++)
	{
		dst[c] = INFINITY;
		i[c] = q[c]->is_intersected(ray, dst[c]);
		if(i[c]==INTERSECTED)
			it.push_back(dst[c]);
	}//计算与所有面的相交情况
	if (it.size() != 0)
	{
		sort(it.begin(), it.end());
		float tmp = *(it.begin());
		for (c = 0; c < 6; c++)
		{
			if (tmp == dst[c] )
			{
				dist = dst[c];
				m_normal = q[c]->get_normal();
				return INTERSECTED;
			}
		}
	}
	else
		return MISS;
}

Vector3 Units::get_normal(Vector3 Point)
{
	return m_normal;
}

INTERSECTION_TYPE Units::is_intersected(Ray ray, float& dist)
{
	INTERSECTION_TYPE i[20];
	float dst[20];
	int c;
	vector<float> it;
	for (c = 0; c < n-2; c++)
	{
		dst[c] = INFINITY;
		i[c] = t[c]->is_intersected(ray, dst[c]);
		if (i[c] == INTERSECTED)
			it.push_back(dst[c]);
	}//计算与所有面的相交情况
	if (it.size() != 0)
	{
		sort(it.begin(), it.end());
		float tmp = *(it.begin());
		for (c = 0; c < n-2; c++)
		{
			if (tmp == dst[c])
			{
				dist = dst[c];
				m_normal = t[c]->get_normal(Vector3(0,0,0));
				return INTERSECTED;
			}
		}
	}
	else
		return MISS;
}

/*光源类*/
PointLight::PointLight()
{
	m_position = Vector3(0,0,0);
	m_color = Vector3(1,1,1);
}

Vector3 PointLight::get_pst()
{
	return m_position;
}

Color PointLight::get_color()
{
	return m_color;
}

/*重新设置光源位置和颜色*/
void PointLight::reset(Vector3 position, Vector3 color)
{
	m_position = position;
	m_color = color;
}


/*平板类*/

Cir_Plane::Cir_Plane()
	:m_normal(0, 0, 0),m_center(0,0,0), m_dist(1), m_radius(1)
{

}

Cir_Plane::Cir_Plane(Vector3 n,Vector3 cent,float d, float r)
	: m_normal(n),m_center(cent),m_dist(d), m_radius(r)
{

}

Vector3 Cir_Plane::get_normal(Vector3 point)
{
	return m_normal;
}

void Cir_Plane::reset_n_d(Vector3 n, float dist,float r)
{
	m_normal = n;
	m_dist = dist;		
	m_radius = r;
}

INTERSECTION_TYPE Cir_Plane::is_intersected(Ray ray, float& dist)
{
	float cos = DOT(m_normal, ray.get_direction());
	if (cos != 0)
	{
		float dst = -(DOT(m_normal, ray.get_origin()) + m_dist) / cos;
		if (dst > 0)
		{
			Vector3 Point = ray.get_origin()+(dst - m_dist) * ray.get_direction();
			if (DOT((Point - m_center),(Point - m_center))< m_radius * m_radius)
			{
				
				if (dst < dist)
				{
					dist = dst;
					return INTERSECTED;
				}
			}
		}
	}
	return MISS;
}

Plane::Plane()
	:m_normal(0,0,0),m_dist(1)
{
}

Plane::Plane(Vector3 n,float d)
	:m_normal(n),m_dist(d)
{
}


Vector3 Plane::get_normal(Vector3 position)
{
	Vector3 p = position;
	return m_normal;
}

float Plane::get_distance()
{
	return m_dist;
}

void Plane::reset_n_d(Vector3 n, float dist)
{
	m_normal = n;
	m_dist = dist;
}

INTERSECTION_TYPE Plane::is_intersected(Ray ray,float& dist)
{
	float cos = DOT(m_normal,ray.get_direction());
	if (cos != 0)
	{
		float dst = -(DOT(m_normal,ray.get_origin()) + m_dist)/cos;
		if (dst > 0)
		{
			if (dst < dist) 
			{
				dist = dst;
				return INTERSECTED;
			}
		}
	}
	return MISS;
}
/*场景类*/
/*场景类*/

Scene::Scene():m_obj_num(0),m_lig_num(0),m_obj(0),m_light(0)
{
}

Scene::~Scene()
{
	delete m_obj;
	delete m_light;
}

/*初始化场景*/
void Scene::init_scene()
{
	/*光源列表*/
	/*点光源*/
	m_light = new PointLight[100];
	int i = 0;/*计算光源个数*/
	m_light[i].reset(Vector3(0.0f,5.0f,5.0f), Vector3(1.5,1.5,1.5));/*光源1*/
	//m_light[++i].reset(Vector3(0,5.0f,-5.0f), Vector3(1,1,1));/*光源2*/
	m_lig_num = i + 1;

	/*场景中物体列表*/
	m_obj = new Object*[100];
	int j = 0;/*计算场景中物体个数*/


	m_obj[j] = new Sphere(Vector3(2,2,-5),1.0f);
	m_obj[j]->reset(Vector3(0.9f,0.9f,0.1f), 0.5f, 0.1f);
	
	m_obj[++j] = new Cube(Vector3(0.25, -1, -3.5), Vector3(0.25, -1, -2), Vector3(0.75, -1, -2), Vector3(0.75, -1, -3.5), 0.3f);
	m_obj[j]->reset(Vector3(1.0f, 0.8f, 1.0f), 0.4f, 0.2f);
	
	m_obj[++j] = new Cube(Vector3(-1, -1, -2.5), Vector3(-1, -1, -2), Vector3(2, -1, -2), Vector3(2, -1, -2.5), 0.3f);
	m_obj[j]->reset(Vector3(1.0f, 0.8f, 1.0f), 0.4f, 0.2f);

	m_obj[++j] = new Cube(Vector3(-1, -1, -2), Vector3(-1, -1, 0), Vector3(-0.5, -1, 0), Vector3(-0.5, -1, -2), 0.3f);
	m_obj[j]->reset(Vector3(1.0f, 0.8f, 1.0f), 0.4f, 0.2f);

	m_obj[++j] = new Cube(Vector3(0.25, -1, -2), Vector3(0.25, -1, 0), Vector3(0.75, -1, 0), Vector3(0.75, -1, -2), 0.3f);
	m_obj[j]->reset(Vector3(1.0f, 0.8f, 1.0f), 0.4f, 0.2f);

	m_obj[++j] = new Cube(Vector3(1.5, -1, -2), Vector3(1.5, -1, 0), Vector3(2 , -1, 0), Vector3(2, -1, -2), 0.3f);
	m_obj[j]->reset(Vector3(1.0f, 0.8f, 1.0f), 0.4f, 0.2f);

	m_obj[++j] = new Cube(Vector3(-1, -1, 0),  Vector3(-1, -1, 0.5),Vector3(2, -1, 0.5),Vector3(2, -1, 0) , 0.3f);
	m_obj[j]->reset(Vector3(1.0f, 0.8f, 1.0f), 0.4f, 0.2f);

	m_obj[++j] = new Cube(Vector3(0.25, -1, 0.5), Vector3(0.25, -1, 1.5), Vector3(0.75, -1, 1.5), Vector3(0.75, -1, 0.5), 0.3f);
	m_obj[j]->reset(Vector3(1.0f, 0.8f, 1.0f), 0.4f, 0.2f);
	/*
	Vector3 Point[8] = { Vector3(0, -1, 1), Vector3(1, -1, 1), Vector3(0, -1, 0),Vector3(1,-1,0),Vector3(0,-1,-1),Vector3(1,-1,-1),Vector3(0,-1,-2),Vector3(1,-1,-2) };
	m_obj[j] = new Units(Point, 8, 0.3);
	m_obj[j]->reset(Vector3(1.0f, 0.8f, 1.0f), 0.4f, 0.2f);*/
	/* 联结体 的 上层点集合模式：
					6———7
					|	   |
					4	   5
					|	   |
					2	   3
					|	   |
					0———1
	从两边分别从0 和 1 开始 以2 递增 
	构造思想与字的书写方式相同。
	*/
	/*平板*/
	m_obj[++j] = new Plane(Vector3(0,1,0),100.0f);/*法向量朝上的平板*/
	m_obj[j]->reset(Vector3(1.0f, 0.8f, 1.0f), 0.4f, 0.2f);

	m_obj_num = j + 1;
}

int Scene::get_o_num()
{
	return m_obj_num;
}

int Scene::get_l_num()
{
	return m_lig_num;
}

/*取出编号为idx的物体*/
Object* Scene::get_objects(int idx)
{
	return m_obj[idx];
}

/*取出编号为idx的光源*/
PointLight Scene::get_light(int idx)
{
	return m_light[idx];
}