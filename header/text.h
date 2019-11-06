#ifndef TEAM16_TEXT_H
#define TEAM16_TEXT_H

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <string>
#include <iostream>


class Text {
	public:
		Text(SDL_Renderer *renderer,const std::string &font_path, int font_size, const std::string &text, const SDL_Color &color);
		void display(int x, int y, SDL_Renderer *renderer) const;
		static SDL_Texture *loadFont(SDL_Renderer *renderer, const std::string &font_path, int font_size, const std::string &message_text, const SDL_Color &color);
	private:
		SDL_Texture *_text_texture = nullptr;
		mutable SDL_Rect _text_rect;
};
#endif //TEAM16_TEXT_H

