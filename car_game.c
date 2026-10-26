#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

int g, h, i, j, k, l, m, n, p, q, r, v, x, y;
int o, b, f, u, s, c, a, t, d, C[1<<18], B=4,  H[4];

int screen_height = 200;	//previous D=200,
const int TIME_SCALE = 53300;	//previous E=53300,
const int INPUT_OFFSET = 65793;	//previous F=65793,
const int SPEED_SCALE = 100;	//previous A=100,
//int e, replaced with screen_width in main

int z=0;		//is_winter
int w=0;		//is_night

int main(int, char **);
char **global_argv;

int draw_mode_loop3_caseA(int value) {
    return main(value, global_argv);
}

int draw_mode_loop3_caseB(int value) {
    return main(value, global_argv); 
}

int draw_mode_loop4_caseA(int value) {
    return main(value, global_argv);  
}

int draw_mode_loop4_caseB(int value) {
    return main(value, global_argv); 
}

int draw_mode_loop4_caseC(int value) {
    return main(value, global_argv); 
}

int draw_mode_loop4_caseD(int value) {
    return main(value, global_argv);
}

int draw_mode_loop5_caseA(int value) {
    return main(value, global_argv); 
}

int draw_mode_loop6_caseA(int value) {
    return main(value, global_argv); 
}

int draw_mode_loop6_caseB(int value) {	
    return main(value, global_argv);
}

#define draw_mode(a) main(a,argv)
#define min(a,b) ((a) < (b) ? (a) : (b))
 
unsigned int color_component(int a, int argc, int y, int v, int z, int w) {
    int temp = (((y / v) * !w) +
               ((((argc + 1) >> a) & 255) * (screen_height + y / (z | (w - 2))) / screen_height));
    return temp << a;
}


void parse_arguments(char *arg) {
    z = (strchr(arg, 'a') != NULL);	//is_winter
    w = (strchr(arg, 'd') != NULL);	//is_night
	//track_number B=4 is default
    if (strchr(arg, '1')) {			
        B = 3;
    } else if (strchr(arg, '2')) {
        B = 2;
    }
}

Display* display;
Window window;
Pixmap pixmap;
GC gc;
char P[99];

