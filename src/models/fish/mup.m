function[res] =  mup(w,u,E)

    global nbCols; global M; global w0;

    integral = 0;
    usave = u;
    dimw = size(w); dimu = size(u);

    if (dimw(1) ~= dimu(1))
    	u(nbCols,:) = [];
    end
    res = trapz(w,v(w).*(1-F(w,usave,E)).*u.*phi(w,w));
end

