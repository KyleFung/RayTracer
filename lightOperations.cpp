ray generateRay (camera eye, int i, int j);
glm::vec3 computeColor (light sources[], int numSources, geometry * shapes[], int numShapes, int currentShape, camera eye, intersection junction);
glm::vec3 computeLight (camera eye, light source, geometry &shape, intersection junction);
int findNearestObject (geometry * shapes[], int numShapes, ray beam);
intersection findNearestIntersection (vector<view> viewVector, ray beam);

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

glm::vec3 computeColor (light sources[], int numSources, geometry * shapes[], int numShapes, int currentShape, camera eye, intersection junction)
   {
   //Calculate color for each light
   glm::vec3 retVal = glm::vec3(0,0,0); 

   for (int i = 0; i < numSources; i++)
      {
      glm::vec3 toSource = 1/(sources[i].position.w) * glm::vec3(sources[i].position) -  1/(junction.position.w) * glm::vec3(junction.position);
      ray pathToSource = {toSource, junction.position + glm::vec4(toSource.x * EPSILON, toSource.y * EPSILON, toSource.z * EPSILON, 0)}; 

      int testShadows = findNearestObject(shapes, numShapes, pathToSource); 
      //Case where there are no intersections
      if (testShadows == -1)
         { 
         retVal += computeLight(eye, sources[i], *shapes[currentShape], junction); 
         } 
      }
   retVal += shapes[currentShape]->ambient;
   return glm::vec3(fminf(retVal.x, 255), fminf(retVal.y, 255), fminf(retVal.z, 255));
   }

glm::vec3 computeLight (camera eye, light source, geometry &shape, intersection junction)
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
   }
   
int findNearestObject (geometry * shapes[], int numShapes, ray beam)
   {
   // the base comparison distance is 1000, anything more than 1000 units away is ignored
   int closestIndex = -1;
   float closestDistance = 1000;
   
   for (int i = 0; i < numShapes; i++)
      {
      intersection junction = shapes[i]->Intersection(beam);
      if (junction.contact && junction.distance < closestDistance)
         {
         closestIndex = i;
         closestDistance = junction.distance;
         }
      }
      
   return closestIndex;
   }

intersection findNearestIntersection (vector<view> viewVector, ray beam)
   {
   intersection closestIntersection;
   closestIntersection.distance = 1000;

   for (int i = 0; i < viewVector.size(); i++)
      {
      //Do ray transformation
      ray transformedRay;
      transformedRay.position = viewVector[i].M_inv * beam.position;
      transformedRay.direction = glm::vec3(viewVector[i].M_inv * glm::vec4(beam.direction, 0));

      for (int j = 0; j < viewVector[i].shapes.size(); j++)
         {
         //Convert intersection position and normal back to normal coordinates
         intersection junction = viewVector[i].shapes[j]->Intersection(transformedRay);
         junction.position = viewVector[i].M * junction.position;
         junction.normal = glm::vec3(viewVector[i].M_T_inv * glm::vec4(junction.normal, 0));

         if (junction.contact && junction.distance < closestIntersection.distance)
            {
            closestIntersection = junction;
            }
         }
      }
   return closestIntersection;;
   }
