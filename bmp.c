/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:22:15 by kgriset           #+#    #+#             */
/*   Updated: 2025/01/19 20:48:04 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void save_img(const char * filename, const unsigned char * pixels, int W, int H) // rgb pixel
{
    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
    unsigned char bmppad[3] = {0,0,0};

    int filesize = 54 + 3 * W * H;
    bmpfileheader[2] = (unsigned char)(filesize);
    bmpfileheader[3] = (unsigned char)(filesize >> 8);
    bmpfileheader[4] = (unsigned char)(filesize >> 16);
    bmpfileheader[5] = (unsigned char)(filesize >> 24);

    bmpinfoheader[4] = (unsigned char)(W);
    bmpinfoheader[5] = (unsigned char)(W >> 8);
    bmpinfoheader[6] = (unsigned char)(W >> 16);
    bmpinfoheader[7] = (unsigned char)(W >> 24);
    bmpinfoheader[8] = (unsigned char)(H);
    bmpinfoheader[9] = (unsigned char)(H >> 8);
    bmpinfoheader[10] = (unsigned char)(H >> 16);
    bmpinfoheader[11] = (unsigned char)(H >> 24);
    
    int fd = open(filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (fd == -1)
    {
        perror("open");
        return;
    }
    size_t bytes = write(fd, bmpfileheader, 14);
    bytes += write(fd, bmpinfoheader, 40);
    if (bytes != 14 + 40)
    {
        perror("write");
        return;
    }
    unsigned char * bgr_pixels = malloc(sizeof(unsigned char)*(W*H *3));
    int i = -1;
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

int main ()
{
    int W = 1024;
    int H = 1024;
    unsigned char * image = malloc(sizeof(unsigned char)*W*H*3);
    int i = -1;
    while (++i < H)
    {
        int j = -1;
        while (++j < W)
        {
            image[(i*W + j) * 3 + 0] = 55;
            image[(i*W + j) * 3 + 1] = 50;
            image[(i*W + j) * 3 + 2] = 100;
        }
    }
    save_img("red.bmp", image, W, H);
    return 0;
}
