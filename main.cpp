//!Includes
#include <GL/glew.h>
#include <GL/glut.h>
#include <Shader.h>
#include <Vector.h>
#include <Matrix.h>
#include <Mesh.h>
#include <Texture.h>
#include <SphericalCameraManipulator.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <iomanip>


//!Function Prototypes
bool initGL(int argc, char** argv);
void display(void);
void keyboard(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void handleKeys();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void Timer(int value);
void initShader(); 			//shader function declaration
void initTexture(std::string filename, GLuint & textureID);
void drawTank();		//function to draw the tank
void drawTurret(); 	//function to draw the turret
void drawCrate(float crate_x, float crate_z);		//function to draw the crates
void drawCoin(float coin_x, float coin_z);		//function to draw the coins
void drawBall();		//function to draw the balls
void fireBall();		//fire ball from turret
void createMap();		//function to draw the map
void drawWheels();		//function to rotate the wheels
void updateTankPosition(); 		//function to update tank position;
void setMenu();
void menu(int option);
void render2dText(std::string text, float r, float g, float b, float x, float y);		//render text to screen
void gameInfo();			//print game info to screen
void setBallPosition();
void updateBallPosition();
void ballShot();		//limit the number of balls shot at one time
void reloadGame();

//! Screen size
int screenWidth   	        = 720;
int screenHeight   	        = 720;



//! Array of key states
bool keyStates[256];

//Global Variables
GLuint shaderProgramID;			                    // Shader Program ID
GLuint vertexPositionAttribute;		                // Vertex Position Attribute Location
GLuint vertexTexcoordAttribute;							// vertex Texcoord Attribute Location
GLuint TextureMapUniformLocation;					// texture Map Location
GLuint texture_tank, texture_crate, texture_coin, texture_ball; 	//OpenGL Texture
GLuint TimeUniformLocation;

Mesh chassis, turret, front_wheel, back_wheel, coin, ball, crate;		//mesh objects

float t_global = 0.0;
int level = 1;			//game starts at level 1
std::string sLevel;		//string to print to screen
bool pause = false;		//becomes true if game is paused
bool outOfTime = false; 	//becomes true if player runs out of time
bool levelComplete = false;		//becomes true when player completes current level
		
Vector3f tankPosition = Vector3f(0.0, 0.9, 0.0);			//current tank position
float tankVelocity = 0.0;			//tank velocity; updates with key presses
float wheelsRotationDegree = 0;		//rotation degree of the wheels
float tankRotation = 0;				//rotation of tank

Vector3f turretPosition = Vector3f(0.0, 0.0, 0.0);		//to calculate the rotation of the turret based on the mouse motion
float turretRotation = 0;		//rotation of the turret based on mouse movement
Vector3f tankCenter = chassis.getMeshCentroid();		//Mesh centroid of the tank

int totalCoins = 0;			//gets the total number of coins in the maze.
int coinScore = 0;			//to keep track of number of coins collected
float coinRotation = 0;

Vector3f ballPosition = Vector3f(0.0, 0.0, 0.0);		//get the position of the ball
float ballDirection; 		//stores cameraManip.getPan() to determine the direction of the ball being shot
bool ballFired = false;			//bool to see if ball is fired
int ballStorage[10]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};		//only 10 balls can be shot at a time

//timer
float timeLimit = 0;

//Viewing
SphericalCameraManipulator cameraManip;
Matrix4x4 ModelViewMatrix;		                    // ModelView Matrix
GLuint MVMatrixUniformLocation;		                // ModelView Matrix Uniform
Matrix4x4 ProjectionMatrix;		                    // Projection Matrix
GLuint ProjectionUniformLocation;	                // Projection Matrix Uniform Location


int Map[20][20];		//predefined 2d array to store map being read in from file.

void Level()
{
	switch (level)
	{
	case 1:
		sLevel = "../models/Map1.txt";
		timeLimit = 30.0;
		break;
	case 2:
		sLevel = "../models/Map2.txt";
		timeLimit = 80.0;
		break;
	case 3:
		sLevel = "../models/Map3.txt";
		timeLimit = 150.0;
		break;
	
	default:
		break;
	}

	std::cout << "Level: " << level << std::endl;
}

