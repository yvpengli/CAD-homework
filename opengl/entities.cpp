#include "entities.h"
#include <cstdio>

Vertex::Vertex(int id, double x, double y, double z, Solid* s)
{
	this->id = id;
	this->x = x;
	this->y = y;
	this->z = z;
	s->addVertex(this);
}

Face::Face(int id, Solid* s)
{
	this->id = id;
	s->addFace(this);
	outLoop = nullptr;
	inLoop = nullptr;
}


// some method to find entities
Solid* findSolidById(Solid* solidList, int id)
{
	Solid* s = solidList;
	for (int i = 0; s->id != id; i++, s = s->next)
	{
		if (i != 0 && s == solidList)
		{
			printf("error, Solid not found\n");
			return nullptr;
		}
	}
	return s;
}

Face* findFaceById(Face* faceList, int id)
{
	Face* face = faceList;
	for (int i = 0; face->id != id; i++, face = face->next)
	{
		if (i != 0 && face == faceList)
		{
			printf("error, face not found\n");
			return nullptr;
		}
	}
	return face;
}

HalfEdge* findPreviousHalfedge(Loop* lp, int vertexId)
{
	if (lp == nullptr || lp->ledge == nullptr)
	{
		printf("error, vertex not in loop\n");
		return nullptr;
	}

	HalfEdge* he = lp->ledge;
	for (int i = 0; he->next->startv->id != vertexId; he = he->next, i++)
	{
		if (i != 0 && he == lp->ledge)
		{
			printf("error!!!, vertex %d not in loop, faceid = %d\n", vertexId, lp->lface->id);
			return nullptr;
		}
	}
	return he;
}

bool testVertexInLoop(Loop* lp, int vertexId)
{
	if (lp == nullptr || lp->ledge == nullptr)
	{
		return false;
	}

	HalfEdge* he = lp->ledge;
	for (int i = 0; he->next->startv->id != vertexId; he = he->next, i++)
	{
		if (i != 0 && he == lp->ledge)
		{
			return false;
		}
	}
	return true;
}

Vertex* findVertexById(Vertex* vertexList, int id)
{
	Vertex* vertex = vertexList;
	for (int i = 0; vertex->id != id; i++, vertex = vertex->next)
	{
		if (vertex == vertexList && i != 0)
		{
			printf("error, vertex not found\n");
			return nullptr;
		}
	}
	return vertex;
}

bool vertexInLoop(Loop* loop, int vertexId)
{
	if (loop == nullptr)
		return false;
	HalfEdge* he = loop->ledge;
	for (int i = 0; he->next->startv->id != vertexId; he = he->next, i++)
	{
		if (i != 0 && he == loop->ledge)
		{
			return false;
		}
	}
	return true;
}

void setHalfEdgeInLoop(Loop* loop)
{
	HalfEdge* he = loop->ledge;
	for (int i = 0; ; i++, he = he->next)
	{
		if (i != 0 && he == loop->ledge)
			break;
		he->loop = loop;
	}
}

int setVertexId(Solid* solid)
{
	Vertex* vertex = solid->vertexList;
	int i;
	// 编号从1开始好了
	for (i = 1; ; i++, vertex = vertex->next)
	{
		if (i != 1 && vertex == solid->vertexList)
		{
			break;
		}
		vertex->id = i;
	}
	return i;
}

int setFaceId(Solid* solid)
{
	Face* face = solid->faceList;
	int i;
	// 编号从1开始好了
	for (i = 1; ; i++, face = face->next)
	{
		if (i != 1 && face == solid->faceList)
		{
			break;
		}
		face->id = i;
	}
	return i;
}

HalfEdge* findBrother(HalfEdge* he)
{
	if (he->edge->he1 == he)
		return he->edge->he2;
	else
		return he->edge->he1;
}

void printLoop(Loop* loop)
{
	HalfEdge* he = loop->ledge;
	for (int i = 0; ; i++, he = he->next)
	{
		if (i != 0 && he == loop->ledge)
			break;
		printf("%d -> ", he->startv->id);
	}
	printf("\n");
}


