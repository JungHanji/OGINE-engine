#include <OGINE/Engine.hpp>
#include <OGINE/Primitives.hpp>
#include <OGINE/GameTools.hpp>
#include <OGINE/net2.hpp>
#include <OGINE/RigidBody.hpp>

#define W_KEY     sf::Keyboard::Key::W
#define S_KEY     sf::Keyboard::Key::S
#define A_KEY     sf::Keyboard::Key::A
#define D_KEY     sf::Keyboard::Key::D
#define E_KEY     sf::Keyboard::Key::E
#define Q_KEY     sf::Keyboard::Key::Q
#define SPACE_KEY sf::Keyboard::Key::Space

Server server;
Client client, client2;
char appType;

void InitOnline(){
    cout << "App type server [s] / client [c]: ";
    cin >> appType;

    if(appType == 's'){
        server.init();
    } else if(appType == 'c'){
        client.init();
    }

    cout<< endl <<"Initialized onlline client"<<endl;
}

string Vec3ToString(vec3 v){
    return join({to_string(v.x), to_string(v.y), to_string(v.z)});
}

vec3 StringToVec3(string s){
    vector<string> a = split(s);
    return {(float)::atof(a[0].c_str()), (float)::atof(a[1].c_str()), (float)::atof(a[2].c_str())};
}

class Quadrontic{
    public:
    
    App sapp;
    dict<string, Object> objects;

    Object SKYBOX;
    float SKYBOX_SIZE = 500;
    int tick = 0;

    bool lighting = false, bOnline = true;
    vec3 ambient = {255, 255, 255};
    Lights lights;

    PhisicsScene pscene;

    Quadrontic(App sapp_){
        sapp=sapp_;
    }

    void addObject(string name, Object object){
        objects.set(name, object);
    }

    Object getObject(string name){
        return objects.get(name);
    }

    void addRigidBody(string name_, bool st = false){
        pscene.rbodys.set(name_, RigidBody(HitBoxFromObject(getObject(name_)), 
                                          &objects.values[index(objects.keys, name_)], 
                                          vec3(0, 0, 0), st));
    }

    void setApp(App sapp_){
        sapp=sapp_;
    }

    void game_initE(){
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.99);
        
        glEnable(GL_TEXTURE_2D);
        
        if(lighting) {glEnable(GL_COLOR_MATERIAL); glEnable(GL_NORMALIZE);}

        if(lighting) {lights.newSource({0, 0, 0}, {0, 1, 0}, GL_LIGHT0); lights.activeLastSource(true);}
    }

    void initE(){
        gladLoadGL();
    }

    void setObjPos(string name, vec3 p){
        Object &t = objects.values[index(objects.keys, name)];
        t.pos = p;
    }

    void setObjRot(string name, vec3 p){
        Object &t = objects.values[index(objects.keys, name)];
        t.rot = p;
    }

    void setObjColor(string name, vec3 p){
        Object &t = objects.values[index(objects.keys, name)];
        t.color = p;
    }

    void setObjScale(string name, vec3 p){
        Object &t = objects.values[index(objects.keys, name)];
        t.scale = p;
    }

    void setObjVisible(string name, bool p){
        Object &t = objects.values[index(objects.keys, name)];
        t.invisible = !p;
    }

    void setObjTex(string name, Texture tex){
        Object &t = objects.values[index(objects.keys, name)];
        t.texture = tex;
    }

    void drawE(){
        glColor3f(normFcolor(ambient.x), normFcolor(ambient.y), normFcolor(ambient.z));
        
        if(lighting) {
            glEnable(GL_LIGHTING);
            lights.sources[0].pos = camera.pos;
            lights.sources[0].dir = camera.dir;
            lights.setLightingBeforeObject(0);
        }

        SKYBOX.texture.active(true);
        SKYBOX.draw();
        SKYBOX.texture.active(false);

        for(int i = 0; i < objects.size(); i++){    
            if(!objects.values[i].invisible){
                objects.values[i].texture.active(true);
                if(lighting) objects.values[i].draw(lights);
                else objects.values[i].draw();
                objects.values[i].texture.active(false);
            }
        }
        tick++;
        if(lighting) glDisable(GL_LIGHTING);
    }

    bool isPressed(sf::Keyboard::Key key){
        return sf::Keyboard::isKeyPressed(key);
    }

    Quadrontic(){}
};