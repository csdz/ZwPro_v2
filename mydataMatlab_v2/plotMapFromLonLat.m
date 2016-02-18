function [ info ] = plotMapFromLonLat( earthCoordinate )
%PLOTMAPFROMLONLAT Summary of this function goes here
%   Detailed explanation goes here
%函数功能：根据大地坐标画出路网抽象图
%输入说明：coordinate是节点大地坐标矩阵，x是经度，y是纬度
ss=size(earthCoordinate);
node_num=ss(1);
for i=1:node_num
    plot(earthCoordinate(i,1)*1.5,earthCoordinate(i,2),'o');
    text(earthCoordinate(i,1)*1.5,earthCoordinate(i,2),num2str(i));
    hold on;
end
info=0;
end

