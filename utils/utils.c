/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 02:43:55 by celamarc          #+#    #+#             */
/*   Updated: 2026/05/28 04:28:04 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include <stdint.h>

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
