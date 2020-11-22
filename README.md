课程名称：三维CAD建模

授课老师：高曙明

学生学号：22021253

学生姓名：李煜鹏

-----------

项目介绍：

项目名称为 opengl。因为是用opengl写的。

源码在opengl文件夹下，半边数据结构和欧拉操作的实现在entities.h和entities.cpp文件中，可运行文件在x64/Release文件夹中。

本项目实现了用欧拉操作构建半边数据结构，并实现了扫成操作。

程序自带3个demo，运行程序后，输入demo1或者demo2或者demo3可以分别查看这三个demo。

demo1：一个带一个亏格的长方体，是通过欧拉操作一步一步构建成的。

demo2：带4个亏格的面，即方向相反的两个面，每个面带4个内环。

demo3：由demo2做扫成操作得到的体。顺便做了一个五角星面，扫成成了体。

本项目还能自己输入欧拉操作，来构建想要的实体。在程序运行后输入build，然后输入想要的操作（函数名称+需要提供的参数），构造完成后输入end，就可以显示构造结果。下面是一个简单的例子：

build
mvfs 1 1 1 20 -20 0
mev 1 1 1 2 20 20 0
mev 1 1 2 3 -20 20 0
mev 1 1 3 4 -20 -20 0
mef 1 1 2 4 1
end

输入结束后可以得到一个正方形（含两个面，两个环）。

能完成的5个欧拉操作函数和扫成操作函数参数列表如下：

```c++
Solid* mvfs(Solid* solidList, int solidId, int faceId, int vertexId, double x, double y, double z);

HalfEdge* mev(Solid* solidList, int solidId, int faceId, int startVertexId, int endVertexId, double x, double y, double z);

Loop* mef(Solid* solidList, int solidId, int oldFaceId, int newFaceId, int startVertexId, int endVertexId);

void kemr(Solid* solidList, int solidId, int faceId, int startVertexId, int endVertexId);

void kfmrh(Solid* solidList, int solidId, int deleteFaceId, int faceId);

void sweep(Solid* solidList, int solidId, int faceId, double dx, double dy, double dz);
```

想要自行通过输入构造实体的时候需要注意：

- 点的坐标范围是[-100, 100]内的实数。
- 函数参数列表中的 solidList 是存储数据结构的指针，是不需要提供的，所以需要提供的第一个参数是 solidId。
- 构造时需要自己记住各个点和面的 ID，不然程序会报错就结束运行了。
- 构造时最好按照一定的规范，如使用 kemr 的时候，起始点 startVertexId 需要在外环上。
- 不支持一边输入一边构造，只能一次性输完。



下面提供三个demo的欧拉操作构造步骤（源码main.cpp中也有）。

demo1:

```c++
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
```

demo2:

```c++
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
```

demo3:

```c++
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
```

