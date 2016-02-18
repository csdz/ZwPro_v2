/*
*  heuristic_SpTurnDelayAlg.cpp
*
*  Create on : 2013-12-25
*     Author : wagnsn1989
*    Version : 1.0
*/

#include "heuristic_Astar.h"
#include "delayTrNet.h"
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <ctime>
using namespace std;

typedef unsigned int uint;

//double min(const double a,const double b){
//	return a>b?b:a;
//}
const int M=10000000;
const double ep = 0.000001;
const double pi=3.141593;
const int E_R=6371004;
const double avgSpeed = 5;

Segment::Segment(int tail,int head,double fee){
	tailENode=tail;
	headENode=head;
	feeOfNode2Segment=fee;
}

Segment::~Segment(){};

bool Segment::operator<(const Segment &other)const{
	if(feeOfNode2Segment - other.feeOfNode2Segment > ep || feeOfNode2Segment - other.feeOfNode2Segment < -1.0*ep)
		return (feeOfNode2Segment<other.feeOfNode2Segment);
	else if(tailENode!=other.tailENode)
		return (tailENode<other.tailENode);
	else
		return (headENode<other.headENode);		
};

//bool Segment::operator==(const Segment &other)const{
//	return (tailENode==other.tailENode && headENode==other.headENode && feeOfNode2Segment==other.feeOfNode2Segment);
//};

SpTurnDelayAlg::SpTurnDelayAlg(LinkedGraph *graph,int src_node,int dst_node,AlgorithmType alg_type, HeuristicType heu_type, double factor):
	totalNode(graph->getTotalNode()), srcNode(src_node), dstNode(dst_node), algType(alg_type), heuType(heu_type), netWorkGraph(graph), 
	heuFactor(factor), heuValue(NULL), nSegmentsTravelsal(0), shortestPathLength(M), preNode(M), preCur(0), runTime(0){
	preArc =new int*[totalNode+1];
	for(int i=0;i<totalNode+1;i++){
		preArc[i]=new int[totalNode+1];
	}
	searchOptimalPath(src_node);
	getOptimalPath();
	sRoute = dRoute;
	currentNode = dRoute.begin();
}

SpTurnDelayAlg::~SpTurnDelayAlg(){};

double SpTurnDelayAlg::getRunTime(){
	return runTime;
}

int SpTurnDelayAlg::getNSegmentsTravelsal(){
	return nSegmentsTravelsal;
}

