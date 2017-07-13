function[res] =   Encfood(w,u,E)
    global nbCols;global gamma1; global q; global mcut;
    global wvec;

    xResource = linspace(0,mcut, nbCols);
    integral1 = 0;integral2 = 0;
    for j=1:(nbCols-1)
        tmp1 = xResource(j)*E(j)*phi(w,xResource(j));
        tmp2 = xResource(j+1)*E(j+1)*phi(w,xResource(j+1));
	   if (j == 1)
		tmp1 = 0;
		tmp2 = 0;
	   end	
        
        integral1 = integral1 + (tmp1+tmp2)*(xResource(j+1)-xResource(j))/2.0;

	tmp1 = wvec(j)*u(j)*phi(w,wvec(j));
        tmp2 = wvec(j+1)*u(j+1)*phi(w,wvec(j+1));	
        integral2 = integral2 + (tmp1+tmp2)*(wvec(j+1)-wvec(j))/2.0;
    end
  
    res = gamma1*(integral1 + integral2).*(w.^q);
end

