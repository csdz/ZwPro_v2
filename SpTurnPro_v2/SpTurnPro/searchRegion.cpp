/*
*  searchRegion.cpp
*
*  Create on : 2014-5-13
*     Author : wagnsn1989
*    Version : 1.0
*/

#include "searchRegion.h"
#include "delayTrNet.h"
#include <vector>
#include <ctime>
typedef unsigned int uint;
using namespace std;
#define EP 10e-6

void SearchRegion::getCurDst(){
	if(_m_distance - _m_length > EP){
		double lam = _m_length/(_m_distance - _m_length);
		_m_curDst.longitude  = (_m_current.longitude + lam*_m_destination.longitude)/(1+lam);
		_m_curDst.latitude  = (_m_current.latitude + lam*_m_destination.latitude)/(1+lam);
	}
}

vector<int> SearchRegion::getInNodes()const{
	return inNodes;
}

bool SearchRegion::findNodesInRegion(LinkedGraph *g, double amoutLimit, double rateLimit){
	if(g == NULL){
		cout<<"no LinkedGraph <findndoesInRegion>"<<endl;
		exit(1);
	}
//	inNodes.clear();
	Coordinate tmp;
	if(*inNodes.begin() == -1)inNodes.clear();
	for(int i = 1; i <= g->getTotalNode(); ++i){
		tmp = g->getFighead(i).getLocation();
		if(isNodeInSearchRegion(tmp)){
			inNodes.push_back(i);
		}
	}
	int count = 0;
	for(uint i = 0; i < inNodes.size(); ++i){
		vector<int> p = g->findParentNode(inNodes[i]);
		vector<int> c = g->findChildNode(inNodes[i]);
		for(uint j = 0; j < p.size(); ++j){
			for(uint k = 0; k < c.size(); ++k){
				if(p[j] == c[k])continue;
				double delayRate = g->getDelayRate(p[j],inNodes[i],c[k]);
				if(g->getDelayRate(p[j],inNodes[i],c[k]) > 1 + rateLimit || g->getDelayRate(p[j],inNodes[i],c[k]) < 1 - rateLimit){
					++count;
					if(count >= amoutLimit*inNodes.size()){
						return true;
					}
				}
			}
			if(g->getDistanceRate(p[j],inNodes[i]) > 1 + rateLimit || g->getDistanceRate(p[j],inNodes[i]) < 1 - rateLimit){
				++count;
				if(count >= amoutLimit*inNodes.size()){
					return true;
				}
			}
		}
	}
	return false;
}

bool InRectRegion::isNodeInSearchRegion(const Coordinate &oneNode){
	double tmp_a = tan(_m_arctan)*(oneNode.longitude - _m_current.longitude) + _m_current.latitude;
	double tmp_b = _m_length *_m_rect_Th1/cos(_m_arctan);
	double tmp_c = _m_diagonal_Th2*_m_length;

	double y1 = max(tmp_a-tmp_b, min(_m_current.latitude,_m_curDst.latitude) - tmp_c);
	double y2 = min(tmp_a+tmp_b, max(_m_current.latitude,_m_curDst.latitude) + tmp_c);

	double x1 = min(_m_current.longitude,_m_curDst.longitude) - tmp_c;
	double x2 = max(_m_current.longitude,_m_curDst.longitude) + tmp_c;

	if(oneNode.longitude > x1 && oneNode.longitude < x2 && oneNode.latitude > y1 && oneNode.latitude < y2)
		return true;
	return false;
}

bool EllipseRegion::isNodeInSearchRegion(const Coordinate &oneNode){
	double a = (_m_current.longitude + _m_curDst.longitude)/2;
	double b = (_m_current.latitude +_m_curDst.latitude)/2;
	double A2 = pow(_m_tau*_m_length/2,2);
	double B2 = A2 - pow(_m_length,2)/4;
	double tmp_a = pow(cos(_m_arctan)*(oneNode.longitude - a) + sin(_m_arctan)*(oneNode.latitude - b),2)/A2;
	double tmp_b = pow(cos(_m_arctan)*(oneNode.latitude - b) - sin(_m_arctan)*(oneNode.longitude - a),2)/B2;
	if(tmp_a + tmp_b < 1)
		return true;
	return false;
}

bool OutRectRegion::isNodeInSearchRegion(const Coordinate &oneNode){
	double a = (_m_current.longitude + _m_curDst.longitude)/2;
	double b = (_m_current.latitude +_m_curDst.latitude)/2;
	double A2 = pow(_m_tau*_m_length/2,2);
	double B2 = A2 - pow(_m_length,2)/4;
	double tmp_a = sqrt(A2*pow(cos(_m_arctan),2) + B2*pow(sin(_m_arctan),2));
	double tmp_b = sqrt(A2*pow(sin(_m_arctan),2) + B2*pow(cos(_m_arctan),2));
	if(oneNode.longitude > a - tmp_a && oneNode.longitude < a + tmp_a && oneNode.latitude > b - tmp_b && oneNode.latitude < b + tmp_b)
		return true;
	return false;
}

bool AllAreaRegion::findNodesInRegion(LinkedGraph *g, double amoutLimit, double rateLimit){
	if(*inNodes.begin() == -1)inNodes.clear();
	for(int i = 1; i <= g->getTotalNode(); ++i){
		inNodes.push_back(i);
	}
	int count = 0;
	for(uint i = 0; i < inNodes.size(); ++i){
		vector<int> p = g->findParentNode(inNodes[i]);
		vector<int> c = g->findChildNode(inNodes[i]);
		for(uint j = 0; j < p.size(); ++j){
			for(uint k = 0; k < c.size(); ++k){
				if(p[j] == c[k])continue;
				double delayRate = g->getDelayRate(p[j],inNodes[i],c[k]);
				if(g->getDelayRate(p[j],inNodes[i],c[k]) > 1 + rateLimit || g->getDelayRate(p[j],inNodes[i],c[k]) < 1 - rateLimit){
					++count;
					if(count >= amoutLimit*inNodes.size()){
						return true;
					}
				}
			}
			if(g->getDistanceRate(p[j],inNodes[i]) > 1 + rateLimit || g->getDistanceRate(p[j],inNodes[i]) < 1 - rateLimit){
				++count;
				if(count >= amoutLimit*inNodes.size()){
					return true;
				}
			}
		}
	}
	return false;
}
