#include <iostream>
#include <GL/freeglut.h>
#include "entities.h"
#include <cmath>
#include <string>

const float PI = 3.14159;
int screenWidth = 800;
int screenHeight = 800;
Solid* solidList = nullptr;

void drawLine(float* p, float* q) {
    glBegin(GL_LINE_STRIP);
    glVertex3fv(p);
    glVertex3fv(q);
    glEnd();
    glPointSize(5.f);
    glBegin(GL_POINTS);
    glVertex3fv(p);
    glVertex3fv(q);
    glEnd();
}

void render() {
    // *****绘制过程*******

    Solid* solid = solidList;
    for (int i = 0; solidList != nullptr ; i++, solid = solid->next)
    {
        // solid == solidList 表示遍历完成
        if (solid == solidList && i != 0)
            break;

        // 遍历solid中的边
        Edge* edge = solid->edgeList;
        for (int j = 0; ; j++, edge = edge->next)
        {
            if (edge == solid->edgeList && j != 0)
                break;

            if (edge == nullptr)
                break;
            Vertex* v1 = edge->he1->startv;
            Vertex* v2 = edge->he2->startv;
            float point1[3] = { (float)v1->x / 100, (float)v1->y / 100, (float)v1->z / 100 };
            float point2[3] = { (float)v2->x / 100, (float)v2->y / 100, (float)v2->z / 100 };

            drawLine(point1, point2);

        }
    }
}

