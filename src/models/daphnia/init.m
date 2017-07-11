function[lengthAtBirth,maximumLength,E0,u0]= init(x)
    %%u0 = gaussmf(x,[0 1]);
    global nbCols; global alpha; global mu0; global r; global K;
    
    alpha = 0.75; mu0 = 0.1; r = 0.5; K = 3;
    dimensions = size(x);
    nbCols = dimensions(1);
    E0 = 10;
    lengthAtBirth = 0;
    maximumLength = 1;
    %%I can't use gaussian function, but you can replace this with gaussmf if you
    %%have required toolbox installed
    mean = 1/2;
    sigma = 0.1;
    %u0 = 0.25*(1/(sigma*sqrt(2*pi)))*exp(-((x-mean).^2)/(2*sigma^2));
    u0 = rand(nbCols,1);
end