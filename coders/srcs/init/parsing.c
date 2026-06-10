/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 01:19:57 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/11 00:34:01 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/codexion.h"

static size_t	ft_strlen(char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] == 0)
			return (0);
		i++;
	}
	return (i);
}

static void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	if (!dest && !src)
		return (NULL);
	i = 0;
	while (i < n)
	{
		*(unsigned char *)(dest + i) = *(unsigned char *)(src + i);
		i++;
	}
	*(unsigned char *)(dest + i) = 0;
	return (dest);
}

static char	*append(char *s1, char *s2)
{
	size_t	len;
	size_t	s1_len;
	size_t	s2_len;
	char	*dest;

	if (!s1 && !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	len = s1_len + s2_len;
	dest = malloc(len + 1);
	if (!dest)
		return (NULL);
	ft_memcpy(dest, s1, s1_len);
	ft_memcpy(dest + s1_len, s2, s2_len);
	free(s1);
	dest[len] = 0;
	return (dest);
}

static int	check_scheduler(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] -= 32;
		i++;
	}
	if (strcmp("FIFO", str) == 0 || strcmp("EDF", str) == 0)
		return (0);
	return (1);
}

int	check_args(t_simulation *sim, char **args)
{
	if (ft_atoi(args[1]) < 1 || ft_atoi(args[1]) > 5000)
		sim->errors = append(sim->errors, "Invalid number of coders value\n");
	if (ft_atoi(args[2]) < 1)
		sim->errors = append(sim->errors, "Invalid time to burnout value\n");
	if (ft_atoi(args[3]) < 1)
		sim->errors = append(sim->errors, "Invalid time to compile value\n");
	if (ft_atoi(args[4]) < 1)
		sim->errors = append(sim->errors, "Invalid time to debug value\n");
	if (ft_atoi(args[5]) < 1)
		sim->errors = append(sim->errors, "Invalid time to refactor value\n");
	if (ft_atoi(args[6]) < 1)
		sim->errors = append(sim->errors, "Invalid number of compile value\n");
	if (ft_atoi(args[7]) < 0)
		sim->errors = append(sim->errors, "Invalid dongle cooldown value\n");
	if (check_scheduler(args[8]))
		sim->errors = append(sim->errors, "Unexpected scheduler\n");
	if (ft_strlen(sim->errors))
		return (1);
	return (0);
}
