void parse(vector<view> &viewVector, vector<light> &lightVector, vector<vertex> &vertexVector, vector<material> &materialVector, camera &eye);

void setCamera(string line, camera &eye);

void createLightPoint(string line, vector<light> &lightVector);

void createLightDirectional(string line, vector<light> &lightVector);

void createVertex(string line, vector<vertex> &vertexVector);

void addTriangle(string line, stack<view> &viewStack, vector<vertex> vertexVector, material givenMaterial);

void addSphere(string line, stack<view> &viewStack, material givenMaterial);

void pushView(stack<view> &viewStack);

void popView(stack<view> &viewStack, vector<view> &viewVector);

void pushScale(string line, stack<view> &viewStack);

void pushTranslate(string line, stack<view> &viewStack);

void pushRotate(string line, stack<view> &viewStack);

void changeAmbient(string line, material &givenMaterial);

void changeDiffuse(string line, material &givenMaterial);

void changeSpecular(string line, material &givenMaterial);

void changeEmissive(string line, material &givenMaterial);

void changeShininess(string line, material &givenMaterial);

void changeDepth(string line);

#include "parseOperations.cpp"
