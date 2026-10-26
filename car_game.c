#include                 <stdio.h>
#include                 <sys/time.h>
#include                 <X11/Xlib.h>
#include                 <X11/keysym.h>

int g, h, i, j, k, l, m, n, p, q, r, v, w, x, y, z;
int o, b, f, u, s, c, a, t, e, d, C[1<<18], A=100, B=4, D=200, E=53300, F=65793, H[4];

int main(int, char **);
char **global_argv;  // global pointer to the arguments

//naming variables: 
//K replaced with display
//L replaced with window
//M replaced with pixmap
//N replaced with gc
//O replaced with event


int draw_mode_loop3_caseA(int value) {
	//static int counter = 0;
   // printf("[draw_mode_loop6_caseB] Call #%d with value: %d\n", ++counter, value);
    printf("draw_mode_loop3_caseA(%d)\n", value);
    return main(value, global_argv);  // use global_argv here
}

int draw_mode_loop3_caseB(int value) {
	//static int counter = 0;
   // printf("[draw_mode_loop6_caseB] Call #%d with value: %d\n", ++counter, value);
   // printf("draw_mode_loop3_caseB(%d)\n", value);
    return main(value, global_argv);  // use global_argv here
}

int draw_mode_loop4_caseA(int value) {
	//static int counter = 0;
   // printf("[draw_mode_loop6_caseB] Call #%d with value: %d\n", ++counter, value);
  //  printf("draw_mode_loop4_caseA(%d)\n", value);
    return main(value, global_argv);  // use global_argv here
}

int draw_mode_loop4_caseB(int value) {
	//static int counter = 0;
   // printf("[draw_mode_loop6_caseB] Call #%d with value: %d\n", ++counter, value);
  //  printf("draw_mode_loop4_caseB(%d)\n", value);
    return main(value, global_argv);  // use global_argv here
}

int draw_mode_loop4_caseC(int value) {
	//static int counter = 0;
   // printf("[draw_mode_loop6_caseB] Call #%d with value: %d\n", ++counter, value);
 //   printf("draw_mode_loop4_caseC(%d)\n", value);
    return main(value, global_argv);  // use global_argv here
}

int draw_mode_loop4_caseD(int value) {
	//static int counter = 0;
   // printf("[draw_mode_loop6_caseB] Call #%d with value: %d\n", ++counter, value);
  //  printf("draw_mode_loop4_caseD(%d)\n", value);
    return main(value, global_argv);  // use global_argv here
}

int draw_mode_loop5_caseA(int value) {
	//static int counter = 0;
   // printf("[draw_mode_loop6_caseB] Call #%d with value: %d\n", ++counter, value);
 //   printf("draw_mode_loop5_caseA(%d)\n", value);
    return main(value, global_argv);  // use global_argv here
}

int draw_mode_loop6_caseA(int value) {
	//static int counter = 0;
   // printf("[draw_mode_loop6_caseB] Call #%d with value: %d\n", ++counter, value);
   // printf("draw_mode_loop6_caseA(%d)\n", value);
    return main(value, global_argv);  // use global_argv here
}

int draw_mode_loop6_caseB(int value) {	//arg=a on the command line = winter driving
	//static int counter = 0;
   // printf("[draw_mode_loop6_caseB] Call #%d with value: %d\n", ++counter, value);
 //   printf("draw_mode_loop6_caseB(%d)\n", value);
    return main(value, global_argv);  // use global_argv here
}





#define draw_mode(a) main(a,argv)

