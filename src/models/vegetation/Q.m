function[res] = Q(z,height)
    global nbCols;

    for j=1:nbCols
         zvec = linspace(z(j),height,nbCols);
         res = zeros(nbCols,1);
         if (z(j) <= height)
             for i=1:(nbCols-1)
                 tmp1 = q(zvec(i),height);
                 tmp2 = q(zvec(i+1),height);
                 res(j) = res(j) + (tmp1 + tmp2)*(zvec(i+1) - zvec(i))/2; 
             end
         else
             res(j) = 0;
         end
    end
end
