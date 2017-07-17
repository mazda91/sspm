function[res] = A(m,E)
    global nbCols; global A0; global nu;
    global mvec;

    omegam = omega(m);
    hvec = h(m);
    Avec = Alf(A0*nu,E);
    res = zeros(nbCols,1);
    for j=1:nbCols
        zvec = linspace(0,hvec(j),nbCols);
        integral = 0;
        for i=1:(nbCols-1)
            tmp1 = Avec(i)*q(zvec(i),hvec(j));
            tmp2 = Avec(i+1)*q(zvec(i+1),hvec(j));

             integral = integral + (zvec(i+1) - zvec(i))*(tmp1 + tmp2)/2.0;
         end

         res(j,1) = omegam(j).*integral;
     end
end
