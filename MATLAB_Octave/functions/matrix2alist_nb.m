function [structAlist] = matrix2alist_nb(parityMatrix, GF)
%MATRIX2ALIST_NB Convert a parity matrix to a NB LDPC alist structure.
%
%
% author: Cédric Marchand, Camille Monière
% License: BSD

i_p = inputParser;

i_p.addRequired('parityMatrix')
i_p.addRequired('GF', @(x) uint16(x) == x);

i_p.parse(parityMatrix, GF);

parityMatrix_ = i_p.Results.parityMatrix;
GF_ = i_p.Results.GF;

H_bin = parityMatrix_;
H_bin( parityMatrix_ > 1 ) = 1;
H_bin( parityMatrix_ < 2 ) = 0;


[M,N] = size(parityMatrix_);
dc = sum(H_bin,2)';
dv = sum(H_bin,1);
dcmax = max(dc);
dvmax = max(dv);

[nlist,~] = find(H_bin);
[mlist,~] = find(H_bin');

pcMatrix = zeros(N, dvmax);
k=0;
for i=1:N
    for j=1:dv(i)
        pcMatrix(i, (j - 1) * 2 + 1) = nlist(k + j );
        pcMatrix(i, j * 2) = parityMatrix_( nlist(k + j  ),i) - 2;
    end
    k=k+dv(i);
end

vnMatrix = zeros(M, dcmax);
k=0;
for i=1:M
    for j=1:dc(i)
        vnMatrix(i, (j - 1) * 2 + 1) = mlist(k + j );
        vnMatrix(i, j * 2) = parityMatrix_( i, mlist(k + j  )) - 2;
    end
    k=k+dc(i);
end

structAlist = struct('N', N, 'M', M, 'GF', GF_, 'dv_max', dvmax, 'dc_max', dcmax, 'dv_vector', dv, 'dc_vector', dc,'pcMatrix', pcMatrix, 'vnMatrix', vnMatrix);

end