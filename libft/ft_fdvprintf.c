#include "ft_printf.h"

int		ft_fdvprintf(int fd, const char *format, va_list ap)
{
	t_part	*wlst;
	t_part	*tmp;
	int		ret;
	char	*str;

	wlst = NULL;
	str = NULL;
	if (format)
		str = apply_font((char *)format, ap);
	if (format)
		wlst = parse_format(str, ap);
	tmp = wlst;
	ft_strdel(&str);
	while (wlst)
	{
		if (wlst->res == NULL)
			wlst->res = make_conv(wlst->conv, ap);
		wlst = wlst->next;
	}
	ret = print_chain(tmp, fd);
	return (ret);
}
