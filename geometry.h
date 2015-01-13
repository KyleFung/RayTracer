//---------------------------------------------------------------
// geometry class

class geometry
   {
   public:
   material lightProperties;

   public:
   virtual intersection Intersection(ray) = 0;
   };

//---------------------------------------------------------------
// triangle class

class triangle: public geometry
   {
   protected:
   glm::vec4 P1;
   glm::vec4 P2;
   glm::vec4 P3;
   glm::vec3 normal;

   public:
   triangle (vertex *A, vertex *B, vertex *C, material givenProperties);
   intersection Intersection(ray beam);
   };

//---------------------------------------------------------------
// sphere class

class sphere: public geometry
   {
   protected:
   glm::vec4 center;
   float radius;

   public:
   sphere(float, glm::vec4, material);
   intersection Intersection(ray);
   void setRadius(float);
   void setCenter(glm::vec4);
   };

#include "geometry.cpp"
