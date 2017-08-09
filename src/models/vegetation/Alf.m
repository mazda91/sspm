function[res] = Alf(m,u,E)
    global cp1; global cp2;
    dimM = size(m); dimE = size(E);

    res = cp1*(E./(E+cp2));
    %if (dimM(1) ~= dimE(1))
	%res(1,:) = [];
    %end
end
