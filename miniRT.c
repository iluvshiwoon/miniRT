/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 00:19:53 by kgriset           #+#    #+#             */
/*   Updated: 2025/01/20 16:10:13 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int main ()
{
    int W = 1024;
    int H = 1024;
    unsigned char * image;
    int i = -1;
	int j = -1;
    t_rt rt;

    rt = (t_rt){};
    rt.graphic_heap = init_alloc(&rt.graphic_heap); 
    rt.current_heap = rt.graphic_heap;
    image = wrap_malloc(&rt, sizeof(unsigned char)*W*H*3);
    while (++i < H)
    {
        j = -1;
        while (++j < W)
        {
            image[(i*W + j) * 3 + 0] = 55;
            image[(i*W + j) * 3 + 1] = 0;
            image[(i*W + j) * 3 + 2] = 0;
        }
    }
    save_img(&rt, image, W, H);
    free_heap(&rt);
    return 0;
}