int main(int argc, char** argv) {	

global_argv = argv;

if (argv == NULL) {
    printf("main called from draw_mode: value = %d\n", argc);
}

int screen_width = screen_height * 2;
XEvent event;


// 1. for-loop for(initialization; condition; increment/decrement)
for (
    p = TIME_SCALE / 5;

    // Conditional initialization depending on argc
    --argc & ~1 ? (
        // Set foreground color using bit-shifted color components
        XSetForeground(display, gc,
			color_component(0, argc, y, v, z, w) |
			color_component(8, argc, y, v, z, w) |
			color_component(16, argc, y, v, z, w)
		),

        // Draw car lines
        XDrawLine(display, pixmap, gc, screen_height + l - m, o, screen_height + l - n, o),
        XDrawLine(display, pixmap, gc, screen_height + l + m, o, screen_height + l + n, o),

        // Record timestamp
        gettimeofday((struct timeval*)H, 0)
    ) : argc ? (
		//fprintf(stderr, "[DEBUG] argc=%d, argv[0]=%s\n", argc, argv[0]),
		//argv[1] ? fprintf(stderr, "[DEBUG] argv[1]=%s\n", argv[1]) : 0,
		
		//z, w, og B er is_winter, is_night, og track_number
		
        // Parse argv "a" - Choice between summer (no "a", z=0) and winter ("a", z=1)
		/*z = **++argv == 'a',
		fprintf(stderr, "[DEBUG] z=%d\n", z),

		// Parse argv "d" - Choice between day (no "d", w=0) and night ("d", w=1)
		w = (*argv)[z] == 'd',
		fprintf(stderr, "[DEBUG] w=%d\n", w),

		// Choice of race-track: track3 => B=4, track2 => B=2, track1 => B=3
		B -= w * ((*argv)[z + 1] % 3),
		fprintf(stderr, "[DEBUG] B after = %d\n", B),*/
		
		parse_arguments(argv[1]),
		fprintf(stderr, "[DEBUG] z=%d, w=%d, B=%d\n", z, w, B),
        // Keep argc unchanged
        argc
    ) : ++argc;

    // Copy drawn area to screen
    XCopyArea(display, pixmap, window, gc, 0, 0, 2 * screen_height, screen_height, 0, 0),
    XSync(display, 0),
    XFlush(display),

    // Wait for a short timeout (non-blocking select)
    select(0, 0, 0, 0, ((struct timeval*)H) + 1)
)

	{	

	// 2. for-loop
	for ( 
		3[H] = INPUT_OFFSET;	// Sets a timestamp or animation counter
		
		p==++r ? (
		// Initial setup when r reaches p
		display = XOpenDisplay (0),
		gc = XCreateGC (display,
		window = XCreateSimpleWindow (display, RootWindow(display, 0), 0, 0, screen_width, screen_height, 0, 0, 0), 0, 0),
		
		// Set input events to listen for key press/release
		XSelectInput (display, window, KeyPressMask | KeyReleaseMask),
		
		// Make the window visible
		XMapWindow (display, window),
		
		// Create a pixmap (offscreen drawing area)
		pixmap = XCreatePixmap (display, window, screen_width, d = screen_height, DefaultDepth(display, 0)), 
		
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
			XPending (display) ? (
				C[INPUT_OFFSET+ XLookupKeysym (&event.xkey, 0 & XNextEvent (display, &event))]^=
					(event.type == KeyPress || event.type == KeyRelease),
					p
			):0;
	);

	// 3. for-loop
	for (
		// 1. Userinput: right and left arrows
		k = C[INPUT_OFFSET + XK_Right] - C[INPUT_OFFSET + XK_Left],
    
		// 2. Jerk acceleration if arrow key is pressed, else decay
		j = k ? j + v * k * (j * k < 8) : j * 0.9, 
    
		// 3. Speed update if space is held (s = skid or boost?)
		u += (s = min(C[INPUT_OFFSET + XK_space] * 3 + s - !!s, a)) / 9, 
    
		// 4. Lap handling: if passed finish line
		c = u > p
			? (q = min(h, q), u %= p, draw_mode_loop3_caseA(c = 0))	// reset current lap
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

		// Loop runs as long as i < 5
		//change below from 5 to 1 for debug?
		i - 5;

		// Drawing logic, depends on state of 'k' (movement/input)
		draw_mode_loop3_caseB(
			(k / 17) 
				? (o++ == y 
					? (m = n = screen_height, f -= SPEED_SCALE, o = 30 + 9 * i++) 
					: 0, 
				n -= 2, 
				m += 3, 
				l = f / 0.7, 
				y * INPUT_OFFSET + SPEED_SCALE - i * k
				) 
				: (
					k | !o 
						? (
							f = l = C[u] / -2 - t, 
							k += 17 * w, 
							o = k++ + 20, 
							n = k + k - k * k / 9, 
							INPUT_OFFSET * p
						) 
						: (
							o-- < a
								? y = 135, SPEED_SCALE * INPUT_OFFSET | screen_height
								: 0
						)
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
				draw_mode_loop4_caseA(z ? r + k * INPUT_OFFSET * 8 : TIME_SCALE - k * SPEED_SCALE * SPEED_SCALE),	
          
				n = 9 + x / screen_height,
				draw_mode_loop4_caseB(!k + z ? r = screen_height * INPUT_OFFSET + 9 : SPEED_SCALE << 17),	
          
				n -= x / 9 / screen_height,
				draw_mode_loop4_caseC(i % p > 6 ? h = r * (3 + z) / 5 : r),	
			
				n = x / 13 / screen_width,
				draw_mode_loop4_caseD(k ? h : r),		
			
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
	for (
		k=i=0,
		x=g,
		l=b; 
		
		(++i == 41 + !f && k) ? (f = k = i = l = 0, x = 190, a = 163) :
		(i == 42 ? (i = 9, k = r ) :
		(!k | (i - 41))); 
		
		n = (min(min(min(75, SPEED_SCALE - i) / 3, 95 - 2 * i), r - k + 2 * i) - k / r) * x / screen_height,
		m = (i < 9)
			? (x * 14 / screen_height)
			: (k ? (i - 31) / 5 * (i % 12) ? 0 : n - 1 : 0),
			o = a - i * x / screen_height,
			y = screen_height - o,
			h = draw_mode_loop5_caseA(k * r + k * f / INPUT_OFFSET)
	);

	// 6. for-loop 
	for (
		// If g is true, keep B; otherwise, set B based on h mod 3
		B = g ? B : 1 - h % 3,

		i = 60,            // seconds per minute
		y = n = m,         // initialize y, n, m to the same value (probably screen position/score)
    
		// Draw status text on screen
		XDrawString(
			display, pixmap, gc, 30, x, P,
			sprintf(
			P,
            "Time: %1d'%02d\"%02d      High: %1d'%02d\"%02d      Speed: %03dmph",
            h / SPEED_SCALE / i, h / SPEED_SCALE % i, h % SPEED_SCALE,       // Current time (min, sec, ticks)
            q / SPEED_SCALE / i, q / SPEED_SCALE % i, q % SPEED_SCALE,       // High score time
            s,
            draw_mode_loop6_caseA(SPEED_SCALE << 17)                // Possibly a color/value update
			)
		),

		x = h;             // Set x to current time/tick count (?)
    
		++f < 2 * z * screen_width;   // Loop limit depends on screen width (e) and difficulty (z)
    
		// Loop body: graphical computation and update
		l = (x *= 3) % screen_width,
		o = (x * x) % a,
		draw_mode_loop6_caseB(-1)      // Some kind of drawing effect with calculated values
	);
	}
	return SPEED_SCALE * *H + 1[H]/p - c;
}
