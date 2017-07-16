function[res] = growthRate(m,u,E)

    res = max((1-r(m)).*P(m,E).*dm(m),0);
end
