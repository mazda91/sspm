function[res] = R(m)
    global nu; global cR1; global rho; global cRs; global cRr;

    res = nu*cR1*omega(m) + (cRs/rho)*(ms(m) + 2*mb(m)) + cRr*mr(m);

end
