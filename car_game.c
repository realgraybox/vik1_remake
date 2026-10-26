/*
 * ============================================================================
 *  PROJECT NAME / vik1_remake
 * ============================================================================
 * 
 *  ORIGINAL WORK CREDIT:
 *  This project is a de-obfuscated, modified, and enhanced version of 'vik1' 
 *  (an interactive X11 car racing game), originally written by Daniel Vik 
 *  for the 2004 International Obfuscated C Code Contest (IOCCC), where it 
 *  won "Best X11 game". The original code was dedicated to the Public Domain.
 * 
 *  LICENSE FOR THIS MODIFIED WORK:
 *  To the extent possible under law, the contributor has dedicated all 
 *  copyright and related and neighboring rights of their additions and 
 *  modifications to the public domain worldwide under the CC0 1.0 Universal 
 *  Public Domain Dedication.
 * 
 *  This work is distributed WITHOUT ANY WARRANTY; without even the implied 
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 *  For the full CC0 legal text, see the 'LICENSE' file or visit:
 *  https://creativecommons.org
 * ============================================================================
 */

/* M.Glargaard (aka graybox) 2026
 * car_game.c - a readable and extended version of "vik1",
 *              a winning entry of the 2004 IOCCC.
 *
 * The original program is kept unchanged in vik1.c.
 *
 * Build:  gcc -O2 -Wall -o car_game car_game.c -lX11
 *
 * Usage:  ./car_game [options]   options are letters in one word, e.g. "ad2"
 *           a   winter (snow, slippery road)
 *           d   night
 *           1-3 track (3 is the default)
 *           f   start in full screen
 *
 * Keys:   Up = accelerate, Left/Right = steer
 *         F or F11 = toggle full screen, ESC = quit
 *         R = new race (on the finish screen)
 *
 * The window can be resized freely. The game always draws into a
 * 400x200 picture, which is scaled up to fill the window while
 * keeping its proportions (black bars where needed).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>

#define min(a,b) ((a) < (b) ? (a) : (b))

#define FRAME_DELAY_US  65800   /* same frame time as the original, about 15 fps */
#define INTERNAL_WIDTH  400     /* the game always draws at 400x200 */
#define INTERNAL_HEIGHT 200
#define DEFAULT_SCALE   2       /* initial window: 800x400 */
#define C_SIZE          (1 << 18)
#define MAX_LAPS        2

/*
 * The original single-letter globals. Most of them are reused for
 * different purposes in different parts of a frame, so they are kept
 * as they are. Some that have a fixed meaning:
 *   u  distance along the track      p  track length (10660)
 *   s  speed                         t  sideways position on the road
 *   j  steering                      c  time at which the current lap started
 *   h  current lap time (between time_trigger() and draw_HID())
 *   q  best lap time                 B  track number at start, later reused
 *                                       as the opponent's lane offset
 *   C  track curve C[0..p], track height C[p..2p], key states C[INPUT_OFFSET+key]
 */
int g, h, i, j, k, l, m, n, p, q, r, v, x, y;
int o, b, f, u, s, c, a, t, d, C[C_SIZE], B = 4;

int screen_height = INTERNAL_HEIGHT;
int screen_width  = INTERNAL_WIDTH;

const int TIME_SCALE   = 53300;   /* also 0x00D034, the green grass colour */
const int INPUT_OFFSET = 65793;   /* 0x010101: key-state offset in C[], and a grey multiplier */
const int SPEED_SCALE  = 100;

int z = 0;   /* is_winter */
int w = 0;   /* is_night  */

/* Race state */
enum GameState { STATE_RACING, STATE_FINISHED };
int game_state = STATE_RACING;
int laps       = -1;   /* -1 until the start line has been crossed the first time */
int total_time = 0;    /* sum of the completed lap times, in 1/100 s */
int max_speed  = 0;
int running    = 1;

/* X11 */
Display *display;
Window   window;
Pixmap   pixmap;
GC       gc;
XEvent   event;
Atom     wm_delete_window, net_wm_state, net_wm_state_fullscreen;
int      window_width  = INTERNAL_WIDTH  * DEFAULT_SCALE;
int      window_height = INTERNAL_HEIGHT * DEFAULT_SCALE;
int      fullscreen    = 0;

static struct timeval program_start;

/* ------------------------------------------------------------------ */
/* Time                                                                */
/* ------------------------------------------------------------------ */

