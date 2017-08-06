function[res] = growthRate(w,u,E)
    global ks; global p; global n;
    res = max(0,(1-psim(w)).*(S(w,u,E) - ks*(w.^p)));
    res(1) = 10*w(1)^n;
end

