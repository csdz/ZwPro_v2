/*
*  DelayTrNet.cpp
*
*  Create on : 2013-8-30
*     Author : 王绍楠=sb，↖(^ω^)↗
*    Version : 1.0
*/

/*
*  DelayTrNet.cpp
*
*  Modify on : 2013-9-13
*     Author : 王绍楠=sb，↖(^ω^)↗
*/

#include "delayTrNet.h"
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
using namespace std;

ENode::ENode(): nextarc(NULL){};

ENode::ENode( const int vertex, const double &weight, int grade, ENode *nextarc, const double *delay, int m){
	adjvex = vertex;
	distance[0] = weight;
	distance[1] = 1;
	roadGrade=grade;
	ENode::nextarc=nextarc;
	data[0] = new double[m+1];
	data[1] = new double[m+1];
	for( int i=0; i<m+1; i++){
		data[0][i]=delay[i];
		data[1][i] = 1;
	}
}

int ENode::getVertex(){
	return adjvex;
}

double *ENode::getDelays(){
	return data[0];
}

ENode * ENode::getNextArc(){
	return nextarc;
}

void ENode::setNextArc(ENode* nextarc_para){
	nextarc=nextarc_para;
}

int ENode::getIndexOfAdjNode(){
	return indexOfAdjNode;
}

void ENode::setIndexOfAdjNode(int index){
	indexOfAdjNode=index;
}

double ENode::getDistance(){
	return distance[0];
}

double ENode::getDistanceRate(){
	return distance[1];
}

//void ENode::setDelayRate(){
//	
//}
//
void ENode::setDistanceRate(double val){
	distance[1] = val;
}

void ENode::setDelaysRate(int adjNum, double val){
	for(int i = 0; i <= adjNum; ++i){
		data[1][i] = val;
	}
}

void ENode::setDelayRate(int j, double val){
	int size = sizeof(data);
	if(j < size)
		data[1][j] = val;
	else{
		cout<<"out of bound <setDelayRate>"<<endl;
		exit(1);
	}
}

int ENode::getRoadGrade(){
	return roadGrade;
}

FigheadArray::FigheadArray(){};

FigheadArray::FigheadArray(int nodeIndex_para,double lon_para, double lat_para,int adjvexNum_para,vector<int> pv_para){
	nodeIndex=nodeIndex_para;
	location= *(new Coordinate(lon_para, lat_para));
	adjvexNum=adjvexNum_para;
	parentVertex=pv_para;
}

//FigheadArray::Coordinate::Coordinate(double lon,double lat){
//	longitude=lon;
//	latitude=lat;
//}
//
//FigheadArray::Coordinate * FigheadArray::getLocation(){
//	return location;
//}

Coordinate::Coordinate(double lon ,double lat):longitude(lon), latitude(lat){
}

Coordinate FigheadArray::getLocation(){
	return location;
}

int FigheadArray::getNodeIndex(){
	return nodeIndex;
}

void FigheadArray::setNodeIndex(int nodeIndex_para){
	nodeIndex=nodeIndex_para;
}

ENode *FigheadArray::getFirstAdjNode(){
	return firstAdjNode;
}

void FigheadArray::setFirstAdjNode(ENode *firstAdjNode_para){
	firstAdjNode=firstAdjNode_para;
}

int FigheadArray::getAdjvexNum(){
	return adjvexNum;
}

void FigheadArray::setParentVertex(vector<int> pv_para){
	parentVertex=pv_para;
}

vector<int> FigheadArray::getParentVertex(){
	return parentVertex;
}

LinkedGraph::LinkedGraph(string &file_name){
	ifstream infile;
	infile.open(file_name);
	if(infile.fail()){
		cout<<"input file failed\n";
		exit(1);
	}
	infile.seekg(0,ios::beg);
	infile >> totalNode;
	arcsLoaded=0;
	fighead=new FigheadArray[totalNode+1];
	vector<int> pv_para;
	for(int i=1; i<=totalNode;i++){
		fighead[i].setNodeIndex(i);
		fighead[i].setParentVertex(pv_para);
	}
	inputGraph(infile);
}

