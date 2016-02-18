function [ info, turnBehavior,travelTime, turnDelay ] = createMyVarietyData( nodeLocationsFile, segmentDistanceFile, accident_intersection, filename )
%CREATEMYDATAFILE Summary of this function goes here
%   Detailed explanation goes here
% 功能:生成Mydata.txt文件，其数据格式见data说明.txt
% 输入：我的地点坐标文件，默认为'nodeLocations.txt'，从Google Earth在标记得到生成kml文件，然后转成txt文件，标记关键词为'nodeLocations#'
%      路段长度及节点相邻情况，数据格式为'节点1 节点2
%      路段距离'，默认名为'SegmentDistance.txt',
%      交叉口集合，accident_intersection, cell
%      size为1*2，第一列为交叉口行为，表示产生拥堵make_accident = 2, 消散拥堵dismiss_accident =
%      -2/3;
%      文件名：filename;
% 输出：mydata.txt文件，按照data说明.txt格式.
% accident_intersection = [49];
% [info, turnBehavior,travelTime, turnDelay] =  createMyVarietyData( 'nodeLocations.txt', 'SegmentDistance.txt', accident_intersection, filename);

%----------------步骤说明_start---------------------------------------------------------------
% step1：参数设置，统一前缀p_
% step2：变量声明，包括转向行为，行程时间，转向延误，节点相邻情况，道路等级默认-1，一些中间变量(统一前缀tmp_)等
% step3：获得节点经纬度，两个节点的连接情况，不相邻值为0，相邻值为路段长度
% step4：获得路段转向行为，0为不能转向，1，左转，2，直行，3，右转，4，掉头，转向延误，0为没有转向
% step5: 按格式书写mydata.txt文件
% step6: 增加事故
%----------------步骤说明_end----------------------------------------------------------------

%--------------step1_start----------------------------------------------------
earthCoordinate = readNodeLocations(nodeLocationsFile);
fnl = fopen('EarthCoordinate.txt','r');
p_totalNodes = fscanf(fnl,'%d',[1,1]);%总节点个数
fclose(fnl);
%p_delayTimeRateOfTravelTime = 1;
%p_leftDelayRate = 0.4 ;
%p_rightDelayRate = 0.1 ;
%p_stratghtDelayRate = 0.2 ;
info = 0;
delta=10;
make_accident = 2;
dismiss_accident = -2/3;
%--------------step1_end------------------------------------------------------

%--------------step2_start----------------------------------------------------
turnBehavior = zeros(p_totalNodes, p_totalNodes, p_totalNodes);%转向行为
turnDelay = zeros(p_totalNodes, p_totalNodes, p_totalNodes);%转向延误变化率
travelTime = zeros(p_totalNodes, p_totalNodes);%行程时间变化率
ifExistSegment=zeros(p_totalNodes, p_totalNodes);%是否存在路段,0不存在，1存在
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
    ifExistSegment(tmp_segInfo(1),tmp_segInfo(2))=1;
    ifExistSegment(tmp_segInfo(2),tmp_segInfo(1))=1;
    travelTime(tmp_segInfo(1),tmp_segInfo(2))= min(max(-0.5, randn(1)/delta), 1);      %路段费用变化率
    travelTime(tmp_segInfo(2),tmp_segInfo(1))= min(max(-0.5, randn(1)/delta), 1);
    if(any(accident_intersection{1,1} == tmp_segInfo(1)))
        travelTime(tmp_segInfo(2),tmp_segInfo(1))= make_accident;
    end
    if(any(accident_intersection{1,2} == tmp_segInfo(1)))
        travelTime(tmp_segInfo(2),tmp_segInfo(1))= dismiss_accident;
    end
    if(any(accident_intersection{1,1} == tmp_segInfo(2)))
        travelTime(tmp_segInfo(1),tmp_segInfo(2))= make_accident;
    end
    if(any(accident_intersection{1,2} == tmp_segInfo(2)))
        travelTime(tmp_segInfo(1),tmp_segInfo(2))= dismiss_accident;
    end
    adjvexNum(tmp_segInfo(1)) = adjvexNum(tmp_segInfo(1))+1;
    adjvexNum(tmp_segInfo(2)) = adjvexNum(tmp_segInfo(2))+1;