void initMap(std::string filename)
{
	std::ifstream mapfile(filename);
	if (!mapfile) {
		std::cerr << "Failed to open file." << std::endl;
		exit(0);
	}

	int rows, cols;

	mapfile >> rows >> cols;

	for (int i = 0 ; i < rows ; i++)
	{
		for (int j = 0 ; j < cols ; j++)
		{
			mapfile >> Map[i][j];
		}
	}

	mapfile.close();

	for(int k = 0; k < 20; k++){
        for(int l = 0; l < 20; l++){
            // std::cout << Map[k][l] << " ";
			if (Map[k][l] == 2)
			{
				totalCoins++;
			}
        }
    }
}

//! Main Program Entry
int main(int argc, char** argv)
{	
	//init OpenGL
	if(!initGL(argc, argv))
		return -1;

    //Init Key States to false;    
    for(int i = 0 ; i < 256; i++)
        keyStates[i] = false;
    
    //Set up your program
    
   	initShader();		//set up shader function
    
	initTexture("../models/hamvee.bmp", texture_tank);
	
	//Init Mesh Geometry
	chassis.loadOBJ("../models/chassis.obj");
	turret.loadOBJ("../models/turret.obj");
	front_wheel.loadOBJ("../models/front_wheel.obj");
	back_wheel.loadOBJ("../models/back_wheel.obj");
    
    
   //creating the maze boxes (crate)
   initTexture("../models/Crate.bmp", texture_crate);
   crate.loadOBJ("../models/cube.obj");

   //coin texture
   initTexture("../models/coin.bmp", texture_coin);
   coin.loadOBJ("../models/coin.obj");

   //ball texture
   initTexture("../models/ball.bmp", texture_ball);
   ball.loadOBJ("../models/ball.obj");

   //create menu
   setMenu();

	//initalize map
	Level();
	initMap(sLevel);

    //Init Camera Manipultor
	cameraManip.setPanTiltRadius(0.0f, -0.90f, 4.0f);

   //Enter main loop
	glutMainLoop();

    //Delete shader program
	glDeleteProgram(shaderProgramID);

    return 0;
}

//! Function to Initlise OpenGL
bool initGL(int argc, char** argv)
{
	//Init GLUT
    glutInit(&argc, argv);
    
	//Set Display Mode
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

	//Set Window Size
    glutInitWindowSize(screenWidth, screenHeight);
    
    // Window Position
    glutInitWindowPosition(200, 200);

	//Create Window
    glutCreateWindow("Tank Assignment");
    
    // Init GLEW
	if (glewInit() != GLEW_OK) 
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return false;
	}
	
	//Set Display function
    glutDisplayFunc(display);

	
	//Set Keyboard Interaction Functions
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyUp); 

	//Set Mouse Interaction Functions
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(motion);
	glutMotionFunc(motion);
	
	//Enable Texture2D
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    //Start start timer function after 100 milliseconds
    glutTimerFunc(100,Timer, 0);

	return true;
}

//Init Shader
void initShader()
{
	//Create shader
    shaderProgramID = Shader::LoadFromFile("shader.vert","shader.frag");
    
    // Get a handle for our vertex position buffer
	vertexPositionAttribute = glGetAttribLocation(shaderProgramID, "aVertexPosition");

	//Get Time Uniform Location
	TimeUniformLocation=glGetUniformLocation(shaderProgramID,"t_uniform");

    //!
	MVMatrixUniformLocation = glGetUniformLocation(shaderProgramID,     "MVMatrix_uniform"); 
	ProjectionUniformLocation = glGetUniformLocation(shaderProgramID,   "ProjMatrix_uniform"); 
	
	vertexTexcoordAttribute = glGetAttribLocation(shaderProgramID, "aVertexTexcoord");
	TextureMapUniformLocation = glGetUniformLocation(shaderProgramID, "TextureMap_uniform");
}


