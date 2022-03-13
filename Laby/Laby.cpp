// Calosc mozna dowolnie edytowac wedle uznania. 

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "font.h"

class Menu : public sf::Drawable
{
private:
    sf::Font font;
    sf::Text* text;
    sf::RectangleShape* rectangle;

    sf::Texture* colors_texture;
    sf::Sprite* colors_sprite;
    sf::Uint8* colors_pixels;
    const unsigned int colors_size_x = 765;
    const unsigned int colors_size_y = 60;
    inline void draw_to_color_pixels(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
    {
        colors_pixels[4 * (y * colors_size_x + x) + 0] = r;
        colors_pixels[4 * (y * colors_size_x + x) + 1] = g;
        colors_pixels[4 * (y * colors_size_x + x) + 2] = b;
        colors_pixels[4 * (y * colors_size_x + x) + 3] = 255;
    }

public:
    Menu()
    {
        font.loadFromMemory(font_data, font_data_size);
        text = new sf::Text;
        text->setFont(font);
        text->setCharacterSize(12);
        text->setFillColor(sf::Color::White);

        rectangle = new sf::RectangleShape(sf::Vector2f(796, 536));
        rectangle->setFillColor(sf::Color::Transparent);
        rectangle->setOutlineColor(sf::Color::White);
        rectangle->setOutlineThickness(1.0f);
        rectangle->setPosition(2, 62);

        unsigned int x, y;
        colors_pixels = new sf::Uint8[colors_size_x * colors_size_y * 4];
        for (x = 0; x < 255; x++)
            for (y = 0; y < 30; y++)
            {
                draw_to_color_pixels(x, y, x, 255, 0);
                draw_to_color_pixels(x + 255, y, 255, 255 - x, 0);
                draw_to_color_pixels(x + 510, y, 255, 0, x);
                draw_to_color_pixels(254 - x, y + 30, 0, 255, 255 - x);
                draw_to_color_pixels(509 - x, y + 30, 0, x, 255);
                draw_to_color_pixels(764 - x, y + 30, 255 - x, 0, 255);
            }

        colors_texture = new sf::Texture();
        colors_texture->create(colors_size_x, colors_size_y);
        colors_texture->update(colors_pixels);

        colors_sprite = new sf::Sprite();
        colors_sprite->setTexture(*colors_texture);
        colors_sprite->setPosition(1, 1);
    }

    void outtextxy(sf::RenderTarget& target, float x, float y, const wchar_t* str) const
    {
        text->setPosition(x, y);
        text->setString(str);
        target.draw(*text);
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        outtextxy(target, 5, 600, L"f - wyb�r koloru rysowania");
        outtextxy(target, 5, 615, L"b - wyb�r koloru wype�niania");
        outtextxy(target, 5, 630, L"l - rysowanie linii");

        outtextxy(target, 200, 600, L"r - rysowanie prostok�ta");
        outtextxy(target, 200, 615, L"a - rysowanie wype�nionego prostok�ta");
        outtextxy(target, 200, 630, L"c - rysowanie okr�gu");

        outtextxy(target, 470, 600, L"w - zapis do pliku");
        outtextxy(target, 470, 615, L"o - odczyt z pliku");
        outtextxy(target, 470, 630, L"esc - wyj�cie");

        outtextxy(target, 650, 615, L"Aktualne:");

        target.draw(*rectangle);
        target.draw(*colors_sprite);
    }
};

std::string fromKtoS(const sf::Keyboard::Key& k) {
    std::string ret;
    switch (k) {

    case sf::Keyboard::A:

        ret = "a";
        break;
    case sf::Keyboard::B:

        ret = "b";
        break;
    case sf::Keyboard::C:

        ret = "c";
        break;
    case sf::Keyboard::D:

        ret = "d";
        break;
    case sf::Keyboard::E:

        ret = "e";
        break;
    case sf::Keyboard::F:

        ret = "f";
        break;
    case sf::Keyboard::G:

        ret = "g";
        break;
    case sf::Keyboard::H:

        ret = "h";
        break;
    case sf::Keyboard::I:

        ret = "i";
        break;
    case sf::Keyboard::J:

        ret = "j";
        break;
    case sf::Keyboard::K:

        ret = "k";
        break;
    case sf::Keyboard::L:

        ret = "l";
        break;
    case sf::Keyboard::M:

        ret = "m";
        break;
    case sf::Keyboard::N:

        ret = "n";
        break;
    case sf::Keyboard::O:

        ret = "o";
        break;
    case sf::Keyboard::P:

        ret = "p";
        break;
    case sf::Keyboard::Q:

        ret = "q";
        break;
    case sf::Keyboard::R:

        ret = "r";
        break;
    case sf::Keyboard::S:

        ret = "s";
        break;
    case sf::Keyboard::T:

        ret = "t";
        break;
    case sf::Keyboard::U:

        ret = "u";
        break;
    case sf::Keyboard::V:

        ret = "v";
        break;
    case sf::Keyboard::W:

        ret = "w";
        break;
    case sf::Keyboard::X:

        ret = "x";
        break;
    case sf::Keyboard::Y:

        ret = "y";
        break;
    case sf::Keyboard::Z:

        ret = "z";
    
    }
    return ret;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 650), "GFK Lab 01", sf::Style::Titlebar | sf::Style::Close);
    sf::Event event;
    Menu menu;

    sf::RectangleShape outline_color_rectangle;
    sf::RectangleShape background_color_rectangle;
    sf::VertexArray line(sf::LinesStrip, 2);
    sf::RectangleShape rectangle;
    sf::CircleShape circle;
    sf::Color background_color(0,0,0,0);
    sf::Color outline_color(0,0,0,0);

    sf::Font font;
    font.loadFromMemory(font_data, font_data_size);

    bool is_mouse_button_pressed = false;
    bool is_key_pressed = false;
    std::string sign_clicked;
    sf::Vector2f begin;
    sf::Vector2f move;
    sf::RenderTexture buffer;
    sf::Text text;
    sf::Texture read;
    if (!buffer.create(800, 650))
        return -1;

    window.setFramerateLimit(60);
    outline_color_rectangle.setPosition(767, 1);
    outline_color_rectangle.setSize(sf::Vector2f(32, 29));
    outline_color_rectangle.setOutlineColor(sf::Color::Transparent);
    outline_color_rectangle.setFillColor(outline_color);
    outline_color_rectangle.setOutlineThickness(0);
    background_color_rectangle.setPosition(767, 31);
    background_color_rectangle.setSize(sf::Vector2f(32, 29));
    background_color_rectangle.setOutlineColor(sf::Color::Transparent);
    background_color_rectangle.setOutlineThickness(0);
    background_color_rectangle.setFillColor(background_color);

    while (window.isOpen())
    {
        window.clear(sf::Color::Black);

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) window.close();
                else { is_key_pressed = true; sign_clicked = fromKtoS(event.key.code); }
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) { begin.x = event.mouseButton.x; begin.y = event.mouseButton.y; is_mouse_button_pressed = true; }
            if (event.type == sf::Event::MouseMoved) { move.x = event.mouseMove.x; move.y = event.mouseMove.y; }
            if (event.type == sf::Event::MouseButtonReleased) { is_mouse_button_pressed = false; buffer.draw(line); buffer.draw(rectangle); buffer.draw(circle); buffer.draw(circle); }
            
        }
        //Draw BEGIN
        buffer.display();

        // wybor koloru

        if(begin.x >=1 && begin.x < 766 && begin.y >= 1 && begin.y < 61)
        {
            char  tab = static_cast<int>(begin.x) / 255;
            sf::Uint8 x = static_cast<int>(begin.x) % 255;
            sf::Uint8 a = 255;
            sf::Uint8 rgb[3] = { x, 255, 0 };
            int pos[3] = { 0, 1  , 2 };
            if (tab == 1)
                for (int i = 0; i < 3; ++i)
                    rgb[i] = 255 - rgb[i];
            if (begin.y > 30)
                std::swap(pos[0], pos[2]);
            for (int i = 0; i < 3; ++i)
                pos[i] = (pos[i] - tab < 0 ? pos[i] - tab + 3 : pos[i] - tab) % 3;
            sf::Color color = sf::Color(rgb[pos[0]], rgb[pos[1]], rgb[pos[2]], a);
            if (sign_clicked == "f") {
                outline_color = color;
                outline_color_rectangle.setFillColor(outline_color);
            }
            else if (sign_clicked == "b") {
                background_color = color;
                background_color_rectangle.setFillColor(background_color);
            }
        }

        // linia

        if(sign_clicked == "l" && is_mouse_button_pressed == true)
        {
        	line[0].position = begin;     line[0].color = outline_color;
            line[1].position = move;      line[1].color = outline_color;
        }

        // prostokat wypelniony

        if(sign_clicked == "a" && is_mouse_button_pressed)
        {
            if(move.x < begin.x && move.y < begin.y)
            {
                rectangle.setPosition(move);
                rectangle.setSize(sf::Vector2f(abs(move.x - begin.x), abs(move.y - begin.y)));
            }
            else if(move.x < begin.x && move.y > begin.y)
            {
                rectangle.setPosition(sf::Vector2f(move.x, begin.y));
                rectangle.setSize(sf::Vector2f(abs(move.x - begin.x), abs(move.y - begin.y)));
            }
            else if (move.x > begin.x && move.y < begin.y)
            {
                rectangle.setPosition(sf::Vector2f(begin.x, move.y));
                rectangle.setSize(sf::Vector2f(abs(move.x - begin.x), abs(move.y - begin.y)));
            }
            else
            {
                rectangle.setPosition(begin);
                rectangle.setSize(sf::Vector2f(abs(move.x - begin.x), abs(move.y - begin.y)));
            }
            rectangle.setFillColor(background_color);
            rectangle.setOutlineColor(outline_color);
            rectangle.setOutlineThickness(1.0f);
        }

        // prostokat niewypelniony

        if (sign_clicked == "r" && is_mouse_button_pressed)
        {
            if (move.x < begin.x && move.y < begin.y)
            {
                rectangle.setPosition(move);
                rectangle.setSize(sf::Vector2f(abs(move.x - begin.x), abs(move.y - begin.y)));
            }
            else if (move.x < begin.x && move.y > begin.y)
            {
                rectangle.setPosition(sf::Vector2f(move.x, begin.y));
                rectangle.setSize(sf::Vector2f(abs(move.x - begin.x), abs(move.y - begin.y)));
            }
            else if (move.x > begin.x && move.y < begin.y)
            {
                rectangle.setPosition(sf::Vector2f(begin.x, move.y));
                rectangle.setSize(sf::Vector2f(abs(move.x - begin.x), abs(move.y - begin.y)));
            }
            else
            {
                rectangle.setPosition(begin);
                rectangle.setSize(sf::Vector2f(abs(move.x - begin.x), abs(move.y - begin.y)));
            }
            rectangle.setFillColor(sf::Color::Transparent);
            rectangle.setOutlineColor(outline_color);
            rectangle.setOutlineThickness(1.0f);
        }

        // okrag

        if(sign_clicked == "c" && is_mouse_button_pressed)
        {
            float radius = sqrt(pow(move.x - begin.x, 2) + pow(move.y - begin.y, 2)) / 2;
            circle.setRadius(radius);
            circle.setOutlineColor(outline_color);
            circle.setOutlineThickness(1.0f);
            circle.setFillColor(sf::Color::Transparent);
            circle.setPosition(begin.x + (move.x - begin.x) / 2 - radius, begin.y + (move.y - begin.y) / 2 - radius);
            circle.setRotation(atan2f(move.x - begin.x, move.y - begin.y));
        }

        // zapis

        if(sign_clicked == "w")
        {
            buffer.getTexture().copyToImage().saveToFile("Lab01.png");
        }

        // odczyt

        if(sign_clicked == "o")
        {
            buffer.clear(sf::Color::Black);
            read.loadFromFile("Lab01.png");
            buffer.draw(sf::Sprite(read));
        }

        // tekst danej literki
        if(is_key_pressed == true)
        {
            text.setFont(font);
            text.setCharacterSize(12);
            text.setFillColor(sf::Color::White);
            text.setPosition(730, 615);
            text.setString(sign_clicked);
        }

        sf::Sprite sprite(buffer.getTexture());
        window.draw(sprite);
        window.draw(background_color_rectangle);
        window.draw(outline_color_rectangle);
        window.draw(text);
        window.draw(rectangle);
        window.draw(circle);
        window.draw(line);
        window.draw(menu);
        window.display();

        //Draw END
    }
    return 0;
}