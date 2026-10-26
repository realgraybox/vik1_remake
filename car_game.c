#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>  // for usleep
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include <signal.h>

const char *last_func = "startup"; // global variable

void segfault_handler(int sig) {
    fprintf(stderr, "\n=== SEGFAULT DETECTED ===\n");
    fprintf(stderr, "Signal: %d (Segmentation Fault)\n", sig);
    fprintf(stderr, "Tip: Run under gdb for backtrace: gdb ./car_game core\n");
    fflush(stderr); 
    exit(1);
}

void install_segfault_handler(void) {
    struct sigaction sa;
    sa.sa_handler = segfault_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGSEGV, &sa, NULL);
}

/* --- tracemacro --- */
#define TRACE_FUNC() (last_func = __func__)

#define TRACE_POINT(name) do { \
    last_func = name; \
    fprintf(stderr, "[TRACE] Reached %s\n", last_func); \
    fflush(stderr); \
} while(0)

#define ARG_DRAW_MODE  (100 << 17)

#define min(a,b) ((a) < (b) ? (a) : (b))
#define FRAME_DELAY_US 65800  // adjust here!

struct timeval tv;

int g, h, i, j, k, l, m, n, p, q, r, v, x, y;
int o, b, f, u, s, c, a, t, d, C[1<<18], B=4,  H[4];
int len;
int screen_height = 200;
int screen_width = 400;

const int TIME_SCALE = 53300;
const int INPUT_OFFSET = 65793;
const int SPEED_SCALE = 100;
int z=0;		//is_winter
int w=0;		//is_night
char P[99];
unsigned int color_val;
//test endgame condition
int laps = -1;
int max_laps = 2;

int max_speed = 0;
int race_start_time = 0;
int lap_start_time = 0;
int total_time = 0;
int fastest_lap = 999999;
int last_lap_time   = 0;

enum GameState {
    STATE_RACING,
    STATE_FINISHED
};
int game_state = STATE_RACING;
//
//scaletest
#define SCALE 2

#define INTERNAL_WIDTH  400
#define INTERNAL_HEIGHT 200

int window_width  = INTERNAL_WIDTH  * SCALE;
int window_height = INTERNAL_HEIGHT * SCALE;

Display* display;
Window window;
Pixmap pixmap;
GC gc;
XEvent event;

unsigned int color_component(int a, int argc, int y, int v, int z, int w) {
    int temp = (((y / v) * !w) +
               ((((argc + 1) >> a) & 255) * (screen_height + y / (z | (w - 2))) / screen_height));
    return temp << a;
}

int draw_mode_setcolors(int argc) {
	
	color_val =  color_component(0, argc, y, v, z, w) |
			color_component(8, argc, y, v, z, w) |
			color_component(16, argc, y, v, z, w);
  
    XSetForeground(display, gc, color_val);
    
    int center_x = screen_height + l;
	XDrawLine(display, pixmap, gc,
		center_x - m, o,
        center_x - n, o);

	XDrawLine(display, pixmap, gc,
        center_x + m, o,
        center_x + n, o);
    
	//gettimeofday((struct timeval*)H, 0); // Record timestamp
    return 0;
}

//banner test
void draw_finish_banner() {

    char buffer[256];

    // Dark overlay effect (simple full-screen rectangle)
    XSetForeground(display, gc, 0x000000);
    XFillRectangle(display, pixmap, gc,
                   0, 0,
                   INTERNAL_WIDTH,
                   INTERNAL_HEIGHT);

    XSetForeground(display, gc, 0xFFFFFF);

    int y = 60;

    XDrawString(display, pixmap, gc,
                120, y,
                "=== RACE FINISHED ===",
                strlen("=== RACE FINISHED ==="));

    y += 30;
	
    snprintf(buffer, sizeof(buffer),
             "Total Time: %d'%02d\"%02d",
             total_time / 100 / 60,
             total_time / 100 % 60,
             total_time % 100);

    XDrawString(display, pixmap, gc, 100, y, buffer, strlen(buffer));

    y += 25;

    snprintf(buffer, sizeof(buffer),
             "Fastest Lap: %d'%02d\"%02d",
             fastest_lap / 100 / 60,
             fastest_lap / 100 % 60,
             fastest_lap % 100);

    XDrawString(display, pixmap, gc, 100, y, buffer, strlen(buffer));

    y += 25;

    snprintf(buffer, sizeof(buffer),
             "Max Speed: %d mph",
             max_speed);

    XDrawString(display, pixmap, gc, 100, y, buffer, strlen(buffer));

    y += 40;

    XDrawString(display, pixmap, gc,
                80, y,
                "Press R to Restart or ESC to Exit",
                strlen("Press R to Restart or ESC to Exit"));
}

