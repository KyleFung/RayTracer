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
   //Setup necessary vectors and populate them
   vector<view> viewVector;
   vector<light> lightVector;
   vector<vertex> vertexVector;
   vector<material> materialVector;
   camera eye;
   parse(viewVector, lightVector, vertexVector, materialVector, eye);

   eye.fovy = 0.785398163;

   //Initialize pixel buffer
   int **pixels = new int*[height];
   for(int i = 0; i < height; ++i)
      {
      pixels[i] = new int[width * 3];
      }

   //Color each pixel
   for (int j = 0; j < height; j++)
      {
      for (int i = 0; i < width; i++)
         {
         ray beam = generateRay (eye, i, j); 
         glm::vec3 color = computeColor(lightVector, viewVector, eye, findNearestIntersection(viewVector, beam));
         pixels[j][i * 3] = color.x;
         pixels[j][i * 3 + 1] = color.y;
         pixels[j][i * 3 + 2] = color.z; 
         }
      }

   //Write pixel buffer to ppm file
   ofstream FH;
   FH.open ("image.ppm");
   FH << "P3" << endl << width << " " << height << endl << "255" << endl; 

   for (int i = 0; i < height; i++)
      {
      for (int j = 0; j < width * 3; j++)
         {
         FH << pixels[i][j] << " ";
         }
      }

   FH.close();
   return 1;
   }
