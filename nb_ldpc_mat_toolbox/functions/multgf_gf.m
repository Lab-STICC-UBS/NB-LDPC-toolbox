function [gf_value] = multgf_gf(operandA, varargin)
%MULTGF_GF Compute 'a' times 'b' in GF (Matlab associated)
%     WARNING: To be functional, buildgf must have been called
%     at least one time.
%
%     MULTGF(a, b) compute a times b with a and b in GF(q), q being 2^p, p
%     being the value used with buildgf.
% 
%     Example with q = 8 (so p = 3):
%     >> buildgf(3);
%     >> MULTGF_GF(7, 4)
% 
%     ans =
%           5 
%     
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
%     See also  BUILDGF, GF2DEC, DEC2GF, MULTGF_DEC,
%               DIVGF_GF, DIVGF_DEC.

i_p = inputParser;

    function [bOk] = validFunc(x)
        if length(varargin) == 1
            if length(x) == 1
                if int64(x) == x
                    bOk = true;
                    return;
                end
            end
        end
        if sum(size(x)) > 2
            bOk = true;
            return
        end
        
        bOk = false;
        return;
    end
i_p.addRequired('operandA', @validFunc);

    function [bOk] = validFuncBis(x)
        if length(varargin) == 1
            if length(x) == 1
                if int64(x) == x
                    bOk = true;
                    return;
                end
            end
        end
        bOk = false;
    end
i_p.addOptional('operandB', [], @validFuncBis);

i_p.addParameter('Reset', false, @islogical);
i_p.parse(operandA, varargin{:});
A = i_p.Results.operandA;
B = i_p.Results.operandB;

persistent mult_table_gf;

if i_p.Results.Reset
    mult_table_gf = A;
else
    if isempty(mult_table_gf)
        error('Value is not initialised! Run buildgf( p_gf, primitive) to initialize.');
    end
    gf_value = mult_table_gf(A, B);
end

end
