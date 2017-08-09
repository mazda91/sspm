function[res] = environment(w,u,E,Tf,M)
    global nbCols;

    dimensions = size(w);
    nbCols = dimensions(1);
    res = E + (Tf/M)*(r(w).*(kappa(w) - E) - mup(w,u,E).*E);
end
