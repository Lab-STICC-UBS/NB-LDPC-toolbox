% build_GF.m
% Author: Cédric Marchand
% build finite field vectors

clear all;

%GF_size=2; primitive= [1 1 1]; %GF[4]
%GF_size=4; primitive= [1 1 0 0 1]; %GF[16], primitive 1+x+x^4
%GF_size=5; primitive= [1 0 1 0 0 1]; %GF[32]
GF_size=6; primitive= [1 1 0 0 0 0 1]; %GF[64]
%GF_size=8; primitive= [1 0 1 1 1 0 0 0 1]; %GF[256]
%GF_size=10; primitive=[1 0 0 1 0 0 0 0 0 0 1]; %GF[1024]
%GF_size=11; primitive=[1 0 1 0 0 0 0 0 0 0 0 1]; %GF[2048]
%GF_size=12; primitive=[1 1 0 0 1 0 1 0 0 0 0 0 1]; % GF[4048]

GF_vector= zeros(1,GF_size+1);
GF_vectors=zeros(2^GF_size,GF_size);
GF_vectors(2,1)=1;
GF_vector(1)=1;

gf2dec = zeros(2^GF_size,1,'uint16');
gf2dec(2)=1;

% build GF to binary vectors (GF_vectors)
% build GF to decimal (GF2dec)
for i=1:2^GF_size-2

    GF_vector=[0 GF_vector(1:end-1)]; %shif right (multiplication by x)

    if (GF_vector(end)==1) % compute modulo primitive
        GF_vector= xor(GF_vector,primitive);
    end
    
    temp = GF_vector(1:end-1);
    GF_vectors(i+2,:)=temp;
    temp2=flip(temp);
    temp3=num2str(temp2);
    gf2dec(i+2)=bin2dec(temp3);

end


%build binary to GF
[~,dec2gf] = sort(gf2dec);


%%
%build multiplication table GF
mult_table_gf = zeros(2^GF_size,2^GF_size,'uint16');
for i=2:2^GF_size
    mult_table_gf(2:end,i)=i-2:2^GF_size+i-4;
end
mult_table_gf(2:end,2:end) =mod(mult_table_gf(2:end,2:end),2^GF_size-1)+1;



%build division table GF
div_table_gf = zeros(2^GF_size,2^GF_size);
for i=2:2^GF_size
    div_table_gf(2:end,i)=-i+2:2^GF_size-i;
end
div_table_gf(2:end,2:end) =mod(div_table_gf(2:end,2:end),2^GF_size-1)+1;

%% build multiplication table decimal

mult_table_dec = zeros(2^GF_size,2^GF_size,'uint16');
for i=1:2^GF_size
    for j=1:2^GF_size
        mult_table_dec(gf2dec(i)+1,gf2dec(j)+1) = gf2dec(mult_table_gf(i,j)+1);
    end
end


% build division table decimal
div_table_dec = zeros(2^GF_size,2^GF_size,'uint16');
for i=1:2^GF_size
    for j=1:2^GF_size
        div_table_dec(gf2dec(i)+1,gf2dec(j)+1) = gf2dec(div_table_gf(i,j)+1);
    end
end


