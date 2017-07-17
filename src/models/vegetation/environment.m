function[res] = environment(m,u,E,Tf,M)
    global nbCols; global cext; 
    global mvec;

    dimensions = size(m);
    nbCols = dimensions(1);
   integral = 0;
   for i=1:(nbCols-1)
       %tmp1 = Q(h(m),h(mvec(i))).*omega(m).*u(i);
       %tmp2 = Q(h(m),h(mvec(i+1))).*omega(m).*u(i+1);
       tmp1 = omega(m).*u(i);
       tmp2 = omega(m).*u(i+1);
       integral = integral + (mvec(i+1) - mvec(i))*(tmp1 + tmp2)/2.0;
    end

    res = exp(-cext*integral);
end