// Euler operation
Solid* mvfs(Solid* solidList, int solidId, int faceId, int vertexId, double x, double y, double z)
{
	Solid* s = new Solid(solidId);
	// create a face and a vertex, put them into a solid
	Face* f = new Face(faceId, s);
	Vertex* v = new Vertex(vertexId, x, y, z, s);
	Loop* lp = new Loop;

	if (solidList == nullptr)
	{
		solidList = s;
		s->next = s;
	}
	else
	{
		s->next = solidList->next;
		solidList->next = s;
	}
	// link face and loop
	f->addLoop(lp);

	// 伪代码中有关联face和solid，但是我的代码中在face的构造函数中就实现了关联

	lp->ledge = nullptr;

	return s;
}

HalfEdge* mev(Solid* solidList, int solidId, int faceId, int startVertexId, int endVertexId, double x, double y, double z)
{
	// find solid and face
	Solid* s = findSolidById(solidList, solidId);
	Face* f = findFaceById(s->faceList, faceId);

	Vertex* v1 = findVertexById(s->vertexList, startVertexId);
	Vertex* v2 = new Vertex(endVertexId, x, y, z, s);

	Loop* lp = f->findLoopByVertexId(startVertexId);
	HalfEdge* he1 = new HalfEdge;
	HalfEdge* he2 = new HalfEdge;
	Edge* eg = new Edge;

	// link halfedge and edge
	eg->he1 = he1;
	eg->he2 = he2;
	he1->edge = eg;
	he2->edge = eg;
	s->addEdge(eg);

	// link halfedge with vertex and loop
	he1->startv = v1;
	he2->startv = v2;
	he1->loop = lp;
	he2->loop = lp;

	he1->next = he2;

	// insert halfedge into loop
	if (lp->ledge == nullptr)
	{
		he2->next = he1;
		lp->ledge = he1;
	}
	else
	{
		HalfEdge* he = findPreviousHalfedge(lp, v1->id);
		he2->next = he->next;
		he->next = he1;
	}

	return he1;
}


Loop* mef(Solid* solidList, int solidId, int oldFaceId, int newFaceId, int startVertexId, int endVertexId)
{
	// 应该需要保证两个起始点和终止点在同一个外环上，才能高正拓扑有效性
	Solid* s = findSolidById(solidList, solidId);

	Face* oldFace = findFaceById(s->faceList, oldFaceId);
	Loop* oldLoop = oldFace->findLoopByVertexId(startVertexId);

	Face* newFace = new Face(newFaceId, s);
	Loop* newLoop = new Loop();
	newLoop->lface = newFace;
	newFace->addLoop(newLoop);

	HalfEdge* he1 = new HalfEdge;
	HalfEdge* he2 = new HalfEdge;
	Vertex* v1 = findVertexById(s->vertexList, startVertexId);
	Vertex* v2 = findVertexById(s->vertexList, endVertexId);
	Edge* edge = new Edge;

	edge->he1 = he1;
	edge->he2 = he2;
	he1->edge = edge;
	he2->edge = edge;
	s->addEdge(edge);

	he1->startv = v1;
	he2->startv = v2;

	HalfEdge* pre1 = findPreviousHalfedge(oldLoop, startVertexId);
	HalfEdge* pre2 = findPreviousHalfedge(oldLoop, endVertexId);

	// 排除掉创建内环时的特殊情况
	if (findBrother(pre2->next)->next->startv == v2)
		pre2 = findBrother(pre2->next);

	// 重构loop
	he2->next = pre1->next;
	pre1->next = he1;
	he1->next = pre2->next;
	pre2->next = he2;

	oldLoop->ledge = he1;
	newLoop->ledge = he2;

	setHalfEdgeInLoop(newLoop);

	return oldLoop;
}

