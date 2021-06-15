clc;
clear all;
close all;

% (1) Read 3D Data %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

FID = fopen('D:\output.txt', 'rt');
DATA = fscanf(FID, '%f', [3, Inf]);
fclose(FID);

% (2) Extract Origin, Vertices and Normals %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

ORG = DATA(:,1);

DATA = DATA(:,2:end);
count = floor(size(DATA,2)/2);

VTX = DATA(:,1:count);
NRM = DATA(:,count+1:end);

clear('DATA');

% (3) Interleave Vertices and Normals %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

NX = [VTX(1,:); NRM(1,:)];
NY = [VTX(2,:); NRM(2,:)];
NZ = [VTX(3,:); NRM(3,:)];

% (4) 3D Plot %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

figure(1);
grid on;
hold on;
plot3(ORG(1), ORG(2), ORG(3), 'ro');
plot3(VTX(1,:), VTX(2,:), VTX(3,:), 'bo');
plot3(NX, NY, NZ, 'g-');
hold off;
