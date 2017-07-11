function[res] =  mup(w,u,E)
    global nbCols; global M; global w0;
    integral = 0;
    for j=1:(nbCols-1)
        tmp1 = v(w(j))*(1-F(w(j),u,E))*u(j)*phi(w(j),w);
        tmp2 = v(w(j))*(1-F(w(j+1),u,E))*u(j)*phi(w(j+1),w);
        integral = integral + (tmp1+tmp2)*(w(j+1)-w(j))/2.0;
    end
    res = integral; 
end