void SpTurnDelayAlg::searchOptimalPath(int src_Node){
	if (algType == Dijkstra){
		//初始化
		//cout<<"Dijkstra"<<endl;

		if((netWorkGraph->isNodeOutBound(src_Node)) || (netWorkGraph->isNodeOutBound(dstNode)))exit(1);
		if(src_Node==dstNode){
			shortestPathLength=0;
			preNode=src_Node;
			preArc[src_Node][dstNode]=0;
			return;
		}
		double **fee =new double*[totalNode+1];
		//double **goal =new double*[totalNode+1];
		//double **heu =new double*[totalNode+1];
		for(int i=0;i<totalNode+1;i++){
			fee[i] =new double[totalNode+1];
			//goal[i]=new double[totalNode+1];
			//heu[i]=new double[totalNode+1];
		}
		//**************开始时间设定********************
		clock_t startTime  = clock();
		//**************开始时间设定*******************

		//step1:---------------begin---------------------------------------------
		//fee[i][j][k]起点i到弧j-k的费用，goal[i][j][k]起点i到弧j-k的目标值
		set<Segment>  open,close,topen,tclose;
		vector<int> dst_parent=netWorkGraph->findParentNode(dstNode);
		for(uint i=0;i<dst_parent.size();i++){
			Segment *s=new Segment(dst_parent[i],dstNode,0);
			topen.insert(*s);
			delete s;
		}
		//step1-----------------end-------------------------------------------------
	
		//step2:----------------begin-----------------------------------------------
		for(int i=0;i<totalNode+1;i++){
			for(int j=0;j<totalNode+1;j++){
				if(i==j){
					fee[i][j]=0;
					continue;
				}
				fee[i][j]=M;
			}
		}
		fee[0][src_Node]=0;
		//heu[0][src_node]=0;
		open.insert(*(new Segment(0,src_Node,0)));
		//step2:----------------end------------------------------------------------

		//step3:----------------begin----------------------------------------------
		set<Segment>::iterator it_set; 
		while(!open.empty()){
			nSegmentsTravelsal++;
			it_set=open.begin();
			if(it_set->headENode == preCur && it_set->tailENode == src_Node){
				open.erase(it_set);
				continue;
			}
			Segment tmp_segment=*it_set;//选取fee最小值;
			close.insert(*it_set);
			open.erase(it_set);
			it_set=topen.find(*(new Segment(tmp_segment.tailENode,tmp_segment.headENode,0)));
			//if(tmp_segment.tailENode == 36)
			//	cout<<"pause"<<endl;
			if(it_set!=topen.end()){
				//如果在topen中找到了tmp_ v segment
				tclose.insert(tmp_segment);
				topen.erase(it_set);
				if(topen.empty())
					break;
				else
					continue;
			}
			//step 3.2----------------------------------------
			int r=tmp_segment.tailENode,s=tmp_segment.headENode;
			//if(r == 47)
			//	cout<<"r: "<<r<<" s: "<<s<<" goal: "<<fee[r][s]<<endl;

			ENode* p=NULL;
			if(netWorkGraph->getFighead(s).getAdjvexNum())
				p=netWorkGraph->getFighead(s).getFirstAdjNode();
			while(p){
				int t=p->getVertex();
				double c=netWorkGraph->getDistance(s,t);
				double d;
				if(s==srcNode)
					d=0;
				else
					d=netWorkGraph->getDelay(r,s,t);
				if(fee[s][t]>fee[r][s]+d+c && fee[r][s]+d+c > 0){
					it_set = open.find(*(new Segment(s,t,fee[s][t])));
					if( it_set != open.end()){
						open.erase(it_set);
					}
					fee[s][t]=fee[r][s]+d+c;
					preArc[s][t]=r;
					open.insert(*(new Segment(s,t,fee[s][t])));
				}
				p=p->getNextArc();
			}
		}
		//step 4--------------------begin----------------------------------------
		if(tclose.empty()){
			//cout<<"impossible path"<<endl;
			return;
		}
		shortestPathLength=(tclose.begin()->feeOfNode2Segment);
		preNode=(tclose.begin()->tailENode);
		//step 4--------------------end-------------------------------------------

		//**************结束时间设定********************
		runTime += double(clock() - startTime) ;
		//**************结束时间设定*******************
	}
	else if(algType == HeuristicAstar){
		//初始化
		//cout<<"HeuristicAstar"<<endl;

		if((netWorkGraph->isNodeOutBound(src_Node)) || (netWorkGraph->isNodeOutBound(dstNode)))exit(1);
		if(src_Node==dstNode){
			shortestPathLength=0;
			preNode=src_Node;
			preArc[src_Node][dstNode]=0;
			return;
		}
		double **fee =new double*[totalNode+1];
		double **goal =new double*[totalNode+1];
		double *heuValue = new double[totalNode + 1];
		for(int i=0; i<totalNode+1; i++){
			if(i == 0 || i == dstNode ){
				heuValue[i] = 0;
				continue;
			}
			Coordinate st_location=netWorkGraph->getFighead(i).getLocation();
			Coordinate ed_location=netWorkGraph->getFighead(dstNode).getLocation();
			double tmp_x = pi*E_R/180*fabs(st_location.latitude-ed_location.latitude);
			double tmp_y = pi*E_R/180*fabs(st_location.longitude-ed_location.longitude);
			if(heuType == ManhattonDistance){
				heuValue[i] = tmp_x + tmp_y;
			}
			else if(heuType == EuclideanDistance){
				heuValue[i] = sqrt(tmp_x*tmp_x + tmp_y*tmp_y);
			}
			heuValue[i] *= heuFactor/avgSpeed;
		}
		for(int i=0;i<totalNode+1;i++){
			fee[i] =new double[totalNode+1];
			goal[i]=new double[totalNode+1];
		}

		//step1:---------------begin---------------------------------------------
		//fee[i][j][k]起点i到弧j-k的费用，goal[i][j][k]起点i到弧j-k的目标值
		set<Segment>  open,close,topen,tclose;
		vector<int> dst_parent=netWorkGraph->findParentNode(dstNode);
		for(uint i=0;i<dst_parent.size();i++){
			Segment *s=new Segment(dst_parent[i],dstNode,0);
			topen.insert(*s);
			delete s;
		}
		//step1-----------------end-------------------------------------------------

		//step2:----------------begin-----------------------------------------------
		for(int i=0;i<totalNode+1;i++){
			for(int j=0;j<totalNode+1;j++){
				if(i==j){
					fee[i][j]=0;
					goal[i][j] = 0;
					continue;
				}
				fee[i][j]=M;
				goal[i][j] = M;
			}
		}
		fee[0][src_Node]=heuValue[src_Node];
		open.insert(*(new Segment(0,src_Node,fee[0][src_Node])));
		//step2:----------------end------------------------------------------------
		//**************开始时间设定********************
		clock_t startTime  = clock();
		//**************开始时间设定*******************

		//step3:----------------begin----------------------------------------------
		set<Segment>::iterator it_set; 
		while(!open.empty()){
			nSegmentsTravelsal++;
			it_set=open.begin();
			if(it_set->headENode == preCur && it_set->tailENode == src_Node){
				open.erase(it_set);
				continue;
			}
			Segment tmp_segment=*it_set;//选取fee最小值;
			open.erase(it_set);
			int r=tmp_segment.tailENode,s=tmp_segment.headENode;
			goal[r][s] = fee[r][s] - heuValue[s];

			//if(r == 47)
			//	cout<<"r: "<<r<<" s: "<<s<<" goal: "<<goal[r][s]<<endl;

			Segment *goal_tmp_segment = new Segment(r,s,goal[r][s]);
			close.insert(*goal_tmp_segment);
			it_set=topen.find(*(new Segment(r,s,0)));
			if(it_set!=topen.end()){
				//如果在topen中找到了tmp_ v segment
				tclose.insert(*goal_tmp_segment);
				topen.erase(it_set);
				if(topen.empty())
					break;
				else
					continue;
			}
			//step 3.2----------------------------------------
			ENode* p=NULL;
			if(netWorkGraph->getFighead(s).getAdjvexNum())
				p=netWorkGraph->getFighead(s).getFirstAdjNode();
			while(p){
				int t=p->getVertex();
				double c=netWorkGraph->getDistance(s,t);
				double d;
				if(s == srcNode)
					d=0;
				else
					d=netWorkGraph->getDelay(r,s,t);
				double h = heuValue[t];

				if(fee[s][t]>goal[r][s]+h+d+c && goal[r][s]+h+d+c > 0 && (goal[s][t] == M || goal[s][t] == 0)){
					it_set = open.find(*(new Segment(s,t,fee[s][t])));
					if( it_set != open.end()){
						open.erase(it_set);
					}
					/*for(it_set = open.begin(); it_set != open.end(); it_set++){
						if(it_set->headENode == t && it_set->tailENode == s){
							open.erase(it_set);
							break;
						}
					}*/
					fee[s][t]=goal[r][s]+h+d+c;
					preArc[s][t]=r;
					open.insert(*(new Segment(s,t,fee[s][t])));
				}
				p=p->getNextArc();
			}
		}
		//step 4--------------------begin----------------------------------------
		if(tclose.empty()){
			//cout<<"impossible path"<<endl;
			return;
		}
		shortestPathLength=(tclose.begin()->feeOfNode2Segment);
		preNode=(tclose.begin()->tailENode);

		//**************结束时间设定********************
		runTime += double(clock() - startTime) ;
		//**************结束时间设定*******************
	}
}

