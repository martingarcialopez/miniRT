#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
//#include <sys/stat.h>

#define BUFSIZE 64

struct		p3
{
	char	x[16];
	char	y[16];
	char	z[16];
};

void		usage(char *prog_name)
{
	printf("%s <file.obj>\n", prog_name);
	exit(0);
}

void		fatal(char *message)
{
	char error_message[100];

	strcpy(error_message, "[!!] Fatal error ");
	strncat(error_message, message, 83);
	perror(error_message);
	exit(-1);
}

void		*ec_malloc(size)
{
	void	*ptr;

	ptr = malloc(size);
	if (ptr == NULL)
		fatal("in ec_malloc() on memory allocation");
	return (ptr);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		len;

	if (!s1 || !s2)
		return (NULL);
	len = strlen(s1) + strlen(s2) + 1;
	if (!(str = (char *)malloc(sizeof(char) * len)))
		return (NULL);
	str[0] = '\0';
	strcat(str, s1);
	strcat(str, s2);
	return (str);
}

char		*readfile(char *str, int fd)
{
	char	buf[BUFSIZE + 1];
	char	*ptr;
	int		ret;

	str = (char *)ec_malloc(sizeof(char) * (BUFSIZE + 1));
	while ((ret = read(fd, buf, BUFSIZE)) > 0)
	{
		ptr = str;
		buf[ret] = '\0';
		if (!(str = ft_strjoin(str, buf)))
			return (NULL);
		free(ptr);
	}
	return (str);
}

void		obj_to_rt(char *str, int fd, int vertex)
{
	struct p3	tab[vertex + 1];
	char		color[20];
	int			idx;
	int			i = 1;
	int			j;

	strcpy(color, "0 0 0 0 255,128,0");
	while (*str)
	{
		if (*str == 'v' && *(str + 1) == ' ')
		{
			str += 2;
			for (j = 0; *str != 32; j++)
				tab[i].x[j] = *str++;
			tab[i].x[j] = 0;
			str++;
			for (j = 0; *str != 32; j++)
				tab[i].y[j] = *str++;
			tab[i].y[j] = 0;
			str++;
			for (j = 0; *str != '\n'; j++)
				tab[i].z[j] = *str++;
			tab[i].z[j] = 0;
			i++;
		}
		else if (*str == 'f' && *(str + 1) == ' ')
		{
			str +=2;
			idx = atoi(str);
			dprintf(fd, "tr %s,%s,%s ", tab[idx].x, tab[idx].y, tab[idx].z);
			while (*str && *str != 32)
				str++;
			str += 1;
			idx = atoi(str);
			dprintf(fd, "%s,%s,%s ", tab[idx].x, tab[idx].y, tab[idx].z);
			while (*str && *str != 32)
				str++;
			idx = atoi(str);
			dprintf(fd, "%s,%s,%s", tab[idx].x, tab[idx].y, tab[idx].z);
			while (*str && *str != '\n')
				str++;
			dprintf(fd, "\t\t\t\t%s\n", color);
		}
		else
			while (*str && *str != '\n')
				str++;
		str++;
	}
	//printf("first vertex is x = %s, y = %s, z = %s\n", tab[546].x, tab[546].y, tab[546].z); 
}

int main(int ac, char **av)
{
	int		fd;
	int		fd2;
	char	*name;
	char	*file;
	int		vertex;
	int		i;

	if (ac != 2)
		usage(av[0]);
	if (!((fd = open(av[1], O_RDONLY)) > 0))
		fatal("in main() while opening file");
	name = (char *)ec_malloc(strlen(av[1]));
	for (i = 0; av[1][i] && av[1][i] != '.'; i++)
		name[i] = av[1][i];
	strcat(name, ".rt");
	//printf("name is %s\n", name);
	if (!((fd2 = open(name, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR)) > 0))
		fatal("in main() while creating file");
	file = readfile(file, fd);
	//printf("file is %s\n", file);

	i = 0;
	vertex = 0;
	while (file[i])
	{
		if (file[i] == 'v' && file[i + 1] == ' ')
			vertex++;
		else
			while (file[i] && file[i] != '\n')
				i++;
		i++;
	}
	//printf("number of vertex is %i\n", vertex);

	obj_to_rt(file, fd2, vertex);

	free(name);
	free(file);
	close(fd);
	close(fd2);
	return (0);
}
