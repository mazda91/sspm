function[res] = P(m,E)
    global cbio; global Y;

    res = cbio*Y*(A(m,E) - R(m) - T(m)); 
end
