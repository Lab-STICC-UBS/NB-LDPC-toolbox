% build_GF.m
% Author :  Cédric Marchand
% build finite field vectors
% GF symbols correspondance : 
% 0        = > 1      = > 1
% alpha_k  = > k + 2  = > natural value + 1, for k > =  0
% Example : 
% q = 8
% GF       = > Code Index  = > Natural value
% 0_gf     = > 1           = > 1
% alpha_0  = > 2           = > 2
% alpha_1  = > 3           = > 3
% alpha_2  = > 4           = > 5
% alpha_3  = > 5           = > 5
% alpha_4  = > 6           = > 7
% alpha_5  = > 7           = > 8
% alpha_6  = > 8           = > 6

function buildGF(p_gf, primitive)
%BUILDGF
%   Build GF tools for code management

i_p = inputParser;

i_p.addRequired('p_gf', @(x) int64(x) == x);
i_p.addOptional('primitive', []);

i_p.parse(p_gf, primitive);


if ~isempty(i_p.Results.primitive)
    if length(i_p.Results.primitive) ~=  i_p.Results.p_gf + 1
        error('primitive has not the right length. %d instead of %d.', length(i_p.Results.primitive), i_p.Results.p_gf + 1);
    else
        for i = 1  : length(i_p.Results.primitive)
            cond = (0 ==  i_p.Results.primitive(i)) || (i_p.Results.primitive(i) == 1);
            if ~cond
                 error('primitive should contains "0" or "1".');
            end
        end
    end  
else
    switch i_p.Results.p_gf
        case 2
            primitive_ = [1 1 1]; %GF[4]
        case 3
            primitive_ = [1 1 0 1]; %GF[8]
        case 4  
            primitive_ = [1 1 0 0 1]; %GF[16], 1 + x + x^4
        case 5  
            primitive_ = [1 0 1 0 0 1]; %GF[32]
        case 6  
            primitive_ = [1 1 0 0 0 0 1]; %GF[64]
        case 8  
            primitive_ = [1 0 1 1 1 0 0 0 1]; %GF[256]
        case 10 
            primitive_ = [1 0 0 1 0 0 0 0 0 0 1]; %GF[1024]
        case 11 
            primitive_ = [1 0 1 0 0 0 0 0 0 0 0 1]; %GF[2048]
        case 12 
            primitive_ = [1 1 0 0 1 0 1 0 0 0 0 0 1]; % GF[4048]
    end
    
end

clear('dec2gf', 'gf2dec_local', 'multgf_gf', 'multgf_dec', 'divgf_gf', 'divgf_dec');

p_gf_ = i_p.Results.p_gf;

q_gf = 2 ^ p_gf_;

GF_vector = zeros(1, p_gf_ + 1);
GF_vectors = zeros(q_gf, p_gf_);
GF_vectors(2, 1) = 1;
GF_vector(1) = 1;

gf2dec_local = zeros(q_gf, 1, 'uint16');
gf2dec_local(2) = 1;

% build GF to binary vectors (GF_vectors)
% build GF to decimal (GF2dec)
for i = 1 : q_gf - 2
    
    GF_vector = [0 GF_vector(1 : end - 1)]; %shif right (multiplication by x)
    
    if (GF_vector(end) == 1) % compute modulo primitive
        GF_vector =  xor(GF_vector, primitive_);
    end
    
    temp = GF_vector(1 : end - 1);
    GF_vectors(i + 2, : ) = temp;
    temp2 = flip(temp);
    temp3 = num2str(temp2);
    gf2dec_local(i + 2) = bin2dec(temp3);
    
end

gf2dec_local = gf2dec_local + 1;

%build binary to GF
[~, dec2gf] = sort(gf2dec_local);


%%
%build multiplication table GF
mult_table_gf = ones(q_gf, q_gf, 'uint16');
for i = 2 : q_gf
    mult_table_gf(2 : end, i) = i - 2 : q_gf + i - 4;
end
mult_table_gf(2 : end, 2 : end)  = mod(mult_table_gf(2 : end, 2 : end), q_gf - 1) + 2;



%build division table GF
div_table_gf = ones(q_gf, q_gf);
for i = 2 : q_gf
    div_table_gf(2 : end, i) =  - i + 2 : q_gf - i;
end
div_table_gf(2 : end, 2 : end)  = mod(div_table_gf(2 : end, 2 : end), q_gf - 1) + 2;

%% build multiplication table decimal

mult_table_dec = zeros(q_gf, q_gf, 'uint16');
for i = 1 : q_gf
    for j = 1 : q_gf
        mult_table_dec(gf2dec_local(i), gf2dec_local(j)) = gf2dec_local(mult_table_gf(i, j));
    end
end


% build division table decimal
div_table_dec = zeros(q_gf, q_gf, 'uint16');
for i = 1 : q_gf
    for j = 1 : q_gf
        div_table_dec(gf2dec_local(i), gf2dec_local(j)) = gf2dec_local(div_table_gf(i, j));
    end
end

gf2dec(gf2dec_local, 'Reset', true);

end

