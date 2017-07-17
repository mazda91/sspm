function[res] = Q(z,height)
    global nbCols;

    res = zeros(nbCols,1);
    for j=1:nbCols
         zvec = linspace(z(j),height,nbCols);
	 qvec = q(zvec,height);
         if (z(j) <= height)
             for i=1:(nbCols-1)
                 tmp1 = qvec(i);
                 tmp2 = qvec(i+1);
                 res(j,1) = res(j,1) + (tmp1 + tmp2)*(zvec(i+1) - zvec(i))/2; 
	     end
         else
             res(j,1) = 0;
         end
    end
end
