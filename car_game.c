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
int argc;

void parse_arguments(int argc, char *argv[]) {
	static int already_called = 0;
	if (already_called) { // If already parsed do nothing
		return;
	}
	printf("[DEBUG parse_arguments]\n");
    if (argc == 1) { 
		already_called = 1;	
		return;
	}
	if (argc == 2 ) {
		z = (strchr(argv[1], 'a') != NULL);	//is_winter
		w = (strchr(argv[1], 'd') != NULL);	//is_night
		if (strchr(argv[1], '1')) {	//track_number B=4 is default			
			B = 3;
		} else if (strchr(argv[1], '2')) {
			B = 2;
		}
	already_called = 1;	
	}
	return;
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

void draw_mode_copy_to_screen() {
	printf("[DEBUG draw_mode_copy_to_screen]\n");	
	// Copy drawn area to screen
	XCopyArea(display, pixmap, window, gc, 0, 0, 2 * screen_height, screen_height, 0, 0);
	XSync(display, 0);
	XFlush(display);
	usleep(FRAME_DELAY_US);  // sleep time (FPS) in milliseconds
    return;
}

void draw_mode_graphics_setup () {
	static int already_called = 0;
	if (already_called) { // If already parsed do nothing
		return;
	}
	printf("[DEBUG draw_mode_graphics_setup]\n");
	display = XOpenDisplay (0);
	
	window = XCreateSimpleWindow (display, RootWindow(display, 0), 0, 0,  screen_height * 2, screen_height, 0, 0, 0);
	gc = XCreateGC (display, window, 0, 0);
	
	XStoreName(display, window, "VIK1 Remake");
    XSetIconName(display, window, "VIK1 Remake");
    
	XSelectInput (display, window, KeyPressMask | KeyReleaseMask); // Input events listen for key press/release
	XMapWindow (display, window);	// Make the window visible
	// Create a pixmap (offscreen drawing area)
	pixmap = XCreatePixmap (display, window, screen_height * 2, d = screen_height, DefaultDepth(display, 0));
	already_called = 1;
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

int main(int argc, char** argv) {

int screen_width = screen_height * 2;

parse_arguments(argc, argv);
draw_mode_graphics_setup ();

// 1. for-loop for(initialization; condition; increment/decrement)
printf("[DEBUG enters loop1]\n");
for (
    p = 10660;
     --argc & ~1 ? (	// Behavior depending on argc
			printf("[DEBUG in loop1 calling gettimeofday]\n"),
			gettimeofday((struct timeval*)H, 0) // Record timestamp
    ) : argc ? (
			printf("[DEBUG in loop1 setting argc]\n"),
			argc	// Keep argc unchanged			
    ) : ++argc;
			draw_mode_copy_to_screen()
)
	{	
	// 2. for-loop
	printf("[DEBUG enters loop2]\n");
	for ( 
		3[H] = INPUT_OFFSET;	// Sets a timestamp?
		p==++r ? ( // Initial setup when r reaches p
		// Reset various control variables?
		u = h = q = TIME_SCALE		//53300
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
	printf("[DEBUG enters loop3]\n");
	for (
		// 1. Userinput: right and left arrows
		k = C[INPUT_OFFSET + XK_Right] - C[INPUT_OFFSET + XK_Left],
 		// 2. Jerk acceleration if arrow key is pressed, else decay?
		j = k ? j + v * k * (j * k < 8) : j * 0.9, 
		// 3. Speed update if space is held (s = skid or boost?)
		u += (s = min(C[INPUT_OFFSET + XK_Up] * 3 + s - !!s, a)) / 9,
		// 4. Initial timing setup/handling: if passed finish line
		c = u > p		// hvis u er større end p
			? (q = min(h, q), u %= p, 
      		reset_current_lap(c=0)
       		)
			: c, 
		// 5. Horizontal movement (track offset)?
		t += j * (s + SPEED_SCALE) / screen_width - s / i * (C[u + 4] - C[u]),    
		// 6. Adjust speed if too fast, or reset if crash?
		s = t * t > p 
			? (t *= 0.97, s * 0.8)	// speed decay?
			: (g > a && b * b < screen_height * 9 ? i : s), 	// crash condition?
		// 7. Reset several control variables?
		i = k = l = 0, 
		o = n = screen_width; 
		i - 5;	// Loop runs as long as i < 5
		
		// Drawing logic, depends on state of 'k' (movement/input)
		draw_mode_setcolors(
			(k / 17) ? (o++ == y ? (m = n = screen_height, f -= SPEED_SCALE, o = 30 + 9 * i++): 0, 
				n -= 2, m += 3, l = f / 0.7, y * INPUT_OFFSET + SPEED_SCALE - i * k) 
		: 	(k | !o ? (	f = l = C[u] / -2 - t, k += 17 * w,	o = k++ + 20, 
						n = k + k - k * k / 9, INPUT_OFFSET * p) 
		: (o-- < a ? y = 135, SPEED_SCALE * INPUT_OFFSET | screen_height : 0 )
			)
		)
	);

	// 4. for-loop
	printf("[DEBUG enters loop4]\n");
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
	printf("[DEBUG enters loop5]\n");
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
			h = (k * r + k * f / INPUT_OFFSET),
			draw_mode_setcolors(h -1),		//update car colors and draw
			h = (k * r + k * f / INPUT_OFFSET)
	);

	// 6. for-loop
	printf("[DEBUG enters loop6]\n");
	for (
		time_trigger(),
		// If g is true, keep B; otherwise, set B based on h mod 3
		B = g ? B : 1 - h % 3,

		i = 60,            // seconds per minute
		y = n = m,     // initialize y, n, m to the same value (probably screen position/score)
               
		// Draw status text on screen
		len = snprintf(P, sizeof(P),
			"Time: %1d'%02d\"%02d      High: %1d'%02d\"%02d      Speed: %03dmph",
			h / 100 / i, h / 100 % i, h % 100,
			q / 100 / i, q / 100 % i, q % 100,
			s
		),
		XDrawString(display, pixmap, gc, 30, x, P, len),
       	x = h; // Set x to current time/tick count (?)
    	++f < 2 * z * screen_width;   // Loop limit depends on screen width (e) and difficulty (z)
    	// Loop body: graphical computation and update?
		l = (x *= 3) % screen_width,
		o = (x * x) % a,
		draw_mode_setcolors(ARG_WINTER_MODE - 1)	// Trigger snow in wintermode
	);
	}
	return SPEED_SCALE * *H + 1[H]/p - c;
}