void SpTurnDelayAlg::getOptimalPath(){
	//list<int> ret;
	if(getShortestPathLength()<M){
		dRoute.push_front(dstNode);
		int temp0=preNode;
		dRoute.push_front(temp0);
		int temp1=preArc[preNode][dstNode];
		while(temp1!=0){
			dRoute.push_front(temp1);
			int temp2=temp1;
			temp1=preArc[temp1][temp0];
			temp0=temp2;				
		}
	}
//	return ret;	
}

double SpTurnDelayAlg::getShortestPathLength(){
	return shortestPathLength;
}

void SpTurnDelayAlg::displayOptimalPath(){
//	list<int> ret=getOptimalPath();
	//cout<<endl<<"------------------Optimal Path----------------------"<<endl;
	if(sRoute.size()){
		list<int>::iterator k;
		cout<<"Optimal Path: "<< *sRoute.begin();
		for(k=++sRoute.begin();k!=sRoute.end();k++)
			cout<<"->"<<*k;
		cout<<endl;
	}else{
		cout<<"impossible path"<<endl;
	}
	//cout<<"----------------------------------------------------"<<endl;
}

void SpTurnDelayAlg::displayPerformace(){
	cout<<endl<<"----------------Performace Report-------------------"<<endl;
	cout<<"Start: "<<srcNode<<" To: "<<dstNode<<endl;
	if(!algType){
		cout<<"Algorithm: Dijkstra"<<endl;
	}
	else{
		cout<<"Algorithm: HeuristicAstar"<<endl;
		if(heuType){
			cout<<"Heuristic function: Euclidean Distance"<<endl;
		}
		else if(heuType == ManhattonDistance){
			cout<<"Heuristic function: Manhatton Distance"<<endl;
		}
		cout<<"Heuristic factor: "<<heuFactor<<endl;
	}
	cout<<"Travel time: "<<(int)(shortestPathLength/60)<<"min"<<endl;
	//cout<<"Travel Time: "<<shortestPathLength<<"sec"<<endl;
	cout<<"Run time: "<<runTime<<"ms"<<endl;
	cout<<"nSegments travelsal: "<<nSegmentsTravelsal<<endl;
	displayOptimalPath();
	cout<<"----------------------------------------------------"<<endl;
}

