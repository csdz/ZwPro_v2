/*
*  searchRegion.h
*
*  Create on : 2014-5-13
*     Author : wagnsn1989
*    Version : 1.0
*/

#ifndef SEARCHREGION_H_
#define SEARCHREGION_H_
#include "delayTrNet.h"
#include <vector>
#include <cmath>
using namespace std;

class SearchRegion
{
public:
	SearchRegion(const Coordinate &ori, const Coordinate &des, double len = 0.03):_m_current(ori), _m_destination(des), _m_curDst(des){
		_m_arctan = atan((_m_current.latitude - _m_destination.latitude)/(_m_current.longitude - _m_destination.longitude));
		_m_distance = sqrt(pow(_m_current.latitude - _m_destination.latitude,2)+pow(_m_current.longitude - _m_destination.longitude,2));
		_m_length = min(len, _m_distance);
		getCurDst();
		inNodes.push_back(-1);
	};
	SearchRegion(){};
	virtual ~SearchRegion(){};
	//在限制区域中，如果cost或是delay超过ratelimit的节点的个数大于numlimit，则返回true，inNodes为在限制区域内的节点集；
	virtual bool findNodesInRegion(LinkedGraph *g, double amoutLimit, double rateLimit);
	vector<int> getInNodes()const;
protected:
	virtual bool isNodeInSearchRegion(const Coordinate &oneNode){return 0;};
	Coordinate _m_current;//当前节点
	Coordinate _m_curDst;//当前节点沿着终点方向，并且距离当前节点_m_length的坐标；
	Coordinate _m_destination;//终点
	double _m_length;//短距离的长度，为 min(0.03, _m_distance);
	double _m_arctan;//起点到终点的连线的斜率的arctan值
	double _m_distance;//起讫线段距离；
	vector<int> inNodes;
	void getCurDst();//获得成员_m_curDst
};

class InRectRegion : public SearchRegion{
public:
	InRectRegion(const Coordinate &ori, const Coordinate &des, double Th1 = 0.33, double Th2 = 0.25):SearchRegion(ori,des),_m_rect_Th1(Th1),_m_diagonal_Th2(Th2){};
//	bool findNodesInRegion(const LinkedGraph *g, int numLimit, double rateLimit);//返回节点序列
private:
	bool isNodeInSearchRegion(const Coordinate& oneNode);
	double _m_rect_Th1;//平行起讫线段（距离为d），向外扩张d*_m_rect_Th1; 
	double _m_diagonal_Th2;//以起讫线段为对角线，获得矩形，此矩形向外扩张d*_m_diagonal_Th2;
};

class EllipseRegion : public SearchRegion{
public:
	EllipseRegion(const Coordinate &ori, const Coordinate &des, double tau = 1.379):SearchRegion(ori,des),_m_tau(tau){};
//	bool findNodesInRegion(const LinkedGraph *g, int numLimit, double rateLimit, vector<int> &inNodes);
private:
	bool isNodeInSearchRegion(const Coordinate& oneNode);
protected:
	double _m_tau;//椭圆的参数；
};

class OutRectRegion : public EllipseRegion{
public:
	OutRectRegion(const Coordinate &ori, const Coordinate &des, double tau = 1.379):EllipseRegion(ori, des, tau){};
//	bool findNodesInRegion(const LinkedGraph *g, int numLimit, double rateLimit, vector<int> &inNodes);//返回节点序列
private:
	bool isNodeInSearchRegion(const Coordinate& oneNode);
};

class AllAreaRegion : public SearchRegion{
public:
	AllAreaRegion(const Coordinate &ori, const Coordinate &des):SearchRegion(ori,des){};
private:
	virtual bool findNodesInRegion(LinkedGraph *g, double amoutLimit, double rateLimit);
};

#endif