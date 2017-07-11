function[res] =  S(w,u,E)
    global alpha;
    res = alpha*(Imax(w).*F(w,u,E));
end

