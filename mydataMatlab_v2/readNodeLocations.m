function [ nodeLocations ] = readNodeLocations( filein )
%READNODELOCATIONS Summary of this function goes here
%   Detailed explanation goes here
% 输入kml文件名，输出节点经纬度，维度n*2，n为节点个数，共2列，第一列为经度，第二列纬度
nodeLocations=zeros(80,2);
fin=fopen(filein,'r');
while ~feof(fin)
    tline=fgetl(fin);
    if findstr(tline, 'nodeLocation')
        d1=str2double(tline(23));
        if tline(24) ~= '<'
           d0 = str2double(tline(24));
           nodeIndex = d1*10 + d0;
        else
           nodeIndex = d1;
        end
        tmpGetLine=fgetl(fin);
        while isempty(findstr(tmpGetLine,'coordinates'))
            tmpGetLine = fgetl(fin);
        end
        c2 = findstr(tmpGetLine,'>');
        c3 = findstr(tmpGetLine,',');
        longitudeStartIndex = c2(1)+1;
        longitudeEndIndex = c3(1)-1;
        latitudeStartIndex = c3(1)+1;
        latitudeEndIndex = c3(2)-1;
        lonstr = tmpGetLine(longitudeStartIndex:longitudeEndIndex);
        latstr = tmpGetLine(latitudeStartIndex:latitudeEndIndex);
        nodeLocations(nodeIndex,1) = str2double(lonstr);
        nodeLocations(nodeIndex,2) = str2double(latstr);
    end
    
end
fclose(fin);

totalNodes =80;
fout=fopen('EarthCoordinate.txt','w');
fprintf(fout,'%d\n',totalNodes);
for i=1:totalNodes
    fprintf(fout,'%f %f\n',nodeLocations(i,1),nodeLocations(i,2));
end
fclose(fout);

end

