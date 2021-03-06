/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtin.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mmoya <mmoya@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/31 15:34:18 by mmoya        #+#   ##    ##    #+#       */
/*   Updated: 2018/09/27 17:47:22 by mmoya       ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		sh_cd(char **arg, char ***env)
{
	char	*cur;
	char	*pwd;
	char	*new;
	int		b;

	new = cd_checkenv(arg, env, &b);
	if (cd_error(new) != 1)
		return (1);
	(cur = ft_strnew(PATH_MAX)) ? 0 : sh_exit(-1, arg, *env);
	if (chdir(new) == 0)
	{
		cur = getcwd(cur, PATH_MAX);
		if (!(pwd = sh_getenv("PWD", *env)))
			pwd = cur;
		if (sh_setenv("OLDPWD", pwd, env, 0) ||
		sh_setenv("PWD", cur, env, 0))
			return (err_shell("cd: OLDPWD not set"));
		b ? sh_pwd(*env) : 0;
		ft_strdel(&cur);
		return (0);
	}
	return (1);
}

int		sh_pwd(char **env)
{
	char	*cur;
	char	*tmp;

	(void)env;
	if (!(cur = ft_strnew(PATH_MAX + 1)))
		sh_exit(-1, NULL, env);
	if (!(cur = getcwd(cur, PATH_MAX)))
		sh_exit(-1, NULL, env);
	if (!(tmp = ft_strjoin(cur, "\n")))
		sh_exit(-1, NULL, env);
	ft_putstr(tmp);
	ft_strdel(&cur);
	ft_strdel(&tmp);
	return (0);
}

int		sh_env(char **env)
{
	int		i;

	i = 0;
	while (env[i])
		ft_putendl(env[i++]);
	return (0);
}

int		sh_unsetenv(char *arg, char **env)
{
	int		i;
	int		j;
	size_t	len;

	i = -1;
	j = 0;
	if (arg == NULL)
		return (err_usage("unsetenv key"));
	len = ft_strlen(arg);
	while (env[++i])
	{
		j += (ft_strncmp(env[i], arg, len) == 0 && env[i][len] == '=');
		if (j)
		{
			ft_strdel(&env[i]);
			if (env[i + j] == NULL)
				env[i] = NULL;
			else
				!(env[i] = ft_strdup(env[i + j])) ? sh_exit(-1, NULL, env) : 0;
		}
	}
	if (j == 0)
		return (err_shell("unsetenv : unable to unsetenv"));
	return (0);
}

int		sh_execute(char **arg, char **env)
{
	char	**bin;
	int		i;
	int		r;

	i = 0;
	!(bin = ft_strsplit(sh_getenv("PATH", env), ':')) ?
	sh_exit(-1, arg, env) : 0;
	while (bin[i])
	{
		if ((r = sh_execute_path(bin[i++], arg, env)) != -1)
		{
			i = -1;
			while (bin[++i])
				ft_strdel(&bin[i]);
			free(bin);
			return (r);
		}
	}
	i = -1;
	while (bin[++i])
		ft_strdel(&bin[i]);
	free(bin);
	return (-1);
}
