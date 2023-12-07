const int ROCK = 0, PAPER = 1, SCISSORS = 2;
const int ROCK_COLOUR = (int)colorRed, PAPER_COLOUR =(int)colorBlue, SCISSORS_COLOUR = (int)colorGreen;
const int ROBOT_WIN = 1, PLAYER_WIN = 0, TIE = 2;

void startup(); //sets motor encoders
void startRound(); //clears screen, prepares input from sound sensor
int robotPlay(); //counts down aloud, chooses one of three moves
void handPosition(int robotChoice); //moves hands to according position from value returned from robotPlay function
int readPlayerMove(); //reads player move from color sensor
int decideChamp(int robotChoice, int playerChoice); //takes input from player choice and robot choice to decide winner
void winnerResponse(int winner, int &robotWins, int &playerWins); //takes input from decideChamp, robot choice and decide winner, to play a sound file
void updateScoreBoard(int robotWins, int playerWins); //displays number of robot wins and player wins
void terminate(int robotWins, int playerWins); //takes input from touch sensor for sportsmanship, as well as robotwins and playerwins to shutdown progra,
void resetHand(); //resets encoders

task main()
{
  eraseDisplay();
  playSoundFile("Start up.rsf");
	startup();
  DisplayCenteredBigTextLine(2, "Ready...");

	int playerWins = 0;
	int robotWins = 0; // To track for the scoreboard

  bool playAgain = true;
  do
  {
    // Play a round
    DisplayCenteredBigTextLine(5, "Speak to start round");
    startRound();

    int robotChoice = robotPlay();
    handPosition(robotChoice);
    int winner = decideChamp(robotChoice, readPlayerMove());
    winnerResponse(winner, robotWins, playerWins);
    eraseDisplay(); // Clear screen
    resetHand();
    updateScoreBoard(robotWins, playerWins);

    // Asking the player if they'd like to play again
    DisplayCenteredBigTextLine(1, "Play again?");
    DisplayCenteredBigTextLine(3, "<-- Yes/No -->");
    DisplayCenteredTextLine(5, "Left button  /  Right button");

    // Getting input from the user
    while (!getButtonPress(buttonLeft)&&!getButtonPress(buttonRight))
      {} // While they haven't pressed either
    playSoundFile("Beep.rsf");
    if(getButtonPress(buttonRight)) // Get the button they pressed
    {
      playAgain = false;
    } // The only other option is right button, which means play again (which is already true)
    while (getButtonPress(buttonLeft)||getButtonPress(buttonRight))
      {} // Wait until they've finished pressing the button
    eraseDisplay(); // Clear screen
  } while (playAgain);

  // Ending the program
  terminate(robotWins, playerWins);
}

// Function which decides the winner of the game. Written by Miranda and Katie
// Returns 0 for a player win, 1 for a robot win, and 2 for a tie
int decideChamp(int robotChoice, int playerChoice) {
	if (robotChoice == playerChoice)
	{
		// A tie
		return TIE;
	}

	if(robotChoice == ROCK)
	{
		if (playerChoice == SCISSORS)
		{
			return ROBOT_WIN;
		}
		else
		{
			return PLAYER_WIN;
		}
	}

  if(robotChoice == PAPER)
	{
		if (playerChoice == ROCK)
		{
			return ROBOT_WIN;
		}
		else
		{
			return PLAYER_WIN;
		}
	}

  if(robotChoice == SCISSORS)
	{
		if (playerChoice == PAPER)
		{
			return ROBOT_WIN;
		}
		else
		{
			return PLAYER_WIN;
		}
	}
	return -1;
}

/* Katie Q
This function starts the countdown for the player and robot to make their choices by. It displays that countdown for the player and the robot chooses its move at the end of it.

rand() function syntax found on https://www.robotc.net/WebHelpArduino
*/
int robotPlay()
{
	time1[T1]=0; // Reset timer
	int secondPassed = 0;
	int displayTime = 3;

	// Counting down 3 seconds
	while (displayTime>=0)
	{
		// Every 1 second
		if (time1[T1]-secondPassed >= 1000)
		{
			// Display countdown to the screen + beep
			DisplayCenteredBigTextLine(5, "%i", displayTime);
			if(displayTime==3)
			{
				playSoundFile("Three.rsf");
			}
			else if (displayTime==2)
			{
				playSoundFile("Two.rsf");
			}
			else if (displayTime==1)
			{
				playSoundFile("One.rsf");
			}
			else
			{
				playSoundFile("Go.rsf");
			}

			displayTime --;
			clearTimer(T1);
		}
	}

	DisplayCenteredBigTextLine(5, "GO!");

	// Robot chooses its move
	int move = (rand() % 2)+1; // between 0 and 2
	return move;
}

//move to hand position
void handPosition(int robotChoice)
{
	// Start moving motors
	motor[motorA]=-10;
	motor[motorB]=-10;
	motor[motorC]=10;

	// Paper
	if(robotChoice==PAPER)
	{
		while(nMotorEncoder[motorA]>65)
		{}
		motor[motorA]=0;

		while(nMotorEncoder[motorB]>-129)
		{}
		motor[motorB]=motor[motorC]=0;
	}

	// Rock
	else if(robotChoice==ROCK)
	{
		while(nMotorEncoder[motorB]<=-5)
		{}
		motor[motorB]=motor[motorC]=0;

		while(nMotorEncoder[motorA]<150)
		{}
		motor[motorA]=0;
	}

	// Scissors
	else{
		motor[motorA]=0;
		motor[motorC]=0;
		while(nMotorEncoder[motorB]>-96)
		{}
		motor[motorB]=0;
	}
	return;
}

