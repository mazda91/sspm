function[E] = environment(x,u,E,Tf,M)
    dimensions = size(x);
    nbCols = dimensions(1);
    integral = 0;
    for i=1:(nbCols - 1)
        integral = integral + (x(i+1) - x(i))*((x(i+1)^2)*u(i+1)+ (x(i)^2)*u(i))/2.0;
    end
    E = E + (Tf/M)*(0.5*E*(1-E/3)-(E/(1+E))*integral);
end
