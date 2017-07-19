function[res] = P(m,E)
    global cbio; global Y;
    Ame = A(m,E);
    res = cbio*Y*(Ame - R(m) - T(m)); 
end