static long long now_us(void)
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return (long long)(tv.tv_sec - program_start.tv_sec) * 1000000LL
         + (tv.tv_usec - program_start.tv_usec);
}

/* Game clock in 1/100 s since program start.
 * (The original used 100*seconds + microseconds/10660, so its
 * "hundredths" only ran from 0 to 93, and the value overflowed int.) */
static int clock_ticks(void)
{
    return (int)(now_us() / 10000);
}

/* Current lap time */
static void time_trigger(void)
{
    h = clock_ticks() - c;
}

/* Keeps the original pace (one frame every FRAME_DELAY_US) even when
 * scaling to a large window takes part of the frame time. */
static void wait_for_next_frame(void)
{
    static long long next_frame = 0;
    long long now = now_us();

    if (next_frame == 0 || now > next_frame + FRAME_DELAY_US)
        next_frame = now;              /* first frame, or far behind */
    next_frame += FRAME_DELAY_US;
    if (next_frame > now)
        usleep((useconds_t)(next_frame - now));
}

/* ------------------------------------------------------------------ */
/* Drawing primitives                                                  */
/* ------------------------------------------------------------------ */

unsigned int color_component(int a, int argc, int y, int v, int z, int w)
{
    int temp = (((y / v) * !w) +
               ((((argc + 1) >> a) & 255) * (screen_height + y / (z | (w - 2))) / screen_height));
    return temp << a;
}

/* The renderer of the original: set a colour and draw two mirrored
 * horizontal line segments on row o, from m to n either side of the
 * centre (screen_height + l). */
void draw_mode_setcolors(int value)
{
    unsigned int color_val = color_component(0, value, y, v, z, w) |
                             color_component(8, value, y, v, z, w) |
                             color_component(16, value, y, v, z, w);

    XSetForeground(display, gc, color_val);

    int center_x = screen_height + l;
    XDrawLine(display, pixmap, gc, center_x - m, o, center_x - n, o);
    XDrawLine(display, pixmap, gc, center_x + m, o, center_x + n, o);
}

/* ------------------------------------------------------------------ */
/* Scaling the 400x200 picture to the window                           */
/* ------------------------------------------------------------------ */

static XImage *frame_src;      /* copy of the 400x200 pixmap */
static XImage *frame_dst;      /* window-sized image */
static int    *x_map;          /* destination column -> source column */
static int     dst_x, dst_y, dst_w, dst_h;   /* where the picture goes in the window */

static void rebuild_scaled_image(void)
{
    int scr = DefaultScreen(display);

    if (frame_dst)
        XDestroyImage(frame_dst);      /* also frees its pixel data */
    free(x_map);
    frame_dst = NULL;
    x_map = NULL;

    /* Largest 2:1 rectangle that fits, centred */
    if (window_width * INTERNAL_HEIGHT <= window_height * INTERNAL_WIDTH) {
        dst_w = window_width;
        dst_h = window_width * INTERNAL_HEIGHT / INTERNAL_WIDTH;
    } else {
        dst_h = window_height;
        dst_w = window_height * INTERNAL_WIDTH / INTERNAL_HEIGHT;
    }
    if (dst_w < 1 || dst_h < 1)
        return;
    dst_x = (window_width  - dst_w) / 2;
    dst_y = (window_height - dst_h) / 2;

    frame_dst = XCreateImage(display, DefaultVisual(display, scr), DefaultDepth(display, scr),
                             ZPixmap, 0, NULL, window_width, window_height, 32, 0);
    if (!frame_dst)
        return;
    /* calloc: the borders stay 0, which is black on normal TrueColor displays */
    frame_dst->data = calloc((size_t)frame_dst->bytes_per_line, (size_t)window_height);
    x_map = malloc(sizeof(int) * (size_t)dst_w);
    if (!frame_dst->data || !x_map) {
        XDestroyImage(frame_dst);
        frame_dst = NULL;
        return;
    }
    for (int xx = 0; xx < dst_w; xx++)
        x_map[xx] = xx * INTERNAL_WIDTH / dst_w;
}

