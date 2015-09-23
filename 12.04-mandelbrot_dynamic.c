#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>

typedef unsigned char pixel_t[3];

void Gradient(double position, pixel_t c);

/* picture resolution */
static const int ImageWidth  = 3000;
static const int ImageHeight = 3000;
static const double CxMin = -2.5;
static const double CxMax =  1.5;
static const double CyMin = -2.0;
static const double CyMax =  2.0;

int main()
{
  double PixelWidth = (CxMax-CxMin)/ImageWidth;

  double PixelHeight = (CyMax-CyMin)/ImageHeight;

  const int MaxColorComponentValue = 255;
  pixel_t *pixels = malloc(sizeof(pixel_t) * ImageHeight * ImageWidth);

  FILE *fp;

# pragma omp parallel shared(pixels)
  {
    int iY;
#   pragma omp for schedule(dynamic)
    for (iY = 0; iY < ImageHeight; iY++) {
      double Cy = CyMin + iY * PixelHeight;

      if (fabs(Cy) < PixelHeight / 2) {
        Cy = 0.0; /* Main antenna */
      }

      int iX;
      for (iX = 0; iX < ImageWidth; iX++) {
        double Cx = CxMin + iX * PixelWidth;

        double Zx = 0.0;
        double Zy = 0.0;

        double Zx2 = Zx*Zx;
        double Zy2 = Zy*Zy;

        /* */
        {
          int it;
          const int itMax = 50;
          const double Bailout = 2; /* bail-out value */
          const double CircleRadius = Bailout * Bailout; /* circle radius */

          for (it = 0; it < itMax && ((Zx2+Zy2) < CircleRadius); it++) {
            Zy  = 2*Zx*Zy + Cy;
            Zx  = Zx2-Zy2 + Cx;
            Zx2 = Zx*Zx;
            Zy2 = Zy*Zy;
          }

          if (it == itMax) {
            pixels[iY *ImageWidth + iX][0] = 0;
            pixels[iY *ImageWidth + iX][1] = 0;
            pixels[iY *ImageWidth + iX][2] = 0;
          }
          else {
            Gradient((double)((it-log2(log2(sqrt(Zx2+Zy2)))))/itMax,
                     pixels[iY*ImageWidth + iX]);
          }
        }
      }
    }
  }

  fp = fopen("MandelbrotSetNEW.ppm", "wb");
  fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", "# no comment",
              ImageWidth, ImageHeight, MaxColorComponentValue);
  fwrite(pixels, sizeof(pixel_t), ImageWidth * ImageHeight, fp);
  fclose(fp);
  free(pixels);

  return EXIT_SUCCESS;
}

void Gradient(double position, pixel_t c) {

  if (position > 1.0) {
    if (position - (int) position == 0.0) position = 1.0;
    else position = position - (int) position;
  }

  unsigned char nmax=7; // number of color bars
  double m = nmax*position;

  int n = (int) m;

  double f = m - n;
  unsigned char t = (int) (f * 255);

  switch (n) {
    case 0:
      c[0] =0;
      c[1] = t;
      c[2] = 255;
      break;
    case 1:
      c[0] = 0;
      c[1] = 255-t;
      c[2] = 255;
      break;
    case 2:
      c[0] = t;
      c[1] = 255;
      c[2] = 0;
      break;
    case 3:
      c[0] = 255;
      c[1] = 255 - t;
      c[2] = 0;
      break;
    case 4:
      c[0] = t;
      c[1] = 0;
      c[2] = 255;
      break;
    case 5:
      c[0] = 255;
      c[1] = 0;
      c[2] = 255 - t;
      break;
    default:
      c[0] = 255;
      c[1] = 0;
      c[2] = 0;
      break;
  }
}
