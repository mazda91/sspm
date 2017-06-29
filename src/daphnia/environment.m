function[b] = environment(x,u,S)
    dimensions = size(u);
    nbCols = dimensions(2);
    integral = 0;
   for i=1:nbCols
        integral = integral + (x(i+1) - x(i))*((x(i+1)^2)*u(i+1)+ (x(i)^2)*u(i))/2.0;
    end
    b = 0.5*S*(1-S/3)-(S/(1+S))*integral;
end
