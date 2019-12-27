/*************************************************
  Copyright(C)2011
  项目描述：光线跟踪
  文件名：scene.h
  作者：wy 日期：
  文件描述：材质类，一般物体类，特殊物体类，场景类，射线类
  函数列表：
**************************************************/

#ifndef _SCENE_H_
#define _SCENE_H_

#include "common.h"
#include <list>
using namespace std;

//射线类
class Ray
{
public:
	Ray();
	Ray(Vector3& org,Vector3& drct);
	
	const Vector3& get_origin();/*读取射线起点*/
	const Vector3& get_direction();/*读取射线方向*/
	void set_origin(Vector3& org);/*设置射线起点*/
	void set_direction(Vector3& drct);/*设置射线方向*/
	Vector3 get_point(float d);/*获取射线上离起点距离d的某点*/

private:
	Vector3 m_origin;/*起点*/
	Vector3 m_direction;/*方向*/
};

/*一般物体类*/
class Object
{
public:
	Object();
	
	Vector3 get_color();
	float get_spec();
	float get_refl();
	float get_diffuse();
	void reset(Vector3 color, float spec, float refl);/*重新设置成员变量值*/

	virtual Vector3 get_normal(Vector3 point) = 0;/*获取物体表面一点的法线*/
	virtual INTERSECTION_TYPE is_intersected(Ray ray,float& dst) = 0;/*判断光线是否与物体相交*/

protected:
	Color m_color;/*颜色*/
	float m_spec;/*镜面反射强度*/
	float m_refl;/*环境反射强度*/
};

class Triangle :public Object
{
public:
	Triangle();
	//Vertices must be given in anticlock order
	Triangle(
		Vector3& v0 = Vector3(0.0f, 0.0f, 0.0f),
		Vector3& v1 = Vector3(0.0f, 0.0f, 0.0f),
		Vector3& v2 = Vector3(0.0f, 0.0f, 0.0f)
	) :
		v0(v0),
		v1(v1),
		v2(v2)
	{
		m_normal = (v0 - v1).cross(v1 - v2);
		m_normal.normalize();
		m_dist = fabs(DOT(v0, m_normal)) / sqrt(DOT(m_normal, m_normal));
	}

	Vector3 get_normal(Vector3 Point);

	Vector3 get_normal();

	float get_distance();

	INTERSECTION_TYPE is_intersected(Ray ray, float& dst);/*判断光线是否与物体相交*/

private:
	float m_dist;/*原点到平板的距离*/
	Vector3 m_normal;
	Vector3 v0, v1, v2;

};



class Qurd :public Object
{
public:
	Qurd();

	Qurd(
		Vector3& v0 = Vector3(0.0f, 0.0f, 0.0f),
		Vector3& v1 = Vector3(0.0f, 0.0f, 0.0f),
		Vector3& v2 = Vector3(0.0f, 0.0f, 0.0f),
		Vector3& v3 = Vector3(0.0f, 0.0f, 0.0f))
		:
		v0(v0),
		v1(v1),
		v2(v2),
		v3(v3)
	{
		m_normal = (v0 - v2).cross(v1 - v3);
		m_normal.normalize();
		m_dist = fabs(DOT(v0, m_normal)) / sqrt(DOT(m_normal, m_normal));
		t1 = new Triangle(v0, v1, v3);
		t2 = new Triangle(v1, v2, v3);
	}

	~Qurd()
	{

	}

	Vector3 get_normal(Vector3 Point);
	Vector3 get_normal();
	float get_distance();
	INTERSECTION_TYPE is_intersected(Ray ray, float& dst);
private:
	Vector3 v0, v1, v2, v3;
	Vector3 m_normal;
	float m_dist;
	Triangle * t1;
	Triangle * t2;
;
};



/*球体类*/
class Sphere:public Object
{
public:
	Sphere();
	Sphere(Vector3 cent,float radi);

	Vector3 get_center();
	float get_radius();
	void reset_c_r(Vector3 cent,float radi);/*重新设置球心和半径*/

	Vector3 get_normal(Vector3 point);/*获取球体表面一点的法线*/
	INTERSECTION_TYPE is_intersected(Ray ray,float& dst);/*判断光线是否与球体相交*/

protected:
	Vector3 m_center;/*球心*/
	float m_radius;/*半径*/
};


/*立方体类*///add
class Cube:public Object
{
public:
	Cube();
	Cube(Vector3 ve0 = Vector3(0.0f, 0.0f, 0.0f),
		Vector3 ve1 = Vector3(0.0f, 0.0f, 0.0f),
		Vector3 ve2 = Vector3(0.0f, 0.0f, 0.0f),
		Vector3 ve3 = Vector3(0.0f, 0.0f, 0.0f),
		float length = 0.0f)
		:
		v0(ve0),
		v1(ve1),
		v2(ve2),
		v3(ve3),
		m_length(length)
	{
		v4 = v0;
		v5 = v1;
		v6 = v2;
		v7 = v3;

		v4.y -= length;
		v5.y -= length; 
		v6.y -= length;
		v7.y -= length;
		q[0] = new Qurd(v0, v1, v2, v3);
		q[1] = new Qurd(v4, v5, v6, v7);
		q[2] = new Qurd(v4, v7, v3, v0);
		q[3] = new Qurd(v5, v6, v2, v1);
		q[4] = new Qurd(v6, v2, v3, v7);
		q[5] = new Qurd(v5, v1, v0, v4);
	}

