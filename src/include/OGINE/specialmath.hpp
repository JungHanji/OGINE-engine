#pragma once
#include "init.hpp"

//Classes

struct svec3{
    float x, y, z;
};

struct scolor3{
    float r, g, b;
};

class vec2{
    public:
    float x,y;
    vec2(float x_=0, float y_=0){
        x=x_;
        y=y_;
    }

    void print(char end='\n'){
        printf("(%f, %f)%c", x, y,end);
    }

    vec2 operator+(vec2 b){
        return {x+b.x, y+b.y};
    }

    vec2 operator-(vec2 b){
        return {x-b.x, y-b.y};
    }

    vec2 operator*(vec2 b){
        return {x*b.x, y*b.y};
    }

    vec2 operator/(vec2 b){
        return {x/b.x, y/b.y};
    }

    void operator+=(vec2 b){
        x += b.x;
        y += b.y;
    }

    void operator-=(vec2 b){
        x -= b.x;
        y -= b.y;
    }

    void operator*=(vec2 b){
        x *= b.x;
        y *= b.y;
    }

    void operator/=(vec2 b){
        x /= b.x;
        y /= b.y;
    }

    bool operator==(vec2 b){
        return (x==b.x) && (y==b.y);
    }
};

class vec2t{
    public:
    float u, v, w;
    vec2t(float u_=0, float v_=0, float w_=1){
        u=u_;
        v=v_;
        w=w_;
    }

    void print(char end='\n'){
        printf("(%f, %f)%c", u, v, end);
    }
};

class vec3{
    public:
    float x,y,z,w;
    vec3(float x_=0, float y_=0, float z_=0, float w_=1){
        x=x_;
        y=y_;
        z=z_;
        w=w_;
    }

    float dot(vec3 v){
        return x*v.x + y*v.y + z*v.z;
    }

    float len(){
        return sqrtf(dot({x, y, z}));
    }

    float pointDist(vec3 &v){
        return sqrt(pow(max(x, v.x) - min(x, v.x), 2) + 
                    pow(max(y, v.y) - min(y, v.y), 2) + 
                    pow(max(z, v.z) - min(z, v.z), 2));
    }

    float dist(vec3 v){
        return sqrtf(dot(v));
    }

    vec3 cross(vec3 &v){
        return {
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        };
    }

    void norm(){
        if(len()!=0){
            x/=len();
            y/=len();
            z/=len();
        }
    }

    vec3 operator+(vec3 b){
        return {x+b.x, y+b.y, z+b.z};
    }

    vec3 operator-(vec3 b){
        return {x-b.x, y-b.y, z-b.z};
    }

    vec3 operator*(vec3 b){
        return {x*b.x, y*b.y, z*b.z};
    }

    vec3 operator*(float b){
        return {x*b, y*b, z*b};
    }

    vec3 operator/(vec3 b){
        return {x/b.x, y/b.y, z/b.z};
    }

    vec3 operator/(float b){
        return {x/b, y/b, z/b};
    }

    void operator+=(vec3 b){
        x += b.x;
        y += b.y;
        z += b.z;
    }

    void operator-=(vec3 b){
        x -= b.x;
        y -= b.y;
        z -= b.z;
    }

    void operator-=(float b){
        x -= b;
        y -= b;
        z -= b;
    }

    void operator+=(float b){
        x += b;
        y += b;
        z += b;
    }

    void operator*=(vec3 b){
        x *= b.x;
        y *= b.y;
        z *= b.z;
    }

    void operator*=(float b){
        x *= b;
        y *= b;
        z *= b;
    }

    void operator/=(vec3 b){
        x /= b.x;
        y /= b.y;
        z /= b.z;
    }

    void operator/=(float b){
        x /= b;
        y /= b;
        z /= b;
    }

    bool operator==(vec3 b){
        return x==b.x && y==b.y && z==b.z && w==b.w;
    }

    bool operator<(float a){
        return x<a && y<a && z<a;
    }

    bool operator>(float a){
        return x>a && y>a && z>a;
    }

    void print(char end='\n'){
        printf("(%.3f, %.3f, %.3f, %.3f)%c", x, y, z, w, end);
    }
};

