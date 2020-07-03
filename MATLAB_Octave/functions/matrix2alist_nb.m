% matrix2alist_nb.m
% autheur: Cédric Marchand
% convert a Non-Binary parity matrix to the alist format, output in a text file


clear all;
clc;

H = load( 'matrix.txt');

%function [N,M,dvmax,dcmax,dv,dc,nlist,mlist] = matrix2alist_nb(H)

fileID =fopen('alist.txt','w');
fclose(fileID);


H_bin = H;
H_bin( H > -1 ) = 1;
H_bin( H<0 ) = 0;


[M,N] = size(H);
dc = sum(H_bin,2);
dv = sum(H_bin,1);
dcmax = max(dc);
dvmax = max(dv);

[nlist,~] = find(H_bin);
[mlist,~] = find(H_bin');

GF=64;

%write in a file named alist.txt
     fileID =fopen('alist.txt','at');    
      fprintf(fileID,'%d %d %d \n %d %d \n ',N,M,GF,dvmax,dcmax);
    for i=1:N
        fprintf(fileID,'%d ',dv(i));
    end
    fprintf(fileID,' \n ');
    for i=1:M
        fprintf(fileID,'%d ',dc(i));
    end
    fprintf(fileID,' \n ');
    k=0;
    for i=1:N
        for j=1:dv(i)
            fprintf(fileID,'%d ',nlist(k + j ));
            fprintf(fileID,'%d ',H( nlist(k + j  ),i));
        end
        k=k+dv(i);
        fprintf(fileID,' \n ');
    end
    
    k=0;
    for i=1:M
        for j=1:dc(i)
            fprintf(fileID,'%d ',mlist(k + j ));
            fprintf(fileID,'%d ',H( i, mlist(k + j  )));
        end
        k=k+dc(i);
        fprintf(fileID,' \n ');
    end

    
      fclose(fileID);   

%end