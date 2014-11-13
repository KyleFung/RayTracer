void parse(vector<view> &viewVector, vector<light> &lightVector, vector<vertex> &vertexVector, vector<material> &materialVector, camera &eye);
void setCamera(string line, camera &eye);
void createLight(string line, vector<light> &lightVector);
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
void changeShininess(string line, material &givenMaterial);

void parse(vector<view> &viewVector, vector<light> &lightVector, vector<vertex> &vertexVector, vector<material> &materialVector, camera &eye)
   { 
   stack<view> viewStack;
   view bottom; 
   viewStack.push(bottom);

   material defaultMaterial = {glm::vec3(1.0, 1.0, 1.0), glm::vec3(10, 10, 10), glm::vec3(20, 20, 20), 3.0};
   materialVector.push_back(defaultMaterial);
   bool shapeWasLastSeen = false;

   ifstream file;
   file.open ("shapes.txt");
   //if (!file.is_open()) return;

   string line;
   while(getline(file, line))
      {
      istringstream iss(line);
      string firstWord;
      iss >> firstWord;

      if ((firstWord == "ambient" || firstWord == "diffuse" || firstWord == "specular" || firstWord == "shininess") 
            && shapeWasLastSeen)
         {
         shapeWasLastSeen = false;
         materialVector.push_back(materialVector[materialVector.size() - 1]);
         }

      else if (firstWord == "tri" || firstWord == "sphere")
         {
         shapeWasLastSeen = true;
         }

      if (firstWord == "camera") setCamera(line, eye);
      else if (firstWord == "point") createLight(line, lightVector);
      else if (firstWord == "vertex") createVertex(line, vertexVector);
      else if (firstWord == "tri") addTriangle(line, viewStack, vertexVector, materialVector[materialVector.size() - 1]);
      else if (firstWord == "sphere") addSphere(line, viewStack, materialVector[materialVector.size() - 1]);
      else if (firstWord == "pushTransform") pushView(viewStack);
      else if (firstWord == "popTransform") popView(viewStack, viewVector);
      else if (firstWord == "scale") pushScale(line, viewStack);
      else if (firstWord == "translate") pushTranslate(line, viewStack);
      else if (firstWord == "rotate") pushRotate(line, viewStack);
      else if (firstWord == "ambient") changeAmbient(line, materialVector[materialVector.size() - 1]);
      else if (firstWord == "diffuse") changeDiffuse(line, materialVector[materialVector.size() - 1]);
      else if (firstWord == "specular") changeSpecular(line, materialVector[materialVector.size() - 1]);
      else if (firstWord == "shininess") changeShininess(line, materialVector[materialVector.size() - 1]);

      }

   viewVector.push_back(viewStack.top()); 
   file.close();
   }

void setCamera(string line, camera &eye)
   {
   cout << line << "\n";
   istringstream settings(line);
   string hold;
   float a, b, c;
   settings >> hold; 
   settings >> eye.position.x;
   settings >> eye.position.y;
   settings >> eye.position.z;
   settings >> a;
   settings >> b;
   settings >> c;
   settings >> eye.up.x;
   settings >> eye.up.y;
   settings >> eye.up.z;
   settings >> fov;

   eye.direction = glm::vec3(a, b, c) - glm::vec3(eye.position);
   eye.position.w = 1.0;
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

void addTriangle(string line, stack<view> &viewStack, vector<vertex> vertexVector, material givenMaterial)
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

   viewStack.top().shapes.push_back(new triangle(&vertexVector[A], &vertexVector[B], &vertexVector[C], givenMaterial));
   }

void addSphere(string line, stack<view> &viewStack, material givenMaterial)
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

   viewStack.top().shapes.push_back(new sphere(radius, position, givenMaterial));
   }

void pushScale(string line, stack<view> &viewStack)
   {
   cout << line << "\n";
   istringstream iss(line);
   string hold;
   float x, y, z;
   iss >> hold;
   iss >> x;
   iss >> y;
   iss >> z;

   viewStack.top().updateMatrix(viewStack.top().M * scale(x, y, z));
   }

void pushTranslate(string line, stack<view> &viewStack)
   {
   cout << line << "\n";
   istringstream iss(line);
   string hold;
   float x, y, z;
   iss >> hold;
   iss >> x;
   iss >> y;
   iss >> z;

   viewStack.top().updateMatrix(viewStack.top().M * translate(x, y, z));
   }

void pushRotate(string line, stack<view> &viewStack)
   {
   cout << line << "\n";
   istringstream iss(line);
   string hold;
   float x, y, z, theta;
   iss >> hold;
   iss >> x;
   iss >> y;
   iss >> z;
   iss >> theta;

   viewStack.top().updateMatrix(viewStack.top().M * glm::mat4(rotate(theta, glm::vec3(x, y, z))));
   }

void changeAmbient(string line, material &givenMaterial)
   {
   cout << line << "\n";
   istringstream iss(line);
   string hold;
   float x, y, z;
   iss >> hold;
   iss >> x;
   iss >> y;
   iss >> z; 

   givenMaterial.ambient = glm::vec3(x * 255, y * 255, z * 255);
   }

void changeDiffuse(string line, material &givenMaterial)
   {
   cout << line << "\n";
   istringstream iss(line);
   string hold;
   float x, y, z;
   iss >> hold;
   iss >> x;
   iss >> y;
   iss >> z; 

   givenMaterial.diffuse = glm::vec3(x * 255, y * 255, z * 255);
   }

void changeSpecular(string line, material &givenMaterial)
   {
   cout << line << "\n";
   istringstream iss(line);
   string hold;
   float x, y, z;
   iss >> hold;
   iss >> x;
   iss >> y;
   iss >> z; 

   givenMaterial.specular = glm::vec3(x * 255, y * 255, z * 255);
   }

void changeShininess(string line, material &givenMaterial)
   {
   cout << line << "\n";
   istringstream iss(line);
   string hold;
   float shininess;
   iss >> hold;
   iss >> shininess;

   givenMaterial.shininess = shininess; 
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
