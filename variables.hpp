#ifndef VARIABLES_H
#define VARIABLES_H
using namespace std;

const int width = 1000;
const int height = 1000;
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
   float distance;
   glm::vec4 position;
   glm::vec3 normal;
   };
   
struct light
   {
   glm::vec3 color;
   glm::vec4 position;
   };

glm::vec4 eye;
glm::vec3 direction;
glm::vec3 up;
#endif /* VARIABLES_H */
