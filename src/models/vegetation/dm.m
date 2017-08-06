function[res] = dm(m)
    global rho; global etac; global theta; global phi; global alpha1; 
    global beta1; global alpha2; global beta2; global alpha3; global b;

    hm = h(m);
    omegam = omega(m);    
    dms = (1+beta1)*(rho*etac/theta)*alpha1*(phi^(-1-beta1))*(m.^beta1);
    dmb = b*dms;
    dmh = rho*etac*alpha2*(beta2/phi)*omegam.^(beta2-1);
    dmr = alpha3/phi + 0*m;
    res = (1 + dms + dmb + dmr + dmh).^(-1);
end
