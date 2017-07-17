function[res] = birthRate(m,u,E)
    global pi0; global cass; global s; global nbCols;

    dimensions = size(m);
    nbCols = dimensions(1);
    res = max((pi0/(cass*s))*pi1(m,E).*r(m).*P(m,E),0);

end
