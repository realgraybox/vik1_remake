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

#define MAX_LOGGED_COLORS 1024  // Adjust if there are many colors
static int logged_colors[MAX_LOGGED_COLORS];
static int logged_color_count = 0;
static bool log_colors_enabled = false; // Toggle color log on/off


// Helper function for logging unique colors
static void log_color(int color) {
    if (!log_colors_enabled) return;

    // Check if the color is already logged
    int i;
    for (i = 0; i < logged_color_count; i++) {
        if (logged_colors[i] == color) return;
    }

    // Save new color
    if (logged_color_count < MAX_LOGGED_COLORS) {
        logged_colors[logged_color_count++] = color;
    }
}

// Print all colors to the console
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
	
	color_val =  color_component(0, argc, y, v, z, w) |
			color_component(8, argc, y, v, z, w) |
			color_component(16, argc, y, v, z, w);
	
         
    XSetForeground(display, gc, color_val);
    /*        Left line             Mid              Right line
	x = ...|--------[m---n]--------| center |--------[m---n]--------|...
	y = o
	*/
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
	XCopyArea(display, pixmap, window, gc, 0, 0, 2 * screen_height, screen_height, 0, 0);
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

int reset_current_lap (int argc) {
	printf("[DEBUG reset_current_lap]\n");
	argc=0;
	u=0; 
	c=0;
	gettimeofday((struct timeval*)H, 0); // Record timestamp
	return SPEED_SCALE * *H + 1[H]/p - c;
}

void loop2(void) {
    printf("[DEBUG enters loop2]\n");

    while (1) {
        // Step 1: Count up and check if we need to reset the game's timers
        r++;
        if (r == p) {  // When r reaches TIME_LIMIT/p
            u = h = q = TIME_SCALE;  // nulstil tidsmålere
        }
        // Step 2: Pre-game setup, track generation
        else if (r < p) {
            //printf("[DEBUG loop2 r: %d]\n", r);

            // Determine the starting line graphic pattern (0..5)
            a = (r / screen_height) % 6;

            // Pre-generate track segment
            r[C + p] = (l += (!a || a % 2) ? g : a - 3) / 4;

            // Additional track/logic based on track number B
            if (2 * r <= p) {
                i -= ((i + 34 && r / 149 % B) - (i - 34 && r / 353 % 3));
                n += i;
                r[C] = n / 55;
            } else {
                r[C] = C[p - r];
            }

            // Adjust speed/visual effect based on winter/normal
            v = 4 - 3 * z;
        }
        // Step 3: Event polling
        else {
            if (XPending(display)) {
                XNextEvent(display, &event);

                int key = XLookupKeysym(&event.xkey, 0);
                if (key == XK_Escape) {
					dump_logged_colors();
                    exit(0); // can later be changed to running = 0
                } else {
                    C[INPUT_OFFSET + key] ^= (event.type == KeyPress || event.type == KeyRelease);
                }
            } else {
                // No events, break out of loop2
                break;
            }
        }
    } // end while(1)
}


