void parse(vector<view> &viewVector, vector<light> &lightVector, vector<vertex> &vertexVector);
void setCamera(string line);
void createLight(string line, vector<light> &lightVector);
void createVertex(string line, vector<vertex> &vertexVector);
void addTriangle(string line, stack<view> &viewStack, vector<vertex> vertexVector);
void addSphere(string line, stack<view> &viewStack);
void pushView(stack<view> &viewStack);
void popView(stack<view> &viewStack, vector<view> &viewVector);

void parse(vector<view> &viewVector, vector<light> &lightVector, vector<vertex> &vertexVector)
   {
   stack<view> viewStack;
   view bottom;
   viewStack.push(bottom);
   
   ifstream file;
   file.open ("shapes.txt");
   //if (!file.is_open()) return;

   string line;
   while(getline(file, line))
      {
      istringstream iss(line);
      string firstWord;
      iss >> firstWord;

      if (firstWord == "camera") setCamera(line); 
      if (firstWord == "point") createLight(line, lightVector);
      if (firstWord == "vertex") createVertex(line, vertexVector);
      if (firstWord == "tri") addTriangle(line, viewStack, vertexVector);
      if (firstWord == "sphere") addSphere(line, viewStack);
      if (firstWord == "pushTransform") pushView(viewStack);
      if (firstWord == "popTransform") popView(viewStack, viewVector);

      }
   file.close(); 
   }

void setCamera(string line)
   {
   cout << line << "\n";
   istringstream settings(line);
   string hold;
   settings >> hold; 
   settings >> eye.x;
   settings >> eye.y;
   settings >> eye.z;
   settings >> direction.x;
   settings >> direction.y;
   settings >> direction.z;
   settings >> up.x;
   settings >> up.y;
   settings >> up.z;
   settings >> fov;
   eye.w = 1.0;
   }

void createLight(string line, vector<light> &lightVector)
   {
   cout << line << "\n";
   istringstream settings(line);
   light point;
   string hold;
   settings >> hold;
   settings >> point.position.x;
   settings >> point.position.y;
   settings >> point.position.z;
   settings >> point.color.x;
   settings >> point.color.y;
   settings >> point.color.z;

   lightVector.push_back(point);
   }

void createVertex(string line, vector<vertex> &vertexVector)
   {
   cout << line << "\n";
   istringstream iss(line);
   string hold;
   iss >> hold; 

   vertex addedVertex;
   iss >> addedVertex.position.x;
   iss >> addedVertex.position.y;
   iss >> addedVertex.position.z;
   addedVertex.position.w = 1;

   vertexVector.push_back(addedVertex);
   }

void addTriangle(string line, stack<view> &viewStack, vector<vertex> vertexVector)
   {
   istringstream iss(line);
   string hold;
   int A;
   int B;
   int C;
   iss >> hold;
   iss >> A;
   iss >> B;
   iss >> C;

   viewStack.top().shapes.push_back(new triangle(&vertexVector[A], &vertexVector[B], &vertexVector[C]));
   }

void addSphere(string line, stack<view> &viewStack)
   {
   glm::vec4 position;
   float radius;
   istringstream iss(line);
   string hold;
   iss >> hold;
   iss >> position.x;
   iss >> position.y;
   iss >> position.z;
   iss >> radius;
   position.w = 1;

   viewStack.top().shapes.push_back(new sphere(radius, position));
   }

void pushView(stack<view> &viewStack)
   {
   cout << "push" << "\n";
   viewStack.push(viewStack.top());
   }

void popView(stack<view> &viewStack, vector<view> &viewVector)
   {
   cout << "pop" << "\n";
   viewVector.push_back(viewStack.top());
   viewStack.pop();
   }
