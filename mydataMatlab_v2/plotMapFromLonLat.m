function [ info ] = plotMapFromLonLat( earthCoordinate )
%PLOTMAPFROMLONLAT Summary of this function goes here
%   Detailed explanation goes here
%�������ܣ����ݴ�����껭��·������ͼ
%����˵����coordinate�ǽڵ����������x�Ǿ��ȣ�y��γ��
ss=size(earthCoordinate);
node_num=ss(1);
for i=1:node_num
    plot(earthCoordinate(i,1)*1.5,earthCoordinate(i,2),'o');
    text(earthCoordinate(i,1)*1.5,earthCoordinate(i,2),num2str(i));
    hold on;
end
info=0;
end

