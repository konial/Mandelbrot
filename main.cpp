#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;
using namespace sf;

const int WINDOW_HEIGHT = 860, WINDOW_WIDTH = 860, DX = 10, DY = 40;
const float MinX_ = -1.5, MinY_ = -1.5, MaxX_ = 1.5, MaxY_ = 1.5;
#define float long double

vector<Color> col;
VertexArray line(LinesStrip, 5);

struct comp
{
    public:
    float re, im;

    comp(float x, float y) {re = x; im = y;}
    comp() {re = im = 0;}

    comp operator+(comp x)
    {
        return (comp(re + x.re, im + x.im));
    }
    comp operator*(comp x)
    {
        return (comp(x.re*re - x.im*im, x.re*im + x.im*re));
    }
};

float abs(comp z)
{
    return(sqrt(z.re*z.re + z.im*z.im));
}

void colorSet()
{
    int R, G, B;
    B = 0;
    while(B < 255)
    {
        col.push_back(Color(0, 0, B)); // BLUE
        B += 15;
    }
    G = 0, B = 255;
    while(G < 255)
    {
        col.push_back(Color(0, G, B)); // grad
        G += 30;
    }
    G = 255, B = 255;
    while(G > 125 || B > 125)
    {
        col.push_back(Color(0, G, B)); //BRIGHT BLUE
        G -= 15;
        B -= 15;
    }
    R = 0, B = 125;
    while(R < 255 || B > 0)
    {
        col.push_back(Color(R, G, B)); //Grad
        R += 30;
        B -= 15;
    }
    R = 255, G = 125;
    while(G < 255) {
            col.push_back(Color(R, G, 0)); //GREEN
            G += 15;
    }
    R = 255, G = 255;
    while(R > 125 || G > 125)
    {
        col.push_back(Color(R, G, 0)); // YELLOW
        R -= 15;
        G -= 15;
    }
    R = 125, G = 125, B = 0;
    while(B < 255)
    {
        col.push_back(Color(R, G, B)); // Grad
        B += 30;
        G -= 15;
    }
    R = 125, B = 255;
    while(R < 255) {
        col.push_back(Color(R, 0, B)); //RED
        R += 15;
    }
    R = 255, B = 255;
    while(R > 125 || B > 125) {
            col.push_back(Color(R, 0, B)); //PINK
            R -= 15;
            B -= 15;
    }
    G = 0;
    while(G < 255) {
            col.push_back(Color(R, G, B)); //grad
            R += 15;
            B += 15;
            G += 30;
    }
    R = 255, G = 255, B = 255;
    while(R > 125 || G > 125 || B > 125)
    {
        col.push_back(Color(R, G, B)); //WHITE
        R -= 15;
        G -= 15;
        B -= 15;
    }
    cout << col.size() << "\n";
}

//comp c(0.285, 0.01);

int build(float RE, float IM)
{
    comp z;
    comp c(RE, IM);
    int i;
    for(i = 0; i < col.size(); ++i) {
        if(abs(z) > 2)
            break;
        z = z * z + c;
    }
    return i;
}

int main()
{
    RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "MANDELBROT", Style::Default);
    window.setPosition(Vector2i(0, 0));

    colorSet();

    Image image;
    Texture texture;
    Sprite sprite;

    bool cnt = false, can = true, ram = false;
    float MinX = MinX_, MinY = MinY_, MaxX = MaxX_, MaxY = MaxY_;
    float g, h;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            Vector2i r = Mouse::getPosition();
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
                window.close();
            if (event.type == Event::MouseButtonPressed && !cnt) {
                g = MinX;
                h = MinY;
                MinX += (MaxX - MinX) * ((float)event.mouseButton.x / (float)WINDOW_WIDTH);
                MinY += (MaxY - MinY) * ((float)event.mouseButton.y / (float)WINDOW_HEIGHT);
                line[0].position = Vector2f(event.mouseButton.x, event.mouseButton.y);
                line[0].color = Color(123, 123, 123);
                cnt = true;
                can = false;
            }
            else if (!Mouse::isButtonPressed(Mouse::Left) && cnt) {
                ram = true;
                line[1].position = Vector2f(r.x - DX, line[0].position.y);
                line[1].color = Color(123, 123, 123);
                line[2].position = Vector2f(r.x - DX, r.y - DY);
                line[2].color = Color(123, 123, 123);
                line[3].position = Vector2f(line[0].position.x, r.y - DY);
                line[3].color = Color(123, 123, 123);
                line[4].position = Vector2f(line[0].position.x, line[0].position.y);
                line[4].color = Color(123, 123, 123);
            }
            else if (event.type == Event::MouseButtonPressed && cnt) {
                MaxX -= (MaxX - g) * ((float)(WINDOW_WIDTH - event.mouseButton.x) / (float)WINDOW_WIDTH);
                MaxY -= (MaxY - h) * ((float)(WINDOW_HEIGHT - event.mouseButton.y) / (float)WINDOW_HEIGHT);
                cnt = false;
                can = true;
                ram = false;
            }

        }


        if(ram) {
            window.clear();
            window.draw(sprite);
            window.draw(line);
        }

        if(can) {
            image.create(WINDOW_WIDTH, WINDOW_HEIGHT);
            int x, y;

            float dx = (MaxX-MinX)/WINDOW_WIDTH;
            float dy = (MaxY-MinY)/WINDOW_HEIGHT;
            for(y = 0; y < WINDOW_HEIGHT; ++y) {
                for(x = 0; x < WINDOW_WIDTH; ++x) {
                    float RE = MinX + x * dx;
                    float IM = MinY + y * dy;
                    int numb = build(RE, IM);
                    image.setPixel(x, y, col[numb]);
                }
            }
            image.saveToFile("Pictures.jpg");
            texture.loadFromImage(image);
            sprite.setTexture(texture, true);
            sprite.setPosition(0, 0);
            window.clear();
            window.draw(sprite);
        }

        window.display();
    }

    return 0;
}
