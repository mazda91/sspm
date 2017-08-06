function[res] = q2(z,height)
    global eta;

    %if (z <=height)
     %   res = 2*eta*(1-(z.^eta)' *(height.^(-eta))').*((z.^(eta-1))'*(height.^(-eta))');
    %else
     %   res = 0;    
    %end
    
    res = 2*eta*(1-(z.^eta)' *(height.^(-eta))').*((z.^(eta-1))'*(height.^(-eta))');
    if (z>height)
        res = 0;
    end;
    %res = 2*eta*(1-(z.^eta)' *(height.^(-eta))).*((z.^(eta-1))'*(height.^(-eta)));
    %res((z>height),:) = 0;
    %res(:,(z>height)) = 0;
end
