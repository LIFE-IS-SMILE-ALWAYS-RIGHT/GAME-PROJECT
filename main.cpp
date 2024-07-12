#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <cmath>
#include <string>

const std::string MESSAGE_YOU_WIN = " You Win!!! ";
const std::string MESSAGE_GAME_OVER = " Game Over ";
const std::string MESSAGE_TRY_AGAIN = " Press any key to exit ";
const char* WINDOW_TITLE = "Brick Breaker!";

#define WIDTH 700
#define HEIGHT 600
#define SPEED 8
#define FONT_SIZE 24
#define BALL_SPEED 8
#define SIZE 16
#define COL 11
#define ROW 7
#define PI 3.14
#define SPACING 10

SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font* font;
bool game_over = false;
bool game_won = false;
int frameCount, timerFPS, lastFrame, fps;
SDL_Color color;
SDL_Rect paddle, ball, lives, brick;
float velY, velX;
int liveCount;
bool bricks[ROW*COL];

void logErrorAndExit(const char* msg, const char* error)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}

SDL_Window* initSDL(int SCREEN_WIDTH, int SCREEN_HEIGHT, const char* WINDOW_TITLE)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logErrorAndExit("SDL_Init", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
        logErrorAndExit( "SDL_image error:", IMG_GetError());

    return window;
}

SDL_Renderer* createRenderer(SDL_Window* window)
{
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

    return renderer;
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    IMG_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_PollEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}

void renderTexture(SDL_Texture *texture, int x, int y, SDL_Renderer* renderer)
{
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(renderer, texture, NULL, &dest);
}

SDL_Texture *loadTexture(const char *filename, SDL_Renderer* renderer)
{
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

	SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
	if (texture == NULL)
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());

	return texture;
}

void resetBricks() {
    for(int i=0; i<COL*ROW; i++) bricks[i]=1;
    liveCount=3;
    paddle.x=(WIDTH/2)-(paddle.w/2);
    ball.y=paddle.y-(paddle.h*4);
    velY=BALL_SPEED/2;
    velX=0;
    ball.x=WIDTH/2-(SIZE/2);
}

void setBricks(int i) {
    brick.x=(((i%COL)+1)*SPACING)+((i%COL)*brick.w)-(SPACING/2);
    brick.y=brick.h*3+(((i%ROW)+1)*SPACING)+((i%ROW)*brick.h)-(SPACING/2);
}

void write(std::string text, int x, int y) {
    SDL_Surface *surface;
    SDL_Texture *texture;
    const char* t=text.c_str();
    surface = TTF_RenderText_Solid(font, t, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    lives.w=surface->w;
    lives.h=surface->h;
    lives.x=x-lives.w;
    lives.y=y-lives.h;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &lives);
    SDL_DestroyTexture(texture);
}

void update() {
    if(liveCount<=0) resetBricks();
    if(SDL_HasIntersection(&ball, &paddle)) {
        double rel=(paddle.x+(paddle.w/2))-(ball.x+(SIZE/2));
        double norm=rel/(paddle.w/2);
        double bounce = norm* (5*PI/12);
        velY=-BALL_SPEED*cos(bounce);
        velX=BALL_SPEED*-sin(bounce);
    }
    if(ball.y<=0) velY=-velY;
    if(ball.y+SIZE>=HEIGHT) {velY=-velY;liveCount--;}
    if(ball.x<=0 || ball.x+SIZE>=WIDTH) velX=-velX;
    ball.x+=velX;
    ball.y+=velY;
    if(paddle.x<0) paddle.x=0;
    if(paddle.x+paddle.w>WIDTH) paddle.x=WIDTH-paddle.w;

    bool reset=1;
    for(int i=0; i<COL*ROW; i++) {
        setBricks(i);
        if(SDL_HasIntersection(&ball, &brick) && bricks[i]) {
            bricks[i]=0;
            if(ball.x >= brick.x) {velX=-velX; ball.x-=20;}
            if(ball.x <= brick.x) {velX=-velX; ball.x+=20;}
            if(ball.y <= brick.y) {velY=-velY; ball.y-=20;}
            if(ball.y >= brick.y) {velY=-velY; ball.y+=20;}
        }
        if(bricks[i]) reset=0;
    }
}

void input() {
    SDL_Event e;

    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT) {
            game_over = true;
            game_won = false;
        }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_SPACE) {
                resetBricks();
                game_over = false;
                game_won = false;
            }
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                game_over = true;
                game_won = false;
            }
        }
    }

    const Uint8 *keystates = SDL_GetKeyboardState(NULL);

    if(keystates[SDL_SCANCODE_LEFT]) paddle.x -= SPEED;
    if(keystates[SDL_SCANCODE_RIGHT]) paddle.x += SPEED;
}