void kemr(Solid* solidList, int solidId, int faceId, int startVertexId, int endVertexId)
{
	// startVertex一定要在外环上
	Solid* s = findSolidById(solidList, solidId);

	Face* face = findFaceById(s->faceList, faceId);
	Loop* loop = face->outLoop;
	Loop* newLoop = new Loop;
	//printLoop(loop);

	Vertex* v1 = findVertexById(s->vertexList, startVertexId);
	Vertex* v2 = findVertexById(s->vertexList, endVertexId);

	HalfEdge* pre1 = findPreviousHalfedge(loop, startVertexId);
	while (pre1->startv->id == endVertexId)
	{
		loop->ledge = pre1->next;
		pre1 = findPreviousHalfedge(loop, startVertexId);
	}

	HalfEdge* pre2 = findPreviousHalfedge(loop, endVertexId);
	while (pre2->startv->id == startVertexId)
	{
		loop->ledge = pre2->next;
		pre2 = findPreviousHalfedge(loop, endVertexId);
	}
	
	//printf("pre1->startv = %d \n pre2->startv = %d\n", pre1->startv->id, pre2->startv->id);

	HalfEdge* he1 = pre1->next;
	HalfEdge* he2 = pre2->next;
	//printf("he1->startv = %d \n he2->startv = %d\n", he1->startv->id, he2->startv->id);
	//printf("he1's brother = %d\n", findBrother(he1)->startv->id);
	// 重新构建环
	pre1->next = he2->next;
	pre2->next = he1->next;

	// 把新的环放入面中
	loop->ledge = pre1;
	newLoop->ledge = pre2;
	setHalfEdgeInLoop(newLoop);
	face->addLoop(newLoop);

	// 删除边，然后把solid中边的链表也重新整理一下，edge的析构函数中会顺便把he1,he2的空间也释放掉
	Edge* tmpEdge, *edge = he1->edge;
	for (tmpEdge = s->edgeList; tmpEdge->next != edge; tmpEdge = tmpEdge->next);
	tmpEdge->next = he1->edge->next;
	
	s->edgeList = tmpEdge;
	//for (int i = 0; ; i++, tmpEdge = tmpEdge->next)
	//{
	//	if (i != 0 && tmpEdge == s->edgeList)
	//		break;
	//	printf("%d -> %d\n", tmpEdge->he1->startv->id, tmpEdge->he2->startv->id);
	//}

	delete edge;
}

void kfmrh(Solid* solidList, int solidId, int deleteFaceId, int faceId)
{
	Solid* s = findSolidById(solidList, solidId);

	Face* deleteFace = findFaceById(s->faceList, deleteFaceId);
	Face* face = findFaceById(s->faceList, faceId);

	// 把删除的面的外环变为face的内环
	face->addLoop(deleteFace->outLoop);

	// 把solid中的faceList中的要删除的面删掉
	Face* pre;
	for (pre = s->faceList; pre->next != deleteFace; pre = pre->next);
	pre->next = deleteFace->next;

	delete deleteFace;

}

void sweep(Solid* solidList, int solidId, int faceId, double dx, double dy, double dz)
{
	// 传进来一个物理面
	Solid* s = findSolidById(solidList, solidId);
	int vertexNum = setVertexId(s);
	int faceNum = setFaceId(s);
	Face* face = findFaceById(s->faceList, faceId);

	Loop* loop = nullptr;
	bool firstOut = true, firstIn = true;
	while (true)
	{
		// 遍历face中的环
		if (firstOut == true)
		{
			loop = face->outLoop;
			firstOut = false;
		}
		else
		{
			if (face->inLoop == nullptr)
			{
				break;
			}
			else if (firstIn == true)
			{
				loop = face->inLoop;
				firstIn = false;
			}
			else
			{
				loop = loop->next;
				if (loop == face->inLoop)
					break;
			}
		}
		// loop作为基本的loop，将faceId设置为2
		//faceId = 2;

		Vertex* firstVertex = loop->ledge->startv;
		//printf("firstVertex: %f, %f, %f\n", firstVertex->x, firstVertex->y, firstVertex->z);
		HalfEdge* heInBaseLoop = loop->ledge;
		// mev的返回是he1

		heInBaseLoop = heInBaseLoop->next;
		HalfEdge* he1 = mev(solidList, solidId, faceId, firstVertex->id, vertexNum++, firstVertex->x + dx, firstVertex->y + dy, firstVertex->z + dz);
		Vertex* firstUp = he1->next->startv;

		Vertex* preUp = firstUp;
		Vertex* next = heInBaseLoop->startv;
		Vertex* up = nullptr;

		while (next != firstVertex)
		{
			heInBaseLoop = heInBaseLoop->next;
			he1 = mev(solidList, solidId, faceId, next->id, vertexNum++, next->x + dx, next->y + dy, next->z + dz);
			up = he1->next->startv;

			// mef返回的是oldLoop,其实这一步后faceId的值应该还是2
			faceId = mef(solidList, solidId, faceId, faceNum++, preUp->id, up->id)->lface->id;

			preUp = up;
			// 选择loop上的下一个点
			next = heInBaseLoop->startv;
		}

		mef(solidList, solidId, faceId, faceNum++, preUp->id, firstUp->id);

	}
}
