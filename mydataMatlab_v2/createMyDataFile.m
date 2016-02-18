function [ info, turnBehavior,travelTime, turnDelay ] = createMyDataFile( nodeLocationsFile, segmentDistanceFile )
%CREATEMYDATAFILE Summary of this function goes here
%   Detailed explanation goes here
% 功能:生成Mydata.txt文件，其数据格式见data说明.txt
% 输入：我的地点坐标文件，默认为'nodeLocations.txt'，从Google Earth在标记得到生成kml文件，然后转成txt文件，标记关键词为'nodeLocations#'
%      路段长度及节点相邻情况，数据格式为'节点1 节点2 路段距离'，默认名为'SegmentDistance.txt'
% 输出：mydata.txt文件，按照data说明.txt格式.
% [info, turnBehavior,travelTime, turnDelay] = createMyDataFile( 'nodeLocations.txt', 'SegmentDistance.txt' );

%----------------步骤说明_start---------------------------------------------------------------
% step1：参数设置，统一前缀p_
% step2：变量声明，包括转向行为，行程时间，转向延误，节点相邻情况，道路等级默认-1，一些中间变量(统一前缀tmp_)等
% step3：获得节点经纬度，两个节点的连接情况，不相邻值为0，相邻值为路段长度
% step4：获得路段转向行为，0为不能转向，1，左转，2，直行，3，右转，4，掉头，转向延误，0为没有转向
% step5: 按格式书写mydata.txt文件
%----------------步骤说明_end----------------------------------------------------------------

%--------------step1_start----------------------------------------------------
earthCoordinate = readNodeLocations(nodeLocationsFile);
fnl = fopen('EarthCoordinate.txt','r');
p_totalNodes = fscanf(fnl,'%d',[1,1]);%总节点个数
fclose(fnl);
p_avgV = 7;   %平均速度10m/s;
%p_delayTimeRateOfTravelTime = 1;
%p_leftDelayRate = 0.4 ;
%p_rightDelayRate = 0.1 ;
%p_stratghtDelayRate = 0.2 ;
info = 0;
%--------------step1_end------------------------------------------------------

%--------------step2_start----------------------------------------------------
turnBehavior = zeros(p_totalNodes, p_totalNodes, p_totalNodes);%转向行为
turnDelay = zeros(p_totalNodes, p_totalNodes, p_totalNodes);%转向延误
travelTime = zeros(p_totalNodes, p_totalNodes);%行程时间
segmentGrade = -1*ones(p_totalNodes,p_totalNodes);%道路等级
adjvexNum = zeros(1,p_totalNodes);
% earthCoordinate = zeros(p_totalNum,2);%大地坐标
tmp_planeCoordinate = zeros(p_totalNodes,2);%平面坐标
%--------------step2_end------------------------------------------------------

%--------------step3_start----------------------------------------------------
% tmp_planeCoordinate = coord_trans('EarthCoordinate.txt','r');
tmp_planeCoordinate(:,1) = earthCoordinate(:,1)*1.5;
tmp_planeCoordinate(:,2) = earthCoordinate(:,2);
fsd = fopen(segmentDistanceFile, 'r');
while ~feof(fsd)
    tmp_segInfo = fscanf(fnl,'%d',[1,3]);
    travelTime(tmp_segInfo(1),tmp_segInfo(2))=tmp_segInfo(3)/p_avgV;
    travelTime(tmp_segInfo(2),tmp_segInfo(1))=tmp_segInfo(3)/p_avgV;
    adjvexNum(tmp_segInfo(1)) = adjvexNum(tmp_segInfo(1))+1;
    adjvexNum(tmp_segInfo(2)) = adjvexNum(tmp_segInfo(2))+1;
end
fclose(fsd);
%--------------step3_end------------------------------------------------------

%--------------step4_start----------------------------------------------------
for i=1:p_totalNodes
    for j = 1:p_totalNodes
        if travelTime(i,j) ~= 0
            for k = 1:p_totalNodes
                if travelTime(j,k) == 0
                    continue;
                end
                if i == k
                    turnBehavior(i,j,k) = 4;
                    %turnDelay(i,j,k) = min(max(travelTime(i,j)*p_delayTimeRateOfTravelTime*p_leftDelayRate, 5), 30);
                    turnDelay(i,j,k)=99999;    %掉头延误99999s
                    continue;
                end
                tmp_vectorA = [tmp_planeCoordinate(j,1)-tmp_planeCoordinate(i,1),tmp_planeCoordinate(j,2)-tmp_planeCoordinate(i,2)];
                tmp_vectorB = [tmp_planeCoordinate(k,1)-tmp_planeCoordinate(j,1),tmp_planeCoordinate(k,2)-tmp_planeCoordinate(j,2)];
                T = turnDirection(tmp_vectorA,tmp_vectorB);
                turnBehavior(i,j,k) = T;
                if T == 1
                    %turnDelay(i,j,k) = min(max(travelTime(i,j)*p_delayTimeRateOfTravelTime*p_leftDelayRate, 5), 30);
                    turnDelay(i,j,k) =65;    %左转延误200s
                elseif T == 2
                    %turnDelay(i,j,k) = min(max(travelTime(i,j)*p_delayTimeRateOfTravelTime*p_stratghtDelayRate, 5), 30);
                    turnDelay(i,j,k) =25;      %直行延误10s
                elseif T == 3
                    %turnDelay(i,j,k) = min(max(travelTime(i,j)*p_delayTimeRateOfTravelTime*p_rightDelayRate,5),30);
                    turnDelay(i,j,k) =5;       %右转延误0s
                end
            end
        end
    end
end
%--------------step4_end------------------------------------------------------

%--------------step5_start----------------------------------------------------
fout = fopen('myDataFromMatlab.txt', 'w');
fprintf(fout,'%d\n',p_totalNodes);
for i=1:p_totalNodes
    fprintf(fout,'%d %f %f %d ',i, earthCoordinate(i,1), earthCoordinate(i,2), adjvexNum(i));
    tmp_adjvexNumi = adjvexNum(i);
    for j=1:p_totalNodes
        if travelTime(i,j) ~= 0
            fprintf(fout,'%d %f %d %d ',j, travelTime(i,j), segmentGrade(i,j), adjvexNum(j));
            tmp_adjvexNumj = adjvexNum(j);
            for k=1:p_totalNodes
                if travelTime(j,k) == 0
                    continue;
                end
                fprintf(fout,'%f',turnDelay(i,j,k));
                tmp_adjvexNumj = tmp_adjvexNumj - 1;
                if(tmp_adjvexNumj ~= 0 || (tmp_adjvexNumi ~= 1 && tmp_adjvexNumj == 0))
                    fprintf(fout, ' ');
                end
            end
            if tmp_adjvexNumj ~= 0
                display('error: wrong value nextVertexNum');
                info = -1;
                return;
            end
            tmp_adjvexNumi = tmp_adjvexNumi - 1;
        end
    end
    if(i < p_totalNodes)
        fprintf(fout,'\n');
    end
    if tmp_adjvexNumj ~= 0
        display('error: wrong value adjvexNum');
        info = -1;
        return;
    end
end
fclose(fout);
%--------------step5_end------------------------------------------------------

end

