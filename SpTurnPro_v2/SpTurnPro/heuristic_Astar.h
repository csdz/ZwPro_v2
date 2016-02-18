/*
*  DelayTrNet.h
*
*  Modify on : 2013-12-25
*	  新建此类，以A*算法求解SpTurn问题
*     Author : 王绍楠=sb，↖(^ω^)↗
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
	//shortest_path_length[i][j]是起点i到终点j的最短路径
	//pre_arc[i][j][k]是起点i到任意弧j-k的前驱节点
	//pre_node[i][j]是起点i到终点的j，终点j的先驱节点
	//主体函数searchOptimalPath
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
	//serchOptimalPath函数
	//参数：*netWorkGraph，存储的路网；
	//src_node,dst_node,起点和终点；
	//alg_type，模式，(alg_type=0,dijkstra算法);(alg_type=1,SpTurnDelayAlg算法）
	void searchOptimalPath(int src_node);
	void getOptimalPath();
	int preCur;
	list<int>::iterator currentNode;
	int srcNode;
	int dstNode;
	list<int> sRoute;//动态路径，step2:调用A*获得最优路径放入sRoute
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
	double rate_threshold;//变化率阈值；
	double amout_threshold;//达到量阈值, 为nodes in region 的倍数；
	LimitedRegion region_type;//限制区域type；
	SearchRegion *region;
	int t_minutes;//t分钟更新一次路权，则寻找文件为string filename = t*i + "_minutesVarietyData.txt" ;
	bool findNextCurrentNode();//寻找经过t分钟之后，会在哪一个节点，输出：更新currentNode;如果遇到终点，返回true，否则返回false
	void calUnRealCur();
//	vector<int> realCur;
	vector<int> unRealCur;

	//-----------------指标统计------------------------
	double realTravelTime;//按照动态路径行驶，所花费的实际时间；
	double unRealTravelTime;//按照静态路径行驶，所花费的实际时间；
	int nReSearch;//除去第一次计算最短路路径，需要动态计算最短路径的次数；
	double searchRegionTime;//形成限制搜索区域，并且遍历交叉口是否在限制区域内所花费的时间；
	//-----------------指标统计-------------------------
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