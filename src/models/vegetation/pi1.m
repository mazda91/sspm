function[res] = pi1(m,u,E)
    global cs0; global Pme;
    
    %Pme = P(m,u,E);
    omegam = omega(m);

    res =  (1+(cs0^2)*(Pme./omegam).^(-2)).^(-1);
    res(Pme <= 0) = 0;

end