void initTexture(std::string filename, GLuint & textureID)
{
	glGenTextures(1, &textureID);						//generate texture
	glBindTexture(GL_TEXTURE_2D, textureID);		//make current texture
	
	//setting internal parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	//get texture data
	int width, height;
	char* data;
	Texture::LoadBMP(filename, width, height, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	//Cleanup data as copied to GPU
	delete[] data;
}


//! Display Loop
void display(void)
{
    //Handle keys
    handleKeys();

	//Set Viewport
	glViewport(0,0,screenWidth, screenHeight);

	glClearColor(0.53, 0.81, 0.92, 0.0);
	
	// Clear the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //Draw your scene
    
    //Use shader
	glUseProgram(shaderProgramID);
	
	//Set Color after program is in use
	glActiveTexture(GL_TEXTURE0);
	
    //Projection Matrix - Perspective Projection
    ProjectionMatrix.perspective(90, 1.0, 0.0001, 100.0);
   
    //Set Projection Matrix
    glUniformMatrix4fv(	
		ProjectionUniformLocation,  //Uniform location
		1,							//Number of Uniforms
		false,						//Transpose Matrix
		ProjectionMatrix.getPtr());	//Pointer to ModelViewMatrixValues
	

	//Apply Camera Manipluator to Set Model View Matrix on GPU
    ModelViewMatrix.toIdentity();
    ModelViewMatrix = cameraManip.apply(ModelViewMatrix);

	//set camera focus
	cameraManip.setFocus(tankPosition);


	//Increment Time and set uniform value
	t_global += 0.5;
	glUniform1f(TimeUniformLocation, t_global);
    
    //Call Draw Geometry Functions
	createMap();
	drawTank();
	drawTurret();
	drawWheels();
	updateTankPosition();

	updateBallPosition();
	reloadGame();

	//Unuse Shader
	glUseProgram(0);

	//print game info to screen
	gameInfo();
    
    //Swap Buffers and post redisplay
	glutSwapBuffers();
	glutPostRedisplay();
}


//! Keyboard Interaction
void keyboard(unsigned char key, int x, int y)
{
	//Quits program when esc is pressed
	if (key == 27)	//esc key code
	{
		exit(0);
	}
    //Set key status
    keyStates[key] = true;
}

//! Handle key up situation
void keyUp(unsigned char key, int x, int y)
{
	keyStates[key] = false;
}


//! Handle Keys
void handleKeys()
{
    //keys should be handled here
	if (!pause)
	{
		if(keyStates['w'] || keyStates['W'])
		{
			//move object forward and start accelerating
			tankVelocity += 0.001;
		}
		if(keyStates['a'] || keyStates['A'])
		{
			//rotate tank left
			tankRotation += 1.5;

		}
		if(keyStates['s'] || keyStates['S'])
		{
			//move object back and decelerate, then accelerate in negative direction
			tankVelocity -= 0.0015;
		}

		if(keyStates['d'] || keyStates['D'])
		{
			//rotate tank right
			tankRotation -= 1.5;
		}
		
		if(keyStates[' '])
		{
			setBallPosition();
		}
	}

	if (keyStates['f'] || keyStates['F'])
	{
		screenWidth = 1920;
		screenHeight = 1080;
		glViewport(0, 0, screenWidth, screenHeight);
		glutFullScreen();
	}

	if (keyStates['o'] || keyStates['O'])
	{
		screenWidth = 720;
		screenHeight = 720;
		glViewport(0, 0, screenWidth, screenHeight);
		glutReshapeWindow(720, 720);
	}
}

//! Mouse Interaction
void mouse(int button, int state, int x, int y)
{
	cameraManip.handleMouse(button, state, x, y);
   	glutPostRedisplay(); 
}

//! Motion
void motion(int x, int y)
{
	glutSetCursor(GLUT_CURSOR_NONE);		//makes cursor invisible inside window
    cameraManip.handleMouseMotion(x,0);		//y=0 => tilt does not change, only pan
    glutPostRedisplay();
}

//! Timer Function
void Timer(int value)
{
    //Call function again after 10 milli seconds
	glutTimerFunc(10,Timer, 0);

	if (!pause)		//timer doesnt increment if game is paused
	{
		timeLimit -= 0.02;
	}
}

//draw Tank function
void drawTank()
{
	glActiveTexture(GL_TEXTURE0);
   	glBindTexture(GL_TEXTURE_2D, texture_tank);
   	glUniform1i(TextureMapUniformLocation, 0);
	tankCenter = chassis.getMeshCentroid();

	Matrix4x4 chassisMatrix = ModelViewMatrix;
   	chassisMatrix.translate(tankPosition.x,tankPosition.y,tankPosition.z);
   	chassisMatrix.scale(0.20, 0.20, 0.20);

	//rotate whole tank when left and right arrow keys are pressed
	chassisMatrix.translate(tankCenter.x, tankCenter.y, tankCenter.z);
	chassisMatrix.rotate(tankRotation, 0, 1, 0);
	chassisMatrix.translate(-tankCenter.x, -tankCenter.y, -tankCenter.z);


	glUniformMatrix4fv(	
		MVMatrixUniformLocation,  	//Uniform location
		1,					        //Number of Uniforms
		false,				        //Transpose Matrix
   	chassisMatrix.getPtr());
   	
   	chassis.Draw(vertexPositionAttribute, -1, vertexTexcoordAttribute);
}

void updateTankPosition()
{
	if( tankVelocity > 0.0001 )		//moving in the positive direction (forwards)
	{
		tankVelocity -= 0.0005;
		wheelsRotationDegree += 100*sin(tankVelocity);		//WheelRotationDegree changes as speed changes
	}
	else if ( tankVelocity < -0.0001 )			//moving in the negative direction (backwards)
	{
		tankVelocity += 0.0005;
		wheelsRotationDegree -= - 100*sin(tankVelocity);			//WheelRotationDegree changes as speed changes
	}

	//terminal velocity
	if ( tankVelocity > 4.0 )
	{
		tankVelocity = 4.0;
	}
	else if ( tankVelocity < -4.0 )
	{
		tankVelocity = -4.0;
	}

	tankPosition.x += sin(tankRotation * (M_PI/180)) * tankVelocity;
	tankPosition.z += cos(tankRotation * (M_PI/180)) * tankVelocity;

	//check if tank is on crate
	int tankX = floor((tankPosition.x+1)/2);
	int tankZ = floor((tankPosition.z+1)/2);

	if ((Map[tankX][tankZ] != 1 && Map[tankX][tankZ] != 2) || tankPosition.y < 0.89)
	{
		tankPosition.y -= 0.1;
	}
}

void drawTurret()
{
	// activate and bind texture
   	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_tank);
	glUniform1i(TextureMapUniformLocation, 0);

	Matrix4x4 turretMatrix = ModelViewMatrix;
   	turretMatrix.translate(tankPosition.x,tankPosition.y,tankPosition.z);
   	turretMatrix.scale(0.20, 0.20, 0.20);
	
	if (!pause)		//doesn't update turret if paused.
	{
		turretRotation = cameraManip.getPan()*180 / M_PI;
	}
	
	turretMatrix.translate(tankCenter.x, tankCenter.y, tankCenter.z);
	turretMatrix.rotate(turretRotation, 0, 1, 0);
	turretMatrix.translate(-tankCenter.x, -tankCenter.y, -tankCenter.z);

	Vector3f turretCenter = turret.getMeshCentroid();

	glUniformMatrix4fv(	
		MVMatrixUniformLocation,  	//Uniform location
		1,					        //Number of Uniforms
		false,				        //Transpose Matrix
   	turretMatrix.getPtr());	 
	
	//draw turret
	turret.Draw(vertexPositionAttribute, -1, vertexTexcoordAttribute);
}

