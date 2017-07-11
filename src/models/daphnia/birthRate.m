function[b] = birthRate(x,u,S)
    global alpha;
    b = 0.75*(x.^2)*S/(1+S);
end
