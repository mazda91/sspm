function[res] = mortalityRate(m,u,E)
    global cd0; global cd1; global cd2; global cd3; global rho; global nbCols;

    dimensions = size(m);
    nbCols = dimensions(1);
    res = cd0*exp(-cd1*rho) + cd2*exp(-cd3*P(m,E)./omega(m));
end
