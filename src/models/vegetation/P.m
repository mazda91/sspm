function[res] = P(m,u,E)
    global cbio; global Y;
    Ame = A(m,u,E);
    res = cbio*Y*(Ame - R(m)) - T(m); 
end
