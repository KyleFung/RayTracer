//---------------------------------------------------------------
// context class
#include "glm/glm.hpp"

class context 
   {
   public:
   glm::mat4 M;
   glm::mat4 M_inv; 
   glm::mat4 M_T;
   glm::mat4 M_T_inv;
 
   context ();
   void updateMatrix(glm::mat4);
   glm::mat4 pushTransform(glm::mat4);
   };

context::context()
   {
   M = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
   M_inv = M;
   M_T = M;
   M_T_inv = M;
   }

void context::updateMatrix(glm::mat4 newM)
   {
   M = newM;
   M_inv = glm::inverse(newM);
   M_T = glm::transpose(newM);
   M_T_inv = glm::inverse(M_T);
   }

glm::mat4 context::pushTransform(glm::mat4 transform)
   {
   return transform * M; 
   }
