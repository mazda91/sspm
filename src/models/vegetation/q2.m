function[res] = q2(z,height)
    global eta;

    res = 2*eta*(1-(z.^eta)' *(height.^(-eta))').*((z.^(eta-1))'*(height.^(-eta))');
    if (z>height)
        res = 0;
    end;
end
