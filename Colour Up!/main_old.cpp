
#include "KeyboardDefinitions.h";	// Header File for Keyboard Definitions
#include <windows.h>				// Header File For Windows
#include "Image_Loading/nvImage.h"
#include <gl\gl.h>					// Header File For The OpenGL32 Library
#include <gl\glu.h>	
#include "GameObject.h"
#include "BoundingBox.h"
#include "CollidableObject.h"
#include "Math/Point2.h"
#include "Math/Point4.h"
#include "Player.h"
#include "console.h"				//Header File for Console	
#include "Colour.h"
#include "Clock.h"
#include <vector>
#include "ActivityManager.h"
#include "StartGame.h"


//*****************FUNCTIONS****************//
void				checkForCollision(const double);
void				updateKeys();
void				spinDisplay();
void				updateBlending();
void				init();
void				reshape(int width, int height);
void				drawGrid();
void				createLevel();
void				createPlayer();
void				restart();
//******************VARIABLES****************//
int					screenWidth=1000;
int					screenHeight=1000;
int					displaySize = 100;
bool				keys[256];						//Stores the state of the keys (pressed/not pressed)
bool				LeftPressed = false;			//Left Mouse Click
int					mouse_x=0, mouse_y=0;
ActivityManager			activityManager;
vector<CollidableObject> obstacles;

enum GameState{
		START, PLAY, GAMEOVER
	};

//OTHER 
const int			FPS = 60;
const double		TIME_PER_FRAME = 1.f / FPS;
Player				player;
Point2f				startingPosition = Point2<float>(40, 10);
Point2f				dimensionsHorizontal;
Point2f				dimensionsVertical;

ConsoleWindow		console;
Color				bgColor = WHITE;


GameObject startEndScreens[2];

GameState gameState;

/****DEBUG OUTPUTS****/
//#define displayFPS
//#define pSpeed
//#define pCollision
//#define pPosition


// This stores a handle to the texture
GLuint characterTex = 0;
GLuint groundTex = 0;


void createLevel(){

	//DEPENDS ON THE LEVEL I GUESS
	bgColor = WHITE;
	//FRAME
	float floorW = 200;
	float floorH = 20;
	dimensionsHorizontal = Point2<float>(floorW, floorH);
	dimensionsVertical = Point2<float>(floorH, floorW);

	groundTex = loadPNG("death.png");
	//groundTex = characterTex;
		
	CollidableObject floor = CollidableObject(Point2f(400,20), Point2<float>(100, -100), CollidableObject::PLATFORM);
	floor.setColour(GREY);
	obstacles.push_back(floor);

	CollidableObject floorDEATH = CollidableObject(Point2f(400, 20), Point2<float>(100, -120), CollidableObject::DEADLYPLATFORM);
	floorDEATH.setColour(BLACK);
	floorDEATH.setTexture(groundTex);
	obstacles.push_back(floorDEATH);

	CollidableObject ceiling = CollidableObject(dimensionsHorizontal, Point2<float>(0, 100), CollidableObject::PLATFORM);
	ceiling.setColour(GREY);
	//obstacles.push_back(ceiling);

	CollidableObject wallLeft = CollidableObject(dimensionsVertical, Point2<float>(-100, 0), CollidableObject::PLATFORM);
	wallLeft.setColour(GREY);
	obstacles.push_back(wallLeft);


	CollidableObject wallRight = CollidableObject(dimensionsVertical, Point2<float>(100, 0), CollidableObject::PLATFORM);
	wallRight.setColour(GREY);
	//obstacles.push_back(wallRight);


	//PLATFORMS
	CollidableObject platform1 = CollidableObject(Point2f(60, 20), Point2<float>(0, -40), CollidableObject::PLATFORM);
	platform1.setColour(BLUE);
	obstacles.push_back(platform1);

	//PLATFORMS
	CollidableObject platform2 = CollidableObject(Point2f(60, 20), Point2<float>(100, -20), CollidableObject::PLATFORM);
	platform2.setColour(BLUE);
	obstacles.push_back(platform2);

	CollidableObject platform3 = CollidableObject(Point2f(60, 20), Point2<float>(220,10), CollidableObject::PLATFORM);
	platform3.setColour(BLUE);
	obstacles.push_back(platform3);


	CollidableObject platform4 = CollidableObject(Point2f(60, 20), Point2<float>(120, 70), CollidableObject::PLATFORM);
	platform4.setColour(GREY);
	obstacles.push_back(platform4);




}


