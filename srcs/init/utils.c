/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 02:43:55 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/10 22:23:09 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/codexion.h"
#include <stdint.h>

static int	check_overflow(int nb, int sign, char digit)
{
	if (sign == 1)
		if ((long)nb * 10 + (digit - '0') > 2147483647)
			return (1);
	if (sign == -1)
		if ((long)nb * 10 + (digit - '0') > (long)2147483648)
			return (-1);
	return (0);
}

static int	atoi_loop(char *str, int sign)
{
	int	i;
	int	nb;
	int	check;

	i = 0;
	nb = 0;
	while (str[i] >= 48 && str[i] <= 57)
	{
		check = check_overflow(nb, sign, str[i]);
		if (check == 1)
			return (2147483647);
		else if (check == -1)
			return (-2147483648);
		else
			nb = nb * 10 + (str[i] - '0');
		i++;
	}
	return (nb * sign);
}

int	ft_atoi(char *str)
{
	int	i;
	int	sign;

	i = 0;
	sign = 1;
	if (!str)
		return (0);
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	while (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -sign;
		i++;
	}
	return (atoi_loop(&str[i], sign));
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
		return (malloc(0));
	if ((len / SIZE_MAX) > size)
		return (NULL);
	ptr = malloc(len * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, (len * size));
	return (ptr);
}