void render() {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 255);
    SDL_RenderClear(renderer);

    frameCount++;
    timerFPS=SDL_GetTicks()-lastFrame;
    if(timerFPS<(1000/60)) {
        SDL_Delay((1000/60)-timerFPS);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddle);
    SDL_RenderFillRect(renderer, &ball);
    write(std::to_string(liveCount), WIDTH/2+FONT_SIZE/2, FONT_SIZE*1.5);

    for(int i=0; i<COL*ROW; i++) {
        SDL_SetRenderDrawColor(renderer, 180, 210, 255, 255);
        if(i%2==0)SDL_SetRenderDrawColor(renderer, 255,192, 203, 255);
        if(bricks[i]) {
            setBricks(i);
            SDL_RenderFillRect(renderer, &brick);
        }
    }
}


int main(int argc, char *argv[])
{

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Failed at SDL_Init()" << std::endl;
        return -1;
    }


    window = initSDL(WIDTH, HEIGHT, WINDOW_TITLE);
    renderer = createRenderer(window);


    SDL_Texture* background = loadTexture("pictures/sky 1.jpg", renderer);
    SDL_RenderCopy( renderer, background, NULL, NULL);


    if (TTF_Init() < 0) {
        std::cout << "Failed at TTF_Init()" << std::endl;
        return -1;
    }
    font = TTF_OpenFont("font.ttf", FONT_SIZE);
    if (font == nullptr) {
        std::cout << "Failed to load font" << std::endl;
        return -1;
    }


    paddle.h = 6;
    paddle.w = WIDTH / 4;
    paddle.y = HEIGHT - paddle.h - 8;
    ball.w = ball.h = SIZE;
    brick.w = (WIDTH - (SPACING * COL)) / COL;
    brick.h = 20;

    resetBricks();




while (!game_over && !game_won) {
    lastFrame = SDL_GetTicks();

    input();


    SDL_RenderClear(renderer);


    renderTexture(background, 0, 0, renderer);


    update();


    SDL_SetRenderDrawColor(renderer, 0, 0, 0 , 255);
    SDL_RenderFillRect(renderer, &paddle);
    SDL_RenderFillRect(renderer, &ball);
    write(std::to_string(liveCount), WIDTH/2+FONT_SIZE/2, FONT_SIZE*1.5);

    for(int i=0; i<COL*ROW; i++) {
        SDL_SetRenderDrawColor(renderer, 180, 210, 255, 255);
        if(i%2==0)SDL_SetRenderDrawColor(renderer, 255,192, 203, 255);
        if(bricks[i]) {
            setBricks(i);
            SDL_RenderFillRect(renderer, &brick);
        }
    }

    auto renderGameOver = [&]() {
        SDL_Surface *surface = TTF_RenderText_Solid(font, MESSAGE_GAME_OVER.c_str(), color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect gameOverRect = { (WIDTH - surface->w) / 2, (HEIGHT - surface->h) / 2, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, NULL, &gameOverRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    };

    auto renderYouWin = [&]() {
        SDL_Surface *surface = TTF_RenderText_Solid(font, MESSAGE_YOU_WIN.c_str(), color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect youWinRect = { (WIDTH - surface->w) / 2, (HEIGHT - surface->h) / 2, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, NULL, &youWinRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    };

    auto renderTryAgain = [&](){
        SDL_Surface *surface = TTF_RenderText_Solid(font, MESSAGE_TRY_AGAIN.c_str(), color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect tryAgainRect = { (WIDTH - surface->w) / 1.5, (HEIGHT - surface->h) / 1.5, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, NULL, &tryAgainRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    };

    if (liveCount <= 0) {
        renderGameOver();
        renderTryAgain();
        game_over = true;
    } else {
        bool allDestroyed = true;
        for (int i = 0; i < COL * ROW; i++) {
            if (bricks[i]) {
                allDestroyed = false;
                break;
            }
        }
        if (allDestroyed) {
            renderYouWin();
            renderTryAgain();
            game_won = true;
        }
    }


    SDL_RenderPresent(renderer);


    timerFPS = SDL_GetTicks() - lastFrame;
    if (timerFPS < (1000 / 60)) {
        SDL_Delay((1000 / 60) - timerFPS);
    }
}

SDL_DestroyTexture(background);
TTF_CloseFont(font);

quitSDL(window, renderer);
TTF_Quit();
IMG_Quit();

return 0;
}
