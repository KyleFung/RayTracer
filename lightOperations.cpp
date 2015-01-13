//Generate a ray struct positioned at the eye going through the (i,j)th pixel
ray generateRay (camera eye, int i, int j)
   {
   ray beam;
   float heightScreen = (float) tan((double) eye.fovy * 0.5);
   float widthScreen = heightScreen * ((float) width/ (float) height);

   float alpha = ((float)i - (width * 0.5) + 0.5) / (width * 0.5);
   float beta = (-1.0) * ((float)j - (height * 0.5) + 0.5) / (height * 0.5);
   alpha *= widthScreen;
   beta *= heightScreen;

   beam.direction = glm::normalize(eye.direction) + alpha * glm::cross(glm::normalize(eye.direction), glm::normalize(eye.up)) + beta * glm::normalize(eye.up);
   beam.position = eye.position;

   return beam;
   }

glm::vec3 computeColor (vector<light> lightVector, vector<view> viewVector, camera eye, intersection junction, int recursionDepth)
   {
   //Return black if the ray never hit a shape
   glm::vec3 retVal = glm::vec3(0, 0, 0);
   if (!junction.contact) return retVal;

   //Calculate color contributed by each light
   for (int i = 0; i < lightVector.size(); i++)
      {
      //Produce a ray that goes from the intersection toward the light
      //Case of point light: use direction vector from intersection to the light
      //Case of directional light: use negative of direction vector from light source
      glm::vec3 toSource;
      if (lightVector[i].position.w) toSource =
         1/(lightVector[i].position.w) * glm::vec3(lightVector[i].position) -  1/(junction.position.w) * glm::vec3(junction.position);
      else toSource = ((float)-1.0) * glm::vec3(lightVector[i].position);

      ray pathToSource = {toSource, junction.position + glm::vec4(toSource.x * EPSILON, toSource.y * EPSILON, toSource.z * EPSILON, 0)};

      intersection testShadows = findNearestIntersection(viewVector, pathToSource);
      //Case where there are no intersections or the closest intersection is behind a point light
      if (!testShadows.contact || (lightVector[i].position.w &&
            (glm::length(glm::vec3(testShadows.position) - glm::vec3(junction.position))
            >= glm::length(glm::vec3(junction.position) - glm::vec3(lightVector[i].position)))))
         {
         retVal += computeLight(eye, lightVector[i], *viewVector[junction.view].shapes[junction.shape], junction);
         }
      }
   retVal += viewVector[junction.view].shapes[junction.shape]->lightProperties.ambient;
   retVal += viewVector[junction.view].shapes[junction.shape]->lightProperties.emissive;

   //Generate a reflection ray and then find closest intersection and then compute color
   if (recursionDepth < maxDepth)//&& viewVector[junction.view].shapes[junction.shape]->lightProperties.specular.length() != 0)
      {
      glm::vec3 reflectionDirection = ((float) 2.0 * junction.normal) - (glm::vec3(eye.position) - glm::vec3(junction.position));
      ray reflectionRay = { reflectionDirection, junction.position + glm::vec4(reflectionDirection.x * EPSILON, reflectionDirection.y * EPSILON, reflectionDirection.z * EPSILON, 0)};
      retVal += viewVector[junction.view].shapes[junction.shape]->lightProperties.specular *
                  computeColor(lightVector, viewVector, eye, findNearestIntersection(viewVector, reflectionRay), recursionDepth + 1);
      }

   return glm::vec3(fminf(retVal.x, 255), fminf(retVal.y, 255), fminf(retVal.z, 255));
   }

//Compute lighting calculation given an intersection and light source
glm::vec3 computeLight (camera eye, light source, geometry &shape, intersection junction)
   {
   glm::vec3 vision = (1/eye.position.w) * glm::vec3(eye.position) - (1/junction.position.w) * glm::vec3(junction.position);
   glm::vec3 light;
   if (source.position.w) light  = (1/source.position.w) * glm::vec3(source.position) - (1/junction.position.w) * glm::vec3(junction.position);
   else light = ((float) -1.0) * glm::vec3(source.position);
   glm::vec3 halfvec = glm::normalize(glm::normalize(light) + glm::normalize(vision));

   float nDotL = glm::dot(glm::normalize(junction.normal), glm::normalize(light));

   glm::vec3 lambert = glm::vec3(source.color.x * shape.lightProperties.diffuse.x * fmaxf(nDotL, 0.0),
                                 source.color.y * shape.lightProperties.diffuse.y * fmaxf(nDotL, 0.0),
                                 source.color.z * shape.lightProperties.diffuse.z * fmaxf(nDotL, 0.0));

   float nDotH = glm::dot(glm::normalize(junction.normal), halfvec);

   glm::vec3 phong = glm::vec3(source.color.x * shape.lightProperties.specular.x * pow(fmaxf(nDotH, 0.0), shape.lightProperties.shininess),
                               source.color.y * shape.lightProperties.specular.y * pow(fmaxf(nDotH, 0.0), shape.lightProperties.shininess),
                               source.color.z * shape.lightProperties.specular.z * pow(fmaxf(nDotH, 0.0), shape.lightProperties.shininess));

   glm::vec3 retVal = lambert + phong;
   retVal = glm::vec3(fminf(retVal.x, 255), fminf(retVal.y, 255), fminf(retVal.z, 255));
   return retVal;
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
         junction.position = (1/junction.position.w) * junction.position;
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