void drawCrate(float crate_x, float crate_z)
{
	//draw crate
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_crate);
	glUniform1i(TextureMapUniformLocation, 0);

	Matrix4x4 cubeMatrix = ModelViewMatrix;
	cubeMatrix.translate(crate_x,0,crate_z);
	glUniformMatrix4fv(	
	MVMatrixUniformLocation,  	//Uniform location
	1,					        //Number of Uniforms
	false,				        //Transpose Matrix
	cubeMatrix.getPtr());	
	crate.Draw(vertexPositionAttribute, -1, vertexTexcoordAttribute);
}

void drawCoin(float coin_x, float coin_z)
{
	//draw coin
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_coin);
	glUniform1i(TextureMapUniformLocation, 0);

	Matrix4x4 coinMatrix = ModelViewMatrix;
	Vector3f coinPosition = Vector3f(coin_x, 1.5, coin_z);
	coinMatrix.translate(coinPosition.x, coinPosition.y, coinPosition.z);
	coinMatrix.scale(0.25, 0.25, 0.25);

	//to rotate coins
	if (!pause)		//if the game is paused, coinRotation doesn't update
	{
		coinRotation = t_global;
	}
	coinMatrix.rotate(coinRotation, 0, 1, 0);
	
	
	glUniformMatrix4fv(	
		MVMatrixUniformLocation,  	//Uniform location
		1,					        //Number of Uniforms
		false,				        //Transpose Matrix
		coinMatrix.getPtr());	 
	coin.Draw(vertexPositionAttribute, -1, vertexTexcoordAttribute);
}

