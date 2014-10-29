void parse(stack<view> &viewStack, vector<light> &lightVector, vector<vertex> &vertexVector);
void setCamera(string line);
void createLight(string line, vector<light> &lightVector);
void createVertex(string line, vector<vertex> &vertexVector);
void addTriangle(string line, stack<view> &pViewStack);
void addSphere(string line, stack<view> &pViewStack);

void parse(stack<view> &viewStack, vector<light> &lightVector, vector<vertex> &vertexVector)
   {
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
      if (firstWord == "tri") addTriangle(line, viewStack);
      if (firstWord == "sphere") addSphere(line, viewStack);
      if (firstWord == "push");
      if (firstWord == "pop");

      //If statements to tell between numbers, shapes, and transforms
      //Register into stack arrays
      //IF comment THEN do nothing
      //IF camera THEN set camera
      //IF light THEN add light to list
      //IF maxverts THEN size up vertex array
      //IF vertex THEN initialize next vertex in array
      //IF tri THEN initialize triangle in geometry array
      //IF sphere THEN initialize sphere in geometry array
      //IF push THEN push a copy of top to stack 
      //IF pop THEN push top to context stack and pop top  
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

void addTriangle(string line, stack<view> &viewStack)
   {
   cout << line << "\n";
   //TODO add this in after rearchitecting triangle
   //triangle shape = new triangle(); 
   
   istringstream iss(line);
   string hold;
   iss >> hold;
   }

void addSphere(string line, stack<view> &viewStack)
   {
   cout << line << "\n";
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
