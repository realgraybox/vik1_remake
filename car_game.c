#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>  // for usleep
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#define ARG_DRAW_MODE  (100 << 17)
#define ARG_WINTER_MODE (-1)

#define min(a,b) ((a) < (b) ? (a) : (b))
#define FRAME_DELAY_US 65800

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

Display* display;
Window window;
Pixmap pixmap;
GC gc;
XEvent event;

#include <stdbool.h>

#define MAX_LOGGED_COLORS 1024
static int logged_colors[MAX_LOGGED_COLORS];
static int logged_color_count = 0;
static bool log_colors_enabled = true;

static void log_color(int color) {
    if (!log_colors_enabled) return;

    int i;
    for (i = 0; i < logged_color_count; i++) {
        if (logged_colors[i] == color) return;
    }

    if (logged_color_count < MAX_LOGGED_COLORS) {
        logged_colors[logged_color_count++] = color;
    }
}

static void dump_logged_colors(void) {
    printf("\n[DEBUG] Colors used in this session:\n");
    int i;
    for (i = 0; i < logged_color_count; i++) {
        printf("  %d\n", logged_colors[i]);
    }
    printf("[DEBUG] Total %d unique colors\n", logged_color_count);
}

unsigned int color_component(int a, int argc, int y, int v, int z, int w) {
    int temp = (((y / v) * !w) +
               ((((argc + 1) >> a) & 255) * (screen_height + y / (z | (w - 2))) / screen_height));
    return temp << a;
}

int draw_mode_setcolors(int argc) {
	log_color(argc);
	
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
    
    gettimeofday((struct timeval*)H, 0); // Record timestamp
    return 0;
}

void draw_mode_copy_to_screen() {
	printf("[DEBUG draw_mode_copy_to_screen]\n");	
	// Copy drawn area to screen
	XCopyArea(display, pixmap, window, gc, 0, 0, screen_width, screen_height, 0, 0);
	XSync(display, 0);
	XFlush(display);
	usleep(FRAME_DELAY_US);  // sleep time (FPS) in milliseconds
    return;
}

int time_trigger () {
	gettimeofday(&tv, 0);
	H[0] = tv.tv_sec;
	H[1] = tv.tv_usec;
	h = SPEED_SCALE * H[0] + H[1] / p - c;
	return h;
}


void loop2(void) {
    printf("[DEBUG enters loop2]\n");

    while (1) {
        r++;
        if (r == p) {
            u = h = q = TIME_SCALE;
        }
        else if (r < p) {
            a = (r / screen_height) % 6;
            r[C + p] = (l += (!a || a % 2) ? g : a - 3) / 4;
            if (2 * r <= p) {
                i -= ((i + 34 && r / 149 % B) - (i - 34 && r / 353 % 3));
                n += i;
                r[C] = n / 55;
            } else {
                r[C] = C[p - r];
            }
            v = 4 - 3 * z;
        }
        else {
            if (XPending(display)) {
                XNextEvent(display, &event);

                int key = XLookupKeysym(&event.xkey, 0);
                if (key == XK_Escape) {
					dump_logged_colors();
                    exit(0); 
                } else {
                    C[INPUT_OFFSET + key] ^= (event.type == KeyPress || event.type == KeyRelease);
                }
            } else {

                break;
            }
        }
    } // end while(1)
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


void loop3(void) {	// Drawing of mountains, sky, sun - the entire background
    printf("[DEBUG enters loop3]\n");

	// --- Read input ---
    int steer = C[INPUT_OFFSET + XK_Right] - C[INPUT_OFFSET + XK_Left];

    // --- Update control ---
    if (steer != 0) {
        if (j * steer < 8) j += v * steer;
    } else {
        j *= 0.9;
    }

    int accel = C[INPUT_OFFSET + XK_Up] * 3; 
    int decel = !!s; 
    int new_s = accel + s - decel;
    if (new_s > a) new_s = a;
    s = new_s;
    u += s / 9;

    if (u > p) {
        if (h < q) q = h; 
        u %= p;
        u = 0; 
        c = 0; 
        gettimeofday((struct timeval*)H, 0);  
        c = SPEED_SCALE * *H + 1[H] / p - c;
    }

    t += j * (s + SPEED_SCALE) / screen_width;

    if (i != 0) {  
        t -= (s / i) * (C[u + 4] - C[u]);
    }

    if ((t * t) > p) {
        t *= 0.97;  
        s *= 0.8;   
    } else if (g > a && (b * b) < (screen_height * 9)) {
        s = i;      
    }

    i = 0;
    k = 0;
    l = 0;

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
                int color_value = y * INPUT_OFFSET + SPEED_SCALE - i * k;
                draw_mode_setcolors(color_value);
            }
        } else {
            if (k | !o) { 
                draw_sun();
            } else {	
                if (o-- < a) {  
                    draw_sky();
                } else {   
                    draw_background_timing();
                }
            }
        }
    }
}

