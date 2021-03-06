/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   execute.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mmoya <mmoya@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/02/07 15:37:30 by mmoya        #+#   ##    ##    #+#       */
/*   Updated: 2018/09/28 17:45:28 by mmoya       ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*execute_path_check(char *path, char **arg, char **env)
{
	char	*tmp;
	char	*tmp2;

	if (path)
	{
		!(tmp2 = ft_strjoin(path, "/")) ? sh_exit(-1, arg, env) : 0;
		!(tmp = ft_strjoin(tmp2, arg[0])) ? sh_exit(-1, arg, env) : 0;
		ft_strdel(&tmp2);
		return (tmp);
	}
	return (ft_strdup(arg[0]));
}

int			sh_execute_path(char *path, char **arg, char **env)
{
	struct stat	path_stat;
	char		*tmp;
	pid_t		pid;
	int			st;

	!(tmp = execute_path_check(path, arg, env)) ? sh_exit(-1, arg, env) : 0;
	stat(tmp, &path_stat);
	if (access(tmp, F_OK) == -1 || S_ISREG(path_stat.st_mode) == 0)
		return (exec_error(-1, tmp, arg[0]));
	if (access(tmp, X_OK) == -1)
		return (exec_error(-2, tmp, arg[0]));
	pid = fork();
	if (pid == 0)
		((execve(tmp, arg, env)) == -1) ? exit(err_shell("execve error")) : 0;
	if (pid != 0)
	{
		wait(&st);
		ft_strdel(&tmp);
		return (WEXITSTATUS(st));
	}
	ft_strdel(&tmp);
	return (-1);
}

int			sh_echo(char **arg, char **env)
{
	int		i;
	int		j;

	i = 1;
	(void)env;
	while (arg[i])
	{
		j = 0;
		while (arg[i][j] != 0)
			ft_putchar(arg[i][j++]);
		ft_putchar(' ');
		i++;
	}
	ft_putstr("\n");
	return (0);
}

int			sh_exit(int out, char **arg, char **env)
{
	int i;

	i = -1;
	if (env != NULL)
	{
		while (env[++i])
			ft_strdel(&env[i]);
		if (arg != NULL)
			if (arg[1] != NULL)
				out = ft_atoi(arg[1]);
	}
	i = -1;
	if (arg != NULL)
		while (arg[++i] != NULL)
			ft_strdel(&arg[i]);
	free(arg);
	i = -1;
	free(env);
	exit(out);
	return (out);
}

char		*sh_lineread(void)
{
	char	*line;
	char	buf[2];
	char	*tmp;
	int		ret;

	line = ft_strnew(0);
	while ((ret = read(1, buf, 1)) != 0)
	{
		buf[ret] = '\0';
		if (buf[0] == 27)
			buf[0] = 0;
		if (ft_strcmp(buf, "\n") == 0)
			break ;
		tmp = line;
		line = ft_strjoin(line, buf);
		ft_strdel(&tmp);
	}
	return (line);
}
