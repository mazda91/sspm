function[b] = growthRate(size,S)
    b = max((S/(1+S))-size,0);
end
