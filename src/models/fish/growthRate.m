function[res] = growthRate(w,u,E)
    global ks; global p; global n;global nbCols;

    dimensions = size(w);
    nbCols = dimensions(1);
    res = max(0,(1-psim(w)).*(S(w,u,E) - ks*(w.^p)));
    res(1) = 10*w(1)^n;
end

