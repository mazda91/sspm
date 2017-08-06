function[res] = Alf(m,u,E)
    global cp1; global cp2;
    env = environment(m,u,E);
    res = cp1*(env/(env+cp2));
end
