function[res] = Alf(unknown,E)
    global cp1; global cp2;
    res = cp1*(E./(E+cp2));
end
