function[lengthAtBirth,maximumLength,E0,u0,N0]= init(w)

    %delcaration of all variables requested in the model
    global nbCols;global beta1; global sigma1; global alpha;global epsilon;
    global h; global ks; global n; global p; global q; global eta;global mu0; global xi; 
    global w0; global f0; global lambda; global kappa0; global mcut; global wr; global r0;
    global gamma1; global M; global m;
    global wvec;

    %variables initialization
    
    beta1 = 100; sigma1 = 1;alpha = 0.6;epsilon = 0.1; h = 85;ks = 10;
    n = 0.75;p = 0.75;q = 0.8;eta = 0.25; mu0=0.84;xi = 0.1;w0=0.0005;f0 = 0.6;
    lambda = 2+q-n;kappa0 = 0.005;mcut = 0.5; wr = 1;r0 = 4; 
    gamma1 = f0*h*(beta1^(2-lambda))*wr/(sqrt(2*pi)*(1-f0)*kappa0*sigma1);
    m = 1;
    M = m/eta;
    wvec = w;   
    dimensions = size(w);
    nbCols = dimensions(1);

    %default values for mesh extremities
    lengthAtBirth = w0;
    maximumLength = M;
     
    
    %initialization of the environment variable and the distribution density
    E0 = mcut*ones(nbCols,1);

    mean1 = (maximumLength-lengthAtBirth)/2;
    sigma2 = 0.1;
    u0 = 0.025*(1/(sigma2*sqrt(2*pi)))*exp(-((w-mean1).^2)/(20*sigma2^2)); 
    N0 = 0.025*(1/(sigma2*sqrt(2*pi)))*exp(-((w-mean1).^2)/(20*sigma2^2));    
    %u0 = rand(nbCols,1);
end
