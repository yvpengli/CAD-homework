#pragma once
#include <cstdio>

class Vertex;
class HalfEdge;
class Edge;
class Loop;
class Face;
class Solid;


// findº¯ÊýÉùÃ÷
Solid* findSolidById(Solid* solidList, int id);
Face* findFaceById(Face* faceList, int id);
HalfEdge* findPreviousHalfedge(Loop* lp, int vertexId);
Vertex* findVertexById(Vertex* vertexList, int id);
bool vertexInLoop(Loop* loop, int vertexId);
void setHalfEdgeInLoop(Loop* loop);
int setVertexId(Solid* solid);
int setFaceId(Solid* solid);
HalfEdge* findBrother(HalfEdge* he);
void printLoop(Loop* loop);
bool testVertexInLoop(Loop* lp, int vertexId);


Solid* mvfs(Solid* solidList, int solidId, int faceId, int vertexId, double x, double y, double z);
HalfEdge* mev(Solid* solidList, int solidId, int faceId, int startVertexId, int endVertexId, double x, double y, double z);
Loop* mef(Solid* solidList, int solidId, int oldFaceId, int newFaceId, int startVertexId, int endVertexId);
void kemr(Solid* solidList, int solidId, int faceId, int startVertexId, int endVertexId);
void kfmrh(Solid* solidList, int solidId, int deleteFaceId, int faceId);
void sweep(Solid* solidList, int solidId, int faceId, double dx, double dy, double dz);


class Vertex 
{
public:
	double x, y, z;
	int id;
	Vertex* next;
	// need a pointer to halfedge?
	// No, one vertex can be shared by mutiple halfedge.
	Vertex(int id, double x, double y, double z, Solid* s);

};

class HalfEdge
{
public:
	Vertex *startv;
	HalfEdge *next;
	Edge *edge;
	Loop *loop;
};

class Edge
{
public:
	HalfEdge *he1 = nullptr, *he2 = nullptr;
	Edge* next;

	~Edge() {
		delete he1;
		delete he2;
	}
};

class Loop
{
public:
	HalfEdge* ledge;
	Face* lface;
	Loop* next;
};

class Face
{
public:
	Loop* outLoop;
	Loop* inLoop;
	Solid* fsolid;
	Face* next;
	int id;
	
	Face(int id, Solid* s);

	void addLoop(Loop* loop)
	{
		loop->lface = this;
		if (outLoop == nullptr)
		{
			outLoop = loop;
		}
		else
		{
			if (inLoop == nullptr)
			{
				loop->next = loop;
				inLoop = loop;
			}
			else
			{
				loop->next = inLoop->next;
				inLoop->next = loop;
			}
		}
	}

	Loop* findLoopByVertexId(int id)
	{
		if (outLoop->ledge == nullptr)
			return outLoop;
		if (vertexInLoop(outLoop, id))
			return outLoop;

		Loop* in = inLoop;

		if (in == nullptr)
		{
			printf("error, vertex not in face");
			return nullptr;
		}

		for (int i = 0; testVertexInLoop(in, id) == false ; i++, in = in->next)
		{
			if (i != 0 && in == inLoop)
			{
				printf("error, vertex not in face");
				return nullptr;
			}
		}
		return in;
	}
};

class Solid
{
public:
	Face* faceList;
	Solid* next;
	Vertex* vertexList;
	Edge* edgeList;
	int id;
	Solid(int id)
	{
		this->id = id;
		faceList = nullptr;
		vertexList = nullptr;
		edgeList = nullptr;
	}

	void addVertex(Vertex* vertex)
	{
		if (vertexList == nullptr)
		{
			vertex->next = vertex;
			vertexList = vertex;
		}
		else
		{
			vertex->next = vertexList->next;
			vertexList->next = vertex;
		}
	}

	void addFace(Face* face)
	{
		if (faceList == nullptr)
		{
			face->next = face;
			faceList = face;
		}
		else
		{
			face->next = faceList->next;
			faceList->next = face;
		}
		face->fsolid = this;
	}

	void addEdge(Edge* edge)
	{
		if (edgeList == nullptr)
		{
			edge->next = edge;
			edgeList = edge;
		}
		else
		{
			edge->next = edgeList->next;
			edgeList->next = edge;
		}
	}
};

