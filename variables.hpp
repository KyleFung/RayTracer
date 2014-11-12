#ifndef VARIABLES_H
#define VARIABLES_H
using namespace std;

const int width = 640;
const int height = 480;
const float EPSILON = 0.00001;

struct ray
   {
   glm::vec3 direction;
   glm::vec4 position;
   };

struct camera
   {
   glm::vec4 position;
   glm::vec3 direction;
   glm::vec3 up;
   float fovy;
   };
   
struct intersection
   {
   bool contact; 
   int view;
   int shape;
   glm::vec4 position;
   glm::vec3 normal;
   };
   
struct shapeIndex
   {
   int view;
   int shape;
   };

struct light
   {
   glm::vec3 color;
   glm::vec4 position;
   };

struct vertex
   {
   glm::vec4 position;
   };

struct material
   { 
   glm::vec3 diffuse;
   glm::vec3 specular; 
   float shininess;
   };

float fov;
#endif /* VARIABLES_H */
