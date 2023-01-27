#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

//Global Variable as they are declared outside of any function, these can be accessed throughout the program anytime we need to know display size
int DISPLAY_WIDTH = 1280; //Global Variable that Stores Display Width
int DISPLAY_HEIGHT = 720; //Global Variable that Stores Display Height
int DISPLAY_SCALE = 1; //Global Variable that Stores Scale of Display

struct GameState
{
	float timer = 0;
	int spriteId = 0;
	int score = 0;
};

GameState gameState;

enum GameObjectType
{
	TYPE_NULL = -1,
	TYPE_AGENT8,
};

void HandlePlayerControls();

// The entry point for a PlayBuffer program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE ) //Allows Access to all functions inside Play.h
{
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );// Using Play:: Namespace Runs Create Manager, Passing Global Display Variables into Function
	Play::CentreAllSpriteOrigins(); // Centres sprite on mouse so that player isnt holding top left corner of sprite
	Play::LoadBackground("Data\\Backgrounds\\background.png"); //Loads desired Background image
	//Play::StartAudioLoop("music"); //Plays Game music plays first audio file it can find in Data\\Audio
	Play::CreateGameObject(TYPE_AGENT8, { 115, 100 }, 50, "agent8"); //Creates Character at Location, defines object collision size and what the name of the obj is
}

// Called by PlayBuffer every frame (60 times a second!) Dependant on FPS I assume this is 60fps Each frame lasts 17 milliseconds
bool MainGameUpdate( float elapsedTime )
{
	gameState.timer += elapsedTime; //Sets timer to current elapsed time
	//Play::ClearDrawingBuffer( Play::cBlack ); //Erases the ClearDrawingBuffer Each Frame and then creates a box in desired colour replaced by DrawBackground and both reset the viewport

	Play::DrawDebugText({ DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 },
						Play::GetSpriteName(gameState.spriteId), //Prints Sprite Name and ID
						Play::cWhite); //Sets Text to white

	Play::DrawSprite(gameState.spriteId, Play::GetMousePos(), gameState.timer);

	if (Play::KeyPressed(VK_SPACE))
		gameState.spriteId++; //Incriments Sprite ID which Cycles through sprites

	Play::DrawBackground();
	HandlePlayerControls();
	Play::PresentDrawingBuffer(); //Copys the Drawing buffer and outputs to screen
	return Play::KeyDown( VK_ESCAPE ); // Allows for exiting the game on keypress in this case its Escape but can be changed by changing ESCAPE - Any other key eg DOWN or UP
}
//Like said Handles All Player Controls (Movement, attacking etc..)
void HandlePlayerControls() 
{
	GameObject& obj_agent8 = Play::GetGameObjectByType(TYPE_AGENT8);
	if (Play::KeyDown(VK_UP)) // if player held key up the spider will climb upwards and play climb anim
	{
		obj_agent8.velocity = { 0, -4 };
		Play::SetSprite(obj_agent8, "agent8_climb", 0.25f);
	}
	else if (Play::KeyDown(VK_DOWN)) //if player holds key Down the spider will fall down and play falling anim
	{
		obj_agent8.acceleration = { 0, 1 }; //Accelerates over time to give illusion of gravity or momentum buildup
		Play::SetSprite(obj_agent8, "agent8_fall", 0);
	}
	else //if not moving then plays hang animation  
	{
		Play::SetSprite(obj_agent8, "agent8_hang", 0.02f); 
		obj_agent8.velocity *= .85f; //slows velocity by multiplying by a fraction higher the number the longer youll continue moving after letting go
		obj_agent8.acceleration = { 0, 0 };
	}
	Play::UpdateGameObject(obj_agent8);

	if (Play::IsLeavingDisplayArea(obj_agent8)) //Prevents player from leaving play space
		obj_agent8.pos = obj_agent8.oldPos;

	Play::DrawLine({ obj_agent8.pos.x, 0 }, obj_agent8.pos, Play::cWhite); // Draws a line between the spider and the top of the screen creating a web
	Play::DrawObjectRotated(obj_agent8); //Allows us to apply rotations to our object 

}



// Gets called once when the player quits the game 
int MainGameExit( void )
{
	Play::DestroyManager(); //Frees up any memeory that is allocated by this program
	return PLAY_OK; // Returns this to find out if it finished without any errors
}

