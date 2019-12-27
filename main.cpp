/*************************************************
  Copyright(C)2011
  项目描述：光线跟踪
  文件名：main.cpp
  作者：wy 日期：
  文件描述：主函数
  函数列表：
**************************************************/

#include "GL/glut.h"
#include "common.h"
#include "rayTracer.h"
#include <iostream>

Tracer tc;

/*显示函数*/
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	float dx = 2.0/VIEW_PIXEL_WIDTH;/*增量*/
	float dy = 2.0/VIEW_PIXEL_HEIGHT;
	float x = -1.0;/*实际初始位置*/
	float y = -1.0;

	glBegin(GL_POINTS);
		for(int i=0;i<VIEW_PIXEL_HEIGHT;i++)
		{
			x = -1.0f;
			for(int j=0;j<VIEW_PIXEL_WIDTH;j++)
			{
				glColor3f(tc.screen_color[i][j].x,tc.screen_color[i][j].y,tc.screen_color[i][j].z);
				glVertex2f(x,y);
				x += dx;
			}
			y += dy;
		}
	glEnd();
	glFlush ();
}

/*重绘函数*/
void reshape(int w, int h)
{
   glViewport(0, 0, w, h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(28.0, w/h, 1.0, 10.0);/*视角，实际窗口的纵横比即x/y，近截面，远截面*/
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);/*眼睛，物体，向上方向*/
}

/*初始化函数*/
void init () 
{
	glClearColor(1,1,1,0);/*背景色，白色*/
	tc.render();/*绘制*/
}

/*主函数*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (500,500); 
	glutInitWindowPosition (300, 300);
	glutCreateWindow ("RayTracing");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