void drawBall()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_ball);
	glUniform1i(TextureMapUniformLocation, 0);
	
	Matrix4x4 ballMatrix = ModelViewMatrix;
	ballMatrix.translate(ballPosition.x, ballPosition.y, ballPosition.z);
	ballMatrix.scale(0.08, 0.08, 0.08);

	glUniformMatrix4fv(	
		MVMatrixUniformLocation,  	//Uniform location
		1,					        //Number of Uniforms
		false,				        //Transpose Matrix
   	ballMatrix.getPtr());	 
	
	//draw ball
	ball.Draw(vertexPositionAttribute, -1, vertexTexcoordAttribute);
}

void setBallPosition()
{
	ballDirection = turretRotation;

	ballPosition.x = tankPosition.x;
	ballPosition.y = 1.30;
	ballPosition.z = tankPosition.z;

	ballFired = true;
}

void updateBallPosition()
{
	if (pause)		//doesn't fire ball if game is paused
	{
		ballFired = false;
	}
	else if (ballFired)
	{
		ballPosition.x += 0.15 * sin(ballDirection * M_PI/180);
		ballPosition.z += 0.15 * cos(ballDirection * M_PI/180);

		//ball range wrt tank
		float BallDistanceTank = (ballPosition - tankPosition).length();
		if (BallDistanceTank > 5.8f)
		{
			ballFired = false;
			ballPosition = Vector3f(0.0, 0.0, 0.0);
		}
		drawBall();
	}
}

void createMap()
{
	for (int i = 0 ; i < 20 ; i++)
	{
		for (int j = 0 ; j < 20 ; j++)
		{
			if (Map[i][j] == 1)		
			{
				drawCrate(i*2, j*2);
			}
			else if (Map[i][j] == 2)
			{
				drawCrate(i*2, j*2);		// i and j * 2 cuz of the crate size; crate corners are (-1,-1) (1,-1) (1,1) (-1,1)

				drawCoin(i*2, j*2);
				Vector3f coinPosition = Vector3f(i*2, 1.5, j*2);

				// Check if tank collides with coin
				float CoinDistanceTank = (coinPosition - tankPosition).length();
				
				if (CoinDistanceTank < 1.1f)
				{
					Map[i][j] = 1;
					coinScore++;
				}

				float CoinDistanceBall = (coinPosition - ballPosition).length();

				if (CoinDistanceBall < 0.5f)
				{
					Map[i][j] = 1;
					coinScore++;

					ballFired = false;
					ballPosition = Vector3f(0.0, 0.0, 0.0);
				}
			}
		}
	}
}

//function for rotating wheels
void drawWheels()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_tank);
	glUniform1i(TextureMapUniformLocation, 0);
	
	//front wheels
	Matrix4x4 fWheelsMatrix = ModelViewMatrix;
	fWheelsMatrix.translate(tankPosition.x,tankPosition.y,tankPosition.z);
	fWheelsMatrix.scale(0.20, 0.20, 0.20);

	//to move wheels with tank rotation
	fWheelsMatrix.translate(tankCenter.x, tankCenter.y, tankCenter.z);
	fWheelsMatrix.rotate(tankRotation, 0, 1, 0);
	fWheelsMatrix.translate(-tankCenter.x, -tankCenter.y, -tankCenter.z);
	
	Vector3f fWheelC = front_wheel.getMeshCentroid();

	fWheelsMatrix.translate(fWheelC.x, fWheelC.y, fWheelC.z);
	fWheelsMatrix.rotate(wheelsRotationDegree, 1, 0, 0);
	fWheelsMatrix.translate(-fWheelC.x, -fWheelC.y, -fWheelC.z);
	
	glUniformMatrix4fv(	
		MVMatrixUniformLocation,  	//Uniform location
		1,					        //Number of Uniforms
		false,				        //Transpose Matrix
		fWheelsMatrix.getPtr());
	
	front_wheel.Draw(vertexPositionAttribute, -1, vertexTexcoordAttribute);
	
	
	//back wheels
	Matrix4x4 bWheelsMatrix = ModelViewMatrix;
	bWheelsMatrix.translate(tankPosition.x,tankPosition.y, tankPosition.z);
	bWheelsMatrix.scale(0.20, 0.20, 0.20);

	//to move wheels with tank rotation
	bWheelsMatrix.translate(tankCenter.x, tankCenter.y, tankCenter.z);
	bWheelsMatrix.rotate(tankRotation, 0, 0.85, 0);
	bWheelsMatrix.translate(-tankCenter.x, -tankCenter.y, -tankCenter.z);
	
	Vector3f bWheelC = back_wheel.getMeshCentroid();
	
	bWheelsMatrix.translate(bWheelC.x, bWheelC.y, bWheelC.z);
	bWheelsMatrix.rotate(wheelsRotationDegree, 1, 0, 0);
	bWheelsMatrix.translate(-bWheelC.x, -bWheelC.y, -bWheelC.z);

	glUniformMatrix4fv(	
		MVMatrixUniformLocation,  	//Uniform location
		1,					        //Number of Uniforms
		false,				        //Transpose Matrix
		bWheelsMatrix.getPtr());
	
   	back_wheel.Draw(vertexPositionAttribute, -1, vertexTexcoordAttribute);
}