static void present_frame(void)
{
    if (!frame_src)
        frame_src = XGetImage(display, pixmap, 0, 0, INTERNAL_WIDTH, INTERNAL_HEIGHT,
                              AllPlanes, ZPixmap);
    else
        XGetSubImage(display, pixmap, 0, 0, INTERNAL_WIDTH, INTERNAL_HEIGHT,
                     AllPlanes, ZPixmap, frame_src, 0, 0);
    if (!frame_src || window_width < 1 || window_height < 1)
        return;

    if (!frame_dst || frame_dst->width != window_width || frame_dst->height != window_height)
        rebuild_scaled_image();
    if (!frame_dst)
        return;

    /* Nearest-neighbour scaling. Fast path for the usual 32 bits per pixel. */
    if (frame_src->bits_per_pixel == 32 && frame_dst->bits_per_pixel == 32 &&
        frame_src->byte_order == frame_dst->byte_order) {
        int prev_sy = -1;
        uint32_t *prev_row = NULL;
        for (int yy = 0; yy < dst_h; yy++) {
            int sy = yy * INTERNAL_HEIGHT / dst_h;
            uint32_t *drow = (uint32_t *)(frame_dst->data +
                             (size_t)(dst_y + yy) * frame_dst->bytes_per_line) + dst_x;
            if (sy == prev_sy) {
                memcpy(drow, prev_row, sizeof(uint32_t) * (size_t)dst_w);
            } else {
                const uint32_t *srow = (const uint32_t *)(frame_src->data +
                                       (size_t)sy * frame_src->bytes_per_line);
                for (int xx = 0; xx < dst_w; xx++)
                    drow[xx] = srow[x_map[xx]];
            }
            prev_sy = sy;
            prev_row = drow;
        }
    } else {
        for (int yy = 0; yy < dst_h; yy++) {
            int sy = yy * INTERNAL_HEIGHT / dst_h;
            for (int xx = 0; xx < dst_w; xx++)
                XPutPixel(frame_dst, dst_x + xx, dst_y + yy, XGetPixel(frame_src, x_map[xx], sy));
        }
    }

    XPutImage(display, window, gc, frame_dst, 0, 0, 0, 0, window_width, window_height);
    XFlush(display);
}

/* ------------------------------------------------------------------ */
/* Scenery                                                             */
/* ------------------------------------------------------------------ */

void draw_mountains(void)
{
    m = n = screen_height;
    f -= SPEED_SCALE;
    o = 30 + 9 * i++;
    n -= 2;
    m += 3;
    l = f / 0.7;
    draw_mode_setcolors(y * INPUT_OFFSET + SPEED_SCALE - i * k);
}

void draw_sun(void)
{
    f = l = C[u] / -2 - t;
    k += 17 * w;
    o = k++ + 20;
    n = k + k - k * k / 9;
    draw_mode_setcolors(INPUT_OFFSET * p);
}

void draw_sky(void)
{
    y = 135;
    draw_mode_setcolors(SPEED_SCALE * INPUT_OFFSET | screen_height);
}

void draw_background_timing(void)
{
    draw_mode_setcolors(0);
}

void draw_grass(void)
{
    n = 2 * screen_width;
    if (z)
        draw_mode_setcolors(r + k * INPUT_OFFSET * 8);                   /* snow */
    else
        draw_mode_setcolors(TIME_SCALE - k * SPEED_SCALE * SPEED_SCALE); /* grass */
}

void draw_side_stripes(void)   /* red/white alternating road edge */
{
    n = 9 + x / screen_height;
    if (!k + z) {
        r = screen_height * INPUT_OFFSET + 9;   /* white */
        draw_mode_setcolors(r);
    } else {
        draw_mode_setcolors(SPEED_SCALE << 17); /* red */
    }
}

void draw_road_color(void)     /* road surface and start line */
{
    n -= x / (9 * screen_height);
    if (i % p > 6) {
        h = r * (3 + z) / 5;
        draw_mode_setcolors(h);
    } else {
        draw_mode_setcolors(r);
    }
}

void draw_center_line(void)
{
    n = x / (13 * screen_width);
    draw_mode_setcolors(k ? h : r);
}

void draw_HID(void)            /* status line */
{
    char text[99];
    int high = laps > 0 ? q : 0;   /* no best time before the first full lap */

    int len = snprintf(text, sizeof(text),
        "Time: %1d'%02d\"%02d      High: %1d'%02d\"%02d      Speed: %03dmph",
        h / 100 / 60, h / 100 % 60, h % 100,
        high / 100 / 60, high / 100 % 60, high % 100,
        s);
    XDrawString(display, pixmap, gc, 30, x, text, len);
}

