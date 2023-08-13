#include "Engine.hpp"
#include "Primitives.hpp"
#include "GameTools.hpp"

struct HitBox{
    vec3 dotMin,
         dotMax,

         lowerCentrP,
         upperCentrP,

         frontCentrP,
         backwardCentrP,

         rightCentrP,
         leftCentrP;
    void computePoints(){
        float xEdge = dotMax.x - dotMin.x, 
              yEdge = dotMax.y - dotMin.y, 
              zEdge = dotMax.z - dotMin.z;

        leftCentrP =     vec3(0, yEdge/2, zEdge/2);
        rightCentrP =    vec3(xEdge, yEdge/2, zEdge/2);

        lowerCentrP =    vec3(xEdge/2, 0, zEdge/2);
        upperCentrP =    vec3(xEdge/2, yEdge, zEdge/2);

        frontCentrP =    vec3(xEdge/2, yEdge/2, 0);
        backwardCentrP = vec3(xEdge/2, yEdge/2, zEdge);
    }
};

HitBox HitBoxFromObject(Object o){
    vec3 dot1={__FLT_MAX__, __FLT_MAX__, __FLT_MAX__}, dot2={__FLT_MIN__, __FLT_MIN__, __FLT_MIN__};
    for(triangle t : o.smesh.tris){
        for(int i=0;i<3;i++){
            if(t.p[i].x < dot1.x){
                dot1.x = t.p[i].x;
            }
            if(t.p[i].x > dot2.x){
                dot2.x = t.p[i].x;
            }
            if(t.p[i].y < dot1.y){
                dot1.y = t.p[i].y;
            }
            if(t.p[i].y > dot2.y){
                dot2.y = t.p[i].y;
            }
            if(t.p[i].z < dot1.z){
                dot1.z = t.p[i].z;
            }
            if(t.p[i].z > dot2.z){
                dot2.z = t.p[i].z;
            }
        }
    }

    HitBox hb = {dot1, dot2};

    float xEdge = hb.dotMax.x - hb.dotMin.x, 
          yEdge = hb.dotMax.y - hb.dotMin.y, 
          zEdge = hb.dotMax.z - hb.dotMin.z;

    hb.leftCentrP =     vec3(0, yEdge/2, zEdge/2);
    hb.rightCentrP =    vec3(xEdge, yEdge/2, zEdge/2);

    hb.lowerCentrP =    vec3(xEdge/2, 0, zEdge/2);
    hb.upperCentrP =    vec3(xEdge/2, yEdge, zEdge/2);

    hb.frontCentrP =    vec3(xEdge/2, yEdge/2, 0);
    hb.backwardCentrP = vec3(xEdge/2, yEdge/2, zEdge);

    return {hb};
}

struct BoundingBox{
    float *cube;
    void getFromHBox(HitBox hCube){
        cube = new float[6]{
            hCube.dotMin.x,
            hCube.dotMin.y,
            hCube.dotMin.z,
            hCube.dotMax.x,
            hCube.dotMax.y,
            hCube.dotMax.z,
        };
    }
};

BoundingBox getFromHBox(HitBox hbox){
    BoundingBox b;
    b.getFromHBox(hbox);
    return {b};
}

bool Inters( float min1, float max1, float min2, float max2 ){
    if (min1 > max2) return false;
    if (max1 < min2) return false;
    return true;
}
 
bool BoundingBoxIntersects(BoundingBox bb1, BoundingBox bb2){
    for (int i = 0; i < 3; ++i)
        if (!Inters(bb1.cube[i], bb1.cube[i + 3], bb2.cube[i], bb2.cube[i + 3])) return false;

    return true;
}

bool BoundingBoxIntersects_x(BoundingBox bb1, BoundingBox bb2){
    return Inters(bb1.cube[0], bb1.cube[3], bb2.cube[0], bb2.cube[3]);
}

bool BoundingBoxIntersects_y(BoundingBox bb1, BoundingBox bb2){
    return Inters(bb1.cube[1], bb1.cube[4], bb2.cube[1], bb2.cube[4]);
}

bool BoundingBoxIntersects_z(BoundingBox bb1, BoundingBox bb2){
    return Inters(bb1.cube[2], bb1.cube[5], bb2.cube[2], bb2.cube[5]);
}

paar<bool, vec3> RayIntersectsTriangle(vec3 rayOrigin, vec3 rayVector, triangle inTriangle)
{
    const float EPSILON = 0.0000001;
    vec3 vertex0 = inTriangle.p[0];
    vec3 vertex1 = inTriangle.p[1];  
    vec3 vertex2 = inTriangle.p[2];
    vec3 outIntersectionPoint;
    vec3 edge1, edge2, h, s, q;
    float a, f, u, v;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = rayVector.cross(edge2);
    a = edge1.dot(h);

    if (a > -EPSILON && a < EPSILON)
        return {false, outIntersectionPoint};    // This ray is parallel to this triangle.

    f = 1.0 / a;
    s = rayOrigin - vertex0;
    u = f * s.dot(h);

    if (u < 0.0 || u > 1.0)
        return {false, outIntersectionPoint};

    q = s.cross(edge1);
    v = f * rayVector.dot(q);

    if (v < 0.0 || u + v > 1.0)
        return {false, outIntersectionPoint};

    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * edge2.dot(q);

    if (t > EPSILON) // ray intersection
    {
        outIntersectionPoint = rayOrigin + rayVector * t;
        return {true, outIntersectionPoint};
    }
    else // This means that there is a line intersection but not a ray intersection.
        return {false, outIntersectionPoint};
}