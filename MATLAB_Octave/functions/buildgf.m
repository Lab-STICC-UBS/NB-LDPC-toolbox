function buildgf(p_gf, varargin)
%BUILDGF Initialize tools for NB-LDPC
%     Authors :  Cédric Marchand, Camille Monière
%     Build finite field vectors and initialize
%     dec2gf, gf2dec, multgf_gf, multgf_dec,
%     divgf_gf and divgf_dec functions.
%
% %     GF symbols correspondance :
% %     GF Symbol  => Code Index  => Matlab Decimal value
% %     ------------------------------------------
% %     0_gf       => 1           => 1
% %     alpha^k    => k + 2       => (true decimal value) + 1
% %         this for k >=  0
% %
% %     Example with q = 8:
% %     > GF Symbol => Code Index  => Matlab Decimal Value
% %     > ------------------------------------------
% %     > 0_gf      => 1           => 1
% %     > alpha^0   => 2           => 2
% %     > alpha^1   => 3           => 3
% %     > alpha^2   => 4           => 5
% %     > alpha^3   => 5           => 5
% %     > alpha^4   => 6           => 7
% %     > alpha^5   => 7           => 8
% %     > alpha^6   => 8           => 6
%
%     See also  DEC2GF, GF2DEC, MULTGF_GF, MULTGF_DEC,
%               DIVGF_GF, DIVGF_DEC.


i_p = inputParser;

i_p.addRequired('p_gf', @(x) int64(x) == x);
i_p.addOptional('primitive', []);
i_p.addParameter('Save', false, @islogical);
i_p.addParameter('Reset', true, @islogical);

i_p.parse(p_gf, varargin{:});


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
        case 1
            primitive_ = [1 0]; %GF[2]
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
    
    GF_vector = [0 GF_vector(1 : end - 1)]; % right shift (multiplication by x)
    
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
[~, dec2gf_local] = sort(gf2dec_local);


%% build multiplication table GF
mult_table_gf = ones(q_gf, q_gf, 'uint16');
for i = 2 : q_gf
    mult_table_gf(2 : end, i) = i - 2 : q_gf + i - 4;
end
mult_table_gf(2 : end, 2 : end)  = mod(mult_table_gf(2 : end, 2 : end), q_gf - 1) + 2;



%% build division table GF
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


%% build division table decimal
div_table_dec = zeros(q_gf, q_gf, 'uint16');
for i = 1 : q_gf
    for j = 1 : q_gf
        div_table_dec(gf2dec_local(i), gf2dec_local(j)) = gf2dec_local(div_table_gf(i, j));
    end
end


%% Save if asked
if i_p.Results.Save
   filename = "GF" + string(q_gf) + ".mat";
   save(filename, "dec2gf_local", "gf2dec_local", "mult_table_dec", "mult_table_gf", "div_table_dec", "div_table_gf", '-v7');
end

%% Reset functions if asked
if i_p.Results.Reset
    clear('dec2gf', 'gf2dec', 'multgf_gf', 'multgf_dec', 'divgf_gf', 'divgf_dec');
    
    gf2dec(gf2dec_local, 'Reset', true);
    dec2gf(dec2gf_local, 'Reset', true);
    multgf_gf(mult_table_gf, 'Reset', true);
    divgf_gf(div_table_gf, 'Reset', true);
    multgf_dec(mult_table_dec, 'Reset', true);
    divgf_dec(div_table_dec, 'Reset', true);
end

end
