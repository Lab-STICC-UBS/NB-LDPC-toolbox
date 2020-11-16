function [gf_value] = dec2gf(dec_value, varargin)
%DEC2GF Get GF value from decimal value (Matlab associated or natural)
%     WARNING: To be functional, buildGF must have been called
%     at least one time.
% 
%     Example:
%     >> buildGF(3);
%     >> gf2dec(5)
%     ans =
%           4
% 
% %     GF symbols correspondance :
% %     GF Symbol  => Code Index  => Matlab Decimal value
% %     ------------------------------------------
% %     0_gf       => 1           => 1
% %     alpha^k    => k + 2       => (true decimal value) + 1
% %         this for k >=  0
% %
% %     Example with q = 8:
% %     > GF Symbol => Mat Index  => Mat Decimal => Natural => Polynomial
% %     > ------------------------------------------
% %     > 0_gf      => 1           => 1          => 0       => 0 0 0
% %     > alpha^0   => 2           => 2          => 1       => 1 0 0
% %     > alpha^1   => 3           => 3          => 2       => 0 1 0
% %     > alpha^2   => 4           => 5          => 4       => 0 0 1
% %     > alpha^3   => 5           => 4          => 3       => 1 1 0
% %     > alpha^4   => 6           => 7          => 6       => 0 1 1
% %     > alpha^5   => 7           => 8          => 7       => 1 1 1
% %     > alpha^6   => 8           => 6          => 5       => 1 0 1
%   
%     See also  BUILDGF, GF2DEC, MULTGF_GF, MULTGF_DEC,
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

i_p.addRequired('dec_value', @validFunc);
i_p.addParameter('Natural', false, @islogical);
i_p.addParameter('Reset', false, @islogical);
i_p.parse(dec_value, varargin{:});
dec_value_ = i_p.Results.dec_value;
bNatural = i_p.Results.Natural;

persistent dec2gf_table;

if i_p.Results.Reset  
    dec2gf_table = dec_value_;
else
    if isempty(dec2gf_table)
        error('Value is not initialised! Run buildGF( p_gf, primitive) to initialize.'); 
    end
    if bNatural == true
        gf_value = dec2gf_table(dec_value_) - 1;
    else
        gf_value = dec2gf_table(dec_value_);
    end
end

end
