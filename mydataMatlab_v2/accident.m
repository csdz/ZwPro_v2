function [travelTimeVary,turnDelayVary]=accident(travelTime,turnDelay)
%travelTime,turnDelay�ֱ�Ϊ·���г�ʱ�����ͽ����ת���������

%-------------------�ֶ�����ͻ���¹ʾ���-------------------------------
m=3;  %����ͻ���¹ʵ�·�θ���
n=1;  %����ͻ���¹ʵĽ���ڸ���
road_emergency=zeros(m,3);   %����ͻ���¹ʵ�·�νڵ�ź���Ӧ�г�ʱ��仯��
inter_emergency=zeros(n,4);   %����ͻ���¹ʵĽ���ڽڵ�ź���Ӧת������仯��
road_emergency=[46 49 2;49 52 2;52 54 2];
inter_emergency=[46 49 50 2];
%--------------------------^-^----------------------------------------
for i=1:m
    r=road_emergency(i,1);   %·�����
    s=road_emergency(i,2);   %·���յ�
    value=road_emergency(i,3);   %�仯��
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
