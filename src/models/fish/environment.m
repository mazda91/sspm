function[b] = environment(w,u,E,Tf,M)
        b = E + (Tf/M)*(r(w).*(kappa(w) - E) - mup(w,u,E).*E);
end