void draw_snow(void)
{
    x = h;   /* start value for the pseudo-random flake positions */
    f = 0;
    while (++f < 2 * z * screen_width) {
        x *= 3;
        l = x % screen_width;
        o = (x * x) % a;
        draw_mode_setcolors(-2);   /* white */
    }
}

static void draw_text(int px, int py, const char *str)
{
    XDrawString(display, pixmap, gc, px, py, str, (int)strlen(str));
}

void draw_finish_banner(void)
{
    char buffer[64];
    int line_y = 60;

    XSetForeground(display, gc, 0x000000);
    XFillRectangle(display, pixmap, gc, 0, 0, INTERNAL_WIDTH, INTERNAL_HEIGHT);
    XSetForeground(display, gc, 0xFFFFFF);

    draw_text(120, line_y, "=== RACE FINISHED ===");
    line_y += 30;

    snprintf(buffer, sizeof(buffer), "Total Time:  %d'%02d\"%02d",
             total_time / 100 / 60, total_time / 100 % 60, total_time % 100);
    draw_text(100, line_y, buffer);
    line_y += 25;

    snprintf(buffer, sizeof(buffer), "Fastest Lap: %d'%02d\"%02d",
             q / 100 / 60, q / 100 % 60, q % 100);
    draw_text(100, line_y, buffer);
    line_y += 25;

    snprintf(buffer, sizeof(buffer), "Max Speed:   %d mph", max_speed);
    draw_text(100, line_y, buffer);
    line_y += 40;

    draw_text(70, line_y, "R = new race   F = full screen   ESC = quit");
}

/* ------------------------------------------------------------------ */
/* Game control                                                        */
/* ------------------------------------------------------------------ */

void reset_game(void)
{
    laps       = 0;            /* u = 0 is the start line, so the next crossing is a full lap */
    total_time = 0;
    max_speed  = 0;
    q = TIME_SCALE;            /* "no best lap yet" */
    u = 0;
    s = 0;
    j = 0;
    t = 0;
    c = clock_ticks();         /* restart the lap clock */

    C[INPUT_OFFSET + XK_Up]    = 0;
    C[INPUT_OFFSET + XK_Left]  = 0;
    C[INPUT_OFFSET + XK_Right] = 0;

    game_state = STATE_RACING;
}

static void set_fullscreen(int on)
{
    XEvent e;
    memset(&e, 0, sizeof(e));
    e.xclient.type         = ClientMessage;
    e.xclient.window       = window;
    e.xclient.message_type = net_wm_state;
    e.xclient.format       = 32;
    e.xclient.data.l[0]    = on ? 1 : 0;     /* _NET_WM_STATE_ADD / _REMOVE */
    e.xclient.data.l[1]    = (long)net_wm_state_fullscreen;
    e.xclient.data.l[3]    = 1;              /* request from a normal application */
    XSendEvent(display, DefaultRootWindow(display), False,
               SubstructureRedirectMask | SubstructureNotifyMask, &e);
    XFlush(display);
    fullscreen = on;
}

static void handle_event(XEvent *ev)
{
    switch (ev->type) {
    case ConfigureNotify:
        window_width  = ev->xconfigure.width;
        window_height = ev->xconfigure.height;
        break;

    case ClientMessage:   /* window close button */
        if ((Atom)ev->xclient.data.l[0] == wm_delete_window)
            running = 0;
        break;

    case KeyPress:
    case KeyRelease: {
        KeySym key  = XLookupKeysym(&ev->xkey, 0);
        int pressed = (ev->type == KeyPress);
        long idx    = (long)INPUT_OFFSET + (long)key;
        int was_down = 0;

        /* Keys with large keysym values (e.g. media keys) fall outside C[] */
        if (idx >= 0 && idx < C_SIZE) {
            was_down = C[idx];
            C[idx] = pressed;
        }
        if (!pressed || was_down)      /* act once per key press, not on auto-repeat */
            break;

        if (key == XK_Escape)
            running = 0;
        else if (key == XK_f || key == XK_F11)
            set_fullscreen(!fullscreen);
        else if (key == XK_r && game_state == STATE_FINISHED)
            reset_game();
        break;
    }
    }
}

/* ------------------------------------------------------------------ */
/* One frame of racing                                                 */
/* ------------------------------------------------------------------ */

