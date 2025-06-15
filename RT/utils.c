#include "../miniRT.h"

int close_win(t_rt * rt)
{
    mlx_destroy_image(rt->mlx, rt->image.img);
    mlx_destroy_window(rt->mlx, rt->win);
    mlx_destroy_display(rt->mlx);
    close(rt->fd_file);
    free_heap(rt);
    exit(0);
}
