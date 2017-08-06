function[res] = A(m,u,E)
    global nbCols; global A0; global nu;
    global mvec;

    hvec = h(m);
    omegam = omega(m);
    res = zeros(nbCols,1);
    %Avec = Alf(m,u,E);
    for j=1:nbCols
        %zvec = linspace(0,hvec(j),nbCols);
        %qmat = q(zvec,hvec);
        %integral = 0;
        %for i=1:(nbCols-1)
            %tmp1 = Avec(i)*q(zvec(i),hvec(j));
         %   tmp1 = Avec(i).*qmat(i,j);
            %tmp2 = Avec(i+1)*q(zvec(i+1),hvec(j));
         %   tmp2 = Avec(i+1).*qmat(i+1,j);
          %   integral = integral + (zvec(i+1) - zvec(i))*(tmp1 + tmp2)/2.0;
        % end
        res1 = integral(@(z)Alf(z,u,E)*q2(z,hvec(j)),0,hvec(j),'ArrayValued',true);
         res(j,1) = omegam(j).*res1;
     end
end
