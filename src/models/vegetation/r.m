function[res] = r(m)
    global cr1; global cr2; global hm;
    
    res = cr1*(1+exp(cr2*(1-h(m)/hm)))
end
