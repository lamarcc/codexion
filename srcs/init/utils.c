/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 02:43:55 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/12 04:09:36 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/codexion.h"
#include <stdint.h>

static int	check_overflow(int nb, char digit)
{
	if ((long)nb * 10 + (digit - '0') > 2147483647)
		return (1);
	return (0);
}

static int	atoi_loop(char *str)
{
	int	i;
	int	nb;

	i = 0;
	nb = 0;
	while (str[i] >= 48 && str[i] <= 57)
	{
		if (check_overflow(nb, str[i]))
		{
			printf("value over INT_MAX, used INT_MAX by default\n\n");
			return (2147483647);
		}
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	if (!(str[i] >= 48 && str[i] <= 57) && str[i] != 0)
		return (-1);
	return (nb);
}

int	ft_atoi(char *str)
{
	int	i;
	int	nb;

	i = 0;
	nb = 0;
	if (!str)
		return (0);
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	while (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			return (-1);
		i++;
	}
	nb = atoi_loop(&str[i]);
	if (nb < 0)
		return (-1);
	return (nb);
}

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i++ < n)
		*(unsigned char *)(s++) = '\0';
}

void	*ft_calloc(size_t len, size_t size)
{
	unsigned char	*ptr;

	if (len == 0 || size == 0)
		return (0);
	if ((len / SIZE_MAX) > size)
		return (NULL);
	ptr = malloc(len * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, (len * size));
	return (ptr);
}
