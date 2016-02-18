function [travelTimeVary,turnDelayVary]=accident(travelTime,turnDelay)
%travelTime,turnDelay分别为路段行程时间矩阵和交叉口转向延误矩阵

%-------------------手动输入突发事故矩阵-------------------------------
m=3;  %发生突发事故的路段个数
n=1;  %发生突发事故的交叉口个数
road_emergency=zeros(m,3);   %发生突发事故的路段节点号和相应行程时间变化率
inter_emergency=zeros(n,4);   %发生突发事故的交叉口节点号和相应转向延误变化率
road_emergency=[46 49 2;49 52 2;52 54 2];
inter_emergency=[46 49 50 2];
%--------------------------^-^----------------------------------------
for i=1:m
    r=road_emergency(i,1);   %路段起点
    s=road_emergency(i,2);   %路段终点
    value=road_emergency(i,3);   %变化率
    travelTime(r,s)=value;
end
for j=1:n
    r=inter_emergency(j,1);    
    s=inter_emergency(j,2);
    t=inter_emergency(j,3);
    value=inter_emergency(j,4);
    turnDelay(r,s,t)=value;
end
travelTimeVary=travelTime;
turnDelayVary=turnDelay;
end
