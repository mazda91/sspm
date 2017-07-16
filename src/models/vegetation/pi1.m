function[res] = pi1(m,E)
    global cs0;

    if (P(m,E) > 0)
        res =  ((cs0^2)*(P(m,E)/omega(m)).^(-2))^(-1);
    else
        res = 0;

end
