/*
*  optimalPath.cpp
*
*  Create on : 2013-9-3
*     Author : wagnsn1989
*    Version : 1.0
*/

#include "optimalPath.h"
#include "delayTrNet.h"
#include <vector>
using namespace std;

//double min(const double a,const double b){
//	return a>b?b:a;
//}
const int M=10000000;

OptimalPath::OptimalPath(LinkedGraph *netWorkGraph){
	total_node=netWorkGraph->getTotalNode();
	shortest_path_length =new double*[total_node+1];
	pre_arc =new int**[total_node+1];
	pre_node=new int*[total_node+1];
	for(int i=1;i<total_node+1;i++){
		shortest_path_length[i]=new double[total_node+1];
		pre_arc[i]=new int*[total_node+1];
		pre_node[i]=new int[total_node+1];
	}
	for(int i=1;i<total_node+1;i++){
		for(int j=1;j<total_node+1;j++){
			pre_arc[i][j]=new int [total_node+1];
		}
	}
	for(int i=1;i<total_node+1;i++){
		shortest_path_length[i][i]=0;
		pre_node[i][i]=i;
	}
	for(int i=1; i<total_node+1; i++){
		searchOptimalPath(netWorkGraph,i);
	}
}

void OptimalPath::searchOptimalPath(LinkedGraph *netWorkGraph, int src_node){
	//step1:initialization
	int k=1;
	vector<int> src_child_node=netWorkGraph->findChildNode(src_node);
	double **temp_path=new double*[total_node+1];
	double **pre_temp_path=new double*[total_node+1];
	for(int i=1;i<total_node+1;i++){
		temp_path[i]=new double[total_node+1];
		pre_temp_path[i]=new double[total_node+1];
	}
	//memset(pre_arc,0,sizeof(pre_arc));
	for(int i=1;i<total_node+1;i++){
		for(int j=1;j<total_node+1;j++){
			pre_temp_path[i][j]=temp_path[i][j]=M;
			pre_arc[src_node][i][j]=0;
		}
	}
	for(unsigned int i=0;i<src_child_node.size();i++){
		pre_temp_path[src_node][src_child_node[i]]=temp_path[src_node][src_child_node[i]]=netWorkGraph->getDistance(src_node, src_child_node[i]);
		//pre_arc[src_node][src_node][src_child_node[i]]=0;
	}
	//step2:iterator
	while(k<total_node){
		k++;
		for(int i=1;i<total_node+1;i++){
			vector<int> i_child_node=netWorkGraph->findChildNode(i);
			for(unsigned int j0=0;j0<i_child_node.size();j0++){
				int j=i_child_node[j0];
				if(netWorkGraph->getParentNumber(i)){
					vector<int> i_parent_node=netWorkGraph->findParentNode(i);
					double min_value=pre_temp_path[i_parent_node[0]][i]+netWorkGraph->getDelay(i_parent_node[0], i, j)+netWorkGraph->getDistance(i,j);
					int min_i_parent_node=i_parent_node[0];
					for(unsigned int t=1; t<i_parent_node.size();t++){
						double temp_value=pre_temp_path[i_parent_node[t]][i]+netWorkGraph->getDelay(i_parent_node[t], i, j)+netWorkGraph->getDistance(i,j);
						if(min_value>temp_value){
							min_value=temp_value;
							min_i_parent_node=i_parent_node[t];
						}
					}
					if(pre_temp_path[i][j]>min_value){
						temp_path[i][j]=min_value;
						pre_arc[src_node][i][j]=min_i_parent_node;
					}
				}
			}
		}
		bool flag=1;
		for(int i=1; i<total_node+1;i++){
			for(int j=1; j<total_node+1; j++){
				if(pre_temp_path[i][j]!=temp_path[i][j]){
					flag=0;
					break;
				}
			}
			if(!flag)break;
		}
		if(flag)break;
		else{
			for(int i=1;i<total_node+1;i++){
				for(int j=1;j<total_node+1;j++){
					pre_temp_path[i][j]=temp_path[i][j];
				}
			}
		}
	}
	for(int j=1; j<total_node+1; j++){
		if(j==src_node)continue;
		pre_node[src_node][j]=1;
		shortest_path_length[src_node][j]=temp_path[1][j];
		for(int i=2; i<total_node+1; i++){
			if(shortest_path_length[src_node][j]>temp_path[i][j]){
				shortest_path_length[src_node][j]=temp_path[i][j];
				pre_node[src_node][j]=i;
			}
		}
	}
	for(int i=1; i<total_node+1;i++){
		delete [] pre_temp_path[i];
	}
	delete [] pre_temp_path;
	for(int i=1; i<total_node+1;i++){
		delete [] temp_path[i];
	}
	delete [] temp_path;
}

list<int> OptimalPath::getOptimalPath(int src, int dst){
	if(src>0 && src<=total_node && dst>0 && dst<=total_node){
		list<int> ret;
		if(getShortestPathLength(src, dst)<M){
			ret.push_front(dst);
			int temp0=pre_node[src][dst];
			ret.push_front(temp0);
			int temp1=pre_arc[src][temp0][dst];
			while(temp1!=0){
				ret.push_front(temp1);
				int temp2=temp1;
				temp1=pre_arc[src][temp1][temp0];
				temp0=temp2;				
			}
		}
		return ret;
	}
	else{
		cout<<"Out of ENode bound"<<endl;
		exit(1);
	}	
}

void OptimalPath::displayOptimalPath(int src, int dst){
	list<int> ret=getOptimalPath(src,dst);
	if(ret.size()){
		list<int>::iterator k;
		cout<< *ret.begin();
		for(k=++ret.begin();k!=ret.end();k++)
			cout<<"->"<<*k;
		cout<<endl;
	}else{
		cout<<"impossible path"<<endl;
	}
}

double OptimalPath::getShortestPathLength(int src, int dst){
	if(src>0 && src<=total_node && dst>0 && dst<=total_node)
		return shortest_path_length[src][dst];
	else{
		cout<<"Out of ENode bound"<<endl;
		exit(1);
	}
}
