function[res] =  mup(w,u,E)
    global nbCols; global M; global w0;
    global wvec;

    integral = 0;
    for j=1:(nbCols-1)
        tmp1 = v(wvec(j))*(1-F(wvec(j),u,E))*u(j)*phi(wvec(j),w);
        tmp2 = v(wvec(j+1))*(1-F(wvec(j+1),u,E))*u(j+1)*phi(wvec(j+1),w);
        integral = integral + (tmp1+tmp2).*(wvec(j+1)-wvec(j))/2.0;
    end
     res = integral;
end

