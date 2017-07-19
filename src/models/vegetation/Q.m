function[res] = Q(z,height)
    global nbCols;

    qmat = zeros(nbCols,1);
    res = zeros(nbCols,1);
    
    for j=1:nbCols
         zvec = linspace(z(j),height,nbCols);
         
         if (z(j) <= height)
             for i=1:nbCols
                  qmat(i,1) = q(zvec(i),height);
             end
             for i=1:(nbCols-1)
                 %tmp1 = q(zvec(i),height);
                 tmp1 = qmat(i,1);
                 %tmp2 = q(zvec(i+1),height);
                 tmp2 = qmat(i+1,1);
                 res(j,1) = res(j,1) + (tmp1 + tmp2)*(zvec(i+1) - zvec(i))/2; 
	     end
         else
             res(j,1) = 0;
         end
    end
end
