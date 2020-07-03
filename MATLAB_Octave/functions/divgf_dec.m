function [dec_value] = divgf_dec(operandA, varargin)
%DIVGF_GF Compute 'a' divided by 'b' in GF (Matlab associated)
%     By convention, anything divided by 0_gf is 0_gf.
%     WARNING: To be functional, buildgf must have been called
%     at least one time.
%
%     Example:
%     >> buildgf(3);
%     >> divgf_dec(5, 2)
%     ans =
%           5
%
% %     GF symbols correspondance :
% %     GF Symbol  => Code Index  => Matlab Decimal value
% %     ------------------------------------------
% %     0_gf       => 1           => 1
% %     alpha_k    => k + 2       => (true decimal value) + 1
% %         this for k >=  0
% %
% %     Example with q = 8:
% %     > GF Symbol => Code Index  => Matlab Decimal Value
% %     > ------------------------------------------
% %     > 0_gf      => 1           => 1
% %     > alpha_0   => 2           => 2
% %     > alpha_1   => 3           => 3
% %     > alpha_2   => 4           => 5
% %     > alpha_3   => 5           => 5
% %     > alpha_4   => 6           => 7
% %     > alpha_5   => 7           => 8
% %     > alpha_6   => 8           => 6
%
%     See also  BUILDGF, GF2DEC, DEC2GF, MULTGF_GF,
%               MULTGF_DEC, DIVGF_DEC.

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

persistent div_table_dec;

if i_p.Results.Reset
    div_table_dec = A;
else
    if isempty(div_table_dec)
        error('Value is not initialised! Run buildgf( p_gf, primitive) to initialize.');
    end
    dec_value = div_table_dec(A, B);
end

end
