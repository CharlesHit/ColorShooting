#include "text.h"

Text::Text(SDL_Renderer *renderer,
	const std::string &font_path, 
	int font_size, 
	const std::string &text, 
	const SDL_Color &color) 
{
	_text_texture = loadFont(renderer, font_path, font_size, text, color);
	SDL_QueryTexture(_text_texture, nullptr, nullptr, &_text_rect.w, &_text_rect.h);
}

void Text::display(int x, int y, SDL_Renderer *renderer) const{
	_text_rect.x = x;
	_text_rect.y = y;
	SDL_RenderCopy(renderer, _text_texture, nullptr, &_text_rect);
}

SDL_Texture * Text::loadFont(SDL_Renderer *renderer, const std::string &font_path, int font_size, const std::string &text, const SDL_Color &color) {
	
	TTF_Font *font = TTF_OpenFont(font_path.c_str(), font_size);
	if (!font) {
		std::cerr << "In text:cpp:23: loadFont: Failed to load font! " << font_path.c_str() << "\n";

		exit(1);
	}
	
	auto text_surface = TTF_RenderText_Solid(font, text.c_str(), color);
	
	if (!text_surface) {
		std::cerr << "Failed to create text surface!\n";
	}
	
	SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	
	if(!text_texture) {
		std::cerr << "Failed to create text surface!\n";
	}
	
	SDL_FreeSurface(text_surface);
	TTF_CloseFont(font);
	return text_texture;
}
