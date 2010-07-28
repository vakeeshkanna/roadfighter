#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <ddraw.h>
#include <string.h>
#include <process.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include <string>


#define SafeDelete(x) if(x) { delete x; x = NULL; }
//#define NEW_APPROACH

#define CELL_SIZE 16
#define BOUNDARY_CELL_SIZE 8

//#define DO_LOG_ON_SCREEN

#define IMAGES_DIR "d:\dev\src\roadfighter\images"

#define MAX_FILE_NAME 256

#define NUM_STAGES 4

//#define SCREEN_WIDTH 1024
//#define SCREEN_HEIGHT 768

#define STAGE_WIDTH 256//(SCREEN_WIDTH - 560)
#define STAGE_HEIGHT 224

#define MAX_STAGE_HEIGHT 14000
#define STAGE_ONE_HEIGHT 22400
#define STAGE_TWO_HEIGHT 22400
#define STAGE_THREE_HEIGHT 22400
#define STAGE_FOUR_HEIGHT 22400

#define MAX_CELL_TRACK 11

#define VIEWPORT_WIDTH (STAGE_WIDTH)
#define VIEWPORT_HEIGHT (STAGE_HEIGHT)

#define HUD_WIDTH 585
#define HUD_HEIGHT 430

#define SCALE_FACTOR 2 //2.49

#define BORDER_GAP  2


// BG colors

#define BLACK 0
#define GRASS 2

#define START_FLAG_X 11
#define START_FLAG_Y 10

#define FINISH_FLAG_X 11
#define FINISH_FLAG_Y 200

#define PROGRESS_CAR_X 12
#define PROGRESS_CAR_Y 200





