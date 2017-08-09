function[lengthAtBirth,maximumLength,E0,u0,N0]= init(x)

    %delcaration of all variables requested in the model
    global nbCols; global alpha; global mu0; global r; global K;
    
    %variables initialization
    alpha = 0.75; mu0 = 0.1; r = 0.5; K = 3;
    dimensions = size(x);
    nbCols = dimensions(1); %necessary to know the size of the handled vectors


    %default values for mesh extremities
    lengthAtBirth = 0;
    maximumLength = 1;

    
    
    %initialization of the environment variable and the distribution density
    E0 = 10;
    mean = (maximumLength-lengthAtBirth)/2; sigma = 0.1;
    u0 = 0.25*(1/(sigma*sqrt(2*pi)))*exp(-((x-mean).^2)/(2*sigma^2)); 
    N0 = 0.25*(1/(sigma*sqrt(2*pi)))*exp(-((x-mean).^2)/(2*sigma^2));
    %u0 = rand(nbCols,1);
end