void display(void)
{
    glClearColor(0.2, 0.2, 0.2, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glRotatef(1, 0.4, 0.2, 0.3);
    render();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    screenWidth = w;
    screenHeight = h; glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w,
            2.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
    else
        glOrtho(-2.0 * (GLfloat)w / (GLfloat)h,
            2.0 * (GLfloat)w / (GLfloat)h, -2.0, 2.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

void demo1()
{
    solidList = mvfs(solidList, 1, 1, 1, 20, -20, 0);
    mev(solidList, 1, 1, 1, 2, 20, 20, 0);
    mev(solidList, 1, 1, 2, 3, -20, 20, 0);
    mev(solidList, 1, 1, 3, 4, -20, -20, 0);
    mef(solidList, 1, 1, 2, 4, 1);

    mev(solidList, 1, 2, 1, 5, 20, -20, 50);
    mev(solidList, 1, 2, 2, 6, 20, 20, 50);
    mef(solidList, 1, 2, 3, 6, 5);

    mev(solidList, 1, 2, 3, 7, -20, 20, 50);
    mef(solidList, 1, 2, 4, 7, 6);
    mev(solidList, 1, 2, 4, 8, -20, -20, 50);
    mef(solidList, 1, 2, 5, 8, 7);
    mef(solidList, 1, 2, 6, 8, 5);

    mev(solidList, 1, 1, 1, 9, 10, -10, 0);
    mev(solidList, 1, 1, 9, 10, 10, 10, 0);
    mev(solidList, 1, 1, 10, 11, -10, 10, 0);
    mev(solidList, 1, 1, 11, 12, -10, -10, 0);
    mef(solidList, 1, 1, 7, 12, 9);
    kemr(solidList, 1, 1, 1, 9);

    mev(solidList, 1, 7, 9, 13, 10, -10, 50);
    mev(solidList, 1, 7, 10, 14, 10, 10, 50);
    mef(solidList, 1, 7, 8, 14, 13);

    mev(solidList, 1, 7, 11, 15, -10, 10, 50);
    mef(solidList, 1, 7, 9, 15, 14);
    mev(solidList, 1, 7, 12, 16, -10, -10, 50);
    mef(solidList, 1, 7, 10, 16, 15);
    mef(solidList, 1, 7, 11, 16, 13);

    kfmrh(solidList, 1, 7, 2);
}

void demo2()
{
    solidList = mvfs(solidList, 1, 1, 1, 50, -50, 0);
    mev(solidList, 1, 1, 1, 2, 50, 50, 0);
    mev(solidList, 1, 1, 2, 3, -50, 50, 0);
    mev(solidList, 1, 1, 3, 4, -50, -50, 0);
    mef(solidList, 1, 1, 2, 4, 1);


    mev(solidList, 1, 1, 1, 5, 40, -40, 0);
    mev(solidList, 1, 1, 5, 6, 40, -10, 0);
    mev(solidList, 1, 1, 6, 7, 10, -10, 0);
    mev(solidList, 1, 1, 7, 8, 10, -40, 0);
    mef(solidList, 1, 1, 3, 8, 5);
    kemr(solidList, 1, 1, 1, 5);
    kfmrh(solidList, 1, 3, 2);

    mev(solidList, 1, 1, 2, 10, 40, 40, 0);
    mev(solidList, 1, 1, 10, 11, 10, 40, 0);
    mev(solidList, 1, 1, 11, 12, 10, 10, 0);
    mev(solidList, 1, 1, 12, 9, 40, 10, 0);
    mef(solidList, 1, 1, 3, 9, 10);
    kemr(solidList, 1, 1, 2, 10);
    kfmrh(solidList, 1, 3, 2);

    mev(solidList, 1, 1, 3, 15, -40, 40, 0);
    mev(solidList, 1, 1, 15, 16, -40, 10, 0);
    mev(solidList, 1, 1, 16, 13, -10, 10, 0);
    mev(solidList, 1, 1, 13, 14, -10, 40, 0);
    mef(solidList, 1, 1, 3, 14, 15);
    kemr(solidList, 1, 1, 3, 15);
    kfmrh(solidList, 1, 3, 2);

    mev(solidList, 1, 1, 4, 20, -40, -40, 0);
    mev(solidList, 1, 1, 20, 17, -10, -40, 0);
    mev(solidList, 1, 1, 17, 18, -10, -10, 0);
    mev(solidList, 1, 1, 18, 19, -40, -10, 0);
    mef(solidList, 1, 1, 3, 19, 20);
    kemr(solidList, 1, 1, 4, 20);
    kfmrh(solidList, 1, 3, 2);
}

void demo3()
{
    solidList = mvfs(solidList, 1, 1, 1, 50, -50, 0);
    mev(solidList, 1, 1, 1, 2, 50, 50, 0);
    mev(solidList, 1, 1, 2, 3, -50, 50, 0);
    mev(solidList, 1, 1, 3, 4, -50, -50, 0);
    mef(solidList, 1, 1, 2, 4, 1);


    mev(solidList, 1, 1, 1, 5, 40, -40, 0);
    mev(solidList, 1, 1, 5, 6, 40, -10, 0);
    mev(solidList, 1, 1, 6, 7, 10, -10, 0);
    mev(solidList, 1, 1, 7, 8, 10, -40, 0);
    mef(solidList, 1, 1, 3, 8, 5);
    kemr(solidList, 1, 1, 1, 5);
    kfmrh(solidList, 1, 3, 2);

    mev(solidList, 1, 1, 2, 10, 40, 40, 0);
    mev(solidList, 1, 1, 10, 11, 10, 40, 0);
    mev(solidList, 1, 1, 11, 12, 10, 10, 0);
    mev(solidList, 1, 1, 12, 9, 40, 10, 0);
    mef(solidList, 1, 1, 3, 9, 10);
    kemr(solidList, 1, 1, 2, 10);
    kfmrh(solidList, 1, 3, 2);

    mev(solidList, 1, 1, 3, 15, -40, 40, 0);
    mev(solidList, 1, 1, 15, 16, -40, 10, 0);
    mev(solidList, 1, 1, 16, 13, -10, 10, 0);
    mev(solidList, 1, 1, 13, 14, -10, 40, 0);
    mef(solidList, 1, 1, 3, 14, 15);
    kemr(solidList, 1, 1, 3, 15);
    kfmrh(solidList, 1, 3, 2);

    mev(solidList, 1, 1, 4, 20, -40, -40, 0);
    mev(solidList, 1, 1, 20, 17, -10, -40, 0);
    mev(solidList, 1, 1, 17, 18, -10, -10, 0);
    mev(solidList, 1, 1, 18, 19, -40, -10, 0);
    mef(solidList, 1, 1, 3, 19, 20);
    kemr(solidList, 1, 1, 4, 20);
    kfmrh(solidList, 1, 3, 2);

    sweep(solidList, 1, 2, 0, 0, 50);

    // draw a star
    int k = 0;
    double R = 30.0;
    double rad = 3.1415926 / 180;
    double r = R * sin(18 * rad) / cos(36 * rad);
    double x = 100, y = 100, z = 100;  // x.y.z记录星星的中心点
    solidList = mvfs(solidList, 2, 1, 1, x - R * cos((90 + k * 72) * rad), y - R * sin((90 + k * 72) * rad), 0);
    mev(solidList, 2, 1, 1, 2, x - r * cos((90 + 36 + k * 72) * rad), y - r * sin((90 + 36 + k * 72) * rad), 0);

    for (k = 1; k < 5; k++)
    {
        mev(solidList, 2, 1, 2 * k, 2 * k + 1, x - R * cos((90 + k * 72) * rad), y - R * sin((90 + k * 72) * rad), 0);
        mev(solidList, 2, 1, 2 * k + 1, 2 * k + 2, x - r * cos((90 + 36 + k * 72) * rad), y - r * sin((90 + 36 + k * 72) * rad), 0);
    }

    mef(solidList, 2, 1, 2, 10, 1);
    sweep(solidList, 2, 2, 0, 0, 20);

}

void input()
{
    std::string op;
    std::cin >> op;
    while (op != "end")
    {
        int solidId, faceId, vertexId;
        double x, y, z;
        int oldFaceId, newFaceId, deleteFaceId;
        int startVertexId, endVertexId;

        if (op == "mvfs")
        {
            std::cin >> solidId >> faceId >> vertexId >> x >> y >> z;
            solidList = mvfs(solidList, solidId, faceId, vertexId, x, y, z);
        }
        else if (op == "mev")
        {
            std::cin >> solidId >> faceId >> startVertexId >> endVertexId >> x >> y >> z;
            mev(solidList, solidId, faceId, startVertexId, endVertexId, x, y, z);
        }
        else if (op == "mef")
        {
            std::cin >> solidId >> oldFaceId >> newFaceId >> startVertexId >> endVertexId;
            mef(solidList, solidId, oldFaceId, newFaceId, startVertexId, endVertexId);
        }
        else if (op == "kemr")
        {
            std::cin >> solidId >> faceId >> startVertexId >> endVertexId;
            kemr(solidList, solidId, faceId, startVertexId, endVertexId);
        }
        else if (op == "kfmrh")
        {
            std::cin >> solidId >> deleteFaceId >> faceId;
            kfmrh(solidList, solidId, deleteFaceId, faceId);
        }
        else if (op == "sweep")
        {
            std::cin >> solidId >> faceId >> x >> y >> z;
            sweep(solidList, solidId, faceId, x, y, z);
        }
        else
        {
            printf("error, unlegal input\n");
            break;
        }

        std::cin >> op;
    }
}



int main(int argc, char **argv) {

    // ******使用欧拉操作构建一个图形*******
    // 输入点的函数坐标范围是[-100, 100]，

    std::string wantToDo;

    std::cin >> wantToDo;

    if (wantToDo == "demo1")
        demo1();
    else if (wantToDo == "demo2")
        demo2();
    else if (wantToDo == "demo3")
        demo3();
    else if (wantToDo == "build")
        input();

    glutInit(&argc, (char**)argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Hello OpenGL lyp");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(display);

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
    glutMainLoop();//enters the GLUT event processing loop.

    
    return 0;
}





