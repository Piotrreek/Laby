#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "font.h"
#include <cmath>


void convert_hsl(float hsl[3], float rgb[3]) {
    float C = (1.0f - fabsf(2.0f * hsl[2] - 1.0f)) * hsl[1];
    float X = C * (1.0f - fabsf(fmodf(hsl[0] * 0.016666f, 2.0f) - 1.0f));
    float m = hsl[2] - C / 2.0f;
    if (hsl[0] < 60) {
        rgb[0] = (C + m) * 255; rgb[1] = (X + m) * 255; rgb[2] = (0 + m) * 255;
    }
    else if (hsl[0] < 120) {
        rgb[0] = (X + m) * 255; rgb[1] = (C + m) * 255; rgb[2] = (0 + m) * 255;
    }
    else if (hsl[0] < 180) {
        rgb[0] = (0 + m) * 255; rgb[1] = (C + m) * 255; rgb[2] = (X + m) * 255;
    }
    else if (hsl[0] < 240) {
        rgb[0] = (0 + m) * 255; rgb[1] = (X + m) * 255; rgb[2] = (C + m) * 255;
    }
    else if (hsl[0] < 300) {
        rgb[0] = (X + m) * 255;  rgb[1] = (0 + m) * 255; rgb[2] = (C + m) * 255;
    }
    else {
        rgb[0] = (C + m) * 255; rgb[1] = (0 + m) * 255; rgb[2] = (X + m) * 255;
    }
}
void convert_hsv(float hsv[3], float rgb[3]) {
    float C = hsv[2] * hsv[1];
    float X = C * (1.0f - fabsf(fmodf(hsv[0] * 0.016666f, 2.0f) - 1.0f));
    float m = hsv[2] - C;
    if (hsv[0] < 60) {
        rgb[0] = (C + m) * 255; rgb[1] = (X + m) * 255; rgb[2] = (0 + m) * 255;
    }
    else if (hsv[0] < 120) {
        rgb[0] = (X + m) * 255; rgb[1] = (C + m) * 255; rgb[2] = (0 + m) * 255;
    }
    else if (hsv[0] < 180) {
        rgb[0] = (0 + m) * 255; rgb[1] = (C + m) * 255; rgb[2] = (X + m) * 255;
    }
    else if (hsv[0] < 240) {
        rgb[0] = (0 + m) * 255; rgb[1] = (X + m) * 255; rgb[2] = (C + m) * 255;
    }
    else if (hsv[0] < 300) {
        rgb[0] = (X + m) * 255; rgb[1] = (0 + m) * 255; rgb[2] = (C + m) * 255;
    }
    else {
        rgb[0] = (C + m) * 255; rgb[1] = (0 + m) * 255; rgb[2] = (X + m) * 255;
    }
}
void convert_rgb(float grb[3], float rgb[3]) {
    rgb[0] = grb[1] * 255;
    rgb[1] = grb[0] * 0.708333f;
    rgb[2] = grb[2] * 255;
}
void convert_cmy(float cmy[3], float rgb[3]) {
    rgb[0] = (1 - cmy[1]) * 255;
    rgb[1] = (1 - cmy[0] * 0.002777f) * 255;
    rgb[2] = (1 - cmy[2]) * 255;
}

