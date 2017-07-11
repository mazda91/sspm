function[res] =   Encfood(w,u,E)
    global nbCols;global gamma1; global q; global mcut;

    xResource = linspace(0,mcut, nbCols);
    integral1 = 0;integral2 = 0;
    for j=1:(nbCols-1)
        tmp1 = xResource(j)*E(j)*phi(w,xResource(j));
        tmp2 = xResource(j+1)*E(j+1)*phi(w,xResource(j+1));
        integral1 = integral1 + (tmp1+tmp2)*(xResource(j+1)-xResource(j))/2.0;
    end
    for j=1:(nbCols-1)
        tmp1 = w(j)*u(j)*phi(w,w(j));
        tmp2 = w(j+1)*u(j+1)*phi(w,w(j+1));

         integral2 = integral2 + (tmp1+tmp2)*(w(j+1)-w(j))/2.0;
    end
     disp(integral1);
     disp(integral2);
    res = gamma1.*(w.^q).*(integral1 + integral2);
end