int time_trigger () {
	gettimeofday(&tv, 0);
	H[0] = tv.tv_sec;  // seconds
	H[1] = tv.tv_usec;  // microseconds
	h = SPEED_SCALE * H[0] + H[1] / p - c;
	return h;
}

void reset_game() {

    laps = 0;
    max_speed = 0;
    fastest_lap = 999999;
    total_time = 0;
    lap_start_time = 0;
	u = 0;
	s = 0;
    
    C[INPUT_OFFSET + XK_Up] = 0;
	C[INPUT_OFFSET + XK_Left] = 0;
	C[INPUT_OFFSET + XK_Right] = 0;
	C[INPUT_OFFSET + XK_space] = 0;

    game_state = STATE_RACING;
}
//scale test
void draw_mode_copy_to_screen() {

    // Get image from internal pixmap (400x200)
    XImage *src = XGetImage(display,
                            pixmap,
                            0, 0,
                            INTERNAL_WIDTH,
                            INTERNAL_HEIGHT,
                            AllPlanes,
                            ZPixmap);

    if (!src) return;

    // Create scaled image
    XImage *scaled = XCreateImage(display,
                                  DefaultVisual(display, 0),
                                  DefaultDepth(display, 0),
                                  ZPixmap,
                                  0,
                                  malloc(window_width * window_height * 4),
                                  window_width,
                                  window_height,
                                  32,
                                  0);

    if (!scaled) {
        XDestroyImage(src);
        return;
    }

    // Nearest neighbor scaling
    int y;
    for (y = 0; y < window_height; y++) {
		int x;
        for (x = 0; x < window_width; x++) {

            int src_x = x / SCALE;
            int src_y = y / SCALE;

            unsigned long pixel =
                XGetPixel(src, src_x, src_y);

            XPutPixel(scaled, x, y, pixel);
        }
    }

    // Blit scaled image to window
    XPutImage(display,
              window,
              gc,
              scaled,
              0, 0,
              0, 0,
              window_width,
              window_height);

    XFlush(display);

    // Cleanup
    XDestroyImage(src);
    XDestroyImage(scaled);

    usleep(FRAME_DELAY_US);
}

void draw_mountains() {
    m = n = screen_height;
    f -= SPEED_SCALE;
    o = 30 + 9 * i++;
    n -= 2;
    m += 3;
    l = f / 0.7;
    int color_value = y * INPUT_OFFSET + SPEED_SCALE - i * k;
    draw_mode_setcolors(color_value);
}

void draw_sun() {
    f = l = C[u] / -2 - t;
    k += 17 * w;
    o = k++ + 20;
    n = k + k - k * k / 9;
    int color_value = INPUT_OFFSET * p;
    draw_mode_setcolors(color_value);
}

void draw_sky() {
    y = 135;
    int color_value = SPEED_SCALE * INPUT_OFFSET | screen_height;
    draw_mode_setcolors(color_value);
}

void draw_background_timing() {
    int color_value = 0;
    draw_mode_setcolors(color_value);
}

void draw_grass() {	//draw grass
	 n = 2 * screen_width;
     if (z) {
		// If winter mode/night mode (or what 'z' represents)
		draw_mode_setcolors(r + k * INPUT_OFFSET * 8);
	} else {
		// Normal condition
		draw_mode_setcolors(TIME_SCALE - k * SPEED_SCALE * SPEED_SCALE);
	}    
}
	
void draw_side_stripes() {	//draw road side stribes red/white alternating
	n = 9 + x / screen_height;
    if ((!k + z)) {	//the white stripe on road side
		r = screen_height * INPUT_OFFSET + 9;
		draw_mode_setcolors(r);
	} else {	//the red strips on road side
		draw_mode_setcolors(13107200); // original: "SPEED_SCALE << 17"
	}
}

void draw_road_color() {	// draw road color and startline
	n -= x / (9 * screen_height);
    if (i % p > 6) {
		h = r * (3 + z) / 5;
		draw_mode_setcolors(h);
		//draw_mode_setcolors(3070); // deepblue
	} else {
		draw_mode_setcolors(r);
		//draw_mode_setcolors(13107200); // red
	}
  
}

void draw_center_line() {	// draw midt strips on road
	n = x / (13 * screen_width);
	if (k) {
		draw_mode_setcolors(h);
		//draw_mode_setcolors(53300); // Green
	} else {
		draw_mode_setcolors(r);
		//draw_mode_setcolors(6579436); // Violet
	}
}

