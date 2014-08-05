//---------------------------------------------------------------
// geometry class

class geometry
   {
   public:
   glm::vec3 diffuse;
   glm::vec3 specular;
   glm::vec3 ambient;
   float shininess;
   
   public:
   virtual intersection Intersection(ray) = 0;
   void setReflectance(glm::vec3, glm::vec3, glm::vec3, float);
   };
   
void geometry::setReflectance(glm::vec3 initDiffuse, glm::vec3 initSpecular, glm::vec3 initAmbient, float initShininess)
   {
   diffuse = initDiffuse;
   specular = initSpecular;
   ambient = initAmbient;
   shininess = initShininess;
   }

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
   triangle (glm::vec4, glm::vec4, glm::vec4);
   intersection Intersection(ray);
   };

triangle::triangle (glm::vec4 A, glm::vec4 B, glm::vec4 C)
   {
   P1 = A;
   P2 = B;
   P3 = C;
   }

intersection triangle::Intersection(ray beam)
   {
   intersection junction;
   // Vertices of triangle and ray origin in dehomogenized coordinates
   glm::vec3 eye = (1/beam.position.w) * glm::vec3(beam.position);
   glm::vec3 A = (1/P1.w) * glm::vec3(P1);
   glm::vec3 B = (1/P2.w) * glm::vec3(P2);
   glm::vec3 C = (1/P3.w) * glm::vec3(P3);

   // Find plane - ray intersection
   glm::vec3 intersection;
   glm::vec3 normal = glm::normalize(glm::cross((B - A), (C - B)));

   float d = glm::dot(normal, beam.direction);
   if (d == 0) return junction;

   // Calculate t of intersection for line in point-vector equation
   float t = (normal.x * (A.x - eye.x) + normal.y * (A.y - eye.y) + normal.z * (A.z - eye.z)) / d;
   if (t < 0) return junction;

   intersection = (t * beam.direction) + eye;

   // Inside-Outside-test the intersection
   if (glm::dot(glm::cross((B - A),(intersection - A)), normal) < 0) return junction;
   if (glm::dot(glm::cross((C - B),(intersection - B)), normal) < 0) return junction;
   if (glm::dot(glm::cross((A - C),(intersection - C)), normal) < 0) return junction;

   junction.contact = true;
   junction.position = glm::vec4(intersection, 1);

   // Scale normal by -1 if it is not facing the right direction relative to the ray
   if (glm::dot(normal, beam.direction) > 0) normal = glm::vec3(normal.x * -1, normal.y * -1, normal.z * -1);
   junction.normal = normal;
   junction.distance = glm::length(t * beam.direction);
   return junction;
   }


//---------------------------------------------------------------
// sphere class

class sphere: public geometry
   {
   protected:
   glm::vec4 center;
   float radius;

   public:
   sphere(float, glm::vec4);
   intersection Intersection(ray);
   void setRadius(float);
   void setCenter(glm::vec4);
   };

sphere::sphere (float length, glm::vec4 position)
   {
   setRadius(length);
   setCenter(position);
   }

void sphere::setRadius(float length)
   {
   radius = length;
   }

void sphere::setCenter(glm::vec4 position)
   {
   center = position;
   }

intersection sphere::Intersection(ray beam)
   {
   intersection junction;
   // direction * direction 
   float a = glm::dot(beam.direction, beam.direction);
   // 2 * direction * (beamPosition - center)
   float b = 2 * glm::dot(beam.direction, (glm::vec3(beam.position)/beam.position.w) - (glm::vec3(center)/center.w));
   // (beamPosition - center) * (beamPosition - center) - radius ^2
   float c = glm::dot((glm::vec3(beam.position)/beam.position.w) - (glm::vec3(center)/center.w), 
      (glm::vec3(beam.position)/beam.position.w) - (glm::vec3(center)/center.w)) - radius * radius;
   float discriminant = (b * b) - (4 * a * c);
   
   if (discriminant < 0)
      {
      junction.contact = false;
      return junction;
      }

   glm::vec3 eye = (1/beam.position.w) * glm::vec3(beam.position);
   float tminus = ((b * (-1.0)) - sqrt(discriminant))/(2.0 * a);
   float tplus = ((b * (-1.0)) + sqrt(discriminant))/(2.0 * a);

   if (tminus >= 0)
      {
      junction.position = glm::vec4((tminus * beam.direction) + eye, 1);
      junction.distance = glm::length(tminus * beam.direction);
      junction.contact = true;
      }
   
   else if (tplus > 0)
      {
      junction.position = glm::vec4((tplus * beam.direction) + eye, 1);
      junction.distance = glm::length(tplus * beam.direction);
      junction.contact = true;
      }
   else
      {
      junction.contact = false;
      return junction;
      }

   junction.normal = glm::vec3(junction.position) - ((1/center.w) * glm::vec3(center));
   return junction;
   }