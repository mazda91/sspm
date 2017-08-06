function[res] = birthRate(m,u,E)
    global pi0; global cass; global s; global nbCols;
    global Pme;

    dimensions = size(m);
    nbCols = dimensions(1);

    Pme = P(m,u,E);
    res = max((pi0/(cass*s))*pi1(m,u,E).*r2(m).*Pme,0);
    res = pi1(m,E);
end
