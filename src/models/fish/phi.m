function[res] = phi(w,wp)
    global beta1;
    global sigma1;
    res = exp(-((log(wp*beta1./w)).^2)/(2*sigma1.^2));
end

