/*
*  DelayTrNet.h
*
*  Create on : 2013-8-30
*     Author : �����=sb,�I(^��^)�J
*    Version : 1.0
*/

/*
*  DelayTrNet.h
*
*  Modify on : 2013-9-13
*     Author : �����=sb���I(^��^)�J
*/

/*
*  DelayTrNet.h
*
*  Modify on : 2013-12-25
* ����Ŀ����ΪSpTurnPro
*     Author : �����=sb���I(^��^)�J
*/

#ifndef DELAYTRNET_H_
#define DELAYTRNET_H_

#include <list>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class ENode{

private:
	int adjvex;  //ENode #
	int indexOfAdjNode;
	double distance[2];  //d[0]Ϊ�ýڵ����ͷ��Ȩ�أ�d[1]Ϊ�������һ��ʱ�̣�Ȩ�صı仯�ʣ�weight of head to the adjvex
	int roadGrade; // ��·�ȼ�
	double *data[2];   //data[0]Ϊ���������ָ�룬data[1]Ϊ����ֵ�ñ仯�ʵ�ָ�룬pointer delayArray
	ENode *nextarc;   //pointer next ENode
	friend class LinkedGraph;

public:
	ENode();
	//Override construct function
	ENode(const int vertex, const double &weight, int grade, ENode *nextarc, const double *delay,int m);
	int getVertex();
	ENode* getNextArc();
	int getRoadGrade();
	int getIndexOfAdjNode();
	void setIndexOfAdjNode(int index);
	void setNextArc(ENode * nextarc_para);
	double getDistance();
	double getDistanceRate();
	double *getDelays();
	double *getDelaysRate();
	void setDistanceRate(double val);//�޸�·�γ��ȵı仯��
	void setDelaysRate(int adjNum, double val);//�����е�ת������ı仯����Ϊval��
	void setDelayRate(int j, double val);//��ת��j·�ڵ�ת������ı仯����Ϊval��
};

class Coordinate{
public:
	double longitude;
	double latitude;
	friend class LinkedGraph;
	Coordinate(double lon = 0,double lat = 0);
	~Coordinate(){};
};

class FigheadArray{//�ڽ������ͷ��
public:
	/*class Coordinate{
	public:
		double longitude;
		double latitude;
		friend class LinkedGraph;
		Coordinate(double lon,double lat);
		~Coordinate();
	};*/
	FigheadArray();
	FigheadArray(int nodeIndex_para,double lon_para,double lat_para,int adjvexNum_para,vector<int> pv_para);
	//ENode * getHeadArray();
	Coordinate getLocation();
	int getAdjvexNum();
	int getNodeIndex();
	void setNodeIndex(int nodeIndex_para);
	ENode * getFirstAdjNode();
	void setFirstAdjNode(ENode *firstAdjNode_para);
	vector<int> getParentVertex();
	void setParentVertex(vector<int> pv_para);
private:
	int nodeIndex;
	Coordinate location;
	int adjvexNum;//��ͷ�ڵ���ڽӽڵ�ĸ�����
	ENode *firstAdjNode;
	vector<int> parentVertex;
	friend class LinkedGraph;
};

class LinkedGraph{

public:
	LinkedGraph(string &file_name);
	void loadVarCostDelay(string &file_name);
	~LinkedGraph();
	int getTotalNode()const;
	bool isExist(int u,int v)const;   //if edge(i,j) is exist,return pointer of v
	double getDelay(int i,int j ,int k)const;//get delay from edge(i,j) to vertex k
	double getDelayRate(int i, int j, int k)const;
	double getDistance(int i, int j)const;//get value of edge(i,j)
	double getDistanceRate(int i, int j)const;
	void setDistanceRate(int i, int j, double val);
	void setDelaysRate(int i, int j, double val);
	void resetDistanceDelayRate();
	vector<int> findChildNode(int u)const;//find child ENodes of ENode u
	ENode* findChildNode(int u,int v)const;//if edge(i,j) is exist,return pointer of v
	//void output();
	vector<int> findParentNode(int u)const;
	int getParentNumber(int u)const;
	FigheadArray getFighead(int u)const;
	bool isNodeOutBound(const int u);

private:
	int totalNode;
	int arcsLoaded;
	FigheadArray *fighead;
	ENode * getHeadArray(int u);
	void inputGraph(ifstream& infile);//Ϊ�洢·�������庯��������add����
	void inputDynamicGraph(ifstream& infile);//Ϊcost��delay�仯�ʴ洢�����庯����
	//bool addNode2Fighead(int u, int v,const double w, const int grade, const double *delay,int m);//add a ENode,return ture if success
	//ENode *current;

};


#endif