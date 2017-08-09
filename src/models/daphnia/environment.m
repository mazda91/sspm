function[res] = environment(x,u,S,Tf,M)
    global nbCols; global r; global K;

    integral = trapz(x,x.*x.*u);
    res = S + (Tf/M)*(r*S*(1-S/K)-(S/(1+S))*integral);
end
