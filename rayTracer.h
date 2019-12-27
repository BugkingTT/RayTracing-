/*************************************************
  Copyright(C)2011
  ��Ŀ���������߸���
  �ļ�����ray.h
  ���ߣ�wy ���ڣ�
  �ļ����������߸�������������������
  �����б�
**************************************************/

#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include "common.h"
#include "scene.h"

//���߸�����
class Tracer
{
public:
	Tracer();
	~Tracer();
	
	//���ٺ��������������е��������壬���ؽ��㴦����ɫ
	Vector3 trace(Ray ray,int depth);
	void render();//����

//protected:
	Scene* m_scn;//����
	Vector3 m_eye;//�۾�λ��	
	Color screen_color[VIEW_PIXEL_WIDTH][VIEW_PIXEL_HEIGHT];//�洢��Ļ�����ص㣨x,y��������ɫ
};

#endif /*_RAYTRACER_H_*/