void loop3(void) {
    printf("[DEBUG enters loop3]\n");

    int screen_width = screen_height * 2;

    // --- Read input ---
    int steer = C[INPUT_OFFSET + XK_Right] - C[INPUT_OFFSET + XK_Left];

    // --- Update control ---
    if (steer != 0) {
        if (j * steer < 8) j += v * steer;
    } else {
        j *= 0.9;
    }

// --- Update acceleration / distance ---
    int accel = C[INPUT_OFFSET + XK_Up] * 3;   // 3 if up arrow, otherwise 0
    int decel = !!s;                            // 1 if s > 0, otherwise
    int new_s = accel + s - decel;
    if (new_s > a) new_s = a;
    s = new_s;
    u += s / 9;

// --- Update lap and reset if we pass the finish line ---
    if (u > p) {
        if (h < q) q = h;     // Save best lap time
        u %= p;               // Wrap distance
        u = 0;                // Reset distance
        c = 0;                // Reset lap counter
        gettimeofday((struct timeval*)H, 0);  
        c = SPEED_SCALE * *H + 1[H] / p - c;
    }

    // --- Update position / offset ---
    t += j * (s + SPEED_SCALE) / screen_width;

    if (i != 0) {  // Protect against division by 0
        t -= (s / i) * (C[u + 4] - C[u]);
    }

    // --- Adjust speed depending on position ---
    if ((t * t) > p) {
        t *= 0.97;  // Reduce steering/position for stability
        s *= 0.8;   // Slows down
    } else if (g > a && (b * b) < (screen_height * 9)) {
        s = i;      // Set speed (typically 0 at the start)
    }

    // --- Reset steering and road parameters ---
    i = 0;
    k = 0;
    l = 0;

    // --- Screen coordinates ---
    o = screen_width;
    n = screen_width;

    // --- Drawing / color loop ---
    while (i < 5) {
        int color_value;

        if (k / 17) {
            // Highway case
            if (o++ == y) {
                m = n = screen_height;
                f -= SPEED_SCALE;
                o = 30 + 9 * i++;
            }
            n -= 2;
            m += 3;
            l = f / 0.7;
            color_value = y * INPUT_OFFSET + SPEED_SCALE - i * k;
        } else {
            if (k | !o) {
                // Alternative path case
                f = l = C[u] / -2 - t;
                k += 17 * w;
                o = k++ + 20;
                n = k + k - k * k / 9;
                color_value = INPUT_OFFSET * p;
            } else {
                if (o-- < a) {
                    y = 135;
                    color_value = SPEED_SCALE * INPUT_OFFSET | screen_height;
                } else {
                    color_value = 0;
                }
            }
        }

        // Draw with calculated color
        draw_mode_setcolors(color_value);

        // Debug print (can be turned on/off)
        // printf("[DEBUG] i=%d, o=%d, n=%d, m=%d, l=%d, color_value=%d\n",
        //        i, o, n, m, l, color_value);
    }
}


void loop4(void) {
    printf("[DEBUG enters loop4]\n");
    int screen_width = screen_height * 2;

    for (
        x = g = m = f = 0,
        a = i = u + screen_height,
        d += 9;
        o < f 
            ? (
				printf("[DEBUG loop4 o:] %d\n",o),
                n = 2 * screen_width,
                draw_mode_setcolors(z ? r + k * INPUT_OFFSET * 8 : TIME_SCALE - k * SPEED_SCALE * SPEED_SCALE),

                n = 9 + x / screen_height,
                draw_mode_setcolors(!k + z ? r = screen_height * INPUT_OFFSET + 9 : SPEED_SCALE << 17),

                n -= x / 9 / screen_height,
                draw_mode_setcolors(i % p > 6 ? h = r * (3 + z) / 5 : r),

                n = x / 13 / screen_width,
                draw_mode_setcolors(k ? h : r),

                o++
            )
            : (
                f = x / screen_height + 70 + C[i + p] - C[u + p],
                o = x ? o : f,
                k = 2 * i / 41 % 2,
                l = C[--i] - C[u] - t,
                x += screen_height - (y = i - u),
                g = (d - i) % 410 ? g : (a = o, b = l + B * x / screen_width, x) / SPEED_SCALE,
                i > u
            );
    );
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
		draw_mode_setcolors(h); // change (h - 1) to (h): no visible difference

        h = (k * r + k * f / INPUT_OFFSET);
    }
}


void loop6(void) {
    printf("[DEBUG enters loop6]\n");
    int screen_width = screen_height * 2;

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
	
window = XCreateSimpleWindow (display, RootWindow(display, 0), 0, 0,  screen_height * 2, screen_height, 0, 0, 0);
gc = XCreateGC (display, window, 0, 0);
	
XStoreName(display, window, argv[0]);
XSetIconName(display, window, argv[0]);
// Input events listen for key press/release  
XSelectInput (display, window, KeyPressMask | KeyReleaseMask);
// Make the window visible
XMapWindow (display, window);
// Create a pixmap (offscreen drawing area)
pixmap = XCreatePixmap (display, window, screen_height * 2, d = screen_height, DefaultDepth(display, 0));

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
