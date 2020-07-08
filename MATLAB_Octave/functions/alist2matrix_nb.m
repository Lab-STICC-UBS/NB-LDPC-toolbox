function [parityMatrix] = alist2matrix_nb(structAlist)
%ALIST2MATRIX_NB Converte NB LDPC Alist structure to parity matrix
%   Detailed explanation goes here

matrix1 = ones(structAlist.M, structAlist.N);

for i = 1 :  structAlist.N
    for j = 1 :  structAlist.dv_vector(i)
       matrix1(structAlist.pcMatrix(i, (j - 1) * 2 + 1), i) = ...
           structAlist.pcMatrix(i, 2 * j) + 2;
    end
end

matrix2 = ones(structAlist.M, structAlist.N);

for i = 1 : structAlist.M
    for j = 1 : structAlist.dc_vector(i)
       matrix2(i, structAlist.vnMatrix(i, 2 * (j - 1) + 1)) = ...
           structAlist.vnMatrix(i, 2 * j) + 2;
    end
end

noerror = isequal(matrix1, matrix2);

if noerror == false
   error("Matrices don't match!"); 
end

parityMatrix = matrix1;

end