void RealTimeLimitedRegion::displayOptimalPath(){
	//cout<<endl<<"------------------Optimal Path----------------------"<<endl;
	if(!dRoute.empty()){
		list<int>::iterator k;
		cout<<"Optimal Path: "<< *dRoute.begin();
		for(k=++dRoute.begin();k!=dRoute.end();k++)
			cout<<"->"<<*k;
		cout<<endl;
	}else{
		cout<<"impossible path"<<endl;
	}
	//cout<<"----------------------------------------------------"<<endl;
}

void RealTimeLimitedRegion::displayPerformace(){
	cout<<endl<<"------------------LimitedRegion Performace----------------------";
	SpTurnDelayAlg::displayPerformace();
	cout<<"Unreal travel time with HeuristicAstar: "<<(int)(unRealTravelTime/60)<<"min"<<endl;
	cout<<"times of researching path : "<<nReSearch<<endl;
	cout<<"total time for searching region: "<<searchRegionTime<<"ms"<<endl;
	switch(region_type){
		case 0: cout<<"Search Region: InRect"<<endl; break;
		case 1: cout<<"Search Region: Ellipse"<<endl; break;
		case 2: cout<<"Search Region: OutRect"<<endl; break;
		case 3: cout<<"Search Region: All Area"<<endl;
	}
	cout<<endl;
}

RealTimeLimitedRegion::RealTimeLimitedRegion(LinkedGraph *netWorkGraph, int src_node, int dst_node, 
		AlgorithmType alg_type, HeuristicType heuType, double factor,  LimitedRegion r, double r_t, double a_t, int t_m)
	:SpTurnDelayAlg(netWorkGraph, src_node, dst_node, alg_type, heuType, factor), rate_threshold(r_t), 
	amout_threshold(a_t), region_type(r), t_minutes(t_m), realTravelTime(0), unRealTravelTime(0),nReSearch(0),searchRegionTime(0)
{
//	getOptimalPath();
//	dRoute = sRoute;
//	currentNode = dRoute.begin();
	calUnRealCur();
	realTimeLimitedRegionSearch();
}

/*realTimeLimitedRegionSearch是该类的一个重要函数，功能是每隔t分钟（一般可认为5分钟）
实时动态读入更新路网路段的权重和转向延误，并且获得该限制区域，如果该区域的变化率超过一
定阈值达到一定量，则认为需要更新路径。
输入：路网，起讫点，一些阈值，限制区域方式（3种，内矩形，椭圆，外矩形）
输出：面对实时动态的路网，获得相对较省时的路径

可分为以下步骤：step 1和step 2已经在SpTurnDelayAlg的构造函数内执行了

----------------SpTurnDelayAlg的构造函数内执行了_start-----------------
step 1： 初始化。读入路网数据，起讫点，出发时刻（一般认为时刻0）
step 2： 调用改进的A*算法计算起点到终点的最优路径
----------------SpTurnDelayAlg的构造函数内执行了_end--------------------

step 3： 计算5分钟后，需要考察的节点，如果是终点，则结束，否则继续；
step 4： 更新路网或是限制区域的路段权重和转向延误；
step 5： 根据当前节点与终点确定限制搜索区域Z；
step 6： 如果在限制区域的变化率超过了一定阈值达到一定量，则调用A*计算当前点到终点的新路径；
否则不需更新路径，回到step 3

*/
string int2string(int i){
	string tmp;
	while(i){
		tmp.push_back(i%10 + '0');
		i /= 10;
	}
	reverse(tmp.begin(), tmp.end());
	return tmp;
}