void race_frame(void)
{
    /* --- track generation (first frame only) and event polling --- */
    while (1) {
        r++;
        if (r == p) {
            /* Track finished. u is set past the finish line, so the first
             * frame "crosses" it, which starts the clock (as in the original). */
            u = h = q = TIME_SCALE;
        } else if (r < p) {
            a = (r / screen_height) % 6;                   /* start line pattern */
            r[C + p] = (l += (!a || a % 2) ? g : a - 3) / 4; /* hills */
            if (2 * r <= p) {                              /* curves, depend on track B */
                i -= ((i + 34 && r / 149 % B) - (i - 34 && r / 353 % 3));
                n += i;
                r[C] = n / 55;
            } else {
                r[C] = C[p - r];                           /* second half mirrors the first */
            }
            v = 4 - 3 * z;                                 /* steering response, less in winter */
        } else {
            if (!XPending(display))
                break;
            XNextEvent(display, &event);
            handle_event(&event);
        }
    }
    if (!running || game_state != STATE_RACING)
        return;

    /* --- steering --- */
    int steer = C[INPUT_OFFSET + XK_Right] - C[INPUT_OFFSET + XK_Left];
    if (steer != 0) {
        if (j * steer < 8)
            j += v * steer;
    } else {
        j *= 0.9;
    }

    /* --- speed and distance --- */
    int accel = C[INPUT_OFFSET + XK_Up] * 3;
    int decel = !!s;
    int new_s = accel + s - decel;
    if (new_s > a)       /* top speed: a is 163 here, left over from drawing the player's car */
        new_s = a;
    s = new_s;
    if (s > max_speed)
        max_speed = s;
    u += s / 9;

    /* --- crossing the finish line --- */
    if (u > p) {
        int now = clock_ticks();
        if (laps >= 0) {                /* laps == -1: this is the start, not a lap */
            int lap_time = now - c;
            total_time += lap_time;
            if (lap_time < q)
                q = lap_time;           /* best lap */
        }
        laps++;
        if (laps >= MAX_LAPS)
            game_state = STATE_FINISHED;
        u %= p;
        c = now;                        /* next lap starts now */
    }

    /* --- sideways position: steering and the pull in curves --- */
    t += j * (s + SPEED_SCALE) / screen_width;
    if (i != 0)
        t -= (s / i) * (C[u + 4] - C[u]);

    /* --- off the road, or hitting the opponent --- */
    if ((t * t) > p) {
        t *= 0.97;
        s *= 0.8;
    } else if (g > a && (b * b) < (screen_height * 9)) {
        s = i;                          /* collision: speed drops to i (60) */
    }

    /* --- sky, sun and mountains --- */
    i = 0; k = 0; l = 0;
    o = screen_width;
    n = screen_width;
    while (i < 5) {
        if (k / 17) {
            if (o++ == y) {
                draw_mountains();
            } else {
                n -= 2;
                m += 3;
                l = f / 0.7;
                draw_mode_setcolors(y * INPUT_OFFSET + SPEED_SCALE - i * k);
            }
        } else if (k | !o) {
            draw_sun();
        } else if (o-- < a) {
            draw_sky();
        } else {
            draw_background_timing();
        }
    }

    /* --- road, stripes and grass, from the horizon down --- */
    x = 0; g = 0; m = 0; f = 0;
    a = i = u + screen_height;
    d += 9;
    while (1) {
        if (o < f) {
            draw_grass();
            draw_side_stripes();
            draw_road_color();
            draw_center_line();
            o++;
        } else {
            f = x / screen_height + 70 + C[i + p] - C[u + p];
            o = x ? o : f;
            k = (2 * i / 41) % 2;
            l = C[--i] - C[u] - t;
            x += screen_height - (y = i - u);
            if (!((d - i) % 410)) {     /* the opponent's position on the road */
                a = o;
                b = l + B * x / screen_width;
                g = x / SPEED_SCALE;
            }
            if (i <= u)
                break;
        }
    }

    /* --- the two cars --- */
    k = i = 0; x = g; l = b;
    while (1) {
        if (++i == 41 + !f && k) {
            f = k = i = l = 0;          /* switch to the player's car */
            x = 190;
            a = 163;
        } else if (i == 42) {
            i = 9;
            k = r;
        } else if (!k || (i - 41)) {
            /* continue */
        } else {
            break;
        }
        n = (min(min(min(75, SPEED_SCALE - i) / 3, 95 - 2 * i), r - k + 2 * i) - k / r) * x / screen_height;
        if (i < 9)
            m = x * 14 / screen_height;
        else if (k)
            m = ((i - 31) / 5 * (i % 12) != 0) ? 0 : n - 1;
        else
            m = 0;
        o = a - i * x / screen_height;
        y = screen_height - o;
        h = (k * r + k * f / INPUT_OFFSET);
        draw_mode_setcolors(h);
    }

    /* --- status line and snow --- */
    time_trigger();
    B = g ? B : 1 - h % 3;   /* opponent lane offset: -1, 0 or 1 */
    i = 60;                  /* as in the original: used as divisor in the curve pull next frame */
    y = n = m;
    draw_HID();
    draw_snow();
}

