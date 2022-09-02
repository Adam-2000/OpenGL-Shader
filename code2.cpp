//#include <gl/glut.h>
//#include<gl/gl.h>
//#include<gl/GLU.h>
//
//void SceneRendering()
//{
//	//清除颜色缓冲区，并将颜色设置为ClearColor
//	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	//渲染四边形
//	glBegin(GL_POLYGON);
//	//顶点颜色/顶点位置
//	glColor3f(1.0, 0.0, 0.0);
//	glVertex2f(-0.5, -0.5);
//	glVertex2f(-0.5, 0.5);
//	glVertex2f(0.5, 0.5);
//	glVertex2f(0.5, -0.5);
//	glEnd();
//	//强制刷新缓冲，保证绘图命令被执行
//	glFlush();
//}
//
//int main(int argc, char** argv)
//{
//	//初始化glut
//	glutInit(&argc, argv);
//
//	//设置显示模式
//	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
//
//	//设定窗口位置与大小
//	glutInitWindowPosition(100, 100);
//	glutInitWindowSize(1080, 720);
//
//	//创建窗口
//	glutCreateWindow("Renderer");
//	//回调渲染函数
//	glutDisplayFunc(SceneRendering);
//	//循环执行
//	glutMainLoop();
//}