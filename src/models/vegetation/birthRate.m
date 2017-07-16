function[res] = birthRate(m,u,E)
    global pi0; global cass; global s;
    res = max((pi0/(cass*s))*pi1(m,E).*r(m).*P(m,E),0);

end
