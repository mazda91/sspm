function[b] = birthRate(x,u,S)
    b = 0.75*(x.^2)*S/(1+S);
end
