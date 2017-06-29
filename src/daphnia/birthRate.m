function[b] = birthRate(size,S)
    b = 0.75*(size.^2)*S/(1+S);
end
