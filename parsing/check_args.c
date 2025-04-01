/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:48:37 by gschwand          #+#    #+#             */
/*   Updated: 2025/03/25 13:58:25 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

// Reverse strncmp
static int	ft_strrncmp(const char *s1, const char *s2, size_t n)
{
	int	lens1;
	int	lens2;
	int	i;

	lens1 = ft_strlen(s1);
	lens2 = ft_strlen(s2);
	i = 0;
	if (n == 0)
		return (0);
	while (lens1 - i >= 0 && lens2 - i >= 0 && i < (int)n)
	{
		if (s1[lens1 - i] != s2[lens2 - i])
			return (((unsigned char *)s1)[lens1 - i]
				- ((unsigned char *)s2)[lens2 - i]);
		i++;
	}
	return (0);
}

int	check_args(int ac, char **av)
{
	if (ac != 2)
		return (ft_putstr_fd("Error: Wrong number of arguments\n", 2), 1);
	if (ft_strrncmp(av[1], ".rt", 3))
		return (ft_putstr_fd("Error: Wrong file extension\n", 2), 1);
	return (0);
}

// int main(void) {
// 	// Test ft_strrncmp
// 	printf("Testing ft_strrncmp:\n");
// 	printf("Test 1: %d\n", ft_strrncmp("hello.rt", ".rt", 3)); // Expected: 0
// 	printf("Test 2: %d\n", ft_strrncmp("hello.txt", ".rt", 3));
		// Expected: non-zero
// 	printf("Test 3: %d\n", ft_strrncmp("hello.rt", ".txt", 3));
		// Expected: non-zero
// 	printf("Test 4: %d\n", ft_strrncmp("hello.rt", ".rt", 0)); // Expected: 0
// 	// Test check_args
// 	printf("\nTesting check_args:\n");
// 	char *args1[] = {"program", "file.rt"};
// 	printf("Test 1: %d\n", check_args(2, args1)); // Expected: 0
// 	char *args2[] = {"program", "file.txt"};
// 	printf("Test 2: %d\n", check_args(2, args2)); // Expected: 1
// 	char *args3[] = {"program"};
// 	printf("Test 3: %d\n", check_args(1, args3)); // Expected: 1
// 	return (0);
// }
