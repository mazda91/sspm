function[res] = birthRate(w,u,E)
    global ks; global p; global epsilon;
    res = max(0,epsilon*psim(w).*(S(w,u,E) - ks*(w.^p)));
end

