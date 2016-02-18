function [ info, turnBehavior,travelTime, turnDelay ] = createMyDataFile( nodeLocationsFile, segmentDistanceFile )
%CREATEMYDATAFILE Summary of this function goes here
%   Detailed explanation goes here
% ����:����Mydata.txt�ļ��������ݸ�ʽ��data˵��.txt
% ���룺�ҵĵص������ļ���Ĭ��Ϊ'nodeLocations.txt'����Google Earth�ڱ�ǵõ�����kml�ļ���Ȼ��ת��txt�ļ�����ǹؼ���Ϊ'nodeLocations#'
%      ·�γ��ȼ��ڵ�������������ݸ�ʽΪ'�ڵ�1 �ڵ�2 ·�ξ���'��Ĭ����Ϊ'SegmentDistance.txt'
% �����mydata.txt�ļ�������data˵��.txt��ʽ.
% [info, turnBehavior,travelTime, turnDelay] = createMyDataFile( 'nodeLocations.txt', 'SegmentDistance.txt' );

%----------------����˵��_start---------------------------------------------------------------
% step1���������ã�ͳһǰ׺p_
% step2����������������ת����Ϊ���г�ʱ�䣬ת�����󣬽ڵ������������·�ȼ�Ĭ��-1��һЩ�м����(ͳһǰ׺tmp_)��
% step3����ýڵ㾭γ�ȣ������ڵ�����������������ֵΪ0������ֵΪ·�γ���
% step4�����·��ת����Ϊ��0Ϊ����ת��1����ת��2��ֱ�У�3����ת��4����ͷ��ת������0Ϊû��ת��
% step5: ����ʽ��дmydata.txt�ļ�
%----------------����˵��_end----------------------------------------------------------------

%--------------step1_start----------------------------------------------------
earthCoordinate = readNodeLocations(nodeLocationsFile);
fnl = fopen('EarthCoordinate.txt','r');
p_totalNodes = fscanf(fnl,'%d',[1,1]);%�ܽڵ����
fclose(fnl);
p_avgV = 7;   %ƽ���ٶ�10m/s;
%p_delayTimeRateOfTravelTime = 1;
%p_leftDelayRate = 0.4 ;
%p_rightDelayRate = 0.1 ;
%p_stratghtDelayRate = 0.2 ;
info = 0;
%--------------step1_end------------------------------------------------------

%--------------step2_start----------------------------------------------------
turnBehavior = zeros(p_totalNodes, p_totalNodes, p_totalNodes);%ת����Ϊ
turnDelay = zeros(p_totalNodes, p_totalNodes, p_totalNodes);%ת������
travelTime = zeros(p_totalNodes, p_totalNodes);%�г�ʱ��
segmentGrade = -1*ones(p_totalNodes,p_totalNodes);%��·�ȼ�
adjvexNum = zeros(1,p_totalNodes);
% earthCoordinate = zeros(p_totalNum,2);%�������
tmp_planeCoordinate = zeros(p_totalNodes,2);%ƽ������
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
                    turnDelay(i,j,k)=99999;    %��ͷ����99999s
                    continue;
                end
                tmp_vectorA = [tmp_planeCoordinate(j,1)-tmp_planeCoordinate(i,1),tmp_planeCoordinate(j,2)-tmp_planeCoordinate(i,2)];
                tmp_vectorB = [tmp_planeCoordinate(k,1)-tmp_planeCoordinate(j,1),tmp_planeCoordinate(k,2)-tmp_planeCoordinate(j,2)];
                T = turnDirection(tmp_vectorA,tmp_vectorB);
                turnBehavior(i,j,k) = T;
                if T == 1
                    %turnDelay(i,j,k) = min(max(travelTime(i,j)*p_delayTimeRateOfTravelTime*p_leftDelayRate, 5), 30);
                    turnDelay(i,j,k) =65;    %��ת����200s
                elseif T == 2
                    %turnDelay(i,j,k) = min(max(travelTime(i,j)*p_delayTimeRateOfTravelTime*p_stratghtDelayRate, 5), 30);
                    turnDelay(i,j,k) =25;      %ֱ������10s
                elseif T == 3
                    %turnDelay(i,j,k) = min(max(travelTime(i,j)*p_delayTimeRateOfTravelTime*p_rightDelayRate,5),30);
                    turnDelay(i,j,k) =5;       %��ת����0s
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

