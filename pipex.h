#ifndef PIPEX_H
# define PIPEX_H
# include <stdio.h>
# include <stdlib.h>

char	**ft_split(const char *s, char c);
char	*ft_strjoin_c(char const *s1, char const *s2);
size_t	ft_strlen(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	ft_error(char *msg);

#endif