int readPlayerMove()// Asmi - reads input of player, has an error trap
{
  while (1) // Keeps looping until valid input is sensed (returning the value will exit the function)
    {
      while(SensorValue[S2]==(int)colorWhite)
      {}
      wait1Msec(500); // To avoid getting the wrong colour reading (from switching the papers often there was a wrong reading at the start)
      if (SensorValue[S2]==ROCK_COLOUR) // User chose rock
	      {
  		    return ROCK;
  	    }
      else if (SensorValue[S2]==PAPER_COLOUR)// User chose paper
  	    {
  		    return PAPER;
  	    }

      else if (SensorValue[S2]==SCISSORS_COLOUR) // User chose scissors
  	    {
  		    return SCISSORS;
  	    }
    }
}

void startup() // Initializing the robot
{
	// Initialize sensors
	SensorType[S1] = sensorEV3_Touch;
	wait1Msec(50);
	SensorType[S2] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S2] = modeEV3Color_Color;
	wait1Msec(100);
  SensorType[S3] = sensorSoundDB;
	wait1Msec(50);
	SensorMode[S3] = modeNXTSound_DB;
	wait1Msec(100);

	// Set hand to initial position
	nMotorEncoder[motorA]=0;
	nMotorEncoder[motorB]=0;
	nMotorEncoder[motorC]=0;
	motor[motorA]=10;
	while(nMotorEncoder[motorA]<173)
	{}
	motor[motorA]=0;
}

void startRound() // Nitya
{
  eraseDisplay(); // Clear screen
  while (SensorValue(S3)<50 && !getButtonPress(buttonEnter))
  {}
}

// This program reacts to the winner of the game with sound files
void winnerResponse(int winner, int &robotWins, int &playerWins)
{
	if(winner==TIE)
	{
		// Tie
    PlaySoundFile("Blip 4.rsf");
	}
	else {
		if(winner == ROBOT_WIN)
		{
			// Robot win
      robotWins++;
			PlaySoundFile("Crying.rsf");
		}
		else {
      playerWins++;
			// Player win
      PlaySoundFile("Bravo.rsf");
      PlaySoundFile("Good job.rsf");
		}
	}
}

void resetHand()
{
	// Move motors
	motor[motorA]=-10;
	motor[motorB]=-10;
	motor[motorC]=10;

	while(nMotorEncoder[motorB]>-129)
	{}
	motor[motorB]=motor[motorC]=0;

	 // Open hand
	while(nMotorEncoder[motorA]>65)
	{}
	motor[motorA]=0;

	 //move motors
	 motor[motorA]=10;
	 motor[motorB]=10;
	 motor[motorC]=-10;

	 //Close hand
	while(nMotorEncoder[motorB]<=-5)
	{}
	motor[motorB]=motor[motorC]=0;

	while(nMotorEncoder[motorA]<150)
	{}
	motor[motorA]=0;
}

void updateScoreBoard(int robotWins, int playerWins)
{
	// Display code
	DisplayBigTextLine(8, "Scoreboard: ");
	DisplayBigTextLine(10, "Player: %i", playerWins);
	DisplayBigTextLine(12, "Robot: %i", robotWins);
}

void terminate(int robotWins, int playerWins)
{
  // Overall winner/loser
  DisplayCenteredBigTextLine(6, "Overall Result:");
  (robotWins==playerWins)? DisplayCenteredBigTextLine(8, "Tie!") : (robotWins>playerWins)? DisplayCenteredBigTextLine(8, "I win!") : DisplayCenteredBigTextLine(8, "You win!");

  DisplayCenteredBigTextLine(10, "Good Game!");
  playSoundFile("Fanfare.rsf");
  wait1Msec(2000);

  // High Five
  eraseDisplay();
  DisplayCenteredBigTextLine(2, "High five!");

  resetHand();
  handPosition(PAPER);

  time1[T1]=0;
  bool goodSport = 0;
  while (time1[T1]<5000 && goodSport==0)
  {
    if(sensorValue[S1]==1)
    {
      playSoundFile("Thank you.rsf");
      goodSport=1;
    }
  }

  if(goodSport) // If they high-fived
  {
    displayString(10, "You were a worthy opponent.");
    displayString(11,"I hope to play against you again some time in the near future!");
    displayString(12, "I will miss you dearly.");
    displayString(13, "Don't think about me too much <3");

  }
  else // If it timed out
  {
    playSoundFile("Boo.rsf");
    displayString(9, "You bloody bastard, you!");
    displayString(10, "Did your mother not teach you any mannars?");
    displayString(11, "You come into my house,");
    displayString(12, "you play my game,");
    displayString(13, "then you have the ODASSEDY to just");
    displayString(14, "up and leave without a handshake.");
    displayString(15, "Shame. This generation has no respect.");
  }
  wait1Msec(5000);
  robotWins =0;
  playerWins = 0;

  motor[motorA]=-10;
	while(nMotorEncoder[motorA]>0)
	{}
	motor[motorA]=0;


	motor[motorB]=10;
	while(nMotorEncoder[motorB]<0)
	{}
	motor[motorB]=0;

	motor[motorC]=-10;
	while(nMotorEncoder[motorC]>0)
	{}
	motor[motorC]=0;


  playSoundFile("Goodbye.rsf");
  playSoundFile("Power down.rsf");
  wait1Msec(2000);
}