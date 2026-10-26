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

int main(int, char **);
char **global_argv = NULL;

Display* display;	
Window window;
Pixmap pixmap;
GC gc;		
XEvent event;	

int draw_mode_loop3_caseA(int value) {
    printf("draw_mode_loop3_caseA(%d)\n", value);
    return main(value, global_argv);
} 

int draw_mode_loop3_caseB(int value) {
    return main(value, global_argv); 
}

void reset_race_state(void) {
    // preserve some globals if needed (fx B, screen_height)
    // Reset lap / timing state
    c = 0;
    q = min(h, q);     // best lap: behaviour kept from original
    u = u % p;         // keep remainder after passing finish (same as before)
    
    // Reset kinematics & offsets
    t = 0;
    j = 0;
    s = 0;
    
    // Reset positional / drawing indexes
   // i = 60;
   // k = 0;
    l = 0;
    m = 0;
    // set n/o equal to screen width as used elsewhere
    n = screen_height * 2;   // your screen_width
    o = n;
    
    // Animation / frame variables
    f = 0;
    // optionally reset g, b if they are used as colors/flags (be cautious)
    // g = 0; b = 0;

    // Clear relevant input state bits so player/opponent keys aren't "stuck"
    C[INPUT_OFFSET + XK_Right] = 0;
    C[INPUT_OFFSET + XK_Left]  = 0;
    C[INPUT_OFFSET + XK_Up]    = 0;
    C[INPUT_OFFSET + XK_space] = 0;

    // Reset time base so loop6 will produce a sensible h sequence
    struct timeval tv;
    gettimeofday(&tv, NULL);
    H[0] = tv.tv_sec;
    H[1] = tv.tv_usec;
    H[2] = 0;
    H[3] = INPUT_OFFSET;  // keep this if other code expects 3[H] = INPUT_OFFSET
    // recompute h similar to prior behavior if needed:
    h = 0; // set to 0 so next iterations increment as before
    fprintf(stderr, "[RESET] after reset: u=%d h=%d q=%d k=%d n=%d o=%d\n", u, h, q, k, n, o);

}

void parse_arguments(char *arg) {
    z = (strchr(arg, 'a') != NULL);	//is_winter
    w = (strchr(arg, 'd') != NULL);	//is_night
    if (strchr(arg, '1')) {	//track_number B=4 is default			
        B = 3;
    } else if (strchr(arg, '2')) {
        B = 2;
    }
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
    XDrawLine(display, pixmap, gc, screen_height + l - m, o, screen_height + l - n, o);
    XDrawLine(display, pixmap, gc, screen_height + l + m, o, screen_height + l + n, o);
    gettimeofday((struct timeval*)H, 0); // Record timestamp
    return 0;
}

int draw_mode_copy_to_screen(int argc) {	
	// Copy drawn area to screen
	XCopyArea(display, pixmap, window, gc, 0, 0, 2 * screen_height, screen_height, 0, 0);
	XSync(display, 0);
	XFlush(display);
	usleep(FRAME_DELAY_US);  // sleep time (FPS) in milliseconds
    return 0;
}

int draw_mode_graphics_setup (int argc) {	
		display = XOpenDisplay (0);
	gc = XCreateGC (display,
		window = XCreateSimpleWindow (display, RootWindow(display, 0), 0, 0,  screen_height * 2, screen_height, 0, 0, 0), 0, 0);
	
	XSelectInput (display, window, KeyPressMask | KeyReleaseMask); // Input events listen for key press/release
	XMapWindow (display, window);	// Make the window visible
	// Create a pixmap (offscreen drawing area)
	pixmap = XCreatePixmap (display, window, screen_height * 2, d = screen_height, DefaultDepth(display, 0));
    return 0;
}

