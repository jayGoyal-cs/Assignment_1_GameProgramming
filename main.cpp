#include "CS3113/cs3113.h"
#include <math.h>


/**

* Author: Jay Goyal

* Assignment: Pong Clone

* Date due: [10/5/2026]

* I pledge that I have completed this assignment without

* collaborating with anyone else, in conformance with the

* NYU School of Engineering Policies and Procedures on

* Academic Misconduct.

**/

// Global Constants (some of these are creditied to previous assignments)
constexpr int   SCREEN_WIDTH  = 1600 / 2,
                SCREEN_HEIGHT = 900 / 2,
                FPS           = 60,
                SIZE          = 200,
                FRAME_LIMIT   = 100;
constexpr float SPEED       = 1.0f;

constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
constexpr Vector2 BASE_SIZE   = { static_cast<float>(SIZE), static_cast<float>(SIZE) };

constexpr char gojo_white[] = "assets/GojoPurpleBase.png",
               gojo_black[] = "assets/gojo_color.png",
               blue[]       = "assets/Blue.png",
               swirl[]      = "assets/Swirl_blue.png",
               red[]        = "assets/Red.png",
               purple[]     = "assets/Purple.png";

// Global Variables (some of these are creditied to previous assignments)
AppStatus gAppStatus     = RUNNING;
float     gScaleFactor   = SIZE,
          gAngle         = 0.0f,
          gPulseTime     = 0.0f;
Vector2   gPosition      = ORIGIN;
Vector2   gScale         = BASE_SIZE;
float     gPreviousTicks = 0.0f;

Vector2 ggwpos = ORIGIN,
        ggbpos = ORIGIN,
        gbpos = {SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 2.0f},
        gspos = gbpos,
        grpos = {SCREEN_WIDTH / 1.25f, SCREEN_HEIGHT / 2.0f},
        gppos = {(SCREEN_WIDTH/2)-20,SCREEN_HEIGHT / 2.0f};

bool  gIsMerging = false;
float gBlueScale = 1.0f;
float gRedScale  = 1.0f;
float gPurpleScale = 0.0f;

bool  gIsAtTopBottom = false;
bool  gIsDoneCircle  = false;
float gOrbitAngle    = 0.0f;
float gPauseTimer    = 0.0f;

 bool Start_Timer = false;
 float timer_to_end = 0.0f;

float gsrot = 0.0f;

Texture2D gGojo_white;
Texture2D gGojo_black;
Texture2D gBlue;
Texture2D gSwirl;
Texture2D gRed;
Texture2D gPurple;

int gframeCount = 0;

// Function Declarations 
void initialise();
void processInput();
void update();
void render();
void shutdown();



void createSprite(Texture2D image, Vector2 Pos, float scale_factor, float offset_x, float offset_y, float angle){

     Rectangle textureArea = {
        0.0f, 0.0f,
        static_cast<float>(image.width),
        static_cast<float>(image.height)
    };
    
    Rectangle destinationArea = {
        Pos.x,
        Pos.y,
        static_cast<float>(gScale.x * scale_factor),
        static_cast<float>(gScale.y * scale_factor) 
    };

    Vector2 originOffset = {
        static_cast<float>(gScale.x) / offset_x,
        static_cast<float>(gScale.y) / offset_y
    };

    DrawTexturePro(
        image,
        textureArea,
        destinationArea,
        originOffset,
        angle,
        WHITE
    );

};


// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures");

    gGojo_white = LoadTexture(gojo_white);
    gGojo_black = LoadTexture(gojo_black);
    gBlue = LoadTexture(blue);
    gSwirl = LoadTexture(swirl);
    gRed = LoadTexture(red);
    gPurple = LoadTexture(purple);

    SetTargetFPS(FPS);
}

void processInput()
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    float ticks = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    gsrot += 5.0f;

    float centerX = (SCREEN_WIDTH / 2.0f) - 20.0f;
    float centerY = SCREEN_HEIGHT / 2.0f;
    float radius = (SCREEN_WIDTH / 1.25f) - centerX;
    float currentSpeed = 150.0f;

    if(Start_Timer){
        timer_to_end += deltaTime;
    }
    if (!gIsAtTopBottom) {
        gOrbitAngle += 2.0f * deltaTime;
        
        if (gOrbitAngle >= PI / 2.0f) {
            gOrbitAngle = PI / 2.0f; 
            gIsAtTopBottom = true;
        }
        
        gbpos.x = centerX + cos(PI + gOrbitAngle) * radius;
        gbpos.y = centerY + sin(PI + gOrbitAngle) * radius;
        grpos.x = centerX + cos(gOrbitAngle) * radius;
        grpos.y = centerY + sin(gOrbitAngle) * radius;
        gspos = gbpos; 
        
    } else if (!gIsDoneCircle) {
        gPauseTimer += deltaTime;
        
        if (gPauseTimer > 0.5f) { 
            gOrbitAngle += 5.0f * deltaTime; 
    
            if (gOrbitAngle >= 4.0f * PI) {
                gOrbitAngle = 4.0f * PI; 
                gIsDoneCircle = true;
            }
            
            gbpos.x = centerX + cos(PI + gOrbitAngle) * radius;
            gbpos.y = centerY + sin(PI + gOrbitAngle) * radius;
            grpos.x = centerX + cos(gOrbitAngle) * radius;
            grpos.y = centerY + sin(gOrbitAngle) * radius;
            gspos = gbpos;
        }
        
    } else if (!gIsMerging) {
        if (grpos.x > centerX) {
            grpos.x -= currentSpeed * deltaTime;   
        }
        if (gbpos.x < centerX) {
            gbpos.x += currentSpeed * deltaTime;  
        }
        gspos = gbpos;

        float distance = grpos.x - gbpos.x;
        if (distance <= 80.0f) { 
            gIsMerging = true;
        }
        
    } else {
        if (gBlueScale > 0.0f) gBlueScale -= 0.02f;
        if (gRedScale > 0.0f)  gRedScale -= 0.02f;
        
        if (gPurpleScale < 2.0f) gPurpleScale += 0.04f;
    }
    if (gPurpleScale >= 2.0f){
        Start_Timer = true;
    }
    if(timer_to_end >= 1.0){
        shutdown();
    }
}

void render()
{
    BeginDrawing();
    
   if (gIsMerging) {
        ClearBackground(BLACK); 
        createSprite(gGojo_black, ggbpos, 2.5, 1.0, 1.0, 0);
    } else {
        ClearBackground(WHITE); 
        createSprite(gGojo_white, ggwpos, 2.0, 1.0, 1.0, 0);
    }
    if (gBlueScale > 0.0f) {
        createSprite(gBlue, gbpos, gBlueScale, 2.0, 2.0, 0);
        createSprite(gSwirl, gspos, 0.5f * gBlueScale, 4, 4, gsrot);
    }
    
    if (gRedScale > 0.0f) {
        createSprite(gRed, grpos, gRedScale, 2.0, 2.0, 0);
    }
    if (gIsMerging && gPurpleScale > 0.0f) {
        createSprite(gPurple, gppos, gPurpleScale, 1.0, 1.0, 0); 
    }
    EndDrawing();
}

void shutdown()
{

    CloseWindow();

    UnloadTexture(gGojo_white);
    UnloadTexture(gGojo_black); 
    UnloadTexture(gBlue);
    UnloadTexture(gSwirl);
    UnloadTexture(gRed);
    UnloadTexture(gPurple);     
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}