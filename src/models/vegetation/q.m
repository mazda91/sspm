function[res] = q(z,h)
    global eta;

    if (z <=h)
        res = 2*eta*(1-(z.^eta)*h^(-eta))*z^(eta-1)*h^(-eta);
    else
        res = 0;    
    end
end
