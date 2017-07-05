function[E0,u0]= init(x)
    %%u0 = gaussmf(x,[0 1]);
    dimensions = size(x);
    nbCols = dimensions(1);
    E0 = 10;
    %%I can't use gaussian function, but replace this with gaussmf if you
    %%have required toolbox installed
    mean = 1/2;
    sigma = 0.1;
    %u0 = 0.25*(1/(sigma*sqrt(2*pi)))*exp(-((x-mean).^2)/(2*sigma^2));
    u0 = rand(nbCols,1);
end