int main(int argc, char** argv) {

if (!global_argv) {
    global_argv = argv;
}

int screen_width = screen_height * 2;

// 1. for-loop for(initialization; condition; increment/decrement)
for (
    p = 10660;
     --argc & ~1 ? (	// Behavior depending on argc
			draw_mode_setcolors(argc) 	//Set foreground color & draw lines
    ) : argc ? (
			parse_arguments(argv[1]), //z, w, og B er winter, night og track_number
			argc	// Keep argc unchanged
    ) : ++argc;
			draw_mode_copy_to_screen(argc)
)
	{	
	// 2. for-loop
	for ( 
		3[H] = INPUT_OFFSET;	// Sets a timestamp or animation counter

		p==++r ? (
		// Initial setup when r reaches p
		draw_mode_graphics_setup (argc),
		// Reset various control variables
		u = h = q = TIME_SCALE
	) : r < p ? (
		// Ongoing pre-game setup (random track generation?)
		a = r / screen_height % 6,
		
		r[C + p] = (l += (!a | a % 2) ? g : a - 3) / 4,
		
		r[C] = 2 * r <= p
			? (n += (i -= ((i + 34 && r	/ 149 % B) - (i - 34 && r / 353 % 3))) ) / 55
			: C[p - r],
		v = 4 - 3 * z
	):
		// Event handling: poll key state changes
		XPending(display) ? (
			XNextEvent(display, &event),
			XLookupKeysym(&event.xkey, 0) == XK_Escape ? exit(0) :
			(C[INPUT_OFFSET + XLookupKeysym(&event.xkey, 0)] ^=
            (event.type == KeyPress || event.type == KeyRelease)),
			p
			):0;
	);

	// 3. for-loop
	for (
		printf("[LOOP3 START] argc=%d c=%d u=%d p=%d h=%d i=%d q=%d s=%d t=%d j=%d k=%d\n",
       argc, c, u, p, h, i, q, s, t, j, k),

		// 1. Userinput: right and left arrows
		k = C[INPUT_OFFSET + XK_Right] - C[INPUT_OFFSET + XK_Left],
    
		// 2. Jerk acceleration if arrow key is pressed, else decay
		j = k ? j + v * k * (j * k < 8) : j * 0.9, 
    
		// 3. Speed update if space is held (s = skid or boost?)
		u += (s = min(C[INPUT_OFFSET + XK_Up] * 3 + s - !!s, a)) / 9, 
    
		// 4. Lap handling: if passed finish line
		printf("[LOOP3 CASEA CALL] argc=%d c=%d u=%d p=%d h=%d i=%d q=%d s=%d t=%d j=%d k=%d\n",
       argc, c, u, p, h, i, q, s, t, j, k),
		c = u > p
			? (q = min(h, q), u %= p, 
			draw_mode_loop3_caseA(c = 0))	// reset current lap
			//reset_race_state(), draw_mode_setcolors(0), 0)
			//draw_mode_setcolors(c = 0))
			: c, 
		
		// 5. Horizontal movement (track offset)
		t += j * (s + SPEED_SCALE) / screen_width - s / i * (C[u + 4] - C[u]), 
    
		// 6. Adjust speed if too fast, or reset if crash
		s = t * t > p 
			? (t *= 0.97, s * 0.8)					// speed decay
			: (g > a && b * b < screen_height * 9 ? i : s), 	// crash condition?
		
		// 7. Reset several control variables
		i = k = l = 0, 
		o = n = screen_width; 
		
		i - 5;	// Loop runs as long as i < 5

		// Drawing logic, depends on state of 'k' (movement/input)
		draw_mode_loop3_caseB(
			(k / 17) ? (o++ == y ? (m = n = screen_height, f -= SPEED_SCALE, o = 30 + 9 * i++): 0, 
				n -= 2, m += 3, l = f / 0.7, y * INPUT_OFFSET + SPEED_SCALE - i * k) 
		: 	(k | !o ? (	f = l = C[u] / -2 - t, k += 17 * w,	o = k++ + 20, 
						n = k + k - k * k / 9, INPUT_OFFSET * p) 
		: (o-- < a ? y = 135, SPEED_SCALE * INPUT_OFFSET | screen_height : 0 )
			)
		)
	);

	// 4. for-loop
	for (
		// Initialization of next for-loop
		x = g = m = f = 0, 
		a = i = u + screen_height, 
		d += 9; 
    
		o < f 
			? 
				n = 2 * screen_width,
				
				draw_mode_setcolors(z ? r + k * INPUT_OFFSET * 8 : TIME_SCALE - k * SPEED_SCALE * SPEED_SCALE),
				
				n = 9 + x / screen_height,
				
				draw_mode_setcolors(!k + z ? r = screen_height * INPUT_OFFSET + 9 : SPEED_SCALE << 17),	
          
				n -= x / 9 / screen_height,
				
				draw_mode_setcolors(i % p > 6 ? h = r * (3 + z) / 5 : r),
				
				n = x / 13 / screen_width,
				
				draw_mode_setcolors(k ? h : r),
			
				o++
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

	// 5. for-loop
	for ( k=i=0, x=g, l=b; 
		
		(++i == 41 + !f && k) ? (f = k = i = l = 0, x = 190, a = 163) :
		(i == 42 ? (i = 9, k = r ) :
		(!k | (i - 41))); 
		
		n = (min(min(min(75, SPEED_SCALE - i) / 3, 95 - 2 * i), r - k + 2 * i) - k / r) * x / screen_height,
		m = (i < 9)
			? (x * 14 / screen_height)
			: (k ? (i - 31) / 5 * (i % 12) ? 0 : n - 1 : 0),
			o = a - i * x / screen_height,
			y = screen_height - o,
			//h = draw_mode_loop5_caseA(k * r + k * f / INPUT_OFFSET) //not needed anymore
			h = (k * r + k * f / INPUT_OFFSET),
			draw_mode_setcolors(h -1),		//update car colors and draw
			h = (k * r + k * f / INPUT_OFFSET)
			
	);

	// 6. for-loop 
	for (
		gettimeofday(&tv, 0),
		H[0] = tv.tv_sec,   // seconds
		H[1] = tv.tv_usec,  // microseconds

		// Calculate h the same way as before
		h = SPEED_SCALE * H[0] + H[1] / p - c,
	
		// If g is true, keep B; otherwise, set B based on h mod 3
		B = g ? B : 1 - h % 3,

		i = 60,            // seconds per minute
		y = n = m,     // initialize y, n, m to the same value (probably screen position/score)
    
		// Debug: print key variables at the start of each iteration
		/*
		 fprintf(stderr, "[LOOP6] iter f=%d h=%d z=%d s=%d x=%d y=%d n=%d m=%d\n",
            f, h, z, s, x, y, n, m), */
            
		// Draw status text on screen
		len = snprintf(P, sizeof(P),
			"Time: %1d'%02d\"%02d      High: %1d'%02d\"%02d      Speed: %03dmph",
			h / 100 / i, h / 100 % i, h % 100,
			q / 100 / i, q / 100 % i, q % 100,
			s
		),
		XDrawString(display, pixmap, gc, 30, x, P, len),
        //draw_mode_loop6_caseA(ARG_DRAW_MODE), // Trigger first part in loop1
		x = h; // Set x to current time/tick count (?)
    	++f < 2 * z * screen_width;   // Loop limit depends on screen width (e) and difficulty (z)
    	// Loop body: graphical computation and update?
		l = (x *= 3) % screen_width,
		o = (x * x) % a,
		//draw_mode_loop6_caseB(ARG_WINTER_MODE) 	// Trigger snow in wintermode not needed anymore
		draw_mode_setcolors(ARG_WINTER_MODE - 1)
	);
	}
	return SPEED_SCALE * *H + 1[H]/p - c;
}
