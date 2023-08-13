#include "Collider.hpp"

struct RigidBody{
    HitBox hitbox;
    Object *robject;
    vec3 velocity,
         vTime;
    bool staticBody = false, throughness = false, collised = false;
    RigidBody(HitBox hitbox_, Object *robject_, vec3 velocity_, bool stBody_ = false, bool throughness_ = false){
        hitbox = hitbox_;
        robject = robject_;
        velocity = velocity_;
        staticBody = stBody_;
        throughness = throughness_;
    }

    void move(vec3 vel){
        velocity += vel;
    }
};

struct PhisicsScene{
    dict<string, RigidBody> rbodys;
    float airResistance = 0.7,
          gravity = 0.15,
          velTreshHold = 0;
    
    paar<paar<HitBox, HitBox>, bool> collideWithAnyBody(int ind){
        
        HitBox HitboxNext = rbodys.values[ind].hitbox;
        HitboxNext.dotMin+=rbodys.values[ind].robject->pos + rbodys.values[ind].velocity;
        HitboxNext.dotMax+=rbodys.values[ind].robject->pos + rbodys.values[ind].velocity;
        for(int i = 0; i<rbodys.values.size(); i++){
            if(i!=ind && !rbodys.values[i].throughness){
                HitBox HitboxNextI = rbodys.values[i].hitbox;
                HitboxNextI.dotMin+=rbodys.values[i].robject->pos + rbodys.values[i].velocity;
                HitboxNextI.dotMax+=rbodys.values[i].robject->pos + rbodys.values[i].velocity;
                
                HitboxNextI.computePoints();
                HitboxNext.computePoints();
                
                if(BoundingBoxIntersects(getFromHBox(HitboxNext), getFromHBox(HitboxNextI))) {
                    return {{HitboxNext, HitboxNextI}, true};
                }
            }
        }

        return {{HitBox(), HitBox()}, false};
    }

    void update(){
        for(int i = 0; i<rbodys.values.size(); i++){
            if(!rbodys.values[i].staticBody){
                rbodys.values[i].velocity.x *= airResistance;
                rbodys.values[i].velocity.z *= airResistance;
                if(abs(rbodys.values[i].velocity.x) <= velTreshHold) rbodys.values[i].velocity.x = 0;
                if(abs(rbodys.values[i].velocity.z) <= velTreshHold) rbodys.values[i].velocity.z = 0;

                rbodys.values[i].velocity.y -= gravity;
                if(rbodys.values[i].velocity.y < 0) rbodys.values[i].velocity.y *= airResistance;

                paar<paar<HitBox, HitBox>, bool> info = collideWithAnyBody(i);
                rbodys.values[i].collised = info.value;

                if(!info.value){
                    rbodys.values[i].robject->pos += rbodys.values[i].velocity;
                } else {
                    HitBox hb = info.key.key;
                    HitBox hbi = info.key.value;

                    if(hbi.upperCentrP.y - hb.lowerCentrP.y < 0 || hb.lowerCentrP.y - hbi.upperCentrP.y <= 0) {
                        rbodys.values[i].velocity.y = 0;
                    }

                    if(abs(hb.dotMin.y-hbi.dotMax.y) > 0.5){
                        if(hbi.leftCentrP.x - hb.rightCentrP.x <= 0 || hb.rightCentrP.x - hbi.leftCentrP.x <= 0) rbodys.values[i].velocity.x = 0;
                        if(hbi.frontCentrP.z - hb.backwardCentrP.z <= 0 || hb.backwardCentrP.z - hbi.frontCentrP.z <= 0) rbodys.values[i].velocity.z = 0;
                    }

                    rbodys.values[i].robject->pos += rbodys.values[i].velocity;
                }
            }
        }
    }
};