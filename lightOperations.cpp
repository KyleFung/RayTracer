ray generateRay (camera eye, int i, int j);
glm::vec3 computeColor (light sources[], int numSources, geometry * shapes[], int numShapes, int currentShape, camera eye, intersection junction);
glm::vec3 computeLight (camera eye, light source, geometry &shape, intersection junction);
int findNearestObject (geometry * shapes[], int numShapes, ray beam);
intersection findNearestIntersection (vector<view> viewVector, ray beam);

//Generate a ray struct positioned at the eye going through the (i,j)th pixel
ray generateRay (camera eye, int i, int j)
   {
   ray beam;
   float heightScreen = tan(eye.fovy);
   float widthScreen = heightScreen * (width/height);

   float alpha = (i - (width / 2.0)) / (width / 2.0);
   float beta = (-1.0) * (j - (height /2.0)) / (height / 2.0);
   alpha *= widthScreen;
   beta *= heightScreen;
   
   beam.direction = glm::normalize(eye.direction) + alpha * glm::normalize((glm::cross(eye.direction, eye.up))) + beta * glm::normalize(eye.up);
   beam.position = eye.position;
   
   return beam;
   }

glm::vec3 computeColor (vector<light> lightVector, vector<view> viewVector, camera eye, intersection junction)
   {
   //Calculate color for each light
   glm::vec3 retVal = glm::vec3(0, 0, 0);

   if (!junction.contact) return retVal;

   for (int i = 0; i < lightVector.size(); i++)
      {
      glm::vec3 toSource = 1/(lightVector[i].position.w) * glm::vec3(lightVector[i].position) -  1/(junction.position.w) * glm::vec3(junction.position);
      ray pathToSource = {toSource, junction.position + glm::vec4(toSource.x * EPSILON, toSource.y * EPSILON, toSource.z * EPSILON, 0)}; 

      intersection testShadows = findNearestIntersection(viewVector, pathToSource); 
      //Case where there are no intersections
      if (!testShadows.contact) 
         { 
         //retVal += computeLight(eye, lightVector[i], *shapes[currentShape], junction); 
         } 
      } 
   retVal += viewVector[junction.view].shapes[junction.shape]->lightProperties->ambient;
   return glm::vec3(fminf(retVal.x, 255), fminf(retVal.y, 255), fminf(retVal.z, 255)); 
   }

//Find intersection closest to ray origin
intersection findNearestIntersection (vector<view> viewVector, ray beam)
   {
   intersection closestIntersection;
   closestIntersection.contact = false;
   float closestDistance = 1000;

   for (int i = 0; i < viewVector.size(); i++)
      {
      //Do ray transformation 
      ray transformedRay = beam;
      transformedRay.direction = glm::vec3(viewVector[i].M_inv * glm::vec4(beam.direction, 0)); 
      transformedRay.position = viewVector[i].M_inv * beam.position; 

      for (int j = 0; j < viewVector[i].shapes.size(); j++)
         {
         //Convert intersection position and normal back to normal coordinates 
         intersection junction = viewVector[i].shapes[j]->Intersection(transformedRay);
         junction.position = viewVector[i].M * junction.position;
         junction.normal = glm::vec3(viewVector[i].M_T_inv * glm::vec4(junction.normal, 0)); 

         if (junction.contact && glm::length(glm::vec3(junction.position - beam.position)) < closestDistance)
            {
            junction.view = i;
            junction.shape = j;
            closestIntersection = junction;
            closestDistance = glm::length(glm::vec3(junction.position - beam.position));
            }
         }
      } 
   return closestIntersection;
   }

//Compute lighting calculation given an intersection and light source
/*glm::vec3 computeLight (camera eye, light source, geometry &shape, intersection junction)
   {
   glm::vec3 direction = glm::normalize((1/eye.position.w) * glm::vec3(eye.position) - (1/junction.position.w) * glm::vec3(junction.position));
   glm::vec3 vision = (1/eye.position.w) * glm::vec3(eye.position) - (1/junction.position.w) * glm::vec3(junction.position);
   glm::vec3 light  = (1/source.position.w) * glm::vec3(source.position) - (1/junction.position.w) * glm::vec3(junction.position);
   glm::vec3 halfvec = glm::normalize(light + vision);
   
   if (glm::dot(light, junction.normal) <= 0)
      {
      return glm::vec3(0, 0, 0);
      }

   float nDotL = glm::dot(glm::normalize(junction.normal), direction);

   glm::vec3 lambert = glm::vec3(source.color.x * shape.diffuse.x * fmaxf(nDotL, 0.0),
                                 source.color.y * shape.diffuse.y * fmaxf(nDotL, 0.0),
                                 source.color.z * shape.diffuse.z * fmaxf(nDotL, 0.0));  

   float nDotH = glm::dot(junction.normal, halfvec); 

   glm::vec3 phong = glm::vec3(source.color.x * shape.specular.x * pow(fmaxf(nDotH, 0.0), shape.shininess),
                               source.color.y * shape.specular.y * pow(fmaxf(nDotH, 0.0), shape.shininess),
                               source.color.z * shape.specular.z * pow(fmaxf(nDotH, 0.0), shape.shininess));

   glm::vec3 retVal = lambert + phong;
   retVal = glm::vec3(fminf(retVal.x, 255), fminf(retVal.y, 255), fminf(retVal.z, 255));
   return retVal;
   }*/ 
