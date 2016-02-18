#include "delayTrNet.h"
#include <iostream>
//#include "optimalPath.h"
#include "heuristic_Astar.h"
#include <list>
#include <set>
using namespace std;

//int main(){
//	ifstream infile;
//	LinkedGraph *g=new LinkedGraph(infile);
//	OptimalPath *op=new OptimalPath(g);
//	for(int i=1;i<g->getTotalNode()+1;i++){
//		for(int j=1;j<g->getTotalNode()+1;j++){
//			cout<<"("<<i<<","<<j<<")="<<op->getShortestPathLength(i,j)<<"  ";
//			op->displayOptimalPath(i,j);
//		}
//	}
//
//	return 0;
//}
int main(){
	string file_name="myDataFromMatlab.txt";
	LinkedGraph *g=new LinkedGraph(file_name);
	ENode *t = g->getFighead(80).getFirstAdjNode();
	int i= 1;
	int j = 56;
	SpTurnDelayAlg *op=new SpTurnDelayAlg(g,i,j,HeuristicAstar,EuclideanDistance,1);
	op->displayPerformace();
	delete op;
	RealTimeLimitedRegion *op1 = new RealTimeLimitedRegion(g, i, j, HeuristicAstar,EuclideanDistance,1,Ellipse, 0.3, 0.5, 5);//����ĸ���������ʾ����������״��rate_threhold�仯����ֵ��amout_threshold����ֵ��time_interval,ʱ����
	//�����Ҫ��th1,t������������״�Ĳ���������tau��h2,��ôת��"searchRegion.h"�ļ������ж�Ӧ���޸�
	op1->displayPerformace();
	delete op1;
	/*for(int i=1;i<g->getTotalNode()+1;i++){
		for(int j=1;j<g->getTotalNode()+1;j++){
			if(i==1 && j == 26)
				cout<<"i=1;i=26"<<endl;
			SpTurnDelayAlg *op=new SpTurnDelayAlg(g,i,j,Dijkstra);
			cout<<"SpTurnDelayAlg ("<<i<<","<<j<<")="<<op->getShortestPathLength()<<"  ";
			op->displayOptimalPath();
		}
	}*/
}