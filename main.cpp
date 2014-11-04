#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <stack>
#include <cmath>
#include "glm/glm.hpp"
#include "variables.hpp"
#include "geometry.hpp"
#include "view.cpp"
#include "transforms.cpp"
#include "lightOperations.cpp"
#include "parseOperations.cpp"

using namespace std;

int main ()
   {
   vector<view> viewVector;
   vector<light> lightVector;
   vector<vertex> vertexVector;
   parse(viewVector, lightVector, vertexVector);

   ofstream FH;
   FH.open ("image.ppm");
   FH << "P3" << endl << width << " " << height << endl << "255" << endl;

   sphere ball = sphere (1.0, glm::vec4(0,0,2,1));
   ball.setReflectance(glm::vec3(1,1,1), glm::vec3(1,1,1), glm::vec3(0,0,10), 50);
   
   //sphere ball2 = sphere (2.0, glm::vec4(0,2,3,1));
   //ball2.setReflectance(glm::vec3(1,1,1), glm::vec3(1,1,1), glm::vec3(0.5,0.5,0.5), 50);

   //triangle shape = triangle(glm::vec4(-1,-1,1,1), glm::vec4(1,1,1.5,1), glm::vec4(0,2,2,1));
   //viewStack.top().shapes[0]->setReflectance(glm::vec3(1,1,1), glm::vec3(1,1,1), glm::vec3(0,0,10), 50);
   
   camera eye = {glm::vec4(0,0,0,1), glm::vec3(0,0,1), glm::vec3(0,1,0), 0.785398163};
   light source = {glm::vec3(150,255,255), glm::vec4(20,20,10,1)};
   light source2 = {glm::vec3(0,255,255), glm::vec4(-20,20,-10,1)};
   
   int numShapes = 1;
   geometry ** shapes;
   shapes = new geometry*[numShapes];

   shapes[0] = &ball;
   //shapes[0] = viewStack.top().shapes[0];
   //shapes[1] = &shape;

   int numLights = 2;
   light * lights;
   lights = new light[numLights];
   lights[0] = source;
   lights[1] = source2;

   int **pixels = new int*[height];
   for(int i = 0; i < height; ++i)
      {
      pixels[i] = new int[width * 3];
      }

   for (int j = 0; j < height; j++)
      {
      for (int i = 0; i < width; i++)
         {
         ray beam = generateRay (eye, i, j);
         int nearestIndex = findNearestObject(shapes, numShapes, beam);
         // case where no shapes are hit
         if (nearestIndex == -1)
            {
            pixels[j][i * 3] = 0;
            pixels[j][i * 3 + 1] = 0;
            pixels[j][i * 3 + 2] = 0;
            }
         // case where a shape is hit
         else 
            {
            glm::vec3 color = computeColor (lights, numLights, shapes, numShapes, nearestIndex, beam, eye, shapes[nearestIndex]->Intersection(beam));
            pixels[j][i * 3] = color.x;
            pixels[j][i * 3 + 1] = color.y;
            pixels[j][i * 3 + 2] = color.z;
            }
         }
      }

   for (int i = 0; i < height; i++)
      {
      for (int j = 0; j < width * 3; j++)
         {
         FH << pixels[i][j] << " ";
         }
      }

   FH.close();
   //system("PAUSE");
   return 1;
   }