void draw_grass() {	
	 n = 2 * screen_width;
     if (z) {
		
		draw_mode_setcolors(r + k * INPUT_OFFSET * 8);
	} else {
		
		draw_mode_setcolors(TIME_SCALE - k * SPEED_SCALE * SPEED_SCALE);

	}    
}
	
void draw_side_stripes() {	
	n = 9 + x / screen_height;
    if ((!k + z)) {	
		r = screen_height * INPUT_OFFSET + 9;
		draw_mode_setcolors(r);
	
	} else {	
		draw_mode_setcolors(13107200); 
		
	}
}

void draw_road_color() {	
	n -= x / (9 * screen_height);
    if (i % p > 6) {
		h = r * (3 + z) / 5;
		draw_mode_setcolors(h);
		
	} else {
		draw_mode_setcolors(r);
		
	}
  
}

void draw_center_line() {	
	n = x / (13 * screen_width);
	if (k) {
		draw_mode_setcolors(h);
	
	} else {
		draw_mode_setcolors(r);
		
	}
}

void loop4(void) {	
    printf("[DEBUG enters loop4]\n");
  
    x = 0;
    g = 0;
    m = 0;
    f = 0;
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
}



void loop5(void) {	
    printf("[DEBUG enters loop5]\n");
    k = i = 0;
    x = g;
    l = b;

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
		
        draw_mode_setcolors(h); 

        h = (k * r + k * f / INPUT_OFFSET);
    }
}


void loop6(void) {
    printf("[DEBUG enters loop6]\n");
    
    time_trigger();


    B = g ? B : 1 - h % 3;

    i = 60;           
    y = n = m;        


    len = snprintf(P, sizeof(P),
        "Time: %1d'%02d\"%02d      High: %1d'%02d\"%02d      Speed: %03dmph",
        h / 100 / i, h / 100 % i, h % 100,
        q / 100 / i, q / 100 % i, q % 100,
        s
    );
    XDrawString(display, pixmap, gc, 30, x, P, len);

    x = h;  
    f = 0;

    while (++f < 2 * z * screen_width) {
        x *= 3;
        l = x % screen_width;
        o = (x * x) % a;

        draw_mode_setcolors(ARG_WINTER_MODE - 1); // Trigger snow if winter mode
    }
}


int main(int argc, char** argv) {

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
	
window = XCreateSimpleWindow (display, RootWindow(display, 0), 0, 0,  screen_width, screen_height, 0, 0, 0);
gc = XCreateGC (display, window, 0, 0);
	
XStoreName(display, window, argv[0]);
XSetIconName(display, window, argv[0]);
// Input events listen for key press/release  
XSelectInput (display, window, KeyPressMask | KeyReleaseMask);
// Make the window visible
XMapWindow (display, window);
// Create a pixmap (offscreen drawing area)
pixmap = XCreatePixmap (display, window, screen_width, d = screen_height, DefaultDepth(display, 0));

int running = 1;

while (running) {
	loop2();
	loop3();
	loop4();
	loop5();
	loop6();
	draw_mode_copy_to_screen();
	}
	return 0;
}
