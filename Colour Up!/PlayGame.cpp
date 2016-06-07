#include "PlayGame.h"


PlayGame::PlayGame()
{
	
	screenWidth = 1000;
	screenHeight = 1000;
	
}


PlayGame::~PlayGame()
{
}


void PlayGame::init(){
	turnOn();
	heightScore = 0;
	totalScore = 0;
	pickUpScore = 0;
	bgColor = BLACK;
	timeScore = 0;
	onMovingY = false;
	startingPosition = Point2<float>(100, -70);
	time = 0;
	timeScore = 0;
	

	bgChanged = false;
	timeElapsed = 0;
	timeElapsedGravity = 0;
	bgChangeTimeLimit = 0.8;
	allowedToChangeBG = true;
	gravityModified = false;


	our_font.init("pier.otf", 22);					    //Build the freetype font
	//initialise all keys to false
	for (int i = 0; i < 256; i++){
		keys[i] = false;
	}

	createLevel();
	createPlayer();
	std::cout << "BG: [C]YAN [M]AGENTA [Y]ELLOW BLACK[K]" << std::endl;

}

void PlayGame::playerDied(void){
	end_score = totalScore;
	end_time = time;
	turnOff();
	
}

void PlayGame::update(const double dt){
	timeScore += dt * ((gravityModified) ? 10 : 2);
	calculateScore();

	time += dt;
	if (gravityModified){
		timeElapsedGravity += dt;
		std::cout << timeElapsedGravity << std::endl;
	}

	if (timeElapsedGravity >= 5){
		player.jumpStartSpeedY = player.jumpStartSpeedYOriginal;
		gravityModified = false;
	}

	if (bgChanged){
		timeElapsed += dt;
		allowedToChangeBG = false;
	}
	if (timeElapsed >= bgChangeTimeLimit){
		bgColor = BLACK;
		allowedToChangeBG = true;
	}

	if (!player.alive){
		playerDied();
	}

	updateBlending();
	//PREDICT NEXT MOVE AND CHECK COLLISION
	updateInput();
	for (int o = 0; o<obstacles.size(); o++){

			obstacles[o].move(dt);

			
		}
	
	player.getNewSpeed(dt);
	checkForCollision(dt);
	player.move(dt);


	bool remove;
	cloudY = obstacles[obstacles.size() - 3].y + 20;
	std::vector<CollidableObject>::iterator i = obstacles.begin();
	while (i != obstacles.end() - 3)
	{
		remove = (*i).toRemove;
		 
		if ((remove || (*i).y <= cloudY) && !(*i).player)
		{
			i = obstacles.erase(i);
		}
		else
		{
			++i;
		}
	}

	onMovingY = false;







}

void PlayGame::drawGrid(){
	glPushMatrix();
	glTranslated(-100, -100, 0);
	glBegin(GL_LINES);
	for (int i = 0; i <= screenWidth; i += 10)
	{
		glVertex3f((float)i, 0.0f, 0.0f);
		glVertex3f((float)i, screenWidth, 0.0f);
		glVertex3f(0.0f, (float)i, 0.0f);
		glVertex3f(screenWidth, (float)i, 0.0f);
	}
	glEnd();
	glPopMatrix();
}

void PlayGame::draw(){
	
	displayBG();
	glPushMatrix();
		glLoadIdentity();
		glTranslated(-player.x, -player.y, 0);
		
		for (int o = 0; o<obstacles.size(); o++){
			
				obstacles[o].draw();
			
		}
		player.draw();
		//drawGrid();
	glPopMatrix();
	
	print(our_font, screenWidth / 2.0, screenHeight * 9/10 , "SCORE: %d", totalScore);
	glPopMatrix();
	glFlush();
}

