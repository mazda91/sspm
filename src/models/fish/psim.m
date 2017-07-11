function[res] = psim(w)
    global m;global eta; global n;
    res = (1+(w./m).^(-10)).^(-1).*(eta*w./m).^(1-n);
end

