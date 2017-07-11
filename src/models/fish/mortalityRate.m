function[b] = mortalityRate(w,u,E)
    global m; global n; global mu0;
    b = mup(w,u,E) + mus(w,u,E) + mu0*m.^(n-1);
end
