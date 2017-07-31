function[res] = pi1(m,E)
    global cs0; global Pme;
    
    %Pme = P(m,E);
    omegam = omega(m);

    res =  (1+(cs0^2)*(Pme./omegam).^(-2)).^(-1);
    res(Pme <= 0) = 0;

end
