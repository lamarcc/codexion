/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 20:05:50 by celamarc          #+#    #+#             */
/*   Updated: 2026/05/19 00:46:07 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*routine()
{
	printf("Test\n");
	sleep(3);
	printf("End\n");
	return (NULL);
}

int	main(int c, char **v)
{
	if (c == 9)
		__builtin_printf("Non");
	else
	{
		(void)v;
		pthread_t t1, t2;
		pthread_create(&t1, NULL, &routine, NULL);
		pthread_create(&t2, NULL, &routine, NULL);
		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
	}
}
