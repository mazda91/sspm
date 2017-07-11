function[res] = r(w)
    global r0; global lambda;
    res = r0*w.^(-lambda);
end

