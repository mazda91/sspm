function[lengthAtBirth,maximumLength,E0,u0]= init(w)

    global nbCols;global beta; global sigma1; global alpha;global epsilon;
    global h; global ks; global n; global p; global q; global eta;global mu0; global xi; 
    global w0; global f0; global lambda; global kappa0; global mcut; global wr; global r0;
    global gamma1; global M; global m;

    beta = 100; sigma1 = 1;alpha = 0.6;epsilon = 0.1; h = 85;ks = 10;
    n = 0.75;p = 0.75;q = 0.8;eta = 0.25; mu0=0.84;xi = 0.1;w0=0.0005;f0 = 0.6;
    lambda = 2+q-n;kappa0 = 0.005;mcut = 0.5; wr = 1;r0 = 4; 
    gamma1 = f0*h*(beta^(2-lambda))*wr/(sqrt(2*pi*(1-f0)*kappa0*sigma1));
    m = 0.5;
    M = m/eta;
       
    lengthAtBirth = w0;
    maximumLength = M;
     
    dimensions = size(w);
    nbCols = dimensions(2);

    E0 = 10*ones(nbCols,1);
    %%I can't use gaussian function, but replace this with gaussmf if you
    %%have required toolbox installed
    mean = (maximumLength-lengthAtBirth)/2;
    sigma = 0.1;
    u0 = 0.25*(1/(sigma*sqrt(2*pi)))*exp(-((w-mean).^2)/(2*sigma^2));
    %u0 = rand(nbCols,1);
end
