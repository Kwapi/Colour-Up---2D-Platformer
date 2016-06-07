#pragma once
#include "Activity.h"

using namespace freetype;
class PlayGame :
	public Activity
{
public:
	PlayGame();
	~PlayGame();

	void draw();
	void init();
	void update(const double dt);
	void updateInput();
	void	drawGrid();
	void				createLevel();
	void				createPlayer();
	void				updateBlending();
	void				checkForCollision(const double);
	void playerDied();
	void getHeight(CollidableObject&);
	void switchBG(enum Color);
	bool onMovingY;
	void calculateScore();
	bool				gravityModified;
	Player				player;
	vector<CollidableObject> obstacles;
	GLuint				characterTex, alphaLeft, alphaRight, hsvTex;
	GLuint				death;
	GLuint				lambdaTex;
	GLuint				CMYKtex;
	GLuint				glitch;
	int					cloudY;
	Color				bgColor; 
	Point2f				startingPosition;
	Point2f				dimensionsHorizontal;
	Point2f				dimensionsVertical;
	float				time;
	int					displaySize; 
	bool				bgChanged;
	float				timeElapsed;
	float timeElapsedGravity ;
	float				bgChangeTimeLimit;
	bool				allowedToChangeBG;
	font_data our_font;
	int heightScore;
	int totalScore;
	int pickUpScore;
	float timeScore;


};

