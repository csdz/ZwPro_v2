function [ info ] = createMyVarietyDataAllTime( nodeLocationsFile, segmentDistanceFile)
%CREATEMYDATAFILEALLTIME Summary of this function goes here
%   Detailed explanation goes here
% ����:����t_minutes*i + "_minutesVarietyData.txt"�ļ��������ݸ�ʽ��data˵��.txt
% ���룺�ҵĵص������ļ���Ĭ��Ϊ'nodeLocations.txt'����Google Earth�ڱ�ǵõ�����kml�ļ���Ȼ��ת��txt�ļ�����ǹؼ���Ϊ'nodeLocations#'
%      ·�γ��ȼ��ڵ�������������ݸ�ʽΪ'�ڵ�1 �ڵ�2
%      ·�ξ���'��Ĭ����Ϊ'SegmentDistance.txt'
% accident_intersection = [49];
% [ info ] =  createMyVarietyDataAllTime( 'nodeLocations.txt', 'SegmentDistance.txt');
% step1����ʼ������
% step2������accident_intersection, �����ļ�

%-------------------------step1_start------------------------
info = 0;
% make_accident = 2;
% dismiss_accident = -2/3;
time_interval = 5;%ÿ����������һ���ļ���
file_count = 10;%�����10���ļ���ʱ��ϵ��Ϊ5min, 10min, 15min,..., 50min
suffix = '_minutesVarietyData.txt' ;%�ļ����ĺ�׺��ʽ
intersection = cell(file_count, 2);%��ʼ��ӵ��·��


%-------------------------------���룺��Ҫ�ı��·��--------------------------------------
%  1.intersection(X,:) = {[N1,N2,N3], [N4]};��ʾ��5*X���ӣ�N1��N2��N3·��ӵ�£�N4·��ӵ����ɢ
%  2.���Ҫά����״����ʲô����д��

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
