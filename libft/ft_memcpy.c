/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:45:31 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/10 00:35:08 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*d_ptr;
	const unsigned char	*s_ptr;

	if (n == 0)
		return (dest);
	if (!dest && !src)
		return (NULL);
	d_ptr = (unsigned char *)dest;
	s_ptr = (const unsigned char *)src;
	while (n--)
	{
		*d_ptr = *s_ptr;
		d_ptr++;
		s_ptr++;
	}
	return (dest);
}
