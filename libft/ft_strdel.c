/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strdel.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mmoya <marvin@le-101.fr>                   +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/01/24 18:18:43 by mmoya        #+#   ##    ##    #+#       */
/*   Updated: 2018/01/24 18:18:44 by mmoya       ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_strdel(char **as)
{
	void **ptr;

	if (as == NULL)
		return ;
	ptr = (void **)as;
	ft_memdel(ptr);
}