end
fclose(fsd);
%--------------step3_end------------------------------------------------------

%--------------step4_start----------------------------------------------------
for i=1:p_totalNodes
    for j = 1:p_totalNodes
        if ifExistSegment(i,j) ~= 0
            for k = 1:p_totalNodes
                if ifExistSegment(j,k) == 0
                    continue;
                end
                if(any(accident_intersection{1,1} == j))
                    turnDelay(i,j,k) = make_accident;
                elseif(any(accident_intersection{1,2} == j))
                    turnDelay(i,j,k) = dismiss_accident;
                else
                    turnDelay(i,j,k)=min(max(-0.5, randn(1)/delta), 1);    %掉头延误变化率
                end
%                 if i == k
%                     turnBehavior(i,j,k) = 4;
%                     %turnDelay(i,j,k) = min(max(travelTime(i,j)*p_delayTimeRateOfTravelTime*p_leftDelayRate, 5), 30);
%                     if(any(accident_intersection == j))
%                         turnDelay = maxCostRate;
%                     else
%                         turnDelay(i,j,k)=min(max(-0.5, randn(1)/delta), 1);    %掉头延误变化率
%                     end
%                     continue;
%                 end
%                 tmp_vectorA = [tmp_planeCoordinate(j,1)-tmp_planeCoordinate(i,1),tmp_planeCoordinate(j,2)-tmp_planeCoordinate(i,2)];
%                 tmp_vectorB = [tmp_planeCoordinate(k,1)-tmp_planeCoordinate(j,1),tmp_planeCoordinate(k,2)-tmp_planeCoordinate(j,2)];
%                 T = turnDirection(tmp_vectorA,tmp_vectorB);
%                 turnBehavior(i,j,k) = T;
%                 if T == 1
%                     %turnDelay(i,j,k) = min(max(travelTime(i,j)*p_delayTimeRateOfTravelTime*p_leftDelayRate, 5), 30);
%                     turnDelay(i,j,k) = min(max(-0.5, randn(1)/delta), 1);    %左转延误变化率
%                 elseif T == 2
%                     %turnDelay(i,j,k) = min(max(travelTime(i,j)*p_delayTimeRateOfTravelTime*p_stratghtDelayRate, 5), 30);
%                     turnDelay(i,j,k) =min(max(-0.5, randn(1)/delta), 1);      %直行延误变化率
%                 elseif T == 3
%                     %turnDelay(i,j,k) = min(max(travelTime(i,j)*p_delayTimeRateOfTravelTime*p_rightDelayRate,5),30);
%                     turnDelay(i,j,k) =min(max(-0.5, randn(1)/delta), 1);       %右转延误变化率
%                 end
            end
        end
    end
end
%--------------step4_end------------------------------------------------------

%--------------step6_start----------------------------------------------------
% [travelTime, turnDelay]=accident(travelTime,turnDelay);

%----------------step6_end-----------------------------------------------------


%--------------step5_start----------------------------------------------------
%fout = fopen('myDataFromMatlab.txt', 'w');
fout = fopen(filename, 'w');
fprintf(fout,'%d\n',p_totalNodes);
for i=1:p_totalNodes
    %fprintf(fout,'%d %f %f %d ',i, earthCoordinate(i,1), earthCoordinate(i,2), adjvexNum(i));
    fprintf(fout,'%d %d ',i, adjvexNum(i));
    tmp_adjvexNumi = adjvexNum(i);
    for j=1:p_totalNodes
        if ifExistSegment(i,j) ~= 0
            %fprintf(fout,'%d %f %d %d ',j, travelTime(i,j), segmentGrade(i,j), adjvexNum(j));
            fprintf(fout,'%d %f %d ',j, travelTime(i,j), adjvexNum(j));
            tmp_adjvexNumj = adjvexNum(j);
            for k=1:p_totalNodes
                if ifExistSegment(j,k) == 0
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
        fprintf(fout, '\n');
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