void LinkedGraph::loadVarCostDelay(string &file_name){
	ifstream infile;
	infile.open(file_name);
	if(infile.fail()){
		cout<<"input file failed\n";
		exit(1);
	}
	infile.seekg(0,ios::beg);
	infile >> totalNode;
	inputDynamicGraph(infile);
}

int LinkedGraph::getTotalNode()const{
	return totalNode;
}

FigheadArray LinkedGraph::getFighead(int u)const{
	return fighead[u];
}

bool LinkedGraph::isNodeOutBound(const int u){
	if(u<=0 || u > totalNode ){//|| exist(u,v)
		cout<<"Index of node ("<<u<<") is out of bound"<< endl;
		return true;
	}
	return false;
}

/*为路网存储的主体函数
*/
void LinkedGraph::inputGraph(ifstream &infile){
	//根据data说明输入每个数据
	char ch;
	int u,v,m,g,adj;//u=vertex,v=adjvex,m=nextVertexNum
	double *vdelay,w,lon,lat;//vdelay=[turnDelay1,turnDelay2,...,turnDelay(nextVertexNum)],w=distance,lon=longitude,lat=latitude,	
	while((ch=infile.peek())!=EOF){
		infile >> u >> lon >> lat >> adj;
		if(isNodeOutBound(u))exit(1);
		fighead[u]=*(new FigheadArray(u,lon,lat,adj,fighead[u].getParentVertex()));
		ENode *tmp0,*tmp1;
		for(int i=0;i<adj;i++){
			infile>> v >> w >> g >> m;
			if(isNodeOutBound(v))exit(1);
			vdelay =new double[m+1];
			vdelay[0]=-1;
			for ( int j=1; j<m+1; j++){
				infile>> vdelay[j];
			}
			tmp1=new ENode(v,w,g,NULL,vdelay,m);
			if(0 == i){
				fighead[u].setFirstAdjNode(tmp1);
			}else{
				tmp0->setNextArc(tmp1);
			}
			fighead[v].parentVertex.push_back(u);
			tmp1->setIndexOfAdjNode(i+1);
			arcsLoaded++;
			tmp0=tmp1;
			delete []vdelay;
		}
	}
	infile.close();
}

void LinkedGraph::inputDynamicGraph(ifstream& infile){//为cost和delay变化率存储的主体函数
	//根据data说明输入每个数据
	char ch;
	int u,v,m,adj;//u=vertex,v=adjvex,m=nextVertexNum
	double *vdelay,w;//vdelay=[turnDelay1,turnDelay2,...,turnDelay(nextVertexNum)],w=distance,lon=longitude,lat=latitude,	
	while((ch=infile.peek())!=EOF){
		infile >> u >> adj;
		if(isNodeOutBound(u))exit(1);
	//	fighead[u]=*(new FigheadArray(u,lon,lat,adj,fighead[u].getParentVertex()));
		ENode *tmp0 = fighead[u].getFirstAdjNode();//获得邻接链表的第一个节点的指针；
		for(int i=0;i<adj;i++){
			infile>> v >> w >> m;
			if(isNodeOutBound(v))exit(1);
			vdelay =new double[m+1];
			vdelay[0]=-1;
			for ( int j=1; j<m+1; j++){
				infile>> vdelay[j];
			}
//			tmp0->data[1] = vdelay;//赋值延误时间变化率
			for(int j = 1; j < m+1; ++j){//对应修改延误时间
				tmp0->data[0][j] *= (1+vdelay[j]);
				tmp0->data[1][j] *= (1+vdelay[j]);
			}
			tmp0->distance[1] *= (1+w);//赋值路段权重变化率改变
			tmp0->distance[0] *= (1+w);//对应修改路段权重
			tmp0 = tmp0->getNextArc();
			delete []vdelay;
		}
	}
	infile.close();
}

