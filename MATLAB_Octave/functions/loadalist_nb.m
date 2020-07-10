function [parityMatrix, structAlist] = loadalist_nb(filePath)
%LOADALIST_NB Load NB-LDPC alist file into Matlab/Octave
% 
%   Load the alist as a parity matrix parityMatrix which follows MATLAB
%   index convention (as described in buildgf). It also load the alist in a
%   structure containing all data of the alist in dedicated field.
%   
%   Examples:
% 
%   LOADALIST_NB('/path/to/alist.txt') return the parity matrix
%   associated with alist.txt.
% 
%   pMat = LOADALIST_NB('/path/to/alist.txt') store the parity matrix in
%   pMat.
% 
%   [pMat, structAlist] = LOADALIST_NB('/path/to/alist.txt') store the
%   parity matrix in pMat and the alist structure in structAlist.
% 
%   [~, structAlist] = LOADALIST_NB('/path/to/alist.txt') ignore the
%   parity matrix output and store the alist structure in structAlist.
% 
%   authors: Cédric Marchand, Camille Monière
%   License: BSD

i_p = inputParser;

i_p.addRequired("filePath", @(x) exist(x, 'file'));
i_p.parse(filePath);

fileID = fopen(i_p.Results.filePath, 'r');

N = fscanf(fileID,'%d',1);
M = fscanf(fileID,'%d',1);
GF = fscanf(fileID,'%d',1);
dvmax = fscanf(fileID,'%d',1);
dcmax = fscanf(fileID,'%d',1);

dv=zeros(1,N);
dc=zeros(1,M);

matrix=ones(M,N);
matrix2=ones(M,N);

for i=1:N
   dv(i) = fscanf(fileID,'%d',1); 
end

for i = 1 : M
   dc(i) = fscanf(fileID, '%d', 1); 
end

pcMatrix = zeros(N, dvmax * 2, 'uint16');

for i = 1 : N
    for j = 1 : dv(i)
       pos = fscanf(fileID, '%d', 1); 
       value = fscanf(fileID, '%d', 1);
       pcMatrix(i, (j - 1) * 2 + 1) = uint16(pos);
       pcMatrix(i, j * 2) = uint16(value);
       matrix(pos, i) = value + 2;
    end
end

vnMatrix = zeros(M, dcmax * 2, 'uint16');

for i = 1 : M
    for j = 1 : dc(i)
       pos = fscanf(fileID, '%d', 1); 
       value = fscanf(fileID, '%d', 1);
       vnMatrix(i, (j - 1) * 2 + 1) = uint16(pos);
       vnMatrix(i, j * 2) = uint16(value);
       matrix2(i, pos) = value + 2;
    end
end

noerror = isequal(matrix, matrix2);

if noerror == false
   error("Matrices don't match!"); 
end

parityMatrix = matrix;
structAlist = struct('N', uint16(N), 'M', uint16(M), 'GF', uint16(GF), ...
    'dv_max', uint16(dvmax), 'dc_max', uint16(dcmax), ...
    'dv_vector', uint16(dv), 'dc_vector', uint16(dc), ...
    'pcMatrix', pcMatrix, 'vnMatrix', vnMatrix);
fclose(fileID);

end

