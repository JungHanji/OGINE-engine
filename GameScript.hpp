#include "gameClasses.hpp" 

class Game : protected Quadrontic { 
    public:

    int o = 0;
    

    void sinit(){
        setApp(App({1200, 700}, "Game", {255, 255, 255})); 
    } 

    void gameInit(){ 
        camera.controll = false;
        camera.speed = 0.1; 
        
        SKYBOX.init({0, 0, 0}, 
                    {SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE}, 
                    {255, 255, 255}, 
                    "res/Models/skybox.obj", 
                    "res/Textures/skybox.png"); 

        addObject("player", initObject(vec3(0, 10, 0), {1, 1, 1}, {100, 0, 100}, "res/Models/player.obj", "res/Textures/WhiteRect.png"));
        addObject("player2", initObject(vec3(20, 20, 0), {1, 1, 1}, {100, 100, 100}, "res/Models/player.obj", "res/Textures/WhiteRect.png"));
        addObject("terrain", Plane({0, 0, 0}, {100, 100}, {0, 255, 0}));
        setObjVisible("player", false);
        cout<<"Intialised game"<<endl;

        addRigidBody("player");
        addRigidBody("player2");
        addRigidBody("terrain", true);
        //addRigidBody("terrain2");

        cout<<"Initialised phisics"<<endl;
    }

    void onlineInit(){ 
        bOnline = false;
        if(bOnline){
            serverIp = sf::IpAddress("26.234.63.105"); 
            port = 2007; 
            InitOnline(); 
        }
    }

    void onlineUpdate(){
        if(appType == 's'){
            server.send(Vec3ToString(pscene.rbodys.get("player").velocity)); 
            pscene.rbodys.getPointerTo("player2")->velocity = StringToVec3(server.receive());
            //setObjPos("player2", StringToVec3(server.receive()));
        } else if(appType == 'c'){
            pscene.rbodys.getPointerTo("player2")->velocity = StringToVec3(client.receive());
            //setObjPos("player2", StringToVec3(client.receive())); 
            client.send(Vec3ToString(pscene.rbodys.get("player").velocity));
        }
    }

    void update(sf::RenderWindow &wind){
        
        if(wind.hasFocus()){
            if(isPressed(W_KEY)){
                //cout<<"w"<<endl;
                pscene.rbodys.getPointerTo("player")->velocity-=vec3(sin(rad(camera.rot.x))*camera.speed, 0, cos(rad(camera.rot.x))*camera.speed);
            } else if(isPressed(S_KEY)){
                pscene.rbodys.getPointerTo("player")->velocity+=vec3(sin(rad(camera.rot.x))*camera.speed, 0, cos(rad(camera.rot.x))*camera.speed);
            }

            if(isPressed(A_KEY)){
                pscene.rbodys.getPointerTo("player")->velocity+=vec3(sin(rad(camera.rot.x+270))*camera.speed, 0, cos(rad(camera.rot.x+270))*camera.speed);
            } else if(isPressed(D_KEY)){
                pscene.rbodys.getPointerTo("player")->velocity-=vec3(sin(rad(camera.rot.x+270))*camera.speed, 0, cos(rad(camera.rot.x+270))*camera.speed);
            }

            if(isPressed(SPACE_KEY) && pscene.rbodys.get("player").collised){
                pscene.rbodys.getPointerTo("player")->velocity+=vec3(0, 1.5, 0);
            }
        }

        camera.pos = getObject("player").pos;
        //SKYBOX.pos = camera.pos;
        pscene.update();
    }

    void game_init(){
        game_initE();
        gameInit();
    }

    void init(){
        initE();
    }

    void draw(sf::RenderWindow &wind){
        if(bOnline) onlineUpdate();
        update(wind);
        drawE();
    }

    void run(){
        sapp.run<Game>(*this);
    }
};