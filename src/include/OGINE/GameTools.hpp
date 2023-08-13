#pragma once

#include "Engine.hpp"

vector<triangle> getClosest(mesh terr, vec3 pos, float dist){
    vector<triangle> tris;
    for(triangle t : terr.tris){
        if(pointDist(pos, t.p[0]) < dist) tris.push_back(t);
    }
    return {tris};
}

float dist(vec2 a, vec2 b){
    return sqrtf(pow((b.x-a.x),2) + pow((b.y-a.y),2));
}

vector<vector<float>> genTerrain(int msize, float g=0){
    vector<vector<float>> heightmap;
    for(int i=0;i<msize;i++){
        heightmap.push_back(vector<float>{});
        for(int j=0;j<msize;j++) 
            heightmap[i].push_back(g);
    }

    vector<vec2> heels, pits;
    for(int i=0;i<msize*0.3;i++) heels.push_back({(float)randint(0, msize), (float)randint(0, msize)});
    for(int i=0;i<msize*0.25;i++) pits.push_back({(float)randint(0, msize), (float)randint(0, msize)});
    
    for(int y=0;y<msize;y++){
        for(int x=0;x<msize;x++){
            float p=0;
            for(vec2 v : heels){
                p+=(max((float)0.1, (float)(dist(v, {(float)x, (float)y}))));
            }
            heightmap[y][x] += p;
            p = 0;
            for(vec2 v : pits){
                p+=(max((float)0.1, (float)(dist(v, {(float)x, (float)y}))));
            }
            heightmap[y][x] -= p;
        }
    }

    return {heightmap};
}