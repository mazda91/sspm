function[res] = A(m,E)
    global nbCols; global A0; global nu;
    global mvec;

   
    res = 0;
    for j=1:nbCols
        zvec = linspace(0,h(m(j)),nbCols);
        integral = 0;
        for i=1:(nbCols-1)
            tmp1 = Alf(A0*nu,E(i))*q(zvec(i),h(m(j)));
            tmp2 = Alf(A0*nu,E(i+1))*q(zvec(i+1),h(m(j)));

             integral = integral + (zvec(i+1) - zvec(i))*(tmp1 + tmp2)/2.0;
         end

         res(j,1) = omega(m(j)).*integral;
     end
end
