function[res] = growthRate(m,u,E)
    global nbCols; 

    dimensions = size(m);
    nbCols = dimensions(1);
    res = max((1-r2(m)).*P(m,u,E).*dm(m),0);
    res(1)=1;
end
