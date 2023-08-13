#include <SFML/Graphics.hpp>
#include <GLUT/glut.h> // Обязательно
#include <cmath>
#include "specialmath.hpp"

using namespace spmath;

vec2 fromsfveci(sf::Vector2i v){
    return {(float)v.x, (float)v.y};
}

float deltaX(float angle, float speed){
    return sin(angle) * speed;
}

float deltaY(float angle, float speed){
    return cos(angle) * speed;
}

float deltaT(float angle, float speed){
    return tan(angle) * speed;
}

vec3 towardVec(float rotx, float speed){
    return {deltaX(rad(rotx), -speed), 0, deltaY(rad(rotx), -speed)};
}

vec3 rightVec(float rotx, float speed){
    return {deltaX(rad(rotx+270), -speed), 0, deltaY(rad(rotx+270), -speed)};
}

vec3 upVec(float speed){
    return {0, speed, 0};
}

class Camera{
    public:
    vec2 mpos;
    vec3 pos, rot, dir;
    float speed=1,
          tAspect=0.5;
    bool controll = true;

    Camera(vec3 pos_, float speed_=1){
        pos=pos_;
        speed=speed_;
    }

    void update(sf::RenderWindow &window){
        if(window.hasFocus()){
            mpos = fromsfveci(sf::Mouse::getPosition(window));
            mpos -= vec2(window.getSize().x/2, window.getSize().y/2);
            rot.x -= mpos.x;
            rot.y -= mpos.y;
            //Что бы нельзя было делать сальто камерой
            if(rot.y<91){
                rot.y=91;
            } else if(rot.y>260){
                rot.y=260;
            }

            if(controll){
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
                    pos+=vec3(deltaX(rad(rot.x), -speed), 0, deltaY(rad(rot.x), -speed));
                } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
                    pos-=vec3(deltaX(rad(rot.x), -speed), 0, deltaY(rad(rot.x), -speed));
                }

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
                    pos-=vec3(deltaX(rad(rot.x+270), -speed), 0, deltaY(rad(rot.x+270), -speed));
                } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
                    pos+=vec3(deltaX(rad(rot.x+270), -speed), 0, deltaY(rad(rot.x+270), -speed));
                }

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)){
                    pos+=vec3(0, speed, 0);
                } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)){
                    pos-=vec3(0, speed, 0);
                }
            }
            
            sf::Mouse::setPosition(sf::Vector2i(window.getSize().x/2, window.getSize().y/2), window);
        }
    }

    Camera(){}
};