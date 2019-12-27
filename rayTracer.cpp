/*************************************************
  Copyright(C)2011
  ��Ŀ���������߸���
  �ļ�����ray.h
  ���ߣ�wy ���ڣ�
  �ļ����������߸�������������������
  �����б�
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


/*���ٺ��������������е��������壬���ؽ��㴦����ɫ*/
Vector3 Tracer::trace(Ray ray,int depth)
{
	float distance = INFINITY;/*��ʼ�����޴����*/
	int dep = depth;/*�ݹ����*/
	Object* aim;/*��������*/
	Vector3 point;/*����*/
	Vector3 n;/*���㴦�ķ�����*/
	Vector3 l;/*���㵽��Դ�ķ���*/
	Vector3 v;/*���㵽�۾��ķ���*/
	Vector3 r;/*������l����������ڽ��㴦�ĳ��䷽��*/
	Color hit_color;/*���㴦����ɫ*/
	int res_hit;/*�ཻ������*/

	/*���������е�ÿһ������,��ȡ���е�һ�����۾�����ĵ���������*/
	for(int k=0;k<m_scn->get_o_num();k++)
	{
		Object* obj = m_scn->get_objects(k);/*��ǰ����*/
		int res;
		if(res = obj->is_intersected(ray,distance))/*�ж��Ƿ��н��㣬�н���ı�distance*/
		{
			aim = obj;/*��ȡĿ������*/
			res_hit = res;
		}
	}
	
	/*���ཻ����*/
	if(distance != INFINITY)
	{
		point = ray.get_point(distance);/*��ȡ����*/

		//std::cout<<"here"<<std::endl;
		n = aim->get_normal(point);/*���㴦�ķ�����*/
		n.normalize();/*��λ��*/

		/*���������������ཻ*/
		if(res_hit == INTERSECTED_IN)
		{
			n = -n;
		}

		v = m_eye - point;/*���㵽�۾��ķ���*/
		v.normalize();

		/*���������е�ÿһ����Դ*/
		for(int k = 0; k < m_scn->get_l_num(); k++)
		{
			PointLight pl = m_scn->get_light(k);/*��Դ*/
			l = pl.get_pst() - point;/*���㵽��Դ�ķ���*/
			l.normalize();
			Ray l_ray = Ray(point + l*SMALL, l);/**/

			float shade = 1.0f;/*��Ӱ,1.0��ʾ���㲻����Ӱ��*/
			float distance = INFINITY;/*��ʼ�����޴����*/

			/*�����Ƿ��������ڵ�ס��Դ*/
			for(int k=0;k<m_scn->get_o_num();k++)
			{
				Object* obj = m_scn->get_objects(k);/*��ǰ����*/
				if(obj->is_intersected(l_ray,distance))
				{
					shade = 0.0f;
					break;
				}
			}
			
			/*������*/
			if(aim->get_diffuse() > 0)
			{
				float cos = DOT(l,n);/*�����뽻�㴦���ߵļнǵ�����*/
				if(cos > 0)
				{
					float diffuse = cos*aim->get_diffuse()*shade;
					/*���������ӵ�������ɫ��*/
					hit_color = hit_color + diffuse*pl.get_color()*aim->get_color();
				}
			}

			/*���淴��*/
			if(aim->get_spec() > 0)
			{
				/*Phong�߹�*/
				Vector3 h = 2*DOT(n,l)*n - l;/*�������*/
				float cos = DOT(h,v);/*������������߷���ļн�����*/
				if(cos > 0)
				{
					float specular = powf(cos,20)*aim->get_spec()*shade;
					hit_color = hit_color + specular*pl.get_color();
				}
			}
		}

		/*�������*/
		if(aim->get_refl() > 0 && dep < TOTALDEPTH)
		{
			Vector3 refl = 2*DOT(n,l)*n - l;/*�������*/
			hit_color = hit_color + trace(Ray(point + refl*0.0001f, refl), ++dep);
		}
	}
	return hit_color;/*û���ཻʱ���ǳ�ʼ��ɫ�����ཻʱ���ǽ��㴦����ɫ*/
}

/*���ƣ���ȡ��ɫ��Ϣ*/
void Tracer::render()
{
	m_eye.set(0.0f,0.0f,4.0f);/*�۾�λ��*/
	m_scn->init_scene();/*���볡��*/

	/*�Ӵ���������*/
	float dx = (float)(VIEW_WIDTH/VIEW_PIXEL_WIDTH);
	float dy = (float)(VIEW_HEIGHT/VIEW_PIXEL_HEIGHT);

	/*�Ӵ���ʼ����*/
	float view_x = - VIEW_WIDTH/2.0;
	float view_y = - VIEW_HEIGHT/2.0;
	
	/*�����������ص�*/
	for(int i=0; i<VIEW_PIXEL_HEIGHT; i++)
	{
		view_x = - VIEW_WIDTH/2.0;
		for(int j=0; j<VIEW_PIXEL_WIDTH; j++)
		{
		   Vector3 dir = Vector3(view_x,view_y,0) - m_eye;/*���߷���*/
		   dir.normalize();
		   Ray ray(m_eye, dir);/*����*/
		   screen_color[i][j] = trace(ray,1);/*���������е���������*/
		   view_x += dx;
		}
		view_y += dy;
	}
}