//#define Y(a,b) ((a) < (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

 
unsigned int color_component(int a, int argc, int D, int y, int v, int z, int w) {
    int temp = (((y / v) * !w) +
               ((((argc + 1) >> a) & 255) * (D + y / (z | (w - 2))) / D));
    return temp << a;
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

XEvent event;

// 1. for-loop
for (
    p = E / 5;

    // Conditional initialization depending on argc
    --argc & ~1 ? (
        // Set foreground color using bit-shifted color components
        XSetForeground(display, gc,
            color_component(0, argc, D, y, v, z, w) |
            color_component(8, argc, D, y, v, z, w) |
            color_component(16, argc, D, y, v, z, w)
        ),

        // Draw car lines
        XDrawLine(display, pixmap, gc, D + l - m, o, D + l - n, o),
        XDrawLine(display, pixmap, gc, D + l + m, o, D + l + n, o),

        // Record timestamp
        gettimeofday((struct timeval*)H, 0)
    ) : argc ? (
        // Parse command-line argument (checks for 'a'?)
        z = 97 == **++argv,

        // Compare with some reference value A
        w = A == z[*argv],

        // Adjust value of B based on lookup result
        B -= w[z + *argv] % 3,

        // Keep argc unchanged
        argc
    ) : ++argc;

    // Copy drawn area to screen
    XCopyArea(display, pixmap, window, gc, 0, 0, e, D, 0, 0),
    XSync(display, 0),
    XFlush(display),

    // Wait for a short timeout (non-blocking select)
    select(0, 0, 0, 0, ((struct timeval*)H) + 1)
)
{ ;

// 2. for-loop
for ( 
		3[H] = F;	// Sets a timestamp or animation counter
		
		p==++r ? (
		// Initial setup when r reaches p
		display = XOpenDisplay (0),
		gc = XCreateGC (display,
		window = XCreateSimpleWindow (display, RootWindow(display, 0), 0, 0, e = D * 2, D, 0, 0, 0), 0, 0),
		
		// Set input events to listen for key press/release
		XSelectInput (display, window, KeyPressMask | KeyReleaseMask),
		
		// Make the window visible
		XMapWindow (display, window),
		
		// Create a pixmap (offscreen drawing area)
		pixmap = XCreatePixmap (display, window, e, d = D, DefaultDepth(display, 0)), 
		
		// Reset various control variables
		u = h = q = E
	) : r < p ? (
		// Ongoing pre-game setup (random track generation?)
		a = r / D % 6,
		
		r[C + p] = (l += (!a | a % 2) ? g : a - 3) / 4,
		
		r[C] = 2 * r <= p
			? (n += (i -= ((i + 34 && r	/ 149 % B) - (i - 34 && r / 353 % 3))) ) / 55
			: C[p - r],
		v = 4 - 3 * z
	):
			// Event handling: poll key state changes
			XPending (display) ? (
				C[F+ XLookupKeysym (&event.xkey, 0 & XNextEvent (display, &event))]^=
					(event.type == KeyPress || event.type == KeyRelease),
					p
			):0;
 );
 
// 3. for-loop
for (
	// 1. Userinput: right and left arrows
    k = C[F + XK_Right] - C[F + XK_Left],
    
    // 2. Jerk acceleration if arrow key is pressed, else decay
    j = k ? j + v * k * (j * k < 8) : j * 0.9, 
    
    // 3. Speed update if space is held (s = skid or boost?)
    u += (s = min(C[F + XK_space] * 3 + s - !!s, a)) / 9, 
    
    // 4. Lap handling: if passed finish line
    c = u > p
		? (q = min(h, q), u %= p, draw_mode_loop3_caseA(c = 0))	// reset current lap
		: c, 
		
	// 5. Horizontal movement (track offset)
    t += j * (s + A) / e - s / i * (C[u + 4] - C[u]), 
    
    // 6. Adjust speed if too fast, or reset if crash
    s = t * t > p 
		? (t *= 0.97, s * 0.8)					// speed decay
		: (g > a && b * b < D * 9 ? i : s), 	// crash condition?
		
	// 7. Reset several control variables
    i = k = l = 0, 
    o = n = e; 

	// Loop runs as long as i < 5
	//change below from 5 to 1 for debug?
    i - 5;

	// Drawing logic, depends on state of 'k' (movement/input)
    draw_mode_loop3_caseB(
        (k / 17) 
            ? (o++ == y 
                ? (m = n = D, f -= A, o = 30 + 9 * i++) 
                : 0, 
              n -= 2, 
              m += 3, 
              l = f / 0.7, 
              y * F + A - i * k
             ) 
            : (
				k | !o 
					? (
						f = l = C[u] / -2 - t, 
						k += 17 * w, 
						o = k++ + 20, 
						n = k + k - k * k / 9, 
						F * p
					) 
					: (
						o-- < a
							? y = 135, A * F | D
							: 0
					)
            )
    )
);

// 4. for-loop
for (
	// Initialization of next for-loop
    x = g = m = f = 0, 
    a = i = u + D, 
    d += 9; 
    
    o < f 
        ? 

			n = 2 * e,
			draw_mode_loop4_caseA(z ? r + k * F * 8 : E - k * A * A),	
          
			n = 9 + x / D,
			draw_mode_loop4_caseB(!k + z ? r = D * F + 9 : A << 17),	
          
			n -= x / 9 / D,
			draw_mode_loop4_caseC(i % p > 6 ? h = r * (3 + z) / 5 : r),	
			
			n = x / 13 / e,
			draw_mode_loop4_caseD(k ? h : r),		
			
			o++		
        : (
            
            f = x / D + 70 + C[i + p] - C[u + p],
            
          
            o = x ? o : f,
            
            k = 2 * i / 41 % 2,		
            
            l = C[--i] - C[u] - t,		
            
            x += D - (y = i - u),	
            
            
            g = (d - i) % 410 ? g : (a = o, b = l + B * x / e, x) / A,
            
           
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
		
		n = (min(min(min(75, A - i) / 3, 95 - 2 * i), r - k + 2 * i) - k / r) * x / D,
		m = (i < 9)
			? (x * 14 / D)
			: (k ? (i - 31) / 5 * (i % 12) ? 0 : n - 1 : 0),
			o = a - i * x / D,
			y = D - o,
			h = draw_mode_loop5_caseA(k * r + k * f / F)
);

// 6. for-loop 
for (
    // If g is true, keep B; otherwise, set B based on h mod 3
    B = g ? B : 1 - h % 3,

    i = 60,            // seconds per minute
    y = n = m,         // initialize y, n, m to the same value (probably screen position/score)
    
    // Draw status text on screen
    XDrawString(
        display, pixmap, gc,
        30, x, P,
        sprintf(
            P,
            "Time: %1d'%02d\"%02d      High: %1d'%02d\"%02d      Speed: %03dmph",
            h / A / i, h / A % i, h % A,       // Current time (min, sec, ticks)
            q / A / i, q / A % i, q % A,       // High score time
            s,
            draw_mode_loop6_caseA(A << 17)                // Possibly a color/value update
        )
    ),

    x = h;             // Set x to current time/tick count (?)
    
    ++f < 2 * z * e;   // Loop limit depends on screen width (e) and difficulty (z)
    
    // Loop body: graphical computation and update
    l = (x *= 3) % e,
    o = (x * x) % a,
    draw_mode_loop6_caseB(-1)      // Some kind of drawing effect with calculated values
);

	}
	return A * *H + 1[H]/p - c;
}
