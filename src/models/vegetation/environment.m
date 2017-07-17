function[res] = environment(m,u,E,Tf,M)
    global nbCols; global cext; 
    global mvec;

    dimensions = size(m);
    nbCols = dimensions(1);
   integral = 0;
   
   omegam = omega(m);
   hvec = h(m);
   qvec = zeros(nbCols,nbCols);
   for i=1:nbCols
	qvec(i,:) =  Q(hvec,hvec(i));
   end
   for i=1:(nbCols-1)
       tmp1 = qvec(i,:).*omegam.*u(i);
       tmp2 = qvec(i+1,:).*omegam.*u(i+1);
       integral = integral + (mvec(i+1) - mvec(i))*(tmp1 + tmp2)/2.0;
    end
    res = exp(-cext*integral);
end
