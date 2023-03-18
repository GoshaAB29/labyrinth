#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "llqueue.h"

#define MAX_BUFF_SIZE 8192

#define IS_VALID(x,y) ((x)*(x) + (y)*(y) > 0)
#define IN_BOUNDS(X, Y) (((X) >= 0) && ((X) < W) && ((Y) >= 0) && ((Y) < H))
#define CELL(X, Y) (*(Lab + W*(Y) + (X)))

List* Queue;
unsigned int* Lab;
unsigned int W, H;
unsigned int IX, IY;
unsigned int OX, OY;
char BUF[MAX_BUFF_SIZE];

typedef struct _cell {
    int x, y;
} Cell;


int walk()
{
	Cell* c;
	Cell** front;
	int dx = 0, dy = 0, stride = 0;
	unsigned int p = 0, p1 = 0, p2 = 0;

	if (!Queue) {
		printf("Front is empty!\n");
		return 0;
	}
	dequeue (&Queue, (void**)&c);
	if ((W * c->y + c->x) <= sizeof (BUF))
        p = CELL(c->x, c->y);
    else
        p = (unsigned int)(-1);

	for (dx = -1; dx <= 1; dx++)
	{
		for (dy = -1; dy <= 1; dy++)
		{

			if (IS_VALID (dx, dy) && IN_BOUNDS(c->x + dx, c->y + dy) && (dx * dy == 0))
			{
                stride = 10;

				if (CELL(c->x + dx, c->y + dy) > p + stride)
				{
					CELL(c->x + dx, c->y + dy) = p + stride;

					enqueue (&Queue, (void***)&front);
					*front = (Cell*)malloc(sizeof(Cell));
					**front = (Cell){c->x + dx, c->y + dy};
				}
			}

			if (IS_VALID (dx, dy) && IN_BOUNDS (c->x + dx, c->y + dy) && (dx * dy != 0)
                && ((CELL(c->x + dx, c->y) > p) || (CELL(c->x, c->y + dy) > p)) )
			{
                stride = 14;
                
				if (CELL(c->x + dx, c->y + dy) > p + stride)
				{
					CELL(c->x + dx, c->y + dy) = p + stride;

					enqueue (&Queue, (void***)&front);
					*front = (Cell*)malloc(sizeof(Cell));
					**front = (Cell){c->x + dx, c->y + dy};
				}
			}

		}
	}
	free(c);
	return walk();
}


void init_walk ()
{
	Cell** init;
	CELL(IX, IY) = 1;
	enqueue (&Queue, (void***)&init);
	*init = malloc (sizeof (Cell));
	**init = (Cell){IX, IY};
}

int lab_read (char* filename)
{
	int i = 0, k = 0;
	FILE *f = fopen (filename, "r");
	if (!f) return -1;

	Lab = NULL;

	while (!feof (f)) {
		if (!fgets (BUF, sizeof (BUF), f)) break;
		if (!H) W = strlen (BUF);

		Lab = realloc (Lab, sizeof (*Lab) * W * (H+1));

		for (i = 0; i < W && BUF[i]; i++) {
			switch (BUF[i]) {
			case 'i': case 'o': case ' ':
				*(Lab + W*H + i) = -1;
				break;
			case '#': default:
				*(Lab + W*H + i) = 0;
			}

			if (BUF[i] == 'i') {
				IX = i;
				IY = H;
			} else if (BUF[i] == 'o') {
				OX = i;
				OY = H;
			}
		}
		while (i < W) *(Lab + W*H + i++) = 0;
		H++;
	}
	fclose (f);

	return 0;
}

int walk_back (int x, int y, int p)
{
	int dx = 0, dy = 0, stride = 0;
	if (p == 1) return 0;

	for (dx = -1; dx <= 1; dx++) {
		for (dy = -1; dy <= 1; dy++) {
            if (dx * dy != 0)
                stride = 14;
            else
                stride = 10;

			if ((IS_VALID (dx,dy)) && (IN_BOUNDS (x+dx, y+dy) )) {
				if (CELL(x+dx, y+dy) == p - stride) {
					CELL(x+dx, y+dy) = (unsigned int)(-2);
					return walk_back (x+dx, y+dy, p - stride);
				}
			}
		}
	}
	return -1;
}

void printf_lab () {
	int x = 0, y = 0;
	for (y = 0; y < H; y++) {
		for (x = 0; x < W; x++) {
			switch (CELL(x, y)) {
			case 0:
				BUF[x] = '#'; break;
			case (unsigned int)(-2):
				BUF[x] = '.'; break;
			default:
                BUF[x] = ' ';
			}

			if (x == IX && y == IY)
                BUF[x] = 'i';
            if (x == OX && y == OY)
                BUF[x] = 'o';
		}

		BUF[W] = '\0';
		puts (BUF);
	}
}

int main (int argc, char* argv[])
{
	//argc = 2;
	//argv[1] = "file1.lab";

	char* filename;
	int err = 0;
	if (argc < 2) {
		fprintf (stderr, "usage labyrinth <file.lab>\n");
		exit (1);
	}

	filename = argv[1];
	err = lab_read (filename);

	if (err != 0) {
		fprintf (stderr, "Error while reading labyrinth: %d\n", err);
		exit (1);
	}
	init_walk ();
	walk ();

	if (CELL(OX, OY) < (unsigned int)(-1)) {
		walk_back (OX, OY, CELL(OX, OY));
		printf_lab ();
	} else {
		fprintf (stderr, "Path not found!(\n");
	}

	free (Lab);

	return 0;
}
