function[res] = A(m,u,E)
    global nbCols; global A0; global nu;
    global mvec;

    hvec = h(m);
    omegam = omega(m);
    res = zeros(nbCols,1);
    Avec = Alf(m,u,E);
    for j=1:nbCols
        zvec = linspace(0,hvec(j),nbCols);
        
        res1 = trapz(zvec,Avec.*q2(zvec,hvec(j)));
        res(j,1) = omegam(j)*res1;
     end
end
