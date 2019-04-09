/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efischer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 11:28:28 by efischer          #+#    #+#             */
/*   Updated: 2019/03/15 16:41:52 by efischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		ft_memjoin_free(t_out *t1, t_tmp *t2)
{
	void	*tmp;

	tmp = t1->str;
	t1->str = ft_memjoin(tmp, t2->str, t1->len, t2->len);
	free(tmp);
	free(t2->str);
}

static void		ft_get_flags(va_list *arg, t_out *out, char *str)
{
	char	*format;
	t_tmp	*tmp;
	t_flag	flag;

	out->str = NULL;
	if (!(tmp = (t_tmp*)malloc(sizeof(t_tmp))))
		return ;
	while ((format = ft_strchr(str, '%')))
	{
		ft_init_flag(&flag);
		tmp->str = ft_strsub(str, 0, format - str);
		tmp->len = ft_strlen(tmp->str);
		ft_memjoin_free(out, tmp);
		out->len += tmp->len;
		str = format;
		if (!(tmp->str = ft_process_flag(&str, arg, &flag)))
		{
			tmp->str = ft_strdup("(null)");
			tmp->len = 6;
		}
		tmp->len = ft_strlen(tmp->str);
		if (flag.null && flag.min && flag.width)
			tmp->len += flag.width;
		else if (flag.null)
			tmp->len++;
		ft_memjoin_free(out, tmp);
		out->len += tmp->len;
		if (*str)
			str++;
	}
	if (*str)
	{
		ft_init_flag(&flag);
		format = out->str;
		out->str = ft_memjoin(out->str, str, out->len, ft_strlen(str));
		out->len += ft_strlen(str);
		free(format);
	}
	free(tmp);
}

int				ft_printf(const char *format, ...)
{
	va_list	arg;
	t_out	*out;
	int		len;

	if (!format)
		return (0);
	if (!(out = (t_out*)malloc(sizeof(t_out))))
		return (-1);
	out->len = 0;
	if (ft_strchr(format, '%'))
	{
		va_start(arg, format);
		ft_get_flags(&arg, out, (char*)format);
		va_end(arg);
		write(1, out->str, out->len);
		free(out->str);
		len = out->len;
	}
	else
	{
		write(1, format, ft_strlen(format));
		len = ft_strlen(format);
	}
	free(out);
	return (len);
}
