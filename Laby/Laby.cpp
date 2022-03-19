#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


void convertHSL(float hsl[3], float rgb[3]) {
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
void convertHSV(float hsv[3], float rgb[3]) {
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
void convertRGB(float grb[3], float rgb[3]) {
    rgb[0] = grb[1] * 255;
    rgb[1] = grb[0] * 0.708333f;
    rgb[2] = grb[2] * 255;
}
void convertCMY(float cmy[3], float rgb[3]) {
    rgb[0] = (1 - cmy[1]) * 255;
    rgb[1] = (1 - cmy[0] * 0.002777f) * 255;
    rgb[2] = (1 - cmy[2]) * 255;
}



int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 650), "GFK Lab 02", sf::Style::Titlebar | sf::Style::Close);
    sf::Clock clock;
    sf::Time time = sf::Time::Zero;
    unsigned int FPS = 0, frame_counter = 0;
    // tablica Uint8 do wyrsowania suwaka
    sf::Uint8* suwak = new sf::Uint8[50 * 255 * 4];
    for(int x = 0; x < 50; x++)
    {
	    for(int y = 0; y < 255; y++)
	    {
            suwak[4 * (y * 50 + x) + 0] = suwak[4 * (y * 50 + x) + 1] = suwak[4 * (y * 50 + x) + 2] = y;
            suwak[4 * (y * 50 + x) + 3] = 255;
	    }
    }
    sf::Texture *suwak_texture = new sf::Texture;
    suwak_texture->create(50, 255);
    suwak_texture->update(suwak);
    sf::RectangleShape suwak_prostokat;
    suwak_prostokat.setTexture(suwak_texture);
    suwak_prostokat.setOutlineColor(sf::Color::Black);
    suwak_prostokat.setOutlineThickness(1);
    suwak_prostokat.setPosition(700, 50);
    suwak_prostokat.setSize(sf::Vector2f(50.,255.));

    sf::VertexArray line(sf::LinesStrip, 2);
    line[0].color = sf::Color::Blue; line[1].color = sf::Color::Blue;
    line[0].position = sf::Vector2f(690, 172.5); line[1].position = sf::Vector2f(760, 172.5);
    bool mouse_pressed = false;

    //inicjalizacja 
    clock.restart().asMilliseconds();
    while (window.isOpen())
    {
        sf::Event event;
        window.clear(sf::Color::White);

        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed) window.close();
            if(event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) window.close();
            }
            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && event.mouseButton.x >= 700
                && event.mouseButton.x <= 750 && event.mouseButton.y >= 50 && event.mouseButton.y <= 305)
            {
	            mouse_pressed = true;
                line[0].position = sf::Vector2f(690, event.mouseButton.y);
                line[1].position = sf::Vector2f(760, event.mouseButton.y);
            }
            if(event.type == sf::Event::MouseMoved && mouse_pressed == true && event.mouseMove.x >= 700 
                && event.mouseMove.x <= 750 && event.mouseMove.y >= 50 && event.mouseMove.y <= 305)
            {
                line[0].position = sf::Vector2f(690, event.mouseMove.y);
                line[1].position = sf::Vector2f(760, event.mouseMove.y);
            }
            if (event.type == sf::Event::MouseButtonReleased) { mouse_pressed = false; }
        }
        //tu wyrysować wszystko na ekran
        window.draw(suwak_prostokat);
        window.draw(line);

        //tu wypisać na ekran wartość FPS
        if (clock.getElapsedTime().asSeconds() >= 1.0f)
        {
            FPS = (unsigned int)((float)frame_counter / clock.getElapsedTime().asSeconds());
            clock.restart();
            frame_counter = 0;
        }
        frame_counter++;

        window.display();
    }

    return 0;
}