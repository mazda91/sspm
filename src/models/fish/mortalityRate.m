function[res] = mortalityRate(w,u,E)
    global m; global n; global mu0;global nbCols;

    dimensions = size(w);
    nbCols = dimensions(1);
    res = mup(w,u,E) + mus(w,u,E) + mu0*m.^(n-1);
end
