function [output] = plotMap(coordinate)
%函数功能：根据平面坐标画出路网抽象图
%输入说明：coordinate是节点平面坐标矩阵，x是横轴，y是纵轴
ss=size(coordinate);
node_num=ss(1);
for i=1:node_num
    plot(coordinate(i,1),coordinate(i,2),'o');
    text(coordinate(i,1),coordinate(i,2),num2str(i));
    hold on;
end
D=zeros(node_num);
for i=1:node_num
    for j=i+1:node_num
        D(i,j)=sqrt((coordinate(i,1)-coordinate(j,1))^2+(coordinate(i,2)-coordinate(j,2))^2);
    end
end
output = D;
end

