function[res] = F(w,u,E)
     res = Encfood(w,u,E)./(Encfood(w,u,E) + (Imax(w)./v(w)));
end