void finish_frame(void)
{
    while (XPending(display)) {
        XNextEvent(display, &event);
        handle_event(&event);
    }
    if (game_state == STATE_FINISHED)
        draw_finish_banner();
}

/* ------------------------------------------------------------------ */

static void usage(const char *prog)
{
    printf("Usage: %s [options]\n"
           "  Options are letters in one word, e.g. \"ad2\":\n"
           "  a\twinter landscape (snow, slippery road)\n"
           "  d\tracing at night\n"
           "  1-3\ttrack selection (3 is the default)\n"
           "  f\tstart in full screen\n"
           "Keys: Up = accelerate, Left/Right = steer, F or F11 = full screen,\n"
           "      ESC = quit, R = new race after the finish\n",
           prog);
}

int main(int argc, char **argv)
{
    int start_fullscreen = 0;

    if (argc > 2 || (argc == 2 && strspn(argv[1], "ad123f") != strlen(argv[1]))) {
        usage(argv[0]);
        return 1;
    }
    if (argc == 2) {
        z = strchr(argv[1], 'a') != NULL;
        w = strchr(argv[1], 'd') != NULL;
        start_fullscreen = strchr(argv[1], 'f') != NULL;
        if (strchr(argv[1], '1'))
            B = 3;
        else if (strchr(argv[1], '2'))
            B = 2;
    }
    p = 10660;
    gettimeofday(&program_start, 0);

    display = XOpenDisplay(0);
    if (!display) {
        fprintf(stderr, "%s: cannot open X display\n", argv[0]);
        return 1;
    }
    int scr = DefaultScreen(display);

    window = XCreateSimpleWindow(display, RootWindow(display, scr), 0, 0,
                                 window_width, window_height, 0,
                                 BlackPixel(display, scr), BlackPixel(display, scr));
    gc = XCreateGC(display, window, 0, 0);

    XStoreName(display, window, "vik1 remake");
    XSetIconName(display, window, "vik1 remake");

    XSizeHints *hints = XAllocSizeHints();
    if (hints) {
        hints->flags      = PMinSize;
        hints->min_width  = INTERNAL_WIDTH;
        hints->min_height = INTERNAL_HEIGHT;
        XSetWMNormalHints(display, window, hints);
        XFree(hints);
    }

    wm_delete_window        = XInternAtom(display, "WM_DELETE_WINDOW", False);
    net_wm_state            = XInternAtom(display, "_NET_WM_STATE", False);
    net_wm_state_fullscreen = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);
    XSetWMProtocols(display, window, &wm_delete_window, 1);

    if (start_fullscreen) {   /* ask for full screen before the window appears */
        XChangeProperty(display, window, net_wm_state, XA_ATOM, 32, PropModeReplace,
                        (unsigned char *)&net_wm_state_fullscreen, 1);
        fullscreen = 1;
    }

    /* Held keys send only KeyPress repeats, no fake KeyRelease in between */
    XkbSetDetectableAutoRepeat(display, True, NULL);

    XSelectInput(display, window, KeyPressMask | KeyReleaseMask | StructureNotifyMask);
    XMapWindow(display, window);

    pixmap = XCreatePixmap(display, window, INTERNAL_WIDTH, INTERNAL_HEIGHT, DefaultDepth(display, scr));
    d = INTERNAL_HEIGHT;

    while (running) {
        if (game_state == STATE_RACING)
            race_frame();
        else
            finish_frame();
        if (!running)
            break;
        present_frame();
        wait_for_next_frame();
    }

    if (frame_src) XDestroyImage(frame_src);
    if (frame_dst) XDestroyImage(frame_dst);
    free(x_map);
    XFreePixmap(display, pixmap);
    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}
