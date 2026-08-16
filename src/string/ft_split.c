/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfurst <sfurst@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 17:04:48 by sfurst            #+#    #+#             */
/*   Updated: 2026/04/24 19:21:21 by sfurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sigma/libft.h>

static int	count_words(const char *str, char delim)
{
	int	count;

	count = 0;
	while (*str)
	{
		while (*str && *str == delim)
			str++;
		if (*str && *str != delim)
		{
			count++;
			while (*str && *str != delim)
				str++;
		}
	}
	return (count);
}

static char	*malloc_word(const char *str, char delim)
{
	char	*word;
	int		i;

	i = 0;
	while (str[i] && str[i] != delim)
		i++;
	word = malloc(sizeof(char) * (usize)(i + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != delim)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static void	free_split(char **arr, int used)
{
	while (used > 0)
		free(arr[--used]);
	free(arr);
}

static int	add_word(char **arr, int *i, const char *str, char c)
{
	arr[*i] = malloc_word(str, c);
	if (!arr[*i])
	{
		free_split(arr, *i);
		return (0);
	}
	(*i)++;
	return (1);
}

char	**ft_split(char const *str, char c)
{
	int		word_count;
	char	**arr;
	int		i;

	if (!str)
		return (NULL);
	word_count = count_words(str, c);
	arr = malloc(sizeof(char *) * (usize)(word_count + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (*str)
	{
		while (*str && *str == c)
			str++;
		if (*str && *str != c)
		{
			if (!add_word(arr, &i, str, c))
				return (NULL);
			while (*str && *str != c)
				str++;
		}
	}
	arr[i] = NULL;
	return (arr);
}
