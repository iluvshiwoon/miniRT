/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:22:15 by kgriset           #+#    #+#             */
/*   Updated: 2025/03/25 13:42:31 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

int init_bmp(int fd, int W, int H)
{
    unsigned char *bmpfileheader;
    unsigned char *bmpinfoheader;
    int filesize;
    size_t bytes;
 
    filesize = 54 + 3 * W * H;
    bmpfileheader = (unsigned char[14]){'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
    bmpfileheader[2] = (unsigned char)(filesize);
    bmpfileheader[3] = (unsigned char)(filesize >> 8);
    bmpfileheader[4] = (unsigned char)(filesize >> 16);
    bmpfileheader[5] = (unsigned char)(filesize >> 24);
    bmpinfoheader = (unsigned char[40]){40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
    bmpinfoheader[4] = (unsigned char)(W);
    bmpinfoheader[5] = (unsigned char)(W >> 8);
    bmpinfoheader[6] = (unsigned char)(W >> 16);
    bmpinfoheader[7] = (unsigned char)(W >> 24);
    bmpinfoheader[8] = (unsigned char)(H);
    bmpinfoheader[9] = (unsigned char)(H >> 8);
    bmpinfoheader[10] = (unsigned char)(H >> 16);
    bmpinfoheader[11] = (unsigned char)(H >> 24);
    bytes = write(fd, bmpfileheader, 14);
    bytes += write(fd, bmpinfoheader, 40);
    return (bytes);
}

int init_img(int W, int H)
{
    int fd;
    size_t bytes;

    fd = open("out.bmp", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (fd == -1)
    {
        perror("open");
        return 0;
    }
    bytes = init_bmp(fd, W, H); 
    if (bytes != 14 + 40)
        return 0;
    return fd;
}

void save_img(t_rt * rt, const unsigned char * pixels, int W, int H) // rgb pixel
{
    unsigned char *bmppad;
    unsigned char * bgr_pixels;
    int i;
    int fd;

    i = -1;
    bmppad = (unsigned char[3]){0,0,0};
    fd = init_img(W, H); 
    bgr_pixels = wrap_malloc(rt, sizeof(unsigned char)*(W*H *3));
    while (++i < W * H)
    {
        bgr_pixels[i * 3] = pixels[i * 3 + 2];
        bgr_pixels[i * 3  + 1] = pixels[i * 3 + 1];
        bgr_pixels[i * 3  + 2] = pixels[i * 3];
    }
    i = -1;
    while (++i < H)
    {
        write(fd, &bgr_pixels[0] + (W*(H - i - 1)*3),3 * W);
        write(fd, bmppad, (4 - ( W * 3 ) % 4) % 4);
    }
    close(fd); 
}