void createPlayer(void){
	player = Player(Point2<float>(20, 20), startingPosition);
	player.textures[0] = loadPNG("CharacterIdle.png");
	player.textures[1] = loadPNG("CharacterLeft.png");
	player.textures[2] = loadPNG("CharacterRight.png");
 
		
	player.setTexture(player.textures[0]);
}

void init(void)
{
	activityManager.top()->init();
	
}

//GAME OVER RESTART THE WHOLE GAME
void restart(void){
	
	gameState = PLAY;
	createPlayer();
	createLevel();

}

//Rebuild level, reset players position
void playerDied(void){
	createLevel();
	player.reset(startingPosition);

}


void display(void)
{
	activityManager.top()->draw();
}

void update(const double dt){
	activityManager.update();
	activityManager.top()->update(dt);

}

void checkForCollision(const double dt){

	float mod = 0.001;

	//flags for assessing the direction of collision
	bool checkLeft;
	bool checkRight;
	bool checkTop;
	bool checkBottom;

	//reset collision contact flags for player
	player.contactTop = player.contactBottom = player.contactLeft = player.contactRight = false;
	
	//initialise distance by which player will be pushed to avoid collision (+ direction)
	float pushDistLeft, pushDistRight, pushDistUp, pushDistDown;
	pushDistLeft = pushDistRight = pushDistUp = pushDistDown = 0;

	

	//CREATE TEMP BOUNDING BOX
	BoundingBox bbtemp = BoundingBox(player.bB.width, player.bB.height);
	


	//LOOP THROUGH ALL OBSTACLES
	for (int i = 0; i < obstacles.size(); i++){

		bbtemp.x = player.x;
		bbtemp.y = player.y;

		//TRANSLATE BY TEMP NEW SPEED
		bbtemp.translate(player.newSpeedX, player.newSpeedY, dt);

		//create an alias
		BoundingBox &otherBB = obstacles[i].bB;

		//CHECK FOR COLLISION (PROVIDING THE OBSTACLE IS NOT BLENDED)
		if (bbtemp.collide(otherBB) && !obstacles[i].blended)
			{
			
				/*
				
				checkRight =	(player.bB.maxX	>=	obstacles[i].bB.minX)	&&	(player.bB.maxX		<= obstacles[i].bB.maxX);
				checkLeft =		(player.bB.minX	<=	obstacles[i].bB.maxX)	&&	(player.bB.minX	>=	obstacles[i].bB.minX);
				checkTop =		(player.bB.maxY		>=	obstacles[i].bB.minY)	&&	(player.bB.maxY	<=	obstacles[i].bB.maxY);		
				checkBottom =	(player.bB.minY	<=	obstacles[i].bB.maxY)	&&	(player.bB.minY	>=	obstacles[i].bB.minY);
				*/
				

				//get the minimal distance to push player out of object
				pushDistRight =		abs(bbtemp.minX - otherBB.maxX) + mod;
				pushDistLeft =		(abs(bbtemp.maxX - otherBB.minX) + mod);
				pushDistDown =		(abs(bbtemp.maxY - otherBB.minY) + mod);
				pushDistUp =		abs(bbtemp.minY - otherBB.maxY) + mod;
				
			
				/*
				//GET THE DIRECTION OF THE CONTACT
			
			//BOTTOM EXCLUSIVE COLLISION
			if ((checkLeft	&&	checkRight	&&	checkBottom && !checkTop)){
				player.contactBottom = true;
			}
			
			//TOP EXCLUSIVE COLLISION
			if ((checkLeft	&&	checkRight	&&	checkTop && !checkBottom)){
				player.contactTop = true;
			}

			//LEFT EXCLUSIVE COLLISION
			if (checkLeft&&checkTop&&checkBottom){
				player.contactLeft = true;
			}

			//RIGHT EXCLUSIVE COLLISION
			if(checkRight&&checkTop&&checkBottom){
				player.contactRight = true;

			}

			//CORNER CASES
			//TOP RIGHT CORNER (BOTTOM OR LEFT COLLISION)
			if(checkBottom	&&	checkLeft && !checkTop	&&	!checkRight){

				//left
				if(pushDistRight <= pushDistUp){
					player.contactLeft = true;
				}
				else{ //bottom
					player.contactBottom = true;
				}
			}

			//TOP LEFT CORNER (BOTTOM OR RIGHT COLLISION)
			if(checkBottom	&&	checkRight && !checkTop	&&	!checkLeft){

				//right
				if(pushDistLeft <= pushDistUp){
					player.contactRight = true;
				}
				else{ //bottom
					player.contactBottom = true;
					}
			}

			//BOTTOM LEFT CORNER (TOP OR RIGHT COLLISION)
			if(checkTop	&&	checkRight && !checkBottom	&&	!checkLeft){

				//right
				if(pushDistLeft <= pushDistDown){
					player.contactRight = true;
				}
				else{ //top
					player.contactTop = true;
				}
			}

			//BOTTOM RIGHT CORNER (TOP OR LEFT COLLISION)
			if(checkTop	&&	checkLeft && !checkBottom	&&	!checkRight){

				//LEFT
				if(pushDistRight <= pushDistDown){
					player.contactLeft = true;
				}
				else{ //top
					player.contactTop = true;
					}
			}
			
			*/

			//MODIFY SPEED

				switch(obstacles[i].platformType){

					case CollidableObject::PLATFORM:
						player.modifySpeed(pushDistRight, pushDistLeft, pushDistDown, pushDistUp, dt);
					break;

					case CollidableObject::DEADLYPLATFORM:
						player.modifySpeed(pushDistRight, pushDistLeft, pushDistDown, pushDistUp, dt);
						player.die();
					break;
				}
				
			}

				

		}
}




