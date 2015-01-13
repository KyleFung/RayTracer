ray generateRay (camera eye, int i, int j);

glm::vec3 computeColor (light sources[], int numSources, geometry * shapes[], int numShapes, int currentShape, camera eye, intersection junction, int recursionDepth);

glm::vec3 computeLight (camera eye, light source, geometry &shape, intersection junction);

intersection findNearestIntersection (vector<view> viewVector, ray beam);

#include "lightOperations.cpp"
