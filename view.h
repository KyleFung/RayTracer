class view
   {
   public:
   glm::mat4 M;
   glm::mat4 M_inv;
   glm::mat4 M_T;
   glm::mat4 M_T_inv;
   std::vector<geometry *> shapes;

   view();
   void updateMatrix(glm::mat4);
   glm::mat4 pushTransform(glm::mat4);
   ~view();
   };

#include "view.cpp"