class Circle : public sf::CircleShape
{
private:
    int _size;
    int _choice;
    float _input_value;
    sf::CircleShape* _circle;
    sf::Texture* _texture;
    sf::Uint8* _pixels;
public:
    Circle(float size, int choice, float x, float y) : _size(2 * static_cast<int>(size)), _choice(choice),
        _input_value(0.5), _circle(new sf::CircleShape(size)),
        _texture(new sf::Texture)
    {
        _pixels = new sf::Uint8[4 * _size * _size];
        _texture->create(_size, _size);
        _circle->setPosition(sf::Vector2f(x, y));
        set_texture();
    }
    void SetInputValue(float val)
    {
        _input_value = val;
        set_texture();
    }
    void set_texture()
    {
        float r, a, tmp[3], rgb[3];
        for (int x = -125; x < 125; x++)
        {
            for (int y = -125; y < 125; y++)
            {
                r = sqrtf(static_cast<float>(x) * x + y * y);
                a = fmodf(atan2f(static_cast<float>(y), x) * 57.29577951f + 180, 360);
                tmp[0] = a; tmp[1] = r * 0.008; tmp[2] = _input_value;
                if (r < 125) 
                {
                    switch (_choice) {
                    case 1: convert_hsl(tmp, rgb); break;
                    case 2: convert_hsv(tmp, rgb); break;
                    case 3: convert_cmy(tmp, rgb); break;
                    case 4: convert_rgb(tmp, rgb); break;
                    }
                    _pixels[4 * ((125 + y) * _size + 125 - x) + 0] = static_cast<sf::Uint8>(rgb[0]);
                    _pixels[4 * ((125 + y) * _size + 125 - x) + 1] = static_cast<sf::Uint8>(rgb[1]);
                    _pixels[4 * ((125 + y) * _size + 125 - x) + 2] = static_cast<sf::Uint8>(rgb[2]);
                    _pixels[4 * ((125 + y) * _size + 125 - x) + 3] = 255;
				}
            }
        }
        _texture->update(_pixels);
        _circle->setTexture(_texture);
    }
    void SetPosition(float x, float y)
    {
        _circle->setPosition(x, y);
    }
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
        target.draw(*_circle);
    }

    ~Circle() override
    {
        delete[] _pixels;
        delete _texture;
        delete _circle;
    }
};
class Text : public sf::Text
{
private:
    sf::Text* _text;
    sf::Font _font;
public:
    Text(const float x, const float y, const std::string& text)
    {
        _font.loadFromMemory(font_data, font_data_size);
        _text = new sf::Text();
        _text->setFont(_font);
        _text->setString(text);
        _text->setPosition(x, y);
        _text->setFillColor(sf::Color::Black);
        _text->setCharacterSize(12);
    }
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(*_text);
    }
    ~Text() override
    {
        delete _text;
    }
};


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 650), "GFK Lab 02", sf::Style::Titlebar | sf::Style::Close);

    sf::Clock clock;
    sf::Time time = sf::Time::Zero;
    unsigned int FPS = 0, frame_counter = 0;
    // tablica Uint8 do wyrsowania suwaka
    sf::Uint8* suwak = new sf::Uint8[40 * 255 * 4];
    for (int x = 0; x < 40; x++)
    {
        for (int y = 0; y < 255; y++)
        {
            suwak[4 * (y * 40 + x) + 0] = suwak[4 * (y * 40 + x) + 1] = suwak[4 * (y * 40 + x) + 2] = y;
            suwak[4 * (y * 40 + x) + 3] = 255;
        }
    }
    sf::Texture* suwak_texture = new sf::Texture;
    suwak_texture->create(40, 255);
    suwak_texture->update(suwak);
    sf::RectangleShape suwak_prostokat;
    suwak_prostokat.setTexture(suwak_texture);
    suwak_prostokat.setOutlineColor(sf::Color::Black);
    suwak_prostokat.setOutlineThickness(1);
    suwak_prostokat.setPosition(700, 60);
    suwak_prostokat.setSize(sf::Vector2f(40., 255.));

    sf::VertexArray line(sf::LinesStrip, 2);
    line[0].color = sf::Color::Blue; line[1].color = sf::Color::Blue;
    line[0].position = sf::Vector2f(690, 182.5); line[1].position = sf::Vector2f(750, 182.5);
    bool mouse_pressed = false;

    Text hsl_text(20, 20, "HSL");
    Text hsv_text(340, 20, "HSV");
    Text cmy_text(30, 350, "CMY");
    Text rgb_text(340, 350, "RGB");
    Circle hslCircle(125, 1, 20, 30);
    Circle hsvCircle(125, 2, 340, 30);
    Circle cmyCircle(125, 3, 30, 360);
    Circle rgbCircle(125, 4, 340, 360);
    int val = 127;

    //inicjalizacja 
    clock.restart().asMilliseconds();
    while (window.isOpen())
    {
        sf::Event event;
        window.clear(sf::Color::White);

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && event.mouseButton.x >= 700
                && event.mouseButton.x <= 740 && event.mouseButton.y >= 60 && event.mouseButton.y <= 315)
            {
                mouse_pressed = true;
                val = event.mouseButton.y - 60;
                line[0].position = sf::Vector2f(690, event.mouseButton.y);
                line[1].position = sf::Vector2f(750, event.mouseButton.y);
                hslCircle.SetInputValue(static_cast<float>(val) / 255);
                hsvCircle.SetInputValue(static_cast<float>(val) / 255);
                rgbCircle.SetInputValue(static_cast<float>(val) / 255);
                cmyCircle.SetInputValue(static_cast<float>(val) / 255);
            }
            if (event.type == sf::Event::MouseMoved && mouse_pressed == true && event.mouseMove.x >= 700
                && event.mouseMove.x <= 740 && event.mouseMove.y >= 60 && event.mouseMove.y <= 315)
            {
                val = event.mouseMove.y - 60;
                hslCircle.SetInputValue(static_cast<float>(val) / 255);
                hsvCircle.SetInputValue(static_cast<float>(val) / 255);
                rgbCircle.SetInputValue(static_cast<float>(val) / 255);
                cmyCircle.SetInputValue(static_cast<float>(val) / 255);
                line[0].position = sf::Vector2f(690, event.mouseMove.y);
                line[1].position = sf::Vector2f(750, event.mouseMove.y);
            }
            if (event.type == sf::Event::MouseButtonReleased) { mouse_pressed = false; }
        }
        Text fps(700, 325, "FPS: " + std::to_string(FPS));
        Text hsl(220, 260, "L=" + std::to_string(static_cast<float>(val) / 255).substr(0,4));
        Text hsv(550, 260, "Y=" + std::to_string(static_cast<float>(val) / 255).substr(0,4));
        Text cmy(220, 610, "Y=" + std::to_string(static_cast<float>(val) / 255 * 100).substr(0,4) + "%");
        Text rgb(550, 610, "B=" + std::to_string(val));
        
        //tu wyrysować wszystko na ekran
        window.draw(suwak_prostokat);
        window.draw(line);
        window.draw(rgb_text);
        window.draw(hsv_text);
        window.draw(cmy_text);
        window.draw(hsl_text);
        window.draw(rgbCircle);
        window.draw(hslCircle);
        window.draw(hsvCircle);
        window.draw(cmyCircle);
        window.draw(fps);
        window.draw(hsl);
        window.draw(hsv);
        window.draw(cmy);
        window.draw(rgb);

        //tu wypisać na ekran wartość FPS
        if (clock.getElapsedTime().asSeconds() >= 1.0f)
        {
            FPS = (unsigned int)((float)frame_counter / clock.getElapsedTime().asSeconds());
            Text fps(700, 325, "FPS: " + std::to_string(FPS));
            clock.restart();
            frame_counter = 0;
        }
        frame_counter++;
        window.display();
    }
    return 0;
}
