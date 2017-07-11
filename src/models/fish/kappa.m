function[res] = kappa(w)
    global kappa0; global lambda;
    res = kappa0*w.^(-lambda);
end

