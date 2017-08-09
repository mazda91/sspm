function show(x,u,E,ti,Tf,M)
    loglog(x,u);
    title(sprintf('t = %0.3f',ti));
    pause(Tf/M);
end