void updateBlending(){
	setBGColour(bgColor);
	for (int i = 0; i < obstacles.size(); i++){
		obstacles[i].checkBlending(bgColor);
	}
}

void updateKeys(){
	//TODO CHECK GAME STATE
	//if (playGame)

	
	//********ARROW KEYS INPUT****************//

	//LEFT


	switch(gameState){
		case START:
			if(keys[VK_SPACE]){
				gameState = PLAY;
			
			}

		break;

		case PLAY:
		
				if (keys[VK_LEFT]){
					player.moveRequestLeft = true;
				}
				else{
					player.moveRequestLeft = false;
				}
				//RIGHT
				if (keys[VK_RIGHT]){
					player.moveRequestRight = true;
				}
				else{
					player.moveRequestRight = false;
				}
	


				////JUMP (UP)
				if (keys[VK_UP]){
					//if the player is allowed to jump send the request to jump
					if(player.allowedToJump)
					{
						player.allowedToJump = false;
						player.moveRequestUp = true;			
					}
					else{		//if the player is not allowed don't send the request
			
						player.moveRequestUp = false;
					}
				}
				else{		//if the key is released allow the player to jump again and stop jumping
					player.allowedToJump = true;
					player.moveRequestUp = false;
		
				}


	
	

				if (keys[VK_SPACE]){
					//		
				}
				//DOWN
				if (keys[VK_DOWN]){
					player.moveRequestDown = true;
				}
				else{
					player.moveRequestDown = false;
				}


				if (keys[VK_1]){
					bgColor = RED;
				}
				if (keys[VK_2]){
					bgColor = GREEN;
				}
				if (keys[VK_3]){
					bgColor = BLUE;
				}
				if (keys[VK_4]){
					bgColor = BLACK;
				}
				if (keys[VK_5]){
					bgColor = WHITE;
				}
				if (keys[VK_6]){
					bgColor = GREY;
				}
				if (keys[VK_7]){
					bgColor = BROWN;
				}
				if (keys[VK_8]){
					bgColor = PURPLE;
				}
				if (keys[VK_9]){
					bgColor = PINK;
				}
	
			break;
		case GAMEOVER:
			if(keys[VK_SPACE]){
				restart();
			
			}
	
	}
}

