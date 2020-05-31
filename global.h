#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <float.h>
#include <grass/raster.h>
#include <grass/vector.h>

#define REALLOC_INCREMENT 1024

#ifndef DBL_MAX
#define DBL_MAX 1.797693E308    /* DBL_MAX approximation */
#endif
#define OUTLET -DBL_MAX

#define NE 1
#define N 2
#define NW 3
#define W 4
#define SW 5
#define S 6
#define SE 7
#define E 8

/* bit */
#define BITSIZE (((rows * cols) >> 3) + ((rows * cols) & 7 ? 1 : 0))
#define SETBIT(b, r, c) \
    b[((r) * cols + (c)) >> 3] = \
    (b[((r) * cols + (c)) >> 3] & ~(1 << (((r) * cols + (c)) & 7))) \
    | (1 << (((r) * cols + (c)) & 7))
#define CLEARBIT(b, r, c) \
    b[((r) * cols + (c)) >> 3] = \
    b[((r) * cols + (c)) >> 3] & ~(1 << (((r) * cols + (c)) & 7))
#define GETBIT(b, r, c) \
    ((b[((r) * cols + (c)) >> 3] & (1 << (((r) * cols + (c)) & 7))) \
     >> (((r) * cols + (c)) & 7))

/* 2 bits */
#define CRUMBSIZE (((rows * cols) >> 2) + ((rows * cols) & 3 ? 1 : 0))
#define SETCRUMB(b, r, c, d) \
    b[((r) * cols + (c)) >> 2] = \
    (b[((r) * cols + (c)) >> 2] & ~(3 << ((((r) * cols + (c)) & 3) << 1))) \
    | ((d) << ((((r) * cols + (c)) & 3) << 1))
#define GETCRUMB(b, r, c) \
    ((b[((r) * cols + (c)) >> 2] & (3 << ((((r) * cols + (c)) & 3) << 1))) \
     >> ((((r) * cols + (c)) & 3) << 1))

/* 4 bits */
#define NIBBLESIZE (((rows * cols) >> 1) + ((rows * cols) & 1 ? 1 : 0))
#define SETNIBBLE(b, r, c, d) \
    b[((r) * cols + (c)) >> 1] = \
    (b[((r) * cols + (c)) >> 1] & ~(15 << (((r) * cols + (c)) & 1 ? 4 : 0))) \
    | ((d) << (((r) * cols + (c)) & 1 ? 4 : 0))
#define GETNIBBLE(b, r, c) \
    ((b[((r) * cols + (c)) >> 1] & (15 << (((r) * cols + (c)) & 1 ? 4 : 0))) \
     >> (((r) * cols + (c)) & 1 ? 4 : 0))

/* done */
#define DONESIZE (neg ? CRUMBSIZE : BITSIZE)
#define SETDONE(b, r, c, d) if(neg) SETCRUMB(b, r, c, d); else SETBIT(b, r, c)
#define GETDONE(b, r, c) (neg ? GETCRUMB(b, r, c) : GETBIT(b, r, c))

struct raster_map
{
    RASTER_MAP_TYPE type;
    int rows, cols;
    union
    {
        void **v;
        CELL **c;
        FCELL **f;
        DCELL **d;
    } map;
};

struct point_list
{
    int nalloc, n;
    double *x, *y;
};

struct line
{
    struct line_pnts *Points;
    double length;
};

struct line_list
{
    int nalloc, n;
    struct line **lines;
};

#ifdef _MAIN_C_
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL int dir_checks[3][3][2]
#ifdef _MAIN_C_
    = {
    {{SE, NW}, {S, N}, {SW, NE}},
    {{E, W}, {0, 0}, {W, E}},
    {{NE, SW}, {N, S}, {NW, SE}}
}
#endif
;

/* raster.c */
void set(struct raster_map *, int, int, double);
double get(struct raster_map *, int, int);
int is_null(struct raster_map *, int, int);
void set_null(struct raster_map *, int, int);

/* point_list.c */
void init_point_list(struct point_list *);
void reset_point_list(struct point_list *);
void free_point_list(struct point_list *);
void add_point(struct point_list *, double, double);

/* line_list.c */
void init_line_list(struct line_list *);
void reset_line_list(struct line_list *);
void free_line_list(struct line_list *);
void add_line(struct line_list *, struct line *);

/* accumulate.c */
void accumulate(char *, struct raster_map *, struct raster_map *, char *,
                char);

/* delineate_streams.c */
void delineate_streams(struct Map_info *, char *, struct raster_map *, double,
                       char);

/* subaccumulate.c */
void subaccumulate(struct Map_info *, char *, struct raster_map *,
                   struct point_list *);

/* calculate_lfp.c */
void calculate_lfp(struct Map_info *, char *, struct raster_map *, int *,
                   char *, struct point_list *);

#endif
