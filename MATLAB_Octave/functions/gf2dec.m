function [dec_value] = gf2dec(gf_value, varargin)
%
%

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
        if sum(size(x)) > 1
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

