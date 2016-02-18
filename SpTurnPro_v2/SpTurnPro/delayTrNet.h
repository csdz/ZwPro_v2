/*
*  DelayTrNet.h
*
*  Create on : 2013-8-30
*     Author : 王绍楠=sb,↖(^ω^)↗
*    Version : 1.0
*/

/*
*  DelayTrNet.h
*
*  Modify on : 2013-9-13
*     Author : 王绍楠=sb，↖(^ω^)↗
*/

/*
*  DelayTrNet.h
*
*  Modify on : 2013-12-25
* 将项目更名为SpTurnPro
*     Author : 王绍楠=sb，↖(^ω^)↗
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
	double distance[2];  //d[0]为该节点与表头的权重，d[1]为相比于上一个时刻，权重的变化率，weight of head to the adjvex
	int roadGrade; // 道路等级
	double *data[2];   //data[0]为延误数组的指针，data[1]为延误值得变化率的指针，pointer delayArray
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
	void setDistanceRate(double val);//修改路段长度的变化率
	void setDelaysRate(int adjNum, double val);//将所有的转弯延误的变化率设为val；
	void setDelayRate(int j, double val);//将转向j路口的转弯延误的变化率设为val；
};

class Coordinate{
public:
	double longitude;
	double latitude;
	friend class LinkedGraph;
	Coordinate(double lon = 0,double lat = 0);
	~Coordinate(){};
};

class FigheadArray{//邻接链表表头类
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
	int adjvexNum;//表头节点的邻接节点的个数；
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
	void inputGraph(ifstream& infile);//为存储路网的主体函数，包含add函数
	void inputDynamicGraph(ifstream& infile);//为cost和delay变化率存储的主体函数。
	//bool addNode2Fighead(int u, int v,const double w, const int grade, const double *delay,int m);//add a ENode,return ture if success
	//ENode *current;

};


#endif