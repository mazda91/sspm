function[res] =   Encfood(w,u,E)
    global nbCols;global gamma1; global q; global mcut;
    global wvec;

    xResource = linspace(0,mcut, nbCols);
    matphi = phi(w,xResource);
    dimw = size(w); dimu = size(u);
    usave = u;
    if (dimw(1) ~= dimu(1))
    	u(nbCols,:) = [];
        E(nbCols,:) = [];
    end
    integral1 = trapz(xResource,(w.*E)'*matphi);
    integral2 = trapz(w,(w.*u)'*matphi);
  
    res = gamma1*(integral1 + integral2).*(w.^q);
end

