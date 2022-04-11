#pragma once

// misc
#define THREADS_NUMBER			4
#define THREADING_TYPE			0 // 0 = multithreading, 1 = monothreading
#define AZERTY_KEYS			    false
#define MOUSE_SENSITIVITY       0.002f

// implementation limitations
#define BOX_MAX_RANGE		    10000.f // used because of the way we implemented the box tests
#define LIGHTS_NUMBER		    8 // used because an UBO can't have a dynamic size
#define SCENES_NUMBER		    4

// rendering related
#define SCR_WIDTH			    1280
#define SCR_HEIGHT			    720
#define PARTICLE_MAX_MODELS     16
#define CUT_RENDERING_CODE		-2

// math/physics related
#define GRAVITY				    2.61f
#define PI					    3.14159265358979323846f

// camera related
#define CAMERA_SPEED		    5.0f
#define CAMERA_LERP		        0.25f
#define CAMERA_MAX_PITCH	    87.5f
#define ORTHOGRAPHIC_SRC_SIZE   10

// game related
#define MAIN_MENU_SCENE			0
#define WIN_SCENE				2
#define GAME_OVER_SCENE			3
