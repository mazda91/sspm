function[res] = birthRate(w,u,E)
    global ks; global p;
    res = max(0,psim(w).*(S(w,u,E) - ks*(w.^p)));
end

