//#include <gl/glut.h>
//#include<gl/gl.h>
//#include<gl/GLU.h>
//
//void SceneRendering()
//{
//	//�����ɫ��������������ɫ����ΪClearColor
//	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	//��Ⱦ�ı���
//	glBegin(GL_POLYGON);
//	//������ɫ/����λ��
//	glColor3f(1.0, 0.0, 0.0);
//	glVertex2f(-0.5, -0.5);
//	glVertex2f(-0.5, 0.5);
//	glVertex2f(0.5, 0.5);
//	glVertex2f(0.5, -0.5);
//	glEnd();
//	//ǿ��ˢ�»��壬��֤��ͼ���ִ��
//	glFlush();
//}
//
//int main(int argc, char** argv)
//{
//	//��ʼ��glut
//	glutInit(&argc, argv);
//
//	//������ʾģʽ
//	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
//
//	//�趨����λ�����С
//	glutInitWindowPosition(100, 100);
//	glutInitWindowSize(1080, 720);
//
//	//��������
//	glutCreateWindow("Renderer");
//	//�ص���Ⱦ����
//	glutDisplayFunc(SceneRendering);
//	//ѭ��ִ��
//	glutMainLoop();
//}