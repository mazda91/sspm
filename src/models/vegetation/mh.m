function[res] = mh(m)
    global rho; global etac; global alpha2; global beta2;

    res = rho*etac*alpha2*(omega(m).^beta2);
end
