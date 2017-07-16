function[res] = dm(m)
    global rho; global etac; global theta; global phi; global alpha1; 
    global beta1; global alpha2; global beta2; global alpha3; global b;

    dms = (-rho*etac/theta)*(h(m)/phi + alpha1*(beta1/phi)*omega(m).*(omega(m)).^(beta1-1))./((omega(m).*h(m)).^2 );
    dmb = b*dms;
    dmh = rho*etac*alpha2*(beta2/phi)*omega(m).^(beta2-1);
    dmr = alpha3/phi + 0*m;
    res = (1 + dms + dmb + dmr + dmh).^(-1);
end
