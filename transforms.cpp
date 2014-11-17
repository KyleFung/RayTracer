glm::mat3 rotate(float degrees, glm::vec3 axis)
   {
   glm::mat3 ret;

   float theta = degrees * 0.0174532925;

   glm::vec3 u = glm::normalize(axis);

   glm::vec3 v;

   if(u != glm::vec3(1.0, 0.0, 0.0))
      v = glm::normalize(glm::cross(glm::vec3(1.0, 0.0, 0.0), u));
   else
      v = glm::normalize(glm::cross(glm::vec3(0.0, 1.0, 0.0), u));

   glm::vec3 w = glm::cross(u, v);

   glm::mat3 Frame = glm::mat3(u.x, v.x, w.x, u.y, v.y, w.y, u.z, v.z, w.z);

   glm::mat3 rotation = glm::mat3(1.0, 0.0, 0.0, 0.0, cos(theta), sin(theta), 0.0, (-1.0)*sin(theta), cos(theta));

   glm::mat3 invFrame = glm::inverse(Frame);

   ret = invFrame * rotation * Frame;

   return ret;
   }

glm::mat4 scale(float sx, float sy, float sz)
   {
   glm::mat4 ret;
   ret = glm::mat4(sx, 0.0, 0.0, 0.0, 0.0, sy, 0.0, 0.0, 0.0, 0.0, sz, 0.0, 0.0, 0.0, 0.0, 1.0);
   return ret;
   }

glm::mat4 translate(float tx, float ty, float tz)
   {
   glm::mat4 ret;
   ret = glm::mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, tx, ty, tz, 1.0);
   return ret;
   }
