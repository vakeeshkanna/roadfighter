#include "player-class.h"

Player::Player(String n,Score sc)
{
	name = n;
	score = sc;
}

Player::Player()
{
	
}
Player::~Player()
{
	
}

void Player::setName(String n)
{
	name = n;
}

String& Player::getName()
{
	return name;
}
void Player::setScore(Score sc)
{
	score = sc;
}

Score& Player::getScore()
{
	return score;
}