void RealTimeLimitedRegion::realTimeLimitedRegionSearch(){
	unsigned int i = 1;
	list<int>::iterator it = sRoute.begin(), next, next2;
	//-------step3---start-----------------------
	bool isFind = findNextCurrentNode();
	int pre_cur = 0;
	while(!isFind || i <= unRealCur.size()){
		//计算unRealTravelTime;
		while(i <= unRealCur.size() && it != sRoute.end() && unRealCur[i-1] != *it){
			next = it;
			if(++next == sRoute.end())break;
			double c = netWorkGraph->getDistance(*it, *next);
			double d = 0;
			if(pre_cur != 0)
				d = netWorkGraph->getDelay(pre_cur, *it, *next);
			unRealTravelTime += d+c;
			pre_cur = *it;
			++it;
		}
		//-------step4---start-----------------------
		string filename = int2string(t_minutes*(i++));
		filename.append("_minutesVarietyData.txt");
		netWorkGraph->loadVarCostDelay(filename);
		if(!isFind){
			Coordinate cur = netWorkGraph->getFighead(*currentNode).getLocation();
			Coordinate dst = netWorkGraph->getFighead(dstNode).getLocation();
		//-------step5---start-----------------------
			//**************开始时间设定********************
			clock_t startTime  = clock();
			//**************开始时间设定*******************
			if(region_type == InRect){
				region = new InRectRegion(cur,dst);
			}else if(region_type == Ellipse){
				region = new EllipseRegion(cur,dst);
			}else if(region_type == OutRect){
				region = new OutRectRegion(cur,dst);
			}else if(region_type == AllArea){
				region = new AllAreaRegion(cur,dst);
			}
		//-------step6---start-----------------------
			bool isNeedReSearchPath = region->findNodesInRegion(netWorkGraph, amout_threshold, rate_threshold);
			searchRegionTime += double(clock()-startTime);
			runTime += double(clock()-startTime);
			if(isNeedReSearchPath){
				list<int>::iterator next = currentNode;
				++next;
				dRoute.erase(next,dRoute.end());
				list<int> cRoute;
				swap(cRoute,dRoute);
				searchOptimalPath(*currentNode);
				getOptimalPath();
				if(*dRoute.begin() == dstNode){
					dRoute.clear();
					dRoute.push_back(dstNode);
				}
				cRoute.pop_back();
				currentNode = dRoute.begin();
				dRoute.splice(dRoute.begin(), cRoute);
				//如果需要重新变更路径，则需要将区域所有的节点全部更新行程时间和转向延误的累积变化率为1
				netWorkGraph->resetDistanceDelayRate();
				++nReSearch;
			}
		}
		isFind = findNextCurrentNode();
	}
	shortestPathLength = min(realTravelTime , unRealTravelTime);
}

//寻找下一个当前节点，如果遇到终点，返回true，否则返回false
bool RealTimeLimitedRegion::findNextCurrentNode(){
	double tRemaining = t_minutes*60;//还剩下多少秒
	list<int>::iterator nextNode = currentNode;
	while(tRemaining > 0 && currentNode != dRoute.end()){
		/*if(tRemaining == t_minutes*60)
			realCur.push_back(*currentNode);*/
		if(++nextNode == dRoute.end())return true;
		double c = netWorkGraph->getDistance(*currentNode, *nextNode);
		double d = 0;
		if(preCur != 0)
			d = netWorkGraph->getDelay(preCur, *currentNode,*nextNode);
		tRemaining -= (d + c);
		realTravelTime += d+c;
	//	if(tRemaining < 0)break;
		preCur = *currentNode;
		++currentNode;
	}
	if(nextNode == dRoute.end())return true;
	return false;
}

void RealTimeLimitedRegion::calUnRealCur(){
//	int count = 1;
	double tRemaining = t_minutes*60;
	double t = 0;
	int pre_cur = 0;
	list<int>::iterator it = sRoute.begin(), next, next2;
	while(it != sRoute.end()){
		if(t > tRemaining){
			unRealCur.push_back(*it);
			t = 0;
		}
		next = it;
		double c = 0 ,d = 0;
		if(++next != sRoute.end()){
			c = netWorkGraph->getDistance(*it, *next);
			if(pre_cur != 0){
				d = netWorkGraph->getDelay(pre_cur, *it, *next);
			}
		}
		t += d+c;
		pre_cur = *it;
		++it;
	}
	if(unRealCur.empty() || unRealCur.back() != sRoute.back())
		unRealCur.push_back(sRoute.back());
}