class triangle{
    public:
    vec3 p[3] = {vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, 0, 0)};
    vec3 color;
    vector<vec2> t ={vec2(), vec2(), vec2()};
    vector<vec3> normals = {vec3(), vec3(), vec3()};
    triangle(vec3 p_[3]){
        p[0] = p_[0];
        p[1] = p_[1];
        p[2] = p_[2];
    }

    triangle(vec3 a, vec3 b, vec3 c, vec2 ua = vec2(), vec2 ub = vec2(), vec2 uc = vec2(), vec3 na = vec3(), vec3 nb = vec3(), vec3 nc = vec3(), vec3 color_ = vec3(255, 0, 255)){
        p[0] = a;
        p[1] = b;
        p[2] = c;
        t[0]=ua;
        t[1]=ub;
        t[2]=uc;
        normals[0]=na;
        normals[1]=nb;
        normals[2]=nc;
    }

    triangle(){;}

    void print(){
        p[0].print(' ');
        p[1].print(' ');
        p[2].print();
    }
};

class mesh{
    public:
    vector<triangle> tris;
    mesh(vector<triangle> tris_){
        tris=tris_;
    }

    mesh(){;}
};

class mat4x4{
    public:
    float m[4][4]{0};
    mat4x4(float m_[][4]){
        for(int i=0;i<4;i++){
            m[i][0] = m_[i][0];
            m[i][1] = m_[i][1];
            m[i][2] = m_[i][2];
            m[i][3] = m_[i][3];
        }
    }

    mat4x4 operator*(mat4x4 m_){
        float tf[4][4]{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
        mat4x4 t(tf);

        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                t.m[i][j] = m[i][0] * m_.m[0][j] + m[i][1] * m_.m[1][j] + m[i][2] * m_.m[2][j] + m[i][3] * m_.m[3][j];
            }
        }

        return mat4x4(t);
    }

    void operator*=(mat4x4 &m_){
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                m[i][j] = m[i][0] * m_.m[0][j] + m[i][1] * m_.m[1][j] + m[i][2] * m_.m[2][j] + m[i][3] * m_.m[3][j];
            }
        }
    }

    mat4x4(){;}
};

// Functions

namespace spmath{
    vec3 Matrix_MultiplyVector(mat4x4 m, vec3 i)
	{
		vec3 v;
		v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
		v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
		v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
		v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
		return v;
	}

    void MultiplyMatrixVector(vec3 i, vec3 &o, mat4x4 m){
        o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
        o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
        o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
        float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];