void draw_HID() {	// draw timer
    // draw status text
    int high = 0;
    if (laps > 0)	//fix initial wrong high time
		high = q;
		
   // printf("[DBG] high=%d, labs=%d\n",high, laps);
    
    len = snprintf(P, sizeof(P),
        "Time: %1d'%02d\"%02d      High: %1d'%02d\"%02d      Speed: %03dmph",
        h / 100 / 60, h / 100 % 60, h % 100,
        high / 100 / 60, high / 100 % 60, high % 100,
        s
    );
    
    XDrawString(display, pixmap, gc, 30, x, P, len);
    fastest_lap = high;
}

void draw_snow() {	// draw snow
	x = h;  // startvalue for calculation
    f = 0;
    while (++f < 2 * z * screen_width) {
        x *= 3;
        l = x % screen_width;
        o = (x * x) % a;
        draw_mode_setcolors(-2); // Trigger snow in winter mode
    }	
}

int main(int argc, char** argv) {

install_segfault_handler();

if ( argc > 2) {
	printf("Usage: %s ['a']['d'][#]\n"
           "  a\tAdvanced racing in a winter landscape.\n"
           "  d\tRacing in at night in the dark.\n"
           "  #\tRace track selection 1-3 are valid numbers\n",
           argv[0]);
    return 0;
} else if ( argc == 2 ) {
	z = (strchr(argv[1], 'a') != NULL);	//is_winter
	w = (strchr(argv[1], 'd') != NULL);	//is_night
	if (strchr(argv[1], '1')) {	//track_number B=4 is default			
		B = 3;
	} else if (strchr(argv[1], '2')) {
		B = 2;
	}
}
p = 10660;

display = XOpenDisplay (0);

window = XCreateSimpleWindow (display, RootWindow(display, 0), 0, 0,  window_width, window_height, 0, 0, 0);
gc = XCreateGC (display, window, 0, 0);
	
XStoreName(display, window, argv[0]);
XSetIconName(display, window, argv[0]);
// Input events listen for key press/release  
XSelectInput (display, window, KeyPressMask | KeyReleaseMask);
// Make the window visible
XMapWindow (display, window);
// Create a pixmap (offscreen drawing area)
pixmap = XCreatePixmap (display, window, INTERNAL_WIDTH, d = INTERNAL_HEIGHT, DefaultDepth(display, 0));

int running = 1;

while (running) {

race_start_time = h;
lap_start_time = h;

if (game_state == STATE_RACING) {
    while (1) {
        // Step 1: count and check if zero game timers
        r++;
        if (r == p) {  // if r reach TIME_LIMIT/p
            u = h = q = TIME_SCALE;  // zero timers
        }
        // Step 2: Pre-game setup, track generation
        else if (r < p) {
            // determine startline graphic pattern (0..5)
            a = (r / screen_height) % 6;
            // Pre-generate track segment
            r[C + p] = (l += (!a || a % 2) ? g : a - 3) / 4;
            // more track/logic based on track number B
            if (2 * r <= p) {
                i -= ((i + 34 && r / 149 % B) - (i - 34 && r / 353 % 3));
                n += i;
                r[C] = n / 55;
            } else {
                r[C] = C[p - r];
            }
            // adjust speed/visual effect based on winter/normal mode
            v = 4 - 3 * z;
        }
        // Step 3: Event polling
        else {
            if (XPending(display)) {
                XNextEvent(display, &event);

                int key = XLookupKeysym(&event.xkey, 0);
                if (key == XK_Escape) {
					//dump_logged_colors();
                    exit(0); // could later be changed to running = 0
                } else {	//below prevents rare segfaults 
						int idx = INPUT_OFFSET + key;
						if (idx >= 0 && idx < (1<<18)) {
							C[idx] = (event.type == KeyPress);
						} else {
							printf("[WARN] Invalid key=%d idx=%d (ignored)\n", key, idx);
						}
                }
            } else {
                // no events, break loop2
                break;
            }
        }
    } // end while(1)

    // --- read input ---
    int steer = C[INPUT_OFFSET + XK_Right] - C[INPUT_OFFSET + XK_Left];
    // --- update stearing ---
    if (steer != 0) {
        if (j * steer < 8) j += v * steer;
    } else {
        j *= 0.9;
    }
    // --- update acceleration / distance ---
    int accel = C[INPUT_OFFSET + XK_Up] * 3;   // 3 if arrow_up, otherwise 0
    int decel = !!s;                            // 1 if s > 0, otherwise 0
    int new_s = accel + s - decel;
    if (new_s > 300) new_s = 300;	//set max speed by changing a to number
    s = new_s;
    //winner banner test
    if (s > max_speed)
    max_speed = s;
    //
    u += s / 9;
    // --- update rounds and reset if passing goalline...
    if (u > p) {	//u = distance along track, p = track length, h = current time, q = best time
		//endgame test
		laps++;
		printf("[endgame test] laps=%d\n", laps);
		total_time = total_time + h - 53300;		
		printf("[endgame test] total_time=%d\n", total_time);
		if (laps >= max_laps) {
			game_state = STATE_FINISHED;
		}
		
        if (h < q) q = h;     // save best time
        
        u %= p;               // Wrap distance
        u = 0;                // zero distance
        c = 0;                // zero rounds
        gettimeofday((struct timeval*)H, 0);  
        c = SPEED_SCALE * *H + 1[H] / p - c;
    }
    // --- update position / offset ---
    t += j * (s + SPEED_SCALE) / screen_width;
    if (i != 0) {  // protect against division with 0
        t -= (s / i) * (C[u + 4] - C[u]);
    }
    // --- adjust speed depending of position ---
    if ((t * t) > p) {
        t *= 0.97;  // Reduce stearing/position for stability
        s *= 0.8;   // reduce speed 
    } else if (g > a && (b * b) < (screen_height * 9)) {
        s = i;      // set speed (often 0 at start)
    }
    // --- zero stearing and road parameters
    i = 0; k = 0; l = 0;
    // --- screen coordinates ---
    o = screen_width;
    n = screen_width;
    // --- drawing / color-loop ---
    while (i < 5) {
        if (k / 17) {                 // draw mountains
            if (o++ == y) {
                draw_mountains();
            } else {
                n -= 2;
                m += 3;
                l = f / 0.7;
                int color_value = y * INPUT_OFFSET + SPEED_SCALE - i * k;
                draw_mode_setcolors(color_value);
            }
        } else {
            if (k | !o) {             // draw sun
                draw_sun();
            } else {	
                if (o-- < a) {        // draw sky
                    draw_sky();
                } else {              // background color for timer
                    draw_background_timing();
                }
            }
        }
    }	

    // initialising (earlier for-loops first section)
    x = 0; g = 0; m = 0; f = 0;
    a = i = u + screen_height;
    d += 9;
    // condition is that we stop when <= u
    while (1) {
        if (o < f) {
            draw_grass(); // draw grass
            draw_side_stripes(); //draw road side stribes red/white alternating
            draw_road_color(); // draw road color and startline
            draw_center_line(); // draw midt strips on road
            o++;
        } else {
            // opdating frequense when o >= f
            f = x / screen_height + 70 + C[i + p] - C[u + p];
            o = x ? o : f;
            k = (2 * i / 41) % 2;
            l = C[--i] - C[u] - t;
            x += screen_height - (y = i - u);
            // calculate g (maybe update g/b/a)
            if (!((d - i) % 410)) {
                a = o;
                b = l + B * x / screen_width;
                g = x / SPEED_SCALE;
            }
            if (i <= u) {
                break; // Stop loop
            }
        }
    }

    k = i = 0; x = g; l = b;
    while (1) {
        // Condition-logic
        if (++i == 41 + !f && k) {
            f = k = i = l = 0;
            x = 190;
            a = 163;
        } else if (i == 42) {
            i = 9;
            k = r;
        } else if (!k || (i - 41)) {
            // continue
        } else {
            break; // exit loop
        }
        n = (min(min(min(75, SPEED_SCALE - i) / 3, 95 - 2 * i), r - k + 2 * i) - k / r) * x / screen_height;
        if (i < 9) {
            m = x * 14 / screen_height;
        } else if (k) {
            m = ((i - 31) / 5 * (i % 12)) ? 0 : n - 1;
        } else {
            m = 0;
        }
        o = a - i * x / screen_height;
        y = screen_height - o;
        h = (k * r + k * f / INPUT_OFFSET);
		draw_mode_setcolors(h); // colors of cars
        h = (k * r + k * f / INPUT_OFFSET);
    }	
   
    time_trigger();

    // B depends on g and can only be -1, 0 or 1
    B = g ? B : 1 - h % 3;

    y = n = m;         // initialize y, n, m to same value
   
	draw_HID();
	
	draw_snow();

    //end previos loop6 code
	draw_mode_copy_to_screen();
	}
	else if (game_state == STATE_FINISHED) {
        draw_finish_banner();
        draw_mode_copy_to_screen();

        while (XPending(display)) {
            XNextEvent(display, &event);
            int key = XLookupKeysym(&event.xkey, 0);

            if (key == XK_Escape) {
                running = 0;
            }
            else if (key == XK_r || key == XK_R) {
                reset_game();
            }
        }

        usleep(FRAME_DELAY_US);
        continue;
    }
}
	return 0;
}
