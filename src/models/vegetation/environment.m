function[res] = environment(m,u,E,Tf,M)
    global nbCols; global cext; global maximumLength;
    global mvec;

    dimensions = size(m);
    nbCols = dimensions(1);
    integral = 0;
    hvec = h(m);
    omegam = omega(m);
   
   integral = trapz(m,Q(hvec,hvec)*(omegam.*u));
   res = exp(-cext*integral);

    
end
