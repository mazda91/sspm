command_line = '';
model = '';
model2 = '';
solverPar = '';
modelPar = '';
list = {' daphnia',' fish',' vegetation'};
[command,v] = listdlg('PromptString','Select a model','SelectionMode','single','ListString',list);
model = list{command};
command_line = strcat(' -model',model);
list = {' FMU',' MMU',' EBT',' CM'};
[command,v] = listdlg('PromptString','Select a model','SelectionMode','single','ListString',list);
command_line = strcat(' -method',list{command});

while ( (strcmp(list{command},'solve') == 0) && (strcmp(list{command},'quit') == 0))
    list = {'change the model','change the method','change a solver parameter','change a current model parameter', 'show a solver parameter','show a current model parameter', 'solve','quit'};
    [command,v] = listdlg('PromptString','next operations','SelectionMode','single','ListString',list);
    if (strcmp(list{command},'change the model') == 1)
	    myicon=imread('../images/warning.png');
        h = msgbox('If the model changes, all operations regarding the previous model parameters will be removed','Warning','custom',myicon);
        uiwait(h);
        list = {' daphnia',' fish',' vegetation'};
    	[command,v] = listdlg('PromptString','Select a model','SelectionMode','single','ListString',list);
    	model2 = list{command};
        if (strcmp(model,model2) == 0)
		    modelPar = '';
            model = model2;
        end
    elseif (strcmp(list{command},'change the method') == 1)
        list = {' FMU',' MMU',' EBT',' CM'};
	    [command,v] = listdlg('PromptString','Select a model','SelectionMode','single','ListString',list);
	    command_line = strcat(command_line,' -method ',list{command});
    elseif (strcmp(list{command},'change a solver parameter') == 1)
        list = {' J',' M',' Tf',' tau'};
        [command,v] = listdlg('PromptString','Select a model','SelectionMode','single','ListString',list);
        prompt = {'Enter a value:'};
        dlg_title = 'Input';
        num_lines = 1
        answer = inputdlg(prompt,dlg_title,num_lines);
        solverPar = strcat(solverPar,' -csp',list{command},[' ',sprintf('%s',answer{1})]);
    elseif (strcmp(list{command},'change a current model parameter') == 1)
        if (strcmp(model,' daphnia') == 1)
            list = {' lb',' lm',' alpha',' mu0',' r',' K'};
            [command,v] = listdlg('PromptString','Select a model','SelectionMode','single','ListString',list);
            prompt = {'Enter a value:'};
            dlg_title = 'Input';
            num_lines = 1
            answer = inputdlg(prompt,dlg_title,num_lines);
            modelPar = strcat(modelPar,' -csp ',list{command},[' ',sprintf('%s',answer{1})]);
        end
    elseif (strcmp(list{command},'show a solver parameter') == 1)
        list = {' J',' M',' Tf',' tau'};
        [command,v] = listdlg('PromptString','Select a model','SelectionMode','single','ListString',list);
	    solverPar = strcat(solverPar,' -ssp ',list{command});
    elseif (strcmp(list{command},'show a current model parameter') == 1)
        if (strcmp(model,' daphnia') == 1)
          list = {' lb',' lm',' alpha',' mu0',' r',' K'};
          [command,v] = listdlg('PromptString','Select a model','SelectionMode','single','ListString',list);
          modelPar = strcat(modelPar,' -smp ',list{command});
        end
    elseif (strcmp(list{command},'quit') == 1)
	    command_line = strcat(command_line,' -q');
    elseif (strcmp(list{command},'solve') == 1)
	    command_line = strcat(command_line,' -solve');
    end
end
command_line = strcat(' -model',model,solverPar,modelPar,command_line);
