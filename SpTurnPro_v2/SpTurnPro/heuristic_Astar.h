/*
*  DelayTrNet.h
*
*  Modify on : 2013-12-25
*	  �½����࣬��A*�㷨���SpTurn����
*     Author : �����=sb���I(^��^)�J
*/

#ifndef HEURISTRC_ASTAR_H_
#define HEURISTIC_ASTAR_H_

#include "delayTrNet.h"
#include <vector>
#include "searchRegion.h"
typedef unsigned int uint;
using namespace std;


enum AlgorithmType:uint
{
	Dijkstra,
	HeuristicAstar
};

enum HeuristicType:uint
{
	ManhattonDistance,
	EuclideanDistance
};

enum LimitedRegion:uint
{
	InRect,
	Ellipse,
	OutRect,
	AllArea
};

class  SpTurnDelayAlg{
protected:
	int totalNode;
	//int **optomal_path_length;
	//shortest_path_length[i][j]�����i���յ�j�����·��
	//pre_arc[i][j][k]�����i�����⻡j-k��ǰ���ڵ�
	//pre_node[i][j]�����i���յ��j���յ�j�������ڵ�
	//���庯��searchOptimalPath
	AlgorithmType algType;
	double heuFactor;
	double shortestPathLength;
	int **preArc;
	int preNode;
	HeuristicType heuType;
	double *heuValue;
	LinkedGraph *netWorkGraph;
	double runTime;
	int nSegmentsTravelsal;
	//serchOptimalPath����
	//������*netWorkGraph���洢��·����
	//src_node,dst_node,�����յ㣻
	//alg_type��ģʽ��(alg_type=0,dijkstra�㷨);(alg_type=1,SpTurnDelayAlg�㷨��
	void searchOptimalPath(int src_node);
	void getOptimalPath();
	int preCur;
	list<int>::iterator currentNode;
	int srcNode;
	int dstNode;
	list<int> sRoute;//��̬·����step2:����A*�������·������sRoute
	list<int> dRoute;
public:
	SpTurnDelayAlg(LinkedGraph *netWorkGraph,int src_node,int dst_node,AlgorithmType alg_type, HeuristicType heuType=ManhattonDistance, double factor = 1);
	virtual void displayOptimalPath();
	double getShortestPathLength();
	double getRunTime();
	double getHeuFacotr();
	int getNSegmentsTravelsal();
	virtual void displayPerformace();
	virtual ~SpTurnDelayAlg();
};

class RealTimeLimitedRegion:public SpTurnDelayAlg{
public:
	RealTimeLimitedRegion(LinkedGraph *netWorkGraph, int src_node, int dst_node, AlgorithmType alg_type, 
		HeuristicType heuType = ManhattonDistance, double factor = 1, LimitedRegion r = Ellipse, 
		double r_t = 0.3, double a_t = 0.5, int t_m = 5);
	virtual ~RealTimeLimitedRegion(){};
	virtual void displayOptimalPath();
	virtual void displayPerformace();
private:
	void realTimeLimitedRegionSearch();
	double rate_threshold;//�仯����ֵ��
	double amout_threshold;//�ﵽ����ֵ, Ϊnodes in region �ı�����
	LimitedRegion region_type;//��������type��
	SearchRegion *region;
	int t_minutes;//t���Ӹ���һ��·Ȩ����Ѱ���ļ�Ϊstring filename = t*i + "_minutesVarietyData.txt" ;
	bool findNextCurrentNode();//Ѱ�Ҿ���t����֮�󣬻�����һ���ڵ㣬���������currentNode;��������յ㣬����true�����򷵻�false
	void calUnRealCur();
//	vector<int> realCur;
	vector<int> unRealCur;

	//-----------------ָ��ͳ��------------------------
	double realTravelTime;//���ն�̬·����ʻ�������ѵ�ʵ��ʱ�䣻
	double unRealTravelTime;//���վ�̬·����ʻ�������ѵ�ʵ��ʱ�䣻
	int nReSearch;//��ȥ��һ�μ������··������Ҫ��̬�������·���Ĵ�����
	double searchRegionTime;//�γ������������򣬲��ұ���������Ƿ������������������ѵ�ʱ�䣻
	//-----------------ָ��ͳ��-------------------------
};

class Segment{
public:
	int headENode;
	int tailENode;
	double feeOfNode2Segment;
	~Segment();
	Segment(int tail,int head,double fee);
	bool operator<(const Segment &other)const;
};

#endif