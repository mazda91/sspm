function[res] = growthRate(w,u,E)
    global ks; global p;
    res = max(0,(1-psim(w)).*S(w,u,E) - ks*(w.^p));
end

