function[b] = environment(x,u,S,Tf,M)
    global nbCols; global r; global K;

    integral = 0;
   for i=1:(nbCols-1)
        integral = integral + (x(i+1) - x(i))*((x(i+1)^2)*u(i+1)+ (x(i)^2)*u(i))/2.0;
    end
    b = S + (Tf/M)*(r*S*(1-S/K)-(S/(1+S))*integral);
end