void PlayGame::updateInput(){
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


	if (player.applyGravity){
		////JUMP (UP)
		if (keys[VK_UP]){
			//if the player is allowed to jump send the request to jump
			if (player.allowedToJump || onMovingY)
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
	}
	else{
		if (keys[VK_UP]){
			player.moveRequestUp = true;
		}
		else{
			player.moveRequestUp = false;
		}
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


	
	if (keys[VK_C]){
		switchBG(CYAN);
	}
	if (keys[VK_M]){
		switchBG(MAGENTA);
	}
	if (keys[VK_Y]){
		switchBG(YELLOW);
	}
	if (keys[VK_G]){
		player.switchGravity();
	}
	




}

void PlayGame::createLevel(){
	obstacles.clear();

	//DEPENDS ON THE LEVEL I GUESS
	bgColor = BLACK;
	//FRAME
	
	dimensionsVertical = Point2<float>(400, 10000);
	Point2f platformDimNormal = Point2<float>(60, 20);
	Point2f platformDimSmall = Point2<float>(40, 20);
	Point2f square = Point2<float>(20, 20);

	death = loadPNG("Enemy_alpha_standard.png");

	alphaLeft = loadPNG("Enemy_alpha_small_left.png");
	alphaRight = loadPNG("Enemy_alpha_small_right.png");
	CMYKtex = loadPNG("color_powerup_pixel.png");
	glitch = loadPNG("scanLine2.png");
	lambdaTex = loadPNG("color_powerup_2_alpha.png");
	
	CollidableObject floor = CollidableObject(Point2f(400, 20), Point2<float>(100, -90), CollidableObject::PLATFORM);
	obstacles.push_back(floor);
	CollidableObject wallLeft = CollidableObject(dimensionsVertical, Point2<float>(-300, 0), CollidableObject::PLATFORM);
	wallLeft.player = true;
	
	


	//PLATFORM1
	CollidableObject platform1 = CollidableObject(platformDimNormal, Point2<float>(0, -40), CollidableObject::PLATFORM);
	obstacles.push_back(platform1);
	CollidableObject enemy1 = CollidableObject(square, Point2<float>(0, 0), CollidableObject::ENEMY);
	enemy1.setTexture(alphaRight);
	enemy1.textures[0] = alphaRight;
	enemy1.textures[1] = alphaLeft;
	enemy1.setSpeedMod(10);
	enemy1.tieNPCtoPlatform(platform1);
	obstacles.push_back(enemy1);

	//PLATFORM2
	CollidableObject platform2 = CollidableObject(platformDimNormal, Point2<float>(220, 10), CollidableObject::PLATFORM);
	obstacles.push_back(platform2);
	CollidableObject lambda = CollidableObject(square, Point2<float>(0, 0), CollidableObject::LAMBDA);
	lambda.setTexture(lambdaTex);
	lambda.tieNPCtoPlatform(platform2);
	obstacles.push_back(lambda);

	//MOVINGY1 (SCAN)
	CollidableObject movingY1 = CollidableObject(platformDimNormal, Point2<float>(100, -20), CollidableObject::MOVINGY);
	movingY1.setMotionDuration(3);
	
	movingY1.setSpeedMod(10);
	
	obstacles.push_back(movingY1);

	//MOVINGX1
	CollidableObject movingX1 = CollidableObject(platformDimNormal, Point2<float>(80, 70), CollidableObject::MOVINGX);
	movingX1.setMotionDuration(2);
	movingX1.setSpeedMod(35);
	movingX1.setTexture(glitch);
	movingX1.setColour(MAGENTA);
	obstacles.push_back(movingX1);

	//PLATFORM3
	CollidableObject platform3 = CollidableObject(platformDimNormal, Point2<float>(0, 120), CollidableObject::PLATFORM);
	obstacles.push_back(platform3);
	
	

	//PLATFORM4
	CollidableObject platform4 = CollidableObject(platformDimNormal, Point2<float>(-70, 10), CollidableObject::PLATFORM);
	obstacles.push_back(platform4);

	//PLATFORM5 (SCAN)
	CollidableObject platform5 = CollidableObject(platformDimNormal, Point2<float>(-60, 60), CollidableObject::PLATFORM);
	platform5.setColour(CYAN);
	platform5.setTexture(glitch);
	obstacles.push_back(platform5);
	lambda.tieNPCtoPlatform(platform5);
	obstacles.push_back(lambda);

	//PLATFORM6(SCAN)
	CollidableObject platform6 = CollidableObject(platformDimNormal, Point2<float>(120, 170), CollidableObject::PLATFORM);
	platform6.setColour(YELLOW);
	platform6.setTexture(glitch);
	obstacles.push_back(platform6);
	

	//PLATFORM7
	CollidableObject platform7 = CollidableObject(platformDimNormal, Point2<float>(270, 70), CollidableObject::PLATFORM);
	obstacles.push_back(platform7);
	CollidableObject cmyk = CollidableObject(square, Point2<float>(0, 0), CollidableObject::CMYK);
	cmyk.setTexture(CMYKtex);
	cmyk.tieNPCtoPlatform(platform7);
	obstacles.push_back(cmyk);

	CollidableObject platform8 = CollidableObject(Point2<float>(100, 20), Point2<float>(220, 210), CollidableObject::PLATFORM);
	obstacles.push_back(platform8);
	enemy1.tieNPCtoPlatform(platform8);
	obstacles.push_back(enemy1);

	

	CollidableObject movingX2 = CollidableObject(platformDimNormal, Point2<float>(50, 260), CollidableObject::MOVINGX);
	movingX2.setMotionDuration(5);
	movingX2.setSpeedMod(30);
	obstacles.push_back(movingX2);


	//cage
	CollidableObject platform9 = CollidableObject(Point2<float>(20,100), Point2<float>(0, 340), CollidableObject::PLATFORM);
	platform9.setColour(CYAN);
	platform9.setTexture(glitch);
	obstacles.push_back(platform9);

	CollidableObject platform10 = CollidableObject(Point2<float>(110,20), Point2<float>(-45, 280), CollidableObject::PLATFORM);
	platform10.setColour(CYAN);
	platform10.setTexture(glitch);
	obstacles.push_back(platform10);

	CollidableObject platform11 = CollidableObject(Point2<float>(20, 120), Point2<float>(0,440), CollidableObject::PLATFORM);
	platform11.setColour(MAGENTA);
	platform11.setTexture(glitch);
	obstacles.push_back(platform11);

	CollidableObject platform12 = CollidableObject(Point2<float>(110, 20), Point2<float>(-45, 510), CollidableObject::PLATFORM);
	platform12.setColour(CYAN);
	platform12.setTexture(glitch);
	enemy1.tieNPCtoPlatform(platform12);
	obstacles.push_back(platform12);
	obstacles.push_back(enemy1);

	//platform in the cage moving up
	CollidableObject movingY2 = CollidableObject(platformDimNormal, Point2<float>(-60, 320), CollidableObject::MOVINGY);
	movingY2.setMotionDuration(2);
	movingY2.setSpeedMod(40);
	obstacles.push_back(movingY2);


	CollidableObject platform13 = CollidableObject(square, Point2<float>(40, 440), CollidableObject::PLATFORM);
	obstacles.push_back(platform13);

	CollidableObject platform14 = CollidableObject(square, Point2<float>(120, 480), CollidableObject::PLATFORM);
	obstacles.push_back(platform14);

	CollidableObject platform15 = CollidableObject(square, Point2<float>(180, 520), CollidableObject::PLATFORM);
	platform15.setColour(YELLOW);
	platform15.setTexture(glitch);
	obstacles.push_back(platform15);

	CollidableObject platform17 = CollidableObject(square, Point2<float>(180, 470), CollidableObject::PLATFORM);
	cmyk.tieNPCtoPlatform(platform17);
	obstacles.push_back(platform17);
	obstacles.push_back(cmyk);

	CollidableObject platform16 = CollidableObject(square, Point2<float>(260, 580), CollidableObject::PLATFORM);
	obstacles.push_back(platform16);

	CollidableObject movingX3 = CollidableObject(platformDimNormal, Point2<float>(50, 600), CollidableObject::MOVINGX);
	movingX3.setMotionDuration(3.2);
	movingX3.setSpeedMod(20);

	CollidableObject platform19 = CollidableObject(platformDimNormal, Point2<float>(-80, 560), CollidableObject::PLATFORM);
	lambda.tieNPCtoPlatform(platform19);
	obstacles.push_back(platform19);
	obstacles.push_back(lambda);

	obstacles.push_back(movingX3);

	//platform in the cage moving up
	CollidableObject movingY3 = CollidableObject(Point2<float>(40, 20), Point2<float>(280, 270), CollidableObject::MOVINGY);
	movingY3.setMotionDuration(2.6);
	movingY3.setSpeedMod(35);
	obstacles.push_back(movingY3);

	
	//platform in the cage moving up
	CollidableObject platform21 = CollidableObject(Point2<float>(20,100), Point2<float>(100, 350), CollidableObject::PLATFORM);
	platform21.setColour(YELLOW);
	platform21.setTexture(glitch);
	obstacles.push_back(platform21);

	CollidableObject platform24 = CollidableObject(Point2<float>(40, 20), Point2<float>(130, 310), CollidableObject::PLATFORM);
	platform24.setColour(CYAN);
	platform24.setTexture(glitch);
	obstacles.push_back(platform24);

	CollidableObject platform25 = CollidableObject(Point2<float>(20, 40), Point2<float>(160, 310), CollidableObject::PLATFORM);
	platform25.setColour(MAGENTA);
	platform25.setTexture(glitch);
	obstacles.push_back(platform25);

	CollidableObject platform26 = CollidableObject(Point2<float>(40, 20), Point2<float>(150, 370), CollidableObject::PLATFORM);
	obstacles.push_back(platform26);



	//platform in the cage moving up
	CollidableObject platform23 = CollidableObject(Point2<float>(40, 20), Point2<float>(280, 420), CollidableObject::PLATFORM);
	platform23.setColour(MAGENTA);
	platform23.setTexture(glitch);
	obstacles.push_back(platform23);
	
	CollidableObject platform27 = CollidableObject(platformDimNormal, Point2<float>(-100, 620), CollidableObject::PLATFORM);
	obstacles.push_back(platform27);
	


	CollidableObject platform28 = CollidableObject(square, Point2<float>(40-30, 440 + 220), CollidableObject::PLATFORM);
	obstacles.push_back(platform28);

	CollidableObject platform29 = CollidableObject(square, Point2<float>(120 - 10, 480 + 230), CollidableObject::PLATFORM);
	obstacles.push_back(platform29);

	CollidableObject platform30 = CollidableObject(square, Point2<float>(180 - 10, 520 + 230), CollidableObject::PLATFORM);
	obstacles.push_back(platform30);

	CollidableObject platform31 = CollidableObject(square, Point2<float>(260 - 20, 580 + 230), CollidableObject::PLATFORM);
	obstacles.push_back(platform31);
	
	CollidableObject platform32 = CollidableObject(square, Point2<float>(180 - 10, 520 + 350), CollidableObject::PLATFORM);
	obstacles.push_back(platform32);


	CollidableObject lastBit0 = CollidableObject(Point2<float>(20, 320), Point2<float>(-90 + 10, 830), CollidableObject::PLATFORM);
	lastBit0.setColour(YELLOW);
	lastBit0.setTexture(glitch);
	obstacles.push_back(lastBit0);

	CollidableObject lastBit1 = CollidableObject(Point2<float>(20, 320), Point2<float>(-20 + 10, 830), CollidableObject::PLATFORM);
	lastBit1.setColour(CYAN);
	lastBit1.setTexture(glitch);
	obstacles.push_back(lastBit1);

	CollidableObject lastBit2 = CollidableObject(Point2<float>(20, 320), Point2<float>(50 + 10, 830), CollidableObject::PLATFORM);
	lastBit2.setColour(MAGENTA);
	lastBit2.setTexture(glitch);
	obstacles.push_back(lastBit2);

	CollidableObject lastBit3 = CollidableObject(Point2<float>(20, 320), Point2<float>(120 + 10, 830), CollidableObject::PLATFORM);
	lastBit3.setColour(YELLOW);
	lastBit3.setTexture(glitch);
	obstacles.push_back(lastBit3);

	CollidableObject lastBit4 = CollidableObject(Point2<float>(20, 320), Point2<float>(190 + 10, 830), CollidableObject::PLATFORM);
	lastBit4.setColour(CYAN);
	lastBit4.setTexture(glitch);
	obstacles.push_back(lastBit4);

	CollidableObject lastBit5 = CollidableObject(Point2<float>(20, 320), Point2<float>(260 + 10, 830), CollidableObject::PLATFORM);
	lastBit5.setColour(MAGENTA);
	lastBit5.setTexture(glitch);
	obstacles.push_back(lastBit5);




	CollidableObject lastBit01 = CollidableObject(Point2<float>(80, 320), Point2<float>(-90, 1120), CollidableObject::PLATFORM);
	lastBit01.setColour(YELLOW);
	lastBit01.setTexture(glitch);
	obstacles.push_back(lastBit01);

	CollidableObject lastBit11 = CollidableObject(Point2<float>(80, 320), Point2<float>(-20, 1120), CollidableObject::PLATFORM);
	lastBit11.setColour(CYAN);
	lastBit11.setTexture(glitch);
	obstacles.push_back(lastBit11);

	CollidableObject lastBit21 = CollidableObject(Point2<float>(80, 320), Point2<float>(50, 1120), CollidableObject::PLATFORM);
	lastBit21.setColour(MAGENTA);
	lastBit21.setTexture(glitch);
	obstacles.push_back(lastBit21);

	CollidableObject lastBit31 = CollidableObject(Point2<float>(80, 320), Point2<float>(120, 1120), CollidableObject::PLATFORM);
	lastBit31.setColour(YELLOW);
	lastBit31.setTexture(glitch);
	obstacles.push_back(lastBit31);

	CollidableObject lastBit41 = CollidableObject(Point2<float>(80, 320), Point2<float>(190, 1120), CollidableObject::PLATFORM);
	lastBit41.setColour(CYAN);
	lastBit41.setTexture(glitch);
	obstacles.push_back(lastBit41);

	CollidableObject lastBit51 = CollidableObject(Point2<float>(80, 320), Point2<float>(260 , 1120), CollidableObject::PLATFORM);
	lastBit51.setColour(MAGENTA);
	lastBit51.setTexture(glitch);
	obstacles.push_back(lastBit51);


	CollidableObject wallRight = CollidableObject(dimensionsVertical, Point2<float>(490, 0), CollidableObject::PLATFORM);
	wallRight.player = true;
	
	hsvTex = loadPNG("hsv.png");
	CollidableObject HSV = CollidableObject(Point2<float>(40, 40), Point2<float>(180 - 10, 520 + 420), CollidableObject::HSV);
	HSV.setTexture(hsvTex);
	obstacles.push_back(HSV);

	CollidableObject floorDEATH = CollidableObject(Point2f(400, 400), Point2<float>(100, -600), CollidableObject::ALPHAFLOOR);
	floorDEATH.setTexture(death);
	floorDEATH.setSpeedMod(15);
	obstacles.push_back(floorDEATH);

	obstacles.push_back(wallRight);
	obstacles.push_back(wallLeft);

	
}

void PlayGame::createPlayer(void){
	player = Player(Point2<float>(20, 20), startingPosition);
	player.textures[0] = loadPNG("char_idle_k.png");
	player.textures[1] = loadPNG("char_left_k.png");
	player.textures[2] = loadPNG("char_right_k.png");
	player.textures[3] = loadPNG("char_idle_cmy.png");
	player.textures[4] = loadPNG("char_left_cmy.png");
	player.textures[5] = loadPNG("char_right_cmy.png");

	player.setTexture(player.textures[0]);
}

void PlayGame::checkForCollision(const double dt){
	
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
			pushDistRight = abs(bbtemp.minX - otherBB.maxX) + mod;
			pushDistLeft = (abs(bbtemp.maxX - otherBB.minX) + mod);
			pushDistDown = (abs(bbtemp.maxY - otherBB.minY) + mod);
			pushDistUp = abs(bbtemp.minY - otherBB.maxY) + mod;


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
			player.modifySpeed(pushDistRight, pushDistLeft, pushDistDown, pushDistUp, dt);
			if (player.y > obstacles[i].y && obstacles[i].platformType != CollidableObject::ENEMY  && obstacles[i].platformType != CollidableObject::ALPHAFLOOR){
					getHeight(obstacles[i]);
				}

			switch (obstacles[i].platformType){
			case CollidableObject::HSV:
				win = true;
				player.die();
				break;
			case CollidableObject::MOVINGY:
					onMovingY = true;
					break;
			case CollidableObject::DEADLYPLATFORM:
				player.die();
				break;
			case CollidableObject::ALPHAFLOOR:
				player.die();
			case CollidableObject::MOVINGX:
				player.x += obstacles[i].speed;
				break;
			case CollidableObject::ENEMY:
				player.die();
				break;
			case CollidableObject::LAMBDA:
				pickUpScore += 100;
				obstacles[i].stopDisplaying();
				break;
			case CollidableObject::CMYK:
				
				player.jumpStartSpeedY = player.jumpStartSpeedY * 1.5;
				gravityModified = true;
				obstacles[i].stopDisplaying();
				break;
			
			}

		}

		

	}
}

void PlayGame::updateBlending(){
	setBGColour(bgColor);
	for (int i = 0; i < obstacles.size(); i++){
		obstacles[i].checkBlending(bgColor);
	}
}

void PlayGame::getHeight(CollidableObject &platform){
	heightScore = platform.y + 100;
}

void PlayGame::switchBG(Color newBG){
	if (allowedToChangeBG){
		timeElapsed = 0;
		bgColor = newBG;
		bgChanged = true;
	}
}

void PlayGame::calculateScore(){
	
	totalScore = heightScore + pickUpScore + timeScore;
}




