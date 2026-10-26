#include                 <stdio.h>
#include                 <sys/time.h>
#include                 <X11/Xlib.h>
#include                 <X11/keysym.h>

int g, h, i, j, k, l, m, n, p, q, r, v, w, x, y, z;
int o, b, f, u, s, c, a, t, e, d, C[1<<18], A=100, B=4, D=200, E=53300, F=65793, H[4];

int main(int, char **);
char **global_argv;  // global pointer to the arguments


int draw_mode_loop3_caseA(int value) {
	//static int counter = 0;
   // printf("[draw_mode_loop6_caseB] Call #%d with value: %d\n", ++counter, value);
    printf("draw_mode_loop6_caseB(%d)\n", value);
    return main(value, global_argv);  // use global_argv here
}

int draw_mode_loop3_caseB(int value) {
	//static int counter = 0;
   // printf("[draw_mode_loop6_caseB] Call #%d with value: %d\n", ++counter, value);
    printf("draw_mode_loop6_caseB(%d)\n", value);
    return main(value, global_argv);  // use global_argv here
}

int draw_mode_loop4_caseA(int value) {
	//static int counter = 0;
   // printf("[draw_mode_loop6_caseB] Call #%d with value: %d\n", ++counter, value);
    printf("draw_mode_loop6_caseB(%d)\n", value);
    return main(value, global_argv);  // use global_argv here
}

int draw_mode_loop4_caseB(int value) {
	//static int counter = 0;
   // printf("[draw_mode_loop6_caseB] Call #%d with value: %d\n", ++counter, value);
    printf("draw_mode_loop6_caseB(%d)\n", value);
    return main(value, global_argv);  // use global_argv here
}

int draw_mode_loop4_caseC(int value) {
	//static int counter = 0;
   // printf("[draw_mode_loop6_caseB] Call #%d with value: %d\n", ++counter, value);
    printf("draw_mode_loop6_caseB(%d)\n", value);
    return main(value, global_argv);  // use global_argv here
}

int draw_mode_loop4_caseD(int value) {
	//static int counter = 0;
   // printf("[draw_mode_loop6_caseB] Call #%d with value: %d\n", ++counter, value);
    printf("draw_mode_loop6_caseB(%d)\n", value);
    return main(value, global_argv);  // use global_argv here
}

int draw_mode_loop5_caseA(int value) {
	//static int counter = 0;
   // printf("[draw_mode_loop6_caseB] Call #%d with value: %d\n", ++counter, value);
    printf("draw_mode_loop6_caseB(%d)\n", value);
    return main(value, global_argv);  // use global_argv here
}
int draw_mode_loop6_caseA(int value) {
	//static int counter = 0;
   // printf("[draw_mode_loop6_caseB] Call #%d with value: %d\n", ++counter, value);
    printf("draw_mode_loop6_caseB(%d)\n", value);
    return main(value, global_argv);  // use global_argv here
}

int draw_mode_loop6_caseB(int value) {	//arg=a on the command line = winter driving
	//static int counter = 0;
   // printf("[draw_mode_loop6_caseB] Call #%d with value: %d\n", ++counter, value);
    printf("draw_mode_loop6_caseB(%d)\n", value);
    return main(value, global_argv);  //use global_argv here
}





#define draw_mode(a) main(a,argv)

//#define Y(a,b) ((a) < (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

 
unsigned int color_component(int a, int argc, int D, int y, int v, int z, int w) {
    int temp = (((y / v) * !w) +
               ((((argc + 1) >> a) & 255) * (D + y / (z | (w - 2))) / D));
    return temp << a;
}


Display*K;
Window L;
Pixmap M;
GC N;
char P[99];


int main(int argc, char** argv) {	

global_argv = argv;  // save argv globally

XEvent O;

// 1. for-loop
for (
    p = E / 5;

    // Conditional initialization depending on argc
    --argc & ~1 ? (
        // Set foreground color using bit-shifted color components
        XSetForeground(K, N,
            color_component(0, argc, D, y, v, z, w) |
            color_component(8, argc, D, y, v, z, w) |
            color_component(16, argc, D, y, v, z, w)
        ),

        // Draw car lines
        XDrawLine(K, M, N, D + l - m, o, D + l - n, o),
        XDrawLine(K, M, N, D + l + m, o, D + l + n, o),

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
    XCopyArea(K, M, L, N, 0, 0, e, D, 0, 0),
    XSync(K, 0),
    XFlush(K),

    // Wait for a short timeout (non-blocking select)
    select(0, 0, 0, 0, ((struct timeval*)H) + 1)
)
{ ;

// 2. for-loop
for ( 
		3[H] = F;	// Sets a timestamp or animation counter
		
		p==++r ? (
		// Initial setup when r reaches p
		K = XOpenDisplay (0),
		N = XCreateGC (K,
		L = XCreateSimpleWindow (K, RootWindow(K, 0), 0, 0, e = D * 2, D, 0, 0, 0), 0, 0),
		
		// Set input events to listen for key press/release
		XSelectInput (K, L, KeyPressMask | KeyReleaseMask),
		
		// Make the window visible
		XMapWindow (K, L),
		
		// Create a pixmap (offscreen drawing area)
		M = XCreatePixmap (K, L, e, d = D, DefaultDepth(K, 0)), 
		
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
			XPending (K) ? (
				C[F+ XLookupKeysym (&O.xkey, 0 & XNextEvent (K, &O))]^=
					(O.type == KeyPress || O.type==KeyRelease),
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
			// Draw first row of road segment or terrain
			n = 2 * e,
			draw_mode_loop4_caseA(z ? r + k * F * 8 : E - k * A * A),	// Color depending on z and k
          
			n = 9 + x / D,
			draw_mode_loop4_caseB(!k + z ? r = D * F + 9 : A << 17),	// Choose r or color
          
			n -= x / 9 / D,
			draw_mode_loop4_caseC(i % p > 6 ? h = r * (3 + z) / 5 : r),	// Lighter line if we're far enough?
			
			n = x / 13 / e,
			draw_mode_loop4_caseD(k ? h : r),		// Use lighter color if k
			
			o++		// Next line
        : (
            // If we are done drawing (o >= f), then recalculate new row
            f = x / D + 70 + C[i + p] - C[u + p],
            
            // The first time we hit here we set o = f
            o = x ? o : f,
            
            k = 2 * i / 41 % 2,		// Toggles between 0 and 1, maybe left/right
            
            l = C[--i] - C[u] - t,		// Terrain difference?
            
            x += D - (y = i - u),	// Propulsion in x, y is used for difference in depth
            
            // Determine new value for g – which can be color or shade?
            g = (d - i) % 410 ? g : (a = o, b = l + B * x / e, x) / A,
            
            // Stay in the loop until we're back to square one.
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
        K, M, N,
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
	return A**H+1[H]/p-c;
}
