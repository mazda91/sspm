function[res] = ms(m)
    global rho; global etac; global theta;

    res = rho*etac*omega(m).*h(m)/theta;
end
