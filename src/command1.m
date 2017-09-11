command_line = '';
solverPar = '';
modelPar = '';
operation = '';

if (nbLoops == 0)
    list = {' FMU',' MMU',' EBT',' CM'};
    [command,v] = listdlg('PromptString','Select a method','SelectionMode','single','ListString',list);
    command_line = strcat(' -method',list{command});
else 
    list = {'quit','continue'};
    [command,v] = listdlg('PromptString','Select one option','SelectionMode','single','ListString',list);
    operation = list{command};
    if (strcmp(operation,'quit') == 1)
        command_line = ' -quit';
    end
end
while ( (strcmp(operation,'solve') == 0) && (strcmp(operation,'quit') == 0) && (strcmp(operation,'show a solver parameter') == 0))
    list = {'change the method','change a solver parameter', 'show a solver parameter', 'solve','quit'};
    [command,v] = listdlg('PromptString','next operations','SelectionMode','single','ListString',list);
    operation = list{command};
    if (strcmp(operation,'change the method') == 1)
        list = {' FMU',' MMU',' EBT',' CM'};
	    [command,v] = listdlg('PromptString','Select a method','SelectionMode','single','ListString',list);
	    command_line = strcat(command_line,' -method ',list{command});
    elseif (strcmp(operation,'change a solver parameter') == 1)
        list = {' J',' M',' Tf',' tau'};
        [command,v] = listdlg('PromptString','Select a parameter','SelectionMode','single','ListString',list);
        prompt = {'Enter a value:'};
        dlg_title = 'Input';
        num_lines = 1
        answer = inputdlg(prompt,dlg_title,num_lines);
        solverPar = strcat(solverPar,' -csp',list{command},[' ',sprintf('%s',answer{1})]);
    elseif (strcmp(operation,'show a solver parameter') == 1)
        list = {' J',' M',' Tf',' tau'};
        [command,v] = listdlg('PromptString','Select a parameter','SelectionMode','single','ListString',list);
	    solverPar = strcat(solverPar,' -ssp ',list{command});
        nbLoops = nbLoops + 1;
    elseif (strcmp(operation,'quit') == 1)
	    command_line = strcat(command_line,' -q');
    elseif (strcmp(operation,'solve') == 1)
	    command_line = strcat(command_line,' -solve');
        nbLoops = nbLoops + 1;
    end
end
command_line = strcat(solverPar,modelPar,command_line);