void reloadGame()
{
	if(tankPosition.y < -8)		//restart game
	{
		//reset tank position to origin
		tankPosition.x = tankPosition.z = 0;
		tankPosition.y = 0.90;
		tankRotation = 0.0;
		tankVelocity = 0.0;
		turretRotation = 0.0;

		timeLimit = 0;			//restart game timer
		coinScore = 0;			//reset coins collected to zero
		totalCoins = 0;			//reset total coins in the maze to zero
		Level();
		initMap(sLevel);			//reload map
	}
	else if ((coinScore == totalCoins) && coinScore != 0)		//go to next level
	{
		if (level < 3)
		{
			level++;
		}
		else if (level > 3)
		{
			level = 1;
		}

		//reset tank position to origin
		tankPosition.x = tankPosition.z = 0;
		tankPosition.y = 0.90;
		tankRotation = 0.0;
		tankVelocity = 0.0;
		turretRotation = 0.0;

		timeLimit = 0;			//restart game timer
		coinScore = 0;			//reset coins collected to zero
		totalCoins = 0;			//reset total coins in the maze to zero
		Level();
		initMap(sLevel);			//load next map
		levelComplete = true;
		pause = true;
	}
	else if (timeLimit <= 0.0)		//if out of time 
	{
		//reset tank position to origin
		tankPosition.x = tankPosition.z = 0;
		tankPosition.y = 0.90;
		tankRotation = 0.0;
		tankVelocity = 0.0;
		turretRotation = 0.0;

		timeLimit = 0;			//restart game timer
		coinScore = 0;			//reset coins collected to zero
		totalCoins = 0;			//reset total coins in the maze to zero
		Level();
		initMap(sLevel);			//reload map
		outOfTime = true;
		pause = true;
	}
}

void render2dText(std::string text, float r, float g, float b, float x, float y)
{
	glColor3f(r,g,b);
	glRasterPos2f(x, y); // window coordinates
	for(unsigned int i = 0; i < text.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}
}

void gameInfo()
{
	//timer
	render2dText("Time: ", 0.0, 0.0, 0.0, 0.46, 0.92);
	std::stringstream tstream;
	tstream << std::fixed << std::setprecision(2) << timeLimit;
	std::string timer_string = tstream.str();
	render2dText(timer_string, 0.0, 0.0, 0.0, 0.835, 0.92);

	//coin score
	render2dText("Coins Collected: ", 0.0, 0.0, 0.0, 0.46, 0.82);
	std::string score = std::to_string(coinScore);
	render2dText(score, 0.0, 0.0, 0.0, 0.92, 0.82);

	//level
	render2dText("Level ", 0.0, 0.0, 0.0, -0.95, 0.92);
	std::string level_string = std::to_string(level);
	render2dText(level_string, 0.0, 0.0, 0.0, -0.75, 0.92);

	//out of time message
	if (outOfTime && pause)
	{
		render2dText("Out of time!", 0.0, 0.0, 0.0, -0.10, 0.0);
		render2dText("Choose 'Continue' or 'Exit' from menu.", 0.0, 0.0, 0.0, -0.50, -0.10);
	}

	//level completed message
	if (levelComplete)
	{
		render2dText("Level Completed!", 0.0, 0.0, 0.0, -0.10, 0.0);
		render2dText("Choose 'Continue' or 'Exit' from menu.", 0.0, 0.0, 0.0, -0.50, -0.10);
	}
}


void setMenu()
{
	int subMenu = glutCreateMenu(menu);

	glutAddMenuEntry("Pause", 1);
	glutAddMenuEntry("Continue", 2);
	glutAddMenuEntry("Exit" , 0);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

void menu(int option)
{
	switch (option){
		case 0:
			exit(0);
		case 1:
			pause = true;
			break;
		case 2:
			pause = false;
			outOfTime = false;
			levelComplete = false;
			break;
		default:
			break;
	}
}