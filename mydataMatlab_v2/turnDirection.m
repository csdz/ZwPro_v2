function [T] = turnDirection(vectorA,vectorB)
%输入说明：vectorA,vectorB分别是要进行转向行为判断的前弧向量和后弧向量
%函数功能：实现对前弧向量vectorA到后弧向量vectorB的转向行为判断,T=1左转，T=2直行，T=3右转

e=1e-6;
xa=vectorA(1);   
ya=vectorA(2);
xb=vectorB(1);
yb=vectorB(2);
%计算前弧向量vectorA和后弧向量vectorB之间的夹角
angleInter=acos((xa*xb+ya*yb)/(sqrt(xa^2+ya^2)*sqrt(xb^2+yb^2)));         

%计算前弧向量vectorA到后弧向量vectorB之间的旋转角
if xa*yb-xb*ya>=0 
    angleRotation=angleInter;
else
    angleRotation=2*pi-angleInter;
end

%根据旋转角判断转向行为
if (angleRotation-pi)<e && (angleRotation-(pi/4))>e
    T=1;
elseif (angleRotation-(7*pi/4))<e && (angleRotation-pi)>e
    T=3;
else
    T=2;
end
end

