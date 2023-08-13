#pragma once

#define u_int unsigned int
#define u_char unsigned char

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <glad/glad.h>
#include <cmath>
#include <fstream>
#include "Camera.hpp"
#include "specialtools.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace spmath;
//using namespace sf;

Camera camera = Camera({0, 0, 0});

sf::Color sfcolor(vec3 color){
    return sf::Color(color.x, color.y, color.z);
}

sf::Vector2f sfvec(vec3 vec){
    return sf::Vector2f(vec.x, vec.y);
}

void DrawLine(vec3 a, vec3 b, vec3 color, sf::RenderWindow &wind){
    sf::Vertex line[] =
    {
        sf::Vertex(sfvec(a), sf::Color(sfcolor(color))),
        sf::Vertex(sfvec(b), sf::Color(sfcolor(color)))
    };

    wind.draw(line, 2, sf::Lines);
}

vec3 ClientToOpenGL(vec2 mpos){
    GLdouble xx,xy,xz;

    int vp[4];
    double mMatrix[16], pMatrix[16];
    float z;

    glGetIntegerv(GL_VIEWPORT, vp);
    mpos.y = vp[3] - mpos.y - 1;

    glGetDoublev(GL_MODELVIEW_MATRIX, mMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, pMatrix);
    glReadPixels(mpos.x, mpos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
    gluUnProject(mpos.x, mpos.y, z, mMatrix, pMatrix, vp, &xx, &xy, &xz);

    return {(float)xx, (float)xy, (float)xz};
}

float vert[] = {
    1, 0, 1,  
    1, 0, -1,  
    -1, 0, -1,  
    -1, 0, 1
};

class App{
    public:

    vec2 size;
    vec3 bgColor;
    string name;

    bool hideCursor,
         cameraEnabled,
         projection, 
         hasFocus = false;

    float elapsedTime;

    vec3 camPos;

    App(){}

    App(vec2 size_, string name_, vec3 bgColor_, bool hideCursor_ = true, bool cameraEnabled_ = true, bool projection_ = true){
        size=size_;
        name=name_;
        bgColor=bgColor_;
        hideCursor=hideCursor_;
        cameraEnabled=cameraEnabled_;
        projection=projection_;
        
        elapsedTime = 1;
    }

    void init(){
        gladLoadGL();
    }

    void showDemo(){
        glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, &vert);
            for(int i = -5; i < 5; i++){
                for(int j = -5; j < 5; j++){
                    glPushMatrix();
                        if((i+j) % 2 == 0) glColor3f(0, 0.5, 0);
                        else glColor3f(1, 1, 1);
                        glTranslatef(i*2, 0, j*2);
                        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                    glPopMatrix();
                }
            }
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    void game_init(){
        ;
    }

    void draw(){
        showDemo();
    }

    template<class Drc>
    void run(Drc drc){
        
        //drc.onlineInit();

        sf::ContextSettings contextSettings;
        contextSettings.depthBits = 24;

        sf::RenderWindow window(sf::VideoMode(size.x, size.y), name, sf::Style::Default, contextSettings);
        window.setVerticalSyncEnabled(true);

        if(hideCursor) window.setMouseCursorVisible(false);

        window.setActive(true);

        drc.init();
        
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glViewport(0, 0, size.x, size.y);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if(!projection) glOrtho(-size.x/size.y, size.x/size.y, -1,1,  0.1, 2000);
        if(projection) glFrustum(-size.x/size.y, size.x/size.y, -1,1,  1, 2000);
        glTranslatef(0, 0, -0.1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        drc.game_init();
        
        float frames = 0, time = 0;
        window.setActive(false);
        sf::Time deltaTime;
        sf::Clock clock;
        sf::Event event;
        while(window.isOpen()){
            
            if(time>=1){
                window.setTitle(name + " | FPS: " + to_string(frames));
                frames=0;
                time=0;
            }

            deltaTime = clock.restart();
            time+=deltaTime.asSeconds();

            while(window.pollEvent(event)){
                if (event.type == sf::Event::Closed) window.close();
            }
            //elapsedTime = clock.getElapsedTime().asMilliseconds();
            
            //hasFocus = window.hasFocus();

            if(cameraEnabled){
                camera.update(window);
            }

            
            window.setActive(true);

            
            glClearColor(normFcolor(bgColor.x), normFcolor(bgColor.y), normFcolor(bgColor.z), 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            if(cameraEnabled) gluLookAt(camera.pos.x, camera.pos.y, camera.pos.z, camera.pos.x-sin(camera.rot.x/180*M_PI), camera.pos.y+tan(camera.rot.y/180*M_PI), camera.pos.z-cos(camera.rot.x/180*M_PI), 0, 1, 0);
            
            camera.dir = {
                sinf(camera.rot.x/180*M_PI),
                tanf(camera.rot.y/180*M_PI),
                cosf(camera.rot.x/180*M_PI)
            };

            glPushMatrix();
                drc.draw(window);
            glPopMatrix();

            window.setActive(false);
            
            window.pushGLStates();
            DrawLine({size.x/2-6, size.y/2}, {size.x/2+5, size.y/2}, {255, 255, 255}, window);
            DrawLine({size.x/2, size.y/2-5}, {size.x/2, size.y/2+5}, {255, 255, 255}, window);
            window.popGLStates();

            window.display();
            frames++;
        }
    }
};

class Texture{
    public:

    u_int texture;
    int width, height, cnt;

    Texture(string fileName){
        u_char *data = stbi_load(fileName.c_str(), &width, &height, &cnt, 0);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, cnt == 4 ? GL_RGBA : GL_RGB, width, height,
                                        0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    }

    void loadFromFile(string fileName){
        u_char *data = stbi_load(fileName.c_str(), &width, &height, &cnt, 0);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, cnt == 4 ? GL_RGBA : GL_RGB, width, height,
                                        0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    }

    void active(bool m){
        if(m){
            //glBindTexture(GL_TEXTURE_2D, 0);
            glBindTexture(GL_TEXTURE_2D, texture);
        }
        else {
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    Texture(){}
};

Texture loadTextureFromFile(string fileName){
    Texture t;
    t.loadFromFile(fileName);
    return {t};
}

template<class cs>
void printv(vector<cs> vs){
    for(cs s: vs){
        cout<<"'"<<s<<"' ";
    }
    cout<<endl;
}

struct light{
    vec3 pos, dir, color;
    GLint lightNum;
};

class Lights{
    public:

    vector<light> sources; 

    Lights(){
        ;
    }

    void newSource(vec3 pos, vec3 dir, GLint lightNum, vec3 color = {255, 255, 255}){
        sources.push_back({pos, dir, color, lightNum});
    }

    void activeLastSource(bool mode){
        if(mode){
            glEnable(sources.back().lightNum);
        } else glDisable(sources.back().lightNum);
    }

    void setLightingBeforeObject(int ind){
        glPushMatrix();
            float lpos[3]{sources[ind].pos.x, sources[ind].pos.y, sources[ind].pos.z};
            float lclr[3]{sources[ind].color.x, sources[ind].color.y, sources[ind].color.z};
            //glTranslatef(lpos[0], lpos[1], lpos[2]);

            glRotatef(sources[ind].dir.x, 1, 0, 0);
            glRotatef(sources[ind].dir.y, 0, 1, 0);
            glRotatef(sources[ind].dir.z, 0, 0, 1);

            GLfloat material_diffuse[] = {1.0, 1.0, 1.0, 1.0};
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);

            // точечный источник света
            // убывание интенсивности с расстоянием
            // задано функцией f(d) = 1.0 / (0.4 * d * d + 0.2 * d)
            GLfloat light2_diffuse[] = {1, 1, 1};
            //glEnable(sources[ind].lightNum);
            glLightfv(sources[ind].lightNum, GL_DIFFUSE, light2_diffuse);
            glLightfv(sources[ind].lightNum, GL_POSITION, lpos);
            glLightf(sources[ind].lightNum, GL_CONSTANT_ATTENUATION, 1);
            glLightf(sources[ind].lightNum, GL_LINEAR_ATTENUATION, 1);
            glLightf(sources[ind].lightNum, GL_QUADRATIC_ATTENUATION, 0.5);

            
        glPopMatrix();
    }
};

class SoundObject{
    public:
    dict<string, sf::Sound> sounds; 
    SoundObject(){
        ;
    }

    void loadSound(string name, string fileName){
        sf::SoundBuffer sb;
        sb.loadFromFile(fileName);
        sf::Sound ts(sb);
        sounds.set(name, ts);
    }

    void setListner(vec3 pos, vec3 dir, float globalVoulume = 100){
        sf::Listener::setPosition(pos.x, pos.y, pos.z);
        sf::Listener::setDirection(dir.x, dir.y, dir.z);
        sf::Listener::setGlobalVolume(globalVoulume);
    }

    void playSound(string name){
        sounds.get(name).play();
    }
};

class Object{
    public:
    GLfloat *vecMesh, *texMesh;

    vec3 pos, rot, scale, color;
    vector<u_int> vertices, uvCoords, vNormals;
    mesh smesh;

    Texture texture;
    vector<Texture> textures;
    int texMode = 0;
    int afterNumTexChangeTex = 0;

    bool noTexture = false, invisible = false;

    Object(){
        ;
    }

    void setScale(vec3 sc){ scale = sc; }
    void setTexMode(int m){ texMode = m; }
    void setNumChangeTex(int n){ afterNumTexChangeTex = n; }
    void setColor(vec3 clr){ color=clr; }
    void setTexture(Texture text, int mode = 0){ 
        if(mode==0) texture = text; 
        else textures.push_back(text);
        texture.active(true);
    }

    void loadFromFile(string fileName){
        string line;
        ifstream ogfile (fileName);
        vector<vec3> vertexes, normals;
        vector<vec2> uvertexes;

        while (!ogfile.eof()){
            getline(ogfile,line);
            vector<string> splitted = split(line);
            
            if(splitted[0]=="v"){
                float x = (float)::atof(splitted[1].c_str()),
                      y = (float)::atof(splitted[2].c_str()),
                      z = (float)::atof(splitted[3].c_str());
                vertexes.push_back({x, y, z});
            } else if(splitted[0]=="vt"){
                float x = (float)::atof(splitted[1].c_str()),
                      y = (float)::atof(splitted[2].c_str());
                uvertexes.push_back({x, y});
            } else if(splitted[0]=="vn"){
                float x = (float)::atof(splitted[1].c_str()),
                      y = (float)::atof(splitted[2].c_str()),
                      z = (float)::atof(splitted[3].c_str());
                normals.push_back({x, y, z});
            } else if(splitted[0]=="f"){
                int a = ::atoi(split(splitted[1], '/')[0].c_str()),
                    b = ::atoi(split(splitted[2], '/')[0].c_str()),
                    c = ::atoi(split(splitted[3], '/')[0].c_str());
                int ua = ::atoi(split(splitted[1], '/')[1].c_str()),
                    ub = ::atoi(split(splitted[2], '/')[1].c_str()),
                    uc = ::atoi(split(splitted[3], '/')[1].c_str());
                int na = ::atoi(split(splitted[1], '/')[2].c_str()),
                    nb = ::atoi(split(splitted[2], '/')[2].c_str()),
                    nc = ::atoi(split(splitted[3], '/')[2].c_str());
                smesh.tris.push_back({vertexes[a-1], vertexes[b-1], vertexes[c-1], 
                                    uvertexes[ua-1], uvertexes[ub-1], uvertexes[uc-1],
                                    normals[na-1], normals[nb-1], normals[nc-1]});
            }
        }

        cout<<"loaded object"<<endl;
    }

    void draw(Lights lightObj, bool texturing = true, bool normals = true){
        glColor3f(normFcolor(color.x), normFcolor(color.y), normFcolor(color.z));
        for(int i = 0; i < lightObj.sources.size(); i++){
            lightObj.setLightingBeforeObject(i);
        }
        
        glPushMatrix();
            
            glTranslatef(pos.x, pos.y, pos.z);
            glScalef(scale.x, scale.y, scale.z);
            for(triangle t : smesh.tris){
                glBegin(GL_TRIANGLES);
                    for(int i=0; i < 3; i++){
                        if(normals) glNormal3f(t.normals[i].x, t.normals[i].y, t.normals[i].z);
                        if(texturing && !noTexture) glTexCoord2f(t.t[i].x, t.t[i].y);
                        glVertex3f(t.p[i].x, t.p[i].y, t.p[i].z);
                    }
                glEnd();
            }
        glPopMatrix();
    }

    void draw(bool texturing = true, bool normals = true){
        glColor3f(normFcolor(color.x), normFcolor(color.y), normFcolor(color.z));
        glPushMatrix();
            glTranslatef(pos.x, pos.y, pos.z);
            glScalef(scale.x, scale.y, scale.z);
            for(triangle t : smesh.tris){
                glBegin(GL_TRIANGLES);
                    for(int i=0; i < 3; i++){
                        if(normals) glNormal3f(t.normals[i].x, t.normals[i].y, t.normals[i].z);
                        if(texturing && !noTexture) glTexCoord2f(t.t[i].x, t.t[i].y);
                        glVertex3f(t.p[i].x, t.p[i].y, t.p[i].z);
                    }
                glEnd();
            }
        glPopMatrix();
    }

    void init(vec3 pos_, vec3 scale_, vec3 color_, string objFile_, string textureName_){
        pos=pos_;
        scale=scale_;
        color=color_;
        if(objFile_ != ""){
            loadFromFile(objFile_);
        }
        if(textureName_ != ""){
            texture=loadTextureFromFile(textureName_);
        } else {
            noTexture = true;
        }
    }
};

Object initObject(vec3 pos_, vec3 scale_, vec3 color_, string objFile_, string textureName_){
    Object o;
    o.init(pos_, scale_, color_, objFile_, textureName_);
    return {o};
}