	~Cube()
	{
	}
	float get_length();
	Vector3 get_normal(Vector3 Point);
	INTERSECTION_TYPE is_intersected(Ray ray, float& dist);


private:
	Vector3 v0, v1, v2, v3;
	Vector3 v4, v5, v6, v7;
	float m_length;
	Vector3 m_normal;

	Qurd * q[6];
};
class Triangular :public Object
{
public:
	Triangular();
	Triangular(Vector3 ve0 = Vector3(0.0f, 0.0f, 0.0f),
			   Vector3 ve1 = Vector3(0.0f, 0.0f, 0.0f),
			   Vector3 ve2 = Vector3(0.0f, 0.0f, 0.0f), 
		       float height = 0.0f)
			  :v0(ve0), v1(ve1), v2(ve2), m_height(height)
	{
		v3 = v0;
		v4 = v1;
		v5 = v2;
		v3.y -= height;
		v4.y -= height;
		v5.y -= height;

		t[0] = new Triangle(v0, v1, v2);
		t[1] = new Triangle(v3, v4, v5);
		q[0] = new Qurd(v0, v3, v5, v2);
		q[1] = new Qurd(v0, v3, v4, v1);
		q[2] = new Qurd(v2, v5, v4, v1);
	}
	~Triangular()
	{

	}
	Vector3 get_normal(Vector3 Point);
	INTERSECTION_TYPE is_intersected(Ray ray, float& dist);
	
private:
	Vector3 v0, v1, v2;
	Vector3 v3, v4, v5;
	float m_height;
	Vector3 m_normal;
	Triangle* t[2];
	Qurd* q[3];

};

/*点光源类*/
class PointLight
{
public:
	PointLight();

	Vector3 get_pst();
	Color get_color();
	void reset(Vector3 position, Vector3 color);/*重新设置光源位置和颜色*/
private:
	Vector3 m_position;/*位置*/
	Color m_color;/*颜色*/
};

/*平板类*/
class Cir_Plane:public Object
{
public:
	Cir_Plane();
	Cir_Plane(Vector3 n,Vector3 cent, float d, float r);

	Vector3 get_normal(Vector3 point);
	void reset_n_d(Vector3 n, float dist,float r);/*重新设置平板法向量和原点到平板的距离*/

	INTERSECTION_TYPE is_intersected(Ray ray,float& dist);/*判断光线是否与平板相交*/

private:
	float m_radius;
	Vector3 m_normal;/*平板法向量*/
	Vector3 m_center;
	float m_dist;/*原点到平板的距离*/
};
class Plane :public Object
{
public:
	Plane();
	Plane(Vector3 n, float d);


	float get_distance();
	//Vector3 get_normal();/*获取平板的法线*/
	Vector3 get_normal(Vector3 position);/*获取平板表面一点的法线*/
	void reset_n_d(Vector3 n, float dist);/*重新设置平板法向量和原点到平板的距离*/

	INTERSECTION_TYPE is_intersected(Ray ray, float& dist);/*判断光线是否与平板相交*/

private:
	float m_length;
	float m_width;
	Vector3 m_normal;/*平板法向量*/
	float m_dist;/*原点到平板的距离*/
};


class Units :public Object
{
public:
	Units()
	{

	}
	Units(Vector3 p[], int n, float length)
		:n(n), m_length(length)
	{
		m_point = new Vector3[n];
		m_point = p;
		for (int i = 0; i < n-2; i++)//用三棱柱填充
		{
			if(i%2==0)
			t[i] = new Triangular(m_point[i], m_point[i + 1], m_point[i + 2], m_length);
			else
			t[i] = new Triangular(m_point[i], m_point[i + 2], m_point[i + 1], m_length);
		}

	}
	~Units()
	{

	}
	Vector3 get_normal(Vector3 position);//获取平板表面一点的法线
	INTERSECTION_TYPE is_intersected(Ray ray, float& dist);

private:
	Vector3* m_point;
	Vector3 m_normal;
	float m_length;
	int n;
	Triangular *t[20];
};

/*场景类*/
class Scene
{
public:
	Scene();
	~Scene();

	void init_scene();/*初始化场景*/
	int get_o_num();/*场景中物体数量*/
	int get_l_num();/**/
	Object* get_objects( int idx );/*取出编号为idx的物体*/
	PointLight get_light(int idx);/*取出编号为idx的光源*/

private:
	int m_obj_num;/*物体数量*/
	int m_lig_num;/*光源数量*/
	Object** m_obj;/*物体实例*/
	PointLight* m_light;/*光源*/
};

#endif /*_。SCENE_H_*/