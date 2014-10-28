void parse(geometry ***viewIndex, light *lights, vertex *vertices);
void setCamera(string line);
void createLight(string line);
void createVertex(string line, vertex *vertices, int *index);
void addTriangle(string line, geometry **shapeIndex);
void addSphere(string line, geometry **shapeIndex, int *currentShape);

void parse(geometry ***viewIndex, light *lights, vertex *vertices)
   {
   ifstream file;
   file.open ("shapes.txt");
   //if (!file.is_open()) return;
   int currentVert = 0;
   int currentView = 0;
   int currentShape = 0;
   string line;
   while(getline(file, line))
      {
      istringstream iss(line);
      string firstWord;
      iss >> firstWord;

      if (firstWord == "camera") setCamera(line); 
      if (firstWord == "point") createLight(line);
      if (firstWord == "vertex") createVertex(line, vertices, &currentVert);
      if (firstWord == "tri");
      if (firstWord == "sphere") addSphere(line, viewIndex[currentView], &currentShape);
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

void createLight(string line)
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
   }

void createVertex(string line, vertex *vertices, int *index)
   {
   cout << line << "\n";
   istringstream iss(line);
   string hold;
   iss >> hold; 
   iss >> vertices[*index].position.x;
   iss >> vertices[*index].position.y;
   iss >> vertices[*index].position.z;
   vertices[*index].position.w = 1;
   *index = *index + 1;
   }

void addTriangle(string line, geometry **shapeIndex)
   {
   cout << line << "\n";
   //TODO add this in after rearchitecting triangle
   //triangle shape = new triangle(); 
   
   istringstream iss(line);
   string hold;
   iss >> hold;
   }

void addSphere(string line, geometry **shapeIndex, int *currentShape)
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

   shapeIndex[*currentShape] = new sphere(radius, position);
   *currentShape += 1;
   }
