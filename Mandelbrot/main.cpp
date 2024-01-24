#include <SDL.h>
#include <Windows.h>
#include <string>

int WIDTH = 1080;
int HEIGHT = 1080;

long double x_min = -2.5;
long double x_max = 1.5;
long double y_min = -2.0;
long double y_max = 2.0;
long double factor = 0.1;
int max_it = 200;

long double map(long double value, long double in_min, long double in_max, long double out_min, long double out_max) {
	return ( value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int iteration(long double x, long double y) {

	int it = 1;

	long double z_r = x;
	long double z_i = y;
	long double thisz_r = 0;

	while ((static_cast<double>(z_r * z_r) + static_cast<double>(z_i * z_i)) < 4 && it < max_it) {
		thisz_r = z_r;
		z_r = static_cast<double>(z_r * z_r) - static_cast<double>(z_i * z_i) + x;
		z_i = static_cast<double>(2 * thisz_r * z_i) + y;
		it += 1;
	}

	return it;
}

int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	//SDL_CreateWindowAndRenderer(1440, 1080, 0, &window, &renderer);
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
	SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

	bool zoom = false;
	int count = 0;
	long double temp_range = x_max - x_min;

	long double zoom_factor = factor;

	while (true) {
		

		if (zoom) {
			zoom_factor = temp_range * factor;
			x_max -= zoom_factor;
			x_min += zoom_factor;
			y_max -= zoom_factor;
			y_min += zoom_factor;
			max_it += 5;
			temp_range = x_max - x_min;
		}
		
		

		if (GetKeyState(VK_OEM_PLUS) & 0x8000) {
			zoom_factor = temp_range * factor;
			x_max -= zoom_factor;
			x_min += zoom_factor;
			y_max -= zoom_factor;
			y_min += zoom_factor;
			max_it += 5;
			temp_range = x_max - x_min;
		}

		if (GetKeyState(VK_OEM_MINUS) & 0x8000) {
			zoom_factor = (temp_range * factor / (1 - factor));
			x_max += zoom_factor;
			x_min -= zoom_factor;
			y_max += zoom_factor;
			y_min -= zoom_factor;
			max_it -= 5;
			temp_range = x_max - x_min;
		}

		if ((GetKeyState('W') & 0x8000) || (GetKeyState(VK_UP) & 0x8000)) {
			y_max -= 0.1 * temp_range;
			y_min -= 0.1 * temp_range;
		}
		if ((GetKeyState('S') & 0x8000) || (GetKeyState(VK_DOWN) & 0x8000)) {
			y_max += 0.1 * temp_range;
			y_min += 0.1 * temp_range;
		}
		if ((GetKeyState('A') & 0x8000) || (GetKeyState(VK_LEFT) & 0x8000)) {
			x_max -= 0.1 * temp_range;
			x_min -= 0.1 * temp_range;
		}
		if ((GetKeyState('D') & 0x8000) || (GetKeyState(VK_RIGHT) & 0x8000)) {
			x_max += 0.1 * temp_range;
			x_min += 0.1 * temp_range;
		}
		

		SDL_RenderPresent(renderer);

		for (int x = 0; x < WIDTH; x++) {
			
			if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
				return 0;
			}
			if (GetKeyState('Q') & 0x8000) {
				return 0;
			}
			
			for (int y = 0; y < HEIGHT; y++) {

				long double a = map(x, 0, WIDTH, x_min, x_max);
				long double b = map(y, 0, HEIGHT, y_min, y_max);
				

				int bright = map(iteration(a,b), 0, max_it, 0, 255);

				//if (bright == 255 || bright < 20) {
				if (bright == 255 ) {
					//bright = 0;
				}

				int red = map(bright, 0, 255, 0, 255);
				int green = map(bright, 0, 255, 0, 64);
				int blue = map(bright, 0, 255, 0, 255);

				/*
				if (bright < 128) { //orange
					red = map(sqrt(bright), 0, sqrt(255), 0, 255);
					green = bright;
					blue = map(bright * bright, 0, 255 * 255, 0, 255);
				}
				else { //purple
					red = map(bright, 0, 255, 0, 255);
					green = map(bright, 0, 255, 0, 64);
					blue = map(bright, 0, 255, 0, 255);
				}
				*/

				SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
				SDL_RenderDrawPoint(renderer, x, y);

			}
		}

		if (GetKeyState('E') & 0x8000) {
		SDL_Surface* sshot = SDL_GetWindowSurface(window);
		SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
		std::string file = "mandelbrotzoom" + std::to_string(count) + ".bpm";
		SDL_SaveBMP(sshot, file.c_str());
		SDL_FreeSurface(sshot);
		}
		count++;

	}

	return 0;
}