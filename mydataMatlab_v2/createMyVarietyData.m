function [ info, turnBehavior,travelTime, turnDelay ] = createMyVarietyData( nodeLocationsFile, segmentDistanceFile, accident_intersection, filename )
%CREATEMYDATAFILE Summary of this function goes here
%   Detailed explanation goes here
% ����:����Mydata.txt�ļ��������ݸ�ʽ��data˵��.txt
% ���룺�ҵĵص������ļ���Ĭ��Ϊ'nodeLocations.txt'����Google Earth�ڱ�ǵõ�����kml�ļ���Ȼ��ת��txt�ļ�����ǹؼ���Ϊ'nodeLocations#'
%      ·�γ��ȼ��ڵ�������������ݸ�ʽΪ'�ڵ�1 �ڵ�2
%      ·�ξ���'��Ĭ����Ϊ'SegmentDistance.txt',
%      ����ڼ��ϣ�accident_intersection, cell
%      sizeΪ1*2����һ��Ϊ�������Ϊ����ʾ����ӵ��make_accident = 2, ��ɢӵ��dismiss_accident =
%      -2/3;
%      �ļ�����filename;
% �����mydata.txt�ļ�������data˵��.txt��ʽ.
% accident_intersection = [49];
% [info, turnBehavior,travelTime, turnDelay] =  createMyVarietyData( 'nodeLocations.txt', 'SegmentDistance.txt', accident_intersection, filename);

%----------------����˵��_start---------------------------------------------------------------
% step1���������ã�ͳһǰ׺p_
% step2����������������ת����Ϊ���г�ʱ�䣬ת�����󣬽ڵ������������·�ȼ�Ĭ��-1��һЩ�м����(ͳһǰ׺tmp_)��
% step3����ýڵ㾭γ�ȣ������ڵ�����������������ֵΪ0������ֵΪ·�γ���
% step4�����·��ת����Ϊ��0Ϊ����ת��1����ת��2��ֱ�У�3����ת��4����ͷ��ת������0Ϊû��ת��
% step5: ����ʽ��дmydata.txt�ļ�
% step6: �����¹�
%----------------����˵��_end----------------------------------------------------------------

%--------------step1_start----------------------------------------------------
earthCoordinate = readNodeLocations(nodeLocationsFile);
fnl = fopen('EarthCoordinate.txt','r');
p_totalNodes = fscanf(fnl,'%d',[1,1]);%�ܽڵ����
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
turnBehavior = zeros(p_totalNodes, p_totalNodes, p_totalNodes);%ת����Ϊ
turnDelay = zeros(p_totalNodes, p_totalNodes, p_totalNodes);%ת������仯��
travelTime = zeros(p_totalNodes, p_totalNodes);%�г�ʱ��仯��
ifExistSegment=zeros(p_totalNodes, p_totalNodes);%�Ƿ����·��,0�����ڣ�1����
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
    ifExistSegment(tmp_segInfo(1),tmp_segInfo(2))=1;
    ifExistSegment(tmp_segInfo(2),tmp_segInfo(1))=1;
    travelTime(tmp_segInfo(1),tmp_segInfo(2))= min(max(-0.5, randn(1)/delta), 1);      %·�η��ñ仯��
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
                    turnDelay(i,j,k)=min(max(-0.5, randn(1)/delta), 1);    %��ͷ����仯��
                end
%                 if i == k
%                     turnBehavior(i,j,k) = 4;
%                     %turnDelay(i,j,k) = min(max(travelTime(i,j)*p_delayTimeRateOfTravelTime*p_leftDelayRate, 5), 30);
%                     if(any(accident_intersection == j))
%                         turnDelay = maxCostRate;
%                     else
%                         turnDelay(i,j,k)=min(max(-0.5, randn(1)/delta), 1);    %��ͷ����仯��
%                     end
%                     continue;
%                 end
%                 tmp_vectorA = [tmp_planeCoordinate(j,1)-tmp_planeCoordinate(i,1),tmp_planeCoordinate(j,2)-tmp_planeCoordinate(i,2)];
%                 tmp_vectorB = [tmp_planeCoordinate(k,1)-tmp_planeCoordinate(j,1),tmp_planeCoordinate(k,2)-tmp_planeCoordinate(j,2)];
%                 T = turnDirection(tmp_vectorA,tmp_vectorB);
%                 turnBehavior(i,j,k) = T;
%                 if T == 1
%                     %turnDelay(i,j,k) = min(max(travelTime(i,j)*p_delayTimeRateOfTravelTime*p_leftDelayRate, 5), 30);
%                     turnDelay(i,j,k) = min(max(-0.5, randn(1)/delta), 1);    %��ת����仯��
%                 elseif T == 2
%                     %turnDelay(i,j,k) = min(max(travelTime(i,j)*p_delayTimeRateOfTravelTime*p_stratghtDelayRate, 5), 30);
%                     turnDelay(i,j,k) =min(max(-0.5, randn(1)/delta), 1);      %ֱ������仯��
%                 elseif T == 3
%                     %turnDelay(i,j,k) = min(max(travelTime(i,j)*p_delayTimeRateOfTravelTime*p_rightDelayRate,5),30);
%                     turnDelay(i,j,k) =min(max(-0.5, randn(1)/delta), 1);       %��ת����仯��
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

