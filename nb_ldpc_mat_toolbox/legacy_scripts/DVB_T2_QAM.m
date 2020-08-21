
% DVB_T2_QAM.m
% author: Cédric Marchand
% construction of DVB-T2 QAM gray constellation 
% plot constellation and write constellation in a file

%clear all;
%clc;

function [constellation] = DVB_T2_QAM(QAM)

gray = @(x) xor(x, [ 0 x(1:end-1) ] ); %convert binary to gray

% % test gray
% for i=0:7
%    bin = dec2bin(i,4);
% bin = bin-'0'; 
% i
% gray(bin) 
%     
% end



%QAM=256;

log_QAM=log2(QAM);

constellation = zeros(QAM,2);

for dec=0:QAM-1

    bin = dec2bin(dec,log_QAM);
    %bin = fliplr(bin); % big indian to little indian
    bin = bin-'0'; %string to binary
    bin_Re= bin(1:2:end);
    bin_Im= bin(2:2:end);

    gray_Re = gray(bin_Re);
    %gray_Re = num2str(gray_Re);
    %gray_Re = bin2dec(gray_Re);

    bin_Re = num2str(bin_Re);
    bin_Re = bin2dec(bin_Re);


    gray_Im = gray(bin_Im);
    %gray_Im = num2str(gray_Im);
    %gray_Im = bin2dec(gray_Im);

    bin_Im = num2str(bin_Im);
    bin_Im = bin2dec(bin_Im);


    max=ones(1,log_QAM/2);
    max= num2str(max);
    max= bin2dec(max);
    Re = max - bin_Re*2;

    Im = max - bin_Im*2;

    symbol=zeros(1,log_QAM);
    symbol(1:2:end)=gray_Re;
    symbol(2:2:end)=gray_Im;

    symbol = fliplr(symbol); % big indian to little indian

    symbol = num2str(symbol);
    symbol = bin2dec(symbol);

    constellation(symbol+1,1)=Re;
    constellation(symbol+1,2)=Im;

end

% show constellation
figure(1);
scatter(constellation(:,1),constellation(:,2),'filled');

x=(0:QAM-1)';
x=dec2bin(x);
x = fliplr(x);  % big indian to little indian
text(constellation(:,1)+0.1, constellation(:,2), x );
xL = xlim;
yL = ylim;
line([0 0], yL);  %x-axis
line(xL, [0 0]);  %y-axis
x=-max:2:max;
text(x,zeros(1,sqrt(QAM)),'+','FontSize',14,'HorizontalAlignment','center');
text(x,zeros(1,sqrt(QAM)),num2str(x'),'VerticalAlignment','Top');

text(zeros(1,sqrt(QAM)),x,'+','FontSize',14,'HorizontalAlignment','center');
text(zeros(1,sqrt(QAM)),x,num2str(x'),'VerticalAlignment','Top');



%print contellation in a file
     fileID =fopen('constellation.txt','w');    
      fprintf(fileID,'table_%dQAM[%d][2]= \n ',QAM,QAM);
      fprintf(fileID,'{ \n ');
    for i=1:QAM
        fprintf(fileID,'{%d, ',constellation(i,1));
        fprintf(fileID,' %d}, \n',constellation(i,2));
    end
    fprintf(fileID,'} \n ');

end