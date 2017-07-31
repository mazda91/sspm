function[lengthAtBirth,maximumLength,E0,u0]= init(m)
    
    global nbCols; global phi; global rho; global hm; global s; global cext;
    global eta; global etac; global theta; global alpha1; global beta1;
    global alpha2; global beta2; global alpha3; global b; global nu; global A0;
    global cR1; global cRr; global cRs; global Y; global cbio; global alpha4; global beta4;
    global kb; global kr; global cass; global cr1; global cr2; global pi0;
    global cs0; global cd0; global cd1; global cd2; global cd3; 
    global cp1; global cp2;

    global mvec;
    mvec = m;

     phi = 0.11; rho = 608; hm = 12; s = 3.8*10^(-5); s = 1.7*10^(-3); cext = 0.5;
     eta = 12; etac = 1 - 2/(1+eta) + 1/(1+2*eta); theta = 4669; alpha1 = 5.44; beta1 = 0.306;
     alpha2 = 6.67*10^(-5); beta2 = 1.75; alpha3 = 0.07; b = 0.17; nu = 1.87*10^(-3); A0 =  1.78*10^5;
     cR1 = 2.1*10^4 ; cRr = 217; cRs = 4012; Y = 0.7; cbio = 2.45*10^(-2); alpha4 = 2.86*10^(-2); beta4 = 1.71;
     kb = 0.2; kr = 1; cass = 4; cr1 = 1; cr2 = 50; pi0 = 0.25;
     cs0 = 0.1; cd0 = 0.52; cd1 = 6.5*10^(-3); cd2 = 5.5; cd3 = 20;
    cp1 = 135.24; cp2 = 0.17;

    dimensions = size(m);
    nbCols = dimensions(1);
    lengthAtBirth = s;
    maximumLength = 4;
    %%I can't use gaussian function, but you can replace this with gaussmf if you
    %%have required toolbox installed
    mean1 = (maximumLength - lengthAtBirth)/2;
    sigma = 0.1;
    u0 = 2.5*(1/(sigma*sqrt(2*pi)))*exp(-((m-mean1).^2)/(200*sigma^2));
    %u0 = rand(nbCols,1);
    E0 = environment(m,u0,0,0,0);
end
