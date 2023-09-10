#include <iostream>
#include <cmath>
#include <stdio.h>
#include <string>
#include <chrono>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
using namespace std;
using namespace chrono;

#define SIZE 16
#define SLIP 4

#define RESET 0
#define FAIL 10
#define SUCCESS 3

const int origtilesize = 32,
	cols = 32,
	rows = 18,
	scale = 1,
	fps = 60;
const int WIDTH = origtilesize*cols*scale,
	HEIGHT = origtilesize*rows*scale,
	tilesize = origtilesize*scale;

const float tsleep = 1000/fps;
float tdelta = 1/fps, rfps = fps;
int tick = 0;
char state = 's';

const string asciic[SIZE] = {
	"\033[0;0m", "\033[0;30m",
	"\033[0;31m", "\033[0;32m",
	"\033[0;33m", "\033[0;34m",
	"\033[0;35m", "\033[0;36m",
	"\033[0;37m", "\033[0;90m",
	"\033[0;91m", "\033[0;92m",
	"\033[0;93m", "\033[0;94m",
	"\033[0;95m", "\033[0;96m"
};

int main(int argc, char* argv[]) {
	high_resolution_clock::time_point tstart = high_resolution_clock::now();
	cout << "Hello, World!" << endl;
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << asciic[FAIL] << "SDL could not initialise!" << asciic[RESET];
		return EXIT_FAILURE;
	}
	
	if (TTF_Init() < 0) {
		cout << asciic[FAIL] << "TTF could not initialise!" << asciic[RESET];
		return EXIT_FAILURE;
	}
	
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		cout << asciic[FAIL] << "MIXER could not initialise!" << asciic[RESET];
		return EXIT_FAILURE;
	}
	
	SDL_Window *window = NULL;
	SDL_Surface *surface = NULL;
	
	window = SDL_CreateWindow("Larion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		cout << asciic[FAIL] << "Window could not be created!" << asciic[RESET];
		return EXIT_FAILURE;
	}
	
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	//SDL_ShowCursor(false);
	
	surface = SDL_GetWindowSurface(window);
	SDL_Event e;
	
	const SDL_Color sdlc[SLIP] = {
		{255, 255, 255}
	};
	const uint32_t sdlrgb[SLIP] = {
		SDL_MapRGB(surface->format, 255, 255, 255)
	};
	
	TTF_Font *bfont = TTF_OpenFont("C:\\doki\\doki\\bin\\VT323-Regular.ttf", 36),
		*sfont = TTF_OpenFont("C:\\doki\\doki\\bin\\VT323-Regular.ttf", 16);
	
	if (bfont == NULL) {
		cout << asciic[FAIL] << "Font file could not be opened!" << asciic[RESET];
		return EXIT_FAILURE;
	}
	
	bool going = true, debug = false;
	int mousex = NULL, mousey = NULL, ttick = NULL, qtick = 0, tmp;
	float px = 0, py = 0, cx = 0, cy = 0;
	
	SDL_Rect rect;
	
	const int bspeed = 5;
	const string btexts[SLIP] = {"Larion'a hoş geldin!",
		"Bu oyunun hikayesi, benim ve arkadaşım olan",
		"Keremle oynadığımız bir oyundan esinlenmiştir.",
		"İyi eğlenceler, Egemen Yalın."
	};
	
	const int brlength = 2;
	SDL_Surface *bruieds[brlength] = {NULL}, *text = NULL;
	
	Mix_Music *music = NULL;
	//Mix_Chunk *sclick = NULL;
	music = Mix_LoadMUS("C:\\doki\\doki\\src\\themesong.mp3");
	Mix_VolumeMusic(MIX_MAX_VOLUME/8);
	//sclick = Mix_LoadMUS("");
	if (music == NULL) {
		cout << asciic[FAIL] << "Audio file could not be opened!" << asciic[RESET];
		return EXIT_FAILURE;
	}
	
	while (going) {
		ttick = SDL_GetTicks();
		SDL_GetMouseState(&mousex, &mousey);
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
				if (qtick+fps > tick) {
					going = false;
				} else {
					qtick = tick;
				}
			} else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_QUOTEDBL:
					debug ^= true;
					break;
				}
			}
		}
		
		if (debug && tick%fps == 0) {
			bruieds[0] = TTF_RenderUTF8_Solid(sfont, (to_string(rfps)+" FPS").c_str(), sdlc[0]);
			bruieds[1] = TTF_RenderUTF8_Solid(sfont, (to_string(tick%9960)+" TICK ("+to_string(tick/fps)+"sec)").c_str(), sdlc[0]);
		}
		
		SDL_FillRect(surface, NULL, 0);
		//rect = {tilesize*3, tilesize*2, tilesize, tilesize};
		//SDL_FillRect(surface, &rect, sdlrgb[0]);
		if (state == 's') {
			if (tick > 1080) {
				state = 'm';
				Mix_PlayMusic(music, 1);
			}
			int bias = 0;
			for (int i = 0; i < SLIP; i++) {
				tmp = tick/bspeed-bias;
				if (tmp > 0) {
					text = TTF_RenderUTF8_Solid(bfont, btexts[i].substr(0, tmp).c_str(), sdlc[0]);
					if (text != NULL) {
						rect = {(WIDTH-text->w)/2, HEIGHT/2-text->h*(2-i)};
						SDL_BlitSurface(text, NULL, surface, &rect);
					}
				}
				bias += btexts[i].length();
			}
		}
		if (debug) {
			for (int i = 0; i < brlength; i++) {
				rect = {SLIP, SLIP+rows*i};
				SDL_BlitSurface(bruieds[i], NULL, surface, &rect);
			}
		}
		SDL_UpdateWindowSurface(window);
		SDL_Delay(tsleep);
		tdelta = (float)(SDL_GetTicks()-ttick)/1000;
		rfps = 1/tdelta;
		tick++;
	}
	
	for (int i = 0; i < brlength; i++) {
		SDL_FreeSurface(bruieds[i]);
		bruieds[i] = NULL;
	}
	Mix_FreeMusic(music);
	music = NULL;
	
	TTF_CloseFont(bfont);
	TTF_CloseFont(sfont);
	bfont = NULL;
	sfont = NULL;
	
	SDL_DestroyWindow(window);
	window = NULL;
	
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
	
	milliseconds durc = duration_cast<milliseconds>(high_resolution_clock::now()-tstart);
	cout << asciic[SUCCESS] << "\nProcess completed in " << durc.count() << "ms." << asciic[RESET];
	return EXIT_SUCCESS;
}