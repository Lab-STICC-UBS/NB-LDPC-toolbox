% alist2matrix_nb.m
% author: Cédric Marchand
% convert a alist format fileto Non-Binary parity matrix , output in a text file

clear all;
clc;




fileID =fopen('alist.txt');


N = fscanf(fileID,'%d',1);
M = fscanf(fileID,'%d',1);
GF = fscanf(fileID,'%d',1);
dvmax = fscanf(fileID,'%d',1);
dcmax = fscanf(fileID,'%d',1);

dc=zeros(1,M);
dv=zeros(1,N);

matrix=zeros(M,N);
matrix=matrix-1;
matrix2=matrix;

for i=1:N
   dv(i) = fscanf(fileID,'%d',1); 
end

for i=1:M
   dc(i) = fscanf(fileID,'%d',1); 
end

for i=1:N
    for j=1:dv(i)
       pos = fscanf(fileID,'%d',1); 
       value = fscanf(fileID,'%d',1);
       matrix(pos,i)=value;
    end
end


for i=1:M
    for j=1:dc(i)
       pos = fscanf(fileID,'%d',1); 
       value = fscanf(fileID,'%d',1);
       matrix2(i,pos)=value;
    end
end

noerror = isequal(matrix,matrix2);


fclose(fileID);




fileID =fopen('matrix.txt','w');


for i=1:M
    for j=1:N
        fprintf(fileID,'%d ',matrix(i,j)); 
    end
    fprintf(fileID,'; \n');
end



fclose(fileID);