bool LinkedGraph::isExist(int u, int v)const{
	if(u<=0 || v<=0 || u>totalNode || v>totalNode || v==u ){//|| exist(u,v)
		return false;
	}
	ENode *p=fighead[u].getFirstAdjNode();
	while(p){
		if(p->adjvex ==v)return true;
		else
			p=p->nextarc;
	}
	return false;
}

ENode * LinkedGraph::findChildNode(int u, int v) const{
	if(u<=0 || v<=0 || u>totalNode || v>totalNode || v==u ){//|| exist(u,v)
		cout<<"NoSuchNode"<<endl;
		exit(1);
	}
	ENode *p=fighead[u].getFirstAdjNode();
	while(p){
		if(p->adjvex ==v)return p;
		else
			p=p->nextarc;
	}
	cout<<"NoSuchNode"<<endl;
	exit(1);
}

vector<int>  LinkedGraph::findChildNode(int u) const{
	vector<int> ret;
	if(u>0 && u<=totalNode){
		ENode *p=fighead[u].getFirstAdjNode();
		while(p){
			ret.push_back(p->getVertex());
			p=p->nextarc;
		}
	}
	return ret;
}

double LinkedGraph::getDelay(int i, int j, int k)const{
	if(i==0)return 0;//当i==0时，即为虚拟节点时，转向延误为0；
	if(!isExist(i,j) || !isExist(j,k))
	{
		cout<<"ENode "<<i<<"/"<<j<<" to "<<j<<"/"<<k<<" are UNCONNECTED."<<endl;
		exit(1);
	}
	ENode *p=findChildNode(i,j);
	return findChildNode(i,j)->data[0][findChildNode(j,k)->getIndexOfAdjNode()];
}

double LinkedGraph::getDelayRate(int i, int j, int k)const{
	if(i==0)return 0;//当i==0时，即为虚拟节点时，转向延误为0；
	if(!isExist(i,j) || !isExist(j,k))
	{
		cout<<"ENode "<<i<<"/"<<j<<" to "<<j<<"/"<<k<<" are UNCONNECTED."<<endl;
		exit(1);
	}
//	ENode *p=findChildNode(i,j);
	return findChildNode(i,j)->data[1][findChildNode(j,k)->getIndexOfAdjNode()];
}

double LinkedGraph::getDistance(int i,int j)const{
	if(isExist(i,j))
		return findChildNode(i,j)->getDistance();
	else{
		cout<<"ENode "<<i<<" to "<<j<<" are UNCONNECTED."<<endl;
		exit(1);
	}
}

double LinkedGraph::getDistanceRate(int i,int j)const{
	if(isExist(i,j))
		return findChildNode(i,j)->getDistanceRate();
	else{
		cout<<"ENode "<<i<<" to "<<j<<" are UNCONNECTED."<<endl;
		exit(1);
	}
}

void LinkedGraph::setDistanceRate(int i, int j, double val){
	if(isExist(i,j))
		findChildNode(i,j)->setDistanceRate(val);
	else{
		cout<<"ENode "<<i<<" to "<<j<<" are UNCONNECTED."<<endl;
		exit(1);
	}
}

void LinkedGraph::setDelaysRate(int i, int j, double val){
	if(isExist(i,j)){
		int adjNum = fighead[j].getAdjvexNum();
		findChildNode(i,j)->setDelaysRate(adjNum, val);
	}
	else{
		cout<<"ENode "<<i<<" to "<<j<<" are UNCONNECTED."<<endl;
		exit(1);
	}
}

void LinkedGraph::resetDistanceDelayRate(){
	for(int i = 1; i < totalNode+1; ++i){
		ENode *p=fighead[i].getFirstAdjNode();
		while(p){
			int adjNum = fighead[p->adjvex].getAdjvexNum();
			p->setDistanceRate(1);
			p->setDelaysRate(adjNum,1);
			p=p->nextarc;
		}
	}
}

ENode * LinkedGraph::getHeadArray(int u){
	return fighead[u].getFirstAdjNode();
}

vector<int> LinkedGraph::findParentNode(int u)const{
	return 	fighead[u].parentVertex;
}

int LinkedGraph::getParentNumber(int u)const{
	return fighead[u].parentVertex.size();
}