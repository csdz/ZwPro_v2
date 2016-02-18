/*
*  optimalPath.h
*
*  Create on : 2013-9-3
*     Author : wagnsn1989
*    Version : 1.0
*/

#ifndef OPTIMALPATH_H_
#define OPTIMALPATH_H_

#include "optimalPath.h"
#include "delayTrNet.h"
#include <vector>
using namespace std;

class OptimalPath{
private:
	int total_node;
	//int **optomal_path_length;
	double **shortest_path_length;
	int ***pre_arc;
	int **pre_node;
	void searchOptimalPath(LinkedGraph *netWorkGraph, int src_node);
public:
	OptimalPath(LinkedGraph *netWorkGraph);
	list<int> getOptimalPath(int src_node, int dst_node);
	void displayOptimalPath(int src_node, int dst_node);
	double getShortestPathLength(int src_node, int dst_node);
};

#endif