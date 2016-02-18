function [ planeCoordinate ] = coord_trans(filename,op)
%函数功能：实现大地经纬度坐标到高斯-克吕格平面坐标的转换
%输入说明：filename为输入文件名；op为文件读写方式；a为椭球长半轴的长度，单位m；e1、e2分别为地球参考椭球的第一、第二偏心率；longiCen为中央经线经度，单位为度
%输出说明：x,y为高斯-克吕格平面坐标，特别声明：x表示纵轴，y表示横轴，纵轴x为由赤道算起的自然值，横轴y由中央经线算起，向东为正。planeCoordinate将x,y对调，x表示横轴，y表示纵轴
format long
%------------------a,e1,e2,longiCen_start---------------
%WGS84版本
a = 6378137;
e1 = 0.0066943799013;
e2 = 0.00673949674227;
longiCen = 120;
%-----------------a,e1,e2,longiCen_end------------------
%读取大地经纬度坐标信息
fcoord=fopen(filename,op);  
node_num = fscanf(fcoord,'%d',[1,1]);
longitude=zeros(1,node_num);  %longitude为经度数组
latitude=zeros(1,node_num);   %latitude为纬度数组
for i=1:node_num
    longitude(i)=fscanf(fcoord,'%f',[1,1]);
    latitude(i)=fscanf(fcoord,'%f',[1,1]); 
end
fclose(fcoord);


%经纬度单位转换为弧度
longiCen_rad=longiCen*pi/180;
longitude_rad=longitude*pi/180;
latitude_rad=latitude*pi/180;
%利用高斯正解公式进行坐标转换
C1=1+(3/4)*(e1^2)+(45/64)*(e1^4)+(175/256)*(e1^6)+(11025/16384)*(e1^8);
C2=(3/4)*(e1^2)+(15/16)*(e1^4)+(525/512)*(e1^6)+(2205/2048)*(e1^8);
C3=(15/64)*(e1^4)+(105/256)*(e1^6)+(2205/4096)*(e1^8);
C4=(35/512)*(e1^6)+(315/2048)*(e1^8);
C5=(315/131072)*(e1^8);
x=zeros(1,node_num);
y=zeros(1,node_num);
for j=1:node_num
    delta_longi=longitude_rad(j)-longiCen_rad;
    t=tan(latitude_rad(j));
    eta2=(e2)^2*(cos(latitude_rad(j))^2);   %eta的平方
    X=a*(1-e1^2)*(C1*latitude_rad(j)-(1/2)*C2*sin(2*latitude_rad(j))+(1/4)*C3*sin(4*latitude_rad(j))-(1/6)*C4*sin(6*latitude_rad(j))+C5*sin(8*latitude_rad(j)));
    N=a/(sqrt(delta_longi-(e1^2)*(sin(latitude_rad(j))^2)));
    x(j)=X+(1/2)*N*sin(latitude_rad(j))*cos(latitude_rad(j))*(delta_longi^2)*(1+(1/12)*(delta_longi^2)*(cos(latitude_rad(j))^2)*(5-t^2+9*eta2+4*(eta2^2))+(1/360)*(delta_longi^4)*(cos(latitude_rad(j))^4)*(61-58*t^2+t^4));
    y(j)=N*cos(latitude(j))*delta_longi*(1+(1/6)*(delta_longi^2)*(cos(latitude(j))^2)*(1-t^2+eta2)+(1/120)*(delta_longi^4)*(cos(latitude(j))^4)*(5-18*t^2+t^4-14*eta2-58*eta2*t^2));
end

planeCoordinate = zeros(node_num,2);
planeCoordinate(:,2) = x;
planeCoordinate(:,1) = y;
end

