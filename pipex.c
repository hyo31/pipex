#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stddef.h>
#include "pipex.h"

int	access_file(char *fname, int type)
{
	int	fd;

	if (type == 0)
		fd = open(fname, O_RDONLY);
	else
		fd = open(fname, O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (fd < 0)
	{
		if (type != 0)
			ft_error("zsh");
		write(STDERR_FILENO, "zsh: No such file or directory: ", 32);
		write(STDERR_FILENO, fname, ft_strlen(fname));
		write(STDERR_FILENO, "\n", 1);
	}
	dup2(fd, type);
	return (fd);
}

char	*get_path(char *cmd, char **env)
{
	char	*path;
	char	**dirs;
	char	*cmdfile;
	int		i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	path = env[i] + 5;
	i = 0;
	dirs = ft_split(path, ':');
	while (dirs[i])
	{
		cmdfile = ft_strjoin_c(dirs[i], cmd);
		free(dirs[i]);
		if (access(cmdfile, F_OK) == 0)
			break ;
		free(cmdfile);
		i++;
	}
	free(dirs);
	if (dirs[i])
		return (cmdfile);
	return (cmd);
}

void	exec_cmd(char *cmdline, char **env)
{
	char	**command;
	char	*path;

	command = ft_split(cmdline, ' ');
	path = get_path(command[0], env);
	execve(path, command, env);
	write(STDERR_FILENO, "zsh: command not found: ", 24);
	write(STDERR_FILENO, cmdline, ft_strlen(cmdline));
	write(STDERR_FILENO, "\n", 1);
	exit(127);
}

void	ft_pipe(char *cmdline, char **env)
{
	int		id;
	int		pipefd[2];

	pipe(pipefd);
	id = fork();
	if (id == -1)
		ft_error("Fork error");
	if (id)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
	}
	else
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		exec_cmd(cmdline, env);
	}
}

int	main(int argc, char **argv, char **env)
{
	int	fdin;
	int	fdout;

	if (argc == 5)
	{
		fdin = access_file(argv[1], 0);
		fdout = access_file(argv[4], 1);
		if (fdin < 0)
			exit(1);
		ft_pipe(argv[2], env);
		exec_cmd(argv[3], env);
	}
	else
		write(STDERR_FILENO, "Invalid number of arguments\n", 28);
	return (1);
}
