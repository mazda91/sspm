function[res] = phi(w,wp)
    global beta
    global sigma1;
    res = exp(-((log(wp*beta./w)).^2)/(2*sigma1.^2));
end

