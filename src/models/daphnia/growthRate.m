function[b] = growthRate(x,u,S)
    b = max((S/(1+S))-x,0);
end
