function [dec_value] = gf2dec(gf_value, varargin)
%GF2DEC Get decimal value from gf value (Matlab associated)
%     WARNING: To be functional, buildGF must have been called
%     at least one time.
% 
%     Example:
%     >> buildGF(3);
%     >> gf2dec(4)
%     ans =
%           5
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
%     See also  BUILDGF, DEC2GF, MULTGF_GF, MULTGF_DEC,
%               DIVGF_GF, DIVGF_DEC.

i_p = inputParser;

    function [bOk] = validFunc(x)
        if isempty(varargin)
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

i_p.addRequired('gf_value', @validFunc);
i_p.addParameter('Reset', false, @islogical);
i_p.parse(gf_value, varargin{:});
gf_value_ = i_p.Results.gf_value;

persistent gf2dec_table;

if i_p.Results.Reset  
    gf2dec_table = gf_value_;
else
    if isempty(gf2dec_table)
        error('Value is not initialised! Run buildGF( p_gf, primitive) to initialize.'); 
    end
    dec_value = gf2dec_table(gf_value_);
end

end

