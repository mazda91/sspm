function[res] = mus(w,u,E)
        global ks; global p; global xi;
       res = max((-S(w,u,E) + ks*(w.^p))./(xi*w),0);
end

