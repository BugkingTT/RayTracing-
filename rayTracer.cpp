/*************************************************
  Copyright(C)2011
  项目描述：光线跟踪
  文件名：ray.h
  作者：wy 日期：
  文件描述：光线跟踪器，其中有射线类
  函数列表：
**************************************************/

#include "rayTracer.h"
#include "common.h"
#include <iostream>

Tracer::Tracer()
{	
	m_scn = new Scene();
}

Tracer::~Tracer()
{
	delete m_scn;
 }


/*跟踪函数，遍历场景中的所有物体，返回交点处的颜色*/
Vector3 Tracer::trace(Ray ray,int depth)
{
	float distance = INFINITY;/*初始化无限大距离*/
	int dep = depth;/*递归深度*/
	Object* aim;/*交点物体*/
	Vector3 point;/*交点*/
	Vector3 n;/*交点处的法向量*/
	Vector3 l;/*交点到光源的方向*/
	Vector3 v;/*交点到眼睛的方向*/
	Vector3 r;/*光线沿l的入射光线在交点处的出射方向*/
	Color hit_color;/*交点处的颜色*/
	int res_hit;/*相交情况结果*/

	/*遍历场景中的每一个物体,获取其中的一个离眼睛最近的点所在物体*/
	for(int k=0;k<m_scn->get_o_num();k++)
	{
		Object* obj = m_scn->get_objects(k);/*当前物体*/
		int res;
		if(res = obj->is_intersected(ray,distance))/*判断是否有交点，有交点改变distance*/
		{
			aim = obj;/*获取目标物体*/
			res_hit = res;
		}
	}
	
	/*有相交物体*/
	if(distance != INFINITY)
	{
		point = ray.get_point(distance);/*获取交点*/

		//std::cout<<"here"<<std::endl;
		n = aim->get_normal(point);/*交点处的法向量*/
		n.normalize();/*单位化*/

		/*在物体内与物体相交*/
		if(res_hit == INTERSECTED_IN)
		{
			n = -n;
		}

		v = m_eye - point;/*交点到眼睛的方向*/
		v.normalize();

		/*遍历场景中的每一个光源*/
		for(int k = 0; k < m_scn->get_l_num(); k++)
		{
			PointLight pl = m_scn->get_light(k);/*光源*/
			l = pl.get_pst() - point;/*交点到光源的方向*/
			l.normalize();
			Ray l_ray = Ray(point + l*SMALL, l);/**/

			float shade = 1.0f;/*阴影,1.0表示交点不在阴影中*/
			float distance = INFINITY;/*初始化无限大距离*/

			/*测试是否有物体遮挡住光源*/
			for(int k=0;k<m_scn->get_o_num();k++)
			{
				Object* obj = m_scn->get_objects(k);/*当前物体*/
				if(obj->is_intersected(l_ray,distance))
				{
					shade = 0.0f;
					break;
				}
			}
			
			/*漫反射*/
			if(aim->get_diffuse() > 0)
			{
				float cos = DOT(l,n);/*光线与交点处法线的夹角的余弦*/
				if(cos > 0)
				{
					float diffuse = cos*aim->get_diffuse()*shade;
					/*将漫反射光加到光线颜色中*/
					hit_color = hit_color + diffuse*pl.get_color()*aim->get_color();
				}
			}

			/*镜面反射*/
			if(aim->get_spec() > 0)
			{
				/*Phong高光*/
				Vector3 h = 2*DOT(n,l)*n - l;/*反射光线*/
				float cos = DOT(h,v);/*反射光线与视线方向的夹角余弦*/
				if(cos > 0)
				{
					float specular = powf(cos,20)*aim->get_spec()*shade;
					hit_color = hit_color + specular*pl.get_color();
				}
			}
		}

		/*反射光线*/
		if(aim->get_refl() > 0 && dep < TOTALDEPTH)
		{
			Vector3 refl = 2*DOT(n,l)*n - l;/*反射光线*/
			hit_color = hit_color + trace(Ray(point + refl*0.0001f, refl), ++dep);
		}
	}
	return hit_color;/*没有相交时，是初始黑色；有相交时，是交点处的颜色*/
}

/*绘制，获取颜色信息*/
void Tracer::render()
{
	m_eye.set(0.0f,0.0f,4.0f);/*眼睛位置*/
	m_scn->init_scene();/*读入场景*/

	/*视窗坐标增量*/
	float dx = (float)(VIEW_WIDTH/VIEW_PIXEL_WIDTH);
	float dy = (float)(VIEW_HEIGHT/VIEW_PIXEL_HEIGHT);

	/*视窗初始坐标*/
	float view_x = - VIEW_WIDTH/2.0;
	float view_y = - VIEW_HEIGHT/2.0;
	
	/*遍历所有像素点*/
	for(int i=0; i<VIEW_PIXEL_HEIGHT; i++)
	{
		view_x = - VIEW_WIDTH/2.0;
		for(int j=0; j<VIEW_PIXEL_WIDTH; j++)
		{
		   Vector3 dir = Vector3(view_x,view_y,0) - m_eye;/*射线方向*/
		   dir.normalize();
		   Ray ray(m_eye, dir);/*射线*/
		   screen_color[i][j] = trace(ray,1);/*遍历场景中的所有物体*/
		   view_x += dx;
		}
		view_y += dy;
	}
}