void reshape(int w, int h)
{
	screenWidth=w;
	screenHeight=h;
		
	int size = 0;
	int wStart = 0;
	int hStart = 0;
	if (w>h){
		size = h;
		wStart = wStart + (w-h)/2;
	}
	else{
		size = w;
		hStart = hStart + (h - w)/2;
	}

	//glViewport(wStart,hStart,(GLsizei) size, (GLsizei) size);
	

	glViewport(0,0,(GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	//specifying the coordinate system 
	//-50 to 50 in the x axis
	//-50 to 50 in the y axis
	gluOrtho2D(-200, 200, -200, 200);
	
	//double aspectRatio = w/(double)h;
	//gluOrtho2D(-displaySize, displaySize, -displaySize/aspectRatio, displaySize/aspectRatio); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void drawGrid(){
	glPushMatrix();
	glTranslated(-displaySize, -displaySize, 0);
	glBegin(GL_LINES);
	for (int i = 0; i <=screenWidth; i += 10)
	{
		glVertex3f((float)i, 0.0f, 0.0f);
		glVertex3f((float)i, screenWidth, 0.0f);
		glVertex3f(0.0f, (float)i, 0.0f);
		glVertex3f(screenWidth, (float)i, 0.0f);
	}
	glEnd();
	glPopMatrix();
}

#pragma region WIN32

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(	HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

//win32 global variabless
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	bool	done=false;								// Bool Variable To Exit Loop

	StartGame n = StartGame();
	activityManager.push(&n);
	console.Open();

	// Create Our OpenGL Window
	if (!CreateGLWindow("OpenGL Win32 Example",screenWidth,screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	Clock clock;
	double timeSinceLastUpdate = 0.0;

	Clock fpsCounterClock;
	double fpsCounterTime = 0.0;
	int fpsCounter = 0;

	clock.start();
	fpsCounterClock.start();

	//*******************************************GAME LOOP***********************************************************//

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if(keys[VK_ESCAPE])
				done = true;

	
			if (!keys[VK_RETURN]){

				fpsCounterTime += fpsCounterClock.restart();

				timeSinceLastUpdate += clock.restart();	

				while (timeSinceLastUpdate > TIME_PER_FRAME) {
						
						fpsCounter++;
						timeSinceLastUpdate -= TIME_PER_FRAME;
						update(TIME_PER_FRAME);
				}

				
				if (fpsCounterTime > 1){
#ifdef displayFPS
					std::cout<<"\t\t\t\t\tFPS: "<<fpsCounter<<std::endl;
#endif
					fpsCounterTime = 0;
					
					fpsCounter = 0;
				}
				
			}
			display();					// Draw The Scene

			SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
		}
	}



	console.Close();

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}
		break;

		case WM_SIZE:								// Resize The OpenGL Window
		{
			reshape(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		break;

		case WM_LBUTTONDOWN:
			{
			activityManager.top()->mouseXY.x = LOWORD(lParam);
			activityManager.top()->mouseXY.y = screenHeight - HIWORD(lParam);
			activityManager.top()->LMBPressed = true;
			}
		break;

		case WM_LBUTTONUP:
			{
			activityManager.top()->LMBPressed = false;
			}
		break;

		case WM_MOUSEMOVE:
			{
			activityManager.top()->mouseXY.x = LOWORD(lParam);
			activityManager.top()->mouseXY.y = screenHeight - HIWORD(lParam);
			}
		break;
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			activityManager.top()->keys[wParam] = true;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}
		break;
		case WM_KEYUP:								// Has A Key Been Released?
		{
			activityManager.top()->keys[wParam] = false;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}
		break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*/
 
bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}
	
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								900, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();
	
	return true;									// Success
}

#pragma endregion WIN32 stuff

