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
	//�����������У����cost����delay����ratelimit�Ľڵ�ĸ�������numlimit���򷵻�true��inNodesΪ�����������ڵĽڵ㼯��
	virtual bool findNodesInRegion(LinkedGraph *g, double amoutLimit, double rateLimit);
	vector<int> getInNodes()const;
protected:
	virtual bool isNodeInSearchRegion(const Coordinate &oneNode){return 0;};
	Coordinate _m_current;//��ǰ�ڵ�
	Coordinate _m_curDst;//��ǰ�ڵ������յ㷽�򣬲��Ҿ��뵱ǰ�ڵ�_m_length�����ꣻ
	Coordinate _m_destination;//�յ�
	double _m_length;//�̾���ĳ��ȣ�Ϊ min(0.03, _m_distance);
	double _m_arctan;//��㵽�յ�����ߵ�б�ʵ�arctanֵ
	double _m_distance;//�����߶ξ��룻
	vector<int> inNodes;
	void getCurDst();//��ó�Ա_m_curDst
};

class InRectRegion : public SearchRegion{
public:
	InRectRegion(const Coordinate &ori, const Coordinate &des, double Th1 = 0.33, double Th2 = 0.25):SearchRegion(ori,des),_m_rect_Th1(Th1),_m_diagonal_Th2(Th2){};
//	bool findNodesInRegion(const LinkedGraph *g, int numLimit, double rateLimit);//���ؽڵ�����
private:
	bool isNodeInSearchRegion(const Coordinate& oneNode);
	double _m_rect_Th1;//ƽ�������߶Σ�����Ϊd������������d*_m_rect_Th1; 
	double _m_diagonal_Th2;//�������߶�Ϊ�Խ��ߣ���þ��Σ��˾�����������d*_m_diagonal_Th2;
};

class EllipseRegion : public SearchRegion{
public:
	EllipseRegion(const Coordinate &ori, const Coordinate &des, double tau = 1.379):SearchRegion(ori,des),_m_tau(tau){};
//	bool findNodesInRegion(const LinkedGraph *g, int numLimit, double rateLimit, vector<int> &inNodes);
private:
	bool isNodeInSearchRegion(const Coordinate& oneNode);
protected:
	double _m_tau;//��Բ�Ĳ�����
};

class OutRectRegion : public EllipseRegion{
public:
	OutRectRegion(const Coordinate &ori, const Coordinate &des, double tau = 1.379):EllipseRegion(ori, des, tau){};
//	bool findNodesInRegion(const LinkedGraph *g, int numLimit, double rateLimit, vector<int> &inNodes);//���ؽڵ�����
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