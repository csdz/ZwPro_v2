function [ info ] = createMyVarietyDataAllTime( nodeLocationsFile, segmentDistanceFile)
%CREATEMYDATAFILEALLTIME Summary of this function goes here
%   Detailed explanation goes here
% 功能:生成t_minutes*i + "_minutesVarietyData.txt"文件，其数据格式见data说明.txt
% 输入：我的地点坐标文件，默认为'nodeLocations.txt'，从Google Earth在标记得到生成kml文件，然后转成txt文件，标记关键词为'nodeLocations#'
%      路段长度及节点相邻情况，数据格式为'节点1 节点2
%      路段距离'，默认名为'SegmentDistance.txt'
% accident_intersection = [49];
% [ info ] =  createMyVarietyDataAllTime( 'nodeLocations.txt', 'SegmentDistance.txt');
% step1：初始化定义
% step2：定义accident_intersection, 生成文件

%-------------------------step1_start------------------------
info = 0;
% make_accident = 2;
% dismiss_accident = -2/3;
time_interval = 5;%每隔五分钟输出一份文件；
file_count = 10;%共输出10份文件，时间系列为5min, 10min, 15min,..., 50min
suffix = '_minutesVarietyData.txt' ;%文件名的后缀格式
intersection = cell(file_count, 2);%初始化拥堵路口


%-------------------------------输入：需要改变的路口--------------------------------------
%  1.intersection(X,:) = {[N1,N2,N3], [N4]};表示第5*X分钟，N1、N2、N3路口拥堵，N4路口拥堵消散
%  2.如果要维持现状，则什么都不写。

intersection(1,:) = {[21,22,33,34,45,50,75,76,77,78,79,80],[]};
intersection(2,:) = {[],[75,76,77,78,79,80]};
intersection(3,:) = {[75,76,77,78,79,80],[]};
%------------------------step1_end-----------------------------

%-------------------------step2_start------------------------
for i = 1:file_count
    filename = num2str(time_interval*i);
    filename = strcat(filename, suffix);
    m_info =  createMyVarietyData(nodeLocationsFile, segmentDistanceFile, intersection(i,:), filename);
    if(m_info == -1)
        info = -1;
        return;
    end
end
%------------------------step2_end-----------------------------
