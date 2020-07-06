function [bOk] = exportgf(p_gf, filePrefix)
%SAVEBUILD Summary of this function goes here
%   Detailed explanation goes here

i_p = inputParser;

i_p.addRequired('p_gf', @(x) int64(x) == x);
i_p.addRequired('filePrefix', @(x) exist(x, 'dir'));

i_p.parse(p_gf, filePrefix);

buildgf(i_p.Results.p_gf, 'Save', true, 'Reset', false) 

matfile = "GF" + string(2^p_gf) + ".mat";

load(matfile, 'gf2dec_local', 'dec2gf_local', 'mult_table_gf', 'mult_table_dec', 'div_table_gf', 'div_table_dec');

filename = filePrefix + "_GF" + string(2^p_gf) + "_gf2dec.txt";
writematrix(gf2dec_local, filename);

filename = filePrefix + "_GF" + string(2^p_gf) + "_dec2gf.txt";
writematrix(dec2gf_local, filename);

filename = filePrefix + "_GF" + string(2^p_gf) + "_multgf_gf.txt";
writematrix(mult_table_gf, filename);

filename = filePrefix + "_GF" + string(2^p_gf) + "_multgf_dec.txt";
writematrix(mult_table_dec, filename);

filename = filePrefix + "_GF" + string(2^p_gf) + "_divgf_gf.txt";
writematrix(div_table_gf, filename);

filename = filePrefix + "_GF" + string(2^p_gf) + "_divgf_dec.txt";
writematrix(div_table_dec, filename);

clear('gf2dec_local', 'dec2gf_local', 'mult_table_gf', 'mult_table_dec', 'div_table_gf', 'div_table_dec');
delete(matfile);

bOk = true;

end

