clc;
clear all;
close all;

FID = fopen('D:\output.txt', 'rt');
DATA = fscanf(FID, '%f', [3, Inf]);
fclose(FID);

ORIGIN = DATA(:,1);
SPHERE = DATA(:,2:end);

clear('DATA');

figure(1);
grid on;
hold on;
plot3(ORIGIN(1), ORIGIN(2), ORIGIN(3), 'ro');
plot3(SPHERE(1,:), SPHERE(2,:), SPHERE(3,:), 'bo');
hold off;
