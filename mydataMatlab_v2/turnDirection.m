function [T] = turnDirection(vectorA,vectorB)
%����˵����vectorA,vectorB�ֱ���Ҫ����ת����Ϊ�жϵ�ǰ�������ͺ�����
%�������ܣ�ʵ�ֶ�ǰ������vectorA��������vectorB��ת����Ϊ�ж�,T=1��ת��T=2ֱ�У�T=3��ת

e=1e-6;
xa=vectorA(1);   
ya=vectorA(2);
xb=vectorB(1);
yb=vectorB(2);
%����ǰ������vectorA�ͺ�����vectorB֮��ļн�
angleInter=acos((xa*xb+ya*yb)/(sqrt(xa^2+ya^2)*sqrt(xb^2+yb^2)));         

%����ǰ������vectorA��������vectorB֮�����ת��
if xa*yb-xb*ya>=0 
    angleRotation=angleInter;
else
    angleRotation=2*pi-angleInter;
end

%������ת���ж�ת����Ϊ
if (angleRotation-pi)<e && (angleRotation-(pi/4))>e
    T=1;
elseif (angleRotation-(7*pi/4))<e && (angleRotation-pi)>e
    T=3;
else
    T=2;
end
end

