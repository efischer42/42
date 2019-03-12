/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_flag.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efischer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 11:56:32 by efischer          #+#    #+#             */
/*   Updated: 2019/03/12 11:56:35 by efischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char		*ft_percent(char c, t_flag *flag)
{
	char	*format;

	format = ft_strdup("%");
	if (flag->width)
		format = ft_width(c, format, flag);
	return (format);
}

static void		ft_precision_width(t_flag *flag, char **s)
{
	if (flag->precision == -1)
		flag->precision = ft_atoi(*s);
	else
		flag->width = ft_atoi(*s);
	while (ft_isdigit(**s))
		(*s)++;
	(*s)--;
}

static char		*ft_flag_error(char *s, t_flag *flag)
{
	char	*format;
	char	*tmp;
	int		len;

	format = ft_strndup(s, 1);
	if ((len = flag->width - ft_strlen(format)) > 0)
	{
		if (!(tmp = (char*)malloc(sizeof(char) * (len + 1))))
		{
			ft_strdel(&format);
			return (NULL);
		}
		tmp[len--] = '\0';
		while (len >= 0)
			tmp[len--] = ' ';
		if (flag->min)
			format = ft_join_free(format, tmp, 3);
		else
			format = ft_join_free(tmp, format, 3);
	}
	return (format);
}

static char		*ft_all_conv(char c, va_list *arg, t_flag *flag)
{
	if (c == 'c' || c == 's' || c == 'p')
		return (ft_manage_str(c, arg, flag));
	else if (c == 'd' || c == 'i' || c == 'o' || c == 'u' || c == 'x'
	|| c == 'X' || c == 'f')
		return (ft_diouxxf(c, arg, flag));
	else
		return (ft_strdup("Z"));
}

char			*ft_process_flag(char **s, va_list *arg, t_flag *flag)
{
	(*s)++;
	while (**s)
	{
		if (**s == '#' || **s == '0' || **s == '-' || **s == '+' || **s == ' '
		|| **s == '.')
			ft_manage_flag(**s, flag);
		else if (**s == '%')
			return (ft_percent(**s, flag));
		else if (**s == 'h' || **s == 'l' || **s == 'L')
			ft_manage_conv_flag(**s, flag);
		else if (**s == 'd' || **s == 'i' || **s == 'o' || **s == 'u'
		|| **s == 'x' || **s == 'X' || **s == 'f' || **s == 'c' || **s == 's'
		|| **s == 'p' || **s == 'Z')
			return (ft_all_conv(**s, arg, flag));
		else if (**s >= '1' && **s <= '9')
			ft_precision_width(flag, s);
		else
			return (ft_flag_error(*s, flag));
		(*s)++;
	}
	return (ft_strdup("\0"));
}