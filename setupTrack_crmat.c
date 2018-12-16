
#include <stdlib.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <assert.h>
#include <string.h>

#include "ctk.h"

/*---------*/
/* globals */
/*---------*/

//volatile extern TRACKINGPAR Pars;
extern TRACKINGPAR Pars;

/*-----------------------------------------------------------------------*/

int
setupTrack_crmat (struct tms *timesThen, STAT * ctkStat, SHELLHIT * shellhit)
{

  /* declarations */

  int i, in, siz, j, zero = 0;
  char str[132], buffer[512];
  FILE *fp0;
  int ir[NUMAGATAPOS], dummy_i[NUMAGATAPOS];

  fprintf (stderr,"entered setupTrack_crmat\n");
  fflush (stdout);

  /* #include "crmat_assign.h" */

  sprintf (str, "crmat.LINUX");
  in = open (str, O_RDONLY, 0);
  if (in > 0)
    fprintf (stderr, "%s is open (input) binary format\n", str);
  else
    {
      fprintf (stderr, "could not open %s\n", str);
      exit (1);
    };
  siz = read (in, (char *) Pars.crmat, sizeof (Pars.crmat));
  fprintf (stderr, "read %i bytes into Pars.crmat\n", siz);
  close (in);

  for (i = 0; i <= 30; i++)
    for (j = 0; j < 4; j++)
      {
        fprintf (stderr, "%3i %i:\n", i, j);
        fprintf (stderr, "%10.5f %10.5f %10.5f %10.5f\n",
                 Pars.crmat[i][j][0][0], Pars.crmat[i][j][0][1], Pars.crmat[i][j][0][2], Pars.crmat[i][j][0][3]);
        fprintf (stderr, "%10.5f %10.5f %10.5f %10.5f\n",
                 Pars.crmat[i][j][1][0], Pars.crmat[i][j][1][1], Pars.crmat[i][j][1][2], Pars.crmat[i][j][1][3]);
        fprintf (stderr, "%10.5f %10.5f %10.5f %10.5f\n",
                 Pars.crmat[i][j][2][0], Pars.crmat[i][j][2][1], Pars.crmat[i][j][2][2], Pars.crmat[i][j][2][3]);
        fprintf (stderr, "%10.5f %10.5f %10.5f %10.5f\n",
                 Pars.crmat[i][j][3][0], Pars.crmat[i][j][3][1], Pars.crmat[i][j][3][2], Pars.crmat[i][j][3][3]);
      };

  fprintf (stderr, "\n");

  /* read in the AGATA rotational/translational data */

  fprintf (stderr,"Pars.AGATA_data=%i\n", Pars.AGATA_data);
  fflush (stdout);
  if (Pars.AGATA_data)
    {

      fprintf (stderr, "read in AGATA rotation and translation data\n");

      fp0 = fopen (Pars.AGATA_data_fn, "r");
      if (fp0 != NULL)
        {
          fprintf (stderr, "%s is open for reading\n", Pars.AGATA_data_fn);

          j = 0;
          for (i = 0; i < 180; i++)
            {

              memset (buffer, zero, sizeof (buffer));
              fgets (buffer, 150, fp0);
              sscanf (buffer, "%d %d %lf %lf %lf ", &ir, &dummy_i, &Pars.TrX[j], &Pars.TrY[j], &Pars.TrZ[j]);

              memset (buffer, zero, sizeof (buffer));
              fgets (buffer, 150, fp0);
              sscanf (buffer, "%d %lf %lf %lf  ", &dummy_i, &Pars.rotxx[j], &Pars.rotxy[j], &Pars.rotxz[j]);

              memset (buffer, zero, sizeof (buffer));
              fgets (buffer, 150, fp0);
              sscanf (buffer, "%d %lf %lf %lf  ", &dummy_i, &Pars.rotyx[j], &Pars.rotyy[j], &Pars.rotyz[j]);

              memset (buffer, zero, sizeof (buffer));
              fgets (buffer, 150, fp0);
              sscanf (buffer, "%d %lf %lf %lf  ", &dummy_i, &Pars.rotzx[j], &Pars.rotzy[j], &Pars.rotzz[j]);

              j++;
            };
          fprintf (stderr, "read %i AGATA rotational/translational matrices\n", j);

          for (j = 0; j < 180; j++)
            {
              fprintf (stderr, "%2i: \n", j);
              fprintf (stderr, "Tr?: %10.5f %10.5f %10.5f\n", Pars.TrX[j], Pars.TrX[j], Pars.TrZ[j]);
              fprintf (stderr, "rx?: %10.5f %10.5f %10.5f\n", Pars.rotxx[j], Pars.rotxy[j], Pars.rotxz[j]);
              fprintf (stderr, "ry?: %10.5f %10.5f %10.5f\n", Pars.rotyx[j], Pars.rotyy[j], Pars.rotyz[j]);
              fprintf (stderr, "rz?: %10.5f %10.5f %10.5f\n", Pars.rotzx[j], Pars.rotzy[j], Pars.rotzz[j]);
            }

        }
      else
        {
          fprintf (stderr, "Error: ascii %s not found, quit\n", Pars.AGATA_data_fn);
          exit (1);
        };

      fclose (fp0);
    };



  /* done */

  fprintf (stderr, "\n");
  return (0);

}