        if(w != 0){
            o.x /= w;
            o.y /= w;
            o.z /= w;
        }
    }

    vec3 Vector_Add(vec3 v1, vec3 v2){
        return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
    }

    vec3 Vector_Sub(vec3 v1, vec3 v2){
        return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
    }

    vec3 Vector_Mlt(vec3 v1, vec3 v2){
        return {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
    }

    vec3 Vector_Div(vec3 v1, vec3 v2){
        return {v1.x / v2.x, v1.y / v2.y, v1.z / v2.z};
    }

    vec3 Vector_Mlt(vec3 v1, float b){
        return {v1.x * b, v1.y * b, v1.z * b};
    }

    vec3 Vector_Div(vec3 v1, float b){
        return {v1.x / b, v1.y / b, v1.z / b};
    }

    float dot(vec3 a, vec3 b){
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    float len(vec3 a){
        return sqrtf(dot(a, a));
    }

    float pointDist(vec3 v1, vec3 v2){
        return sqrt(pow(max(v2.x, v1.x) - min(v2.x, v1.x), 2) + 
                    pow(max(v2.y, v1.y) - min(v2.y, v1.y), 2) + 
                    pow(max(v2.z, v1.z) - min(v2.z, v1.z), 2));
    }

    vec3 cross(vec3 a, vec3 b){
        return {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }

    float intersectsTriangle(vec3 origv, vec3 dirv, triangle tri){
        vec3 e1 = Vector_Sub(tri.p[1], tri.p[0]);
        vec3 e2 = Vector_Sub(tri.p[2], tri.p[0]);

        vec3 pvec = cross(dirv, e2);
        float det = dot(e1, pvec);

        if (det < 1e-8 and det > -1e-8){
            return 0;
        }

        float inv_det = 1 / det;
        vec3 tvec = Vector_Sub(origv, tri.p[0]);
        float u = dot(tvec, pvec) * inv_det;

        if (u < 0 or u > 1){
            return 0;
        }

        vec3 qvec = cross(tvec, e1);
        float v = dot(dirv, qvec);
        
        if (v < 0 or u + v > 1){
            return 0;
        }

        return dot(e2, qvec) * inv_det;
    }

    vec3 norm(vec3 v){
        if(len(v)!=0){
            return {
                v.x / len(v),
                v.y / len(v),
                v.z / len(v)
            };
        }

        return {v.x, v.y, v.z};
    }

    vec3 IntersectPlane(vec3 &plane_p, vec3 &plane_n, vec3 &lineStart, vec3 &lineEnd, float &t)
	{
		plane_n = norm(plane_n);
		float plane_d = -dot(plane_n, plane_p);
		float ad = dot(lineStart, plane_n);
		float bd = dot(lineEnd, plane_n);
		t = (-plane_d - ad) / (bd - ad);
		vec3 lineStartToEnd = Vector_Sub(lineEnd, lineStart);
		vec3 lineToIntersect = lineStartToEnd * t;
		return Vector_Add(lineStart, lineToIntersect);
	}

    mat4x4 MatrixIdentity(){
        float tf[4][4]{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
        return {tf};
    }

    mat4x4 MatrixTranslation(float x, float y, float z){
        float tf[4][4]{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {x, y, z, 1}};
        return {tf};
    }

    mat4x4 MatrixProjection(float fov, float aspectR, float near_, float far_){
        float fovRad = 1/tan(fov *0.5 / 180 * M_PI);
        float tf[4][4]{{aspectR * fovRad, 0, 0, 0}, {0, fovRad, 0, 0}, {0, 0, far_ / (far_ - near_), 1}, {0, 0, (-far_ * near_) / (far_ - near_), 0}};
        return {tf};
    }

    mat4x4 PointMatrix(vec3 up, vec3 forward, vec3 right, vec3 pos){
        float tf[4][4]{{right.x, right.y, right.z, 0}, {up.x, up.y, up.z, 0}, {forward.x, forward.y, forward.z, 0}, {pos.x, pos.y, pos.z, 1}};
        return {tf};
    }

    mat4x4 MatrixPointAt(vec3 pos, vec3 target, vec3 up){
        vec3 newForward = target;
        newForward = norm(Vector_Sub(newForward, pos));
        vec3 a = Vector_Mlt(newForward, dot(up, newForward));
        vec3 newUp = norm(Vector_Sub(up, a));
        vec3 newRight = cross(newUp, newForward);
        return PointMatrix(newUp, newForward, newRight, pos);
    }

    mat4x4 MatrixQuickInverse(mat4x4 &m){
        mat4x4 t;
        
        t.m[0][0] = m.m[0][0]; t.m[0][1] = m.m[1][0]; t.m[0][2] = m.m[2][0]; t.m[0][3] = 0;
        t.m[1][0] = m.m[0][1]; t.m[1][1] = m.m[1][1]; t.m[1][2] = m.m[2][1]; t.m[1][3] = 0;
        t.m[2][0] = m.m[0][2]; t.m[2][1] = m.m[1][2]; t.m[2][2] = m.m[2][2]; t.m[2][3] = 0;
        t.m[3][0] = -(m.m[3][0] * t.m[0][0] + m.m[3][1] * t.m[1][0] + m.m[3][2] * t.m[2][0]);
        t.m[3][1] = -(m.m[3][0] * t.m[0][1] + m.m[3][1] * t.m[1][1] + m.m[3][2] * t.m[2][1]);
        t.m[3][2] = -(m.m[3][0] * t.m[0][2] + m.m[3][1] * t.m[1][2] + m.m[3][2] * t.m[2][2]);
        t.m[3][3] = 1;

        return mat4x4(t);
    }

    mat4x4 MatrixRotateX(float angle, float speed = 1){
        float tf[4][4]{{1, 0, 0, 0}, {0, (float)cos(angle*0.5)*speed, (float)sin(angle*0.5)*speed, 0}, {0, (float)-sin(angle*0.5)*speed, (float)cos(angle*0.5)*speed, 0}, {0, 0, 0, 1}};
        return {tf};
    }

    mat4x4 MatrixRotateY(float angle, float speed = 1){
        float tf[4][4]{{(float)cos(angle)*speed, 0, (float)sin(angle)*speed, 0}, {0, 1, 0, 0}, {(float)-sin(angle)*speed, 0, (float)cos(angle)*speed, 0}, {0, 0, 0, 1}};
        return {tf};
    }

    mat4x4 MatrixRotateZ(float angle, float speed = 1){
        float tf[4][4]{{(float)cos(angle)*speed, (float)sin(angle)*speed, 0, 0}, {(float)-sin(angle)*speed, (float)cos(angle)*speed, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
        return {tf};
    }

    mat4x4 MatrixMultiply(mat4x4 m1, mat4x4 m2){
        float tf[4][4]{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
        mat4x4 t(tf);

        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                t.m[i][j] = m1.m[i][0] * m2.m[0][j] + m1.m[i][1] * m2.m[1][j] + m1.m[i][2] * m2.m[2][j] + m1.m[i][3] * m2.m[3][j];
            }
        }

        return {t};
    }

    float rad(float angle){
        return angle/180*M_PI;
    }
}