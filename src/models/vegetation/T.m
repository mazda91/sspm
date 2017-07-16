function[res] = T(m)
    global alpha4; global phi; global beta4; global kb; global kr;

    res = alpha4*(phi^(-beta4))*m + kb*mb(m) + kr*mr(m); 

end
