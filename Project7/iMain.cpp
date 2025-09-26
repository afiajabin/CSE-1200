#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int walkRight[6], walkLeft[6];
int characterRight, characterLeft;
int bg1Image, menuImage, levelBgImage, clubBgImage, level1bg, aqlevel1bg, arlevel1bg, terralevel1bg;
int level1Image, level2Image, level3Image, level4Image;
int level1Locked, level2Locked, level3Locked, level4Locked;
int settingsImage; 
bool musicEnabled = true;
bool soundEnabled = true;
bool soundButtonHovered = false;
bool musicButtonHovered = false;
bool settingsBackHovered = false;


int currentLevel = 1; 
int currentEnemyClub = 0; // 1=Ignis,2=Aquae,3=Terra,4=Aeris,5=Voidwalker
void resetLevel2Battle();


// Level unlock flags: index 1..4 used. Level 1 unlocked initially.
int levelUnlocked[5] = { 0, 1, 0, 0, 0 }; // [0] unused

int startButton, instructionButton, settingButton, exitButton;

int ignisClub, aquaeClub, terraClub, aerisClub;
int ignisHover, aquaeHover, terraHover, aerisHover;


int playerAttackFrames[7];
int playerSpecialFrames[14];
int playerDefendFrames[3];
int enemyAttackFrames[9];
int enemySpecialFrames[7];

// Movement and animation variables
int enemyAnimationTimer = 0;
int playerAnimationTimer = 0;
bool enemyMoving = false;
int gameStartTimer = 0; // Timer to start enemy movement after 4 seconds
bool enemyStarted = false;

int playerLives = 3;
bool gameOver = false;


int initialPlayerX = -100; // Start from left side 
int initialEnemyX = 1400;  // Start from right side

// Movement speed
int walkSpeed = 5; // Pixels per frame
int animationSpeed = 8; // Frames before changing walk animation

// Battle state variables
typedef enum {
	BATTLE_INIT,
	BATTLE_IDLE,
	BATTLE_PLAYER_ATTACK,
	BATTLE_PLAYER_SPECIAL,
	BATTLE_PLAYER_DEFEND,
	BATTLE_ENEMY_ATTACK,
	BATTLE_ENEMY_SPECIAL,
	BATTLE_WIN,
	BATTLE_LOSE
} BattleState;

void playBeepIfEnabled() {
	if (soundEnabled) {
		mciSendString("play beep from 0", NULL, 0, NULL);
	}
}

BattleState battleState = BATTLE_INIT;
int playerHealth = 100;
int enemyHealth = 100;
int playerMana = 50;
int enemyMana = 50;


int currentBattleFrame = 0;
int animationTimer = 0;
int battleTimer = 0;
int enemyAttackCooldown = 0;
int continuousAttackTimer = 0;
bool playerTurn = true;
bool battleStarted = false;
bool isDefending = false;

// Position variables (for battle)
int playerBattleX = 100;
int playerBattleY = 80;
int enemyBattleX = 900;
int enemyBattleY = 80;

//for hover
bool startHovered = false, instructionHovered = false, settingHovered = false, exitHovered = false;
bool ignisHovered = false, aquaeHovered = false, terraHovered = false, aerisHovered = false;

bool facingRight = true;
bool characterLoaded = false;
bool imagesLoaded = false;
bool battleImagesLoaded = false;

int currentFrame = 0;

// Variables for key press states
bool wPressed = false, aPressed = false, sPressed = false, dPressed = false;
bool lastWPressed = false, lastAPressed = false, lastSPressed = false, lastDPressed = false;

// Battle key states
bool zPressed = false, xPressed = false, cPressed = false, enterPressed = false;
bool lastZPressed = false, lastXPressed = false, lastCPressed = false, lastEnterPressed = false;

int counter = 0;
double rec_x = 235;
double rec_y = 10;
int x = 0;
int y = 0;
int currentPage = 1;
int instructionBgImage;
int on, off;
bool soundOn = true;
bool hoveringLevel1 = false;
int mouseX = 0, mouseY = 0;
int selectedClub = 0, selectedLevel = 0;

// Button positions
int BUTTON_WIDTH = 320;
int BUTTON_HEIGHT = 90;
int EXIT_BUTTON_HEIGHT = 120;
int BUTTON_CENTER_X = ((1280 - BUTTON_WIDTH) / 2);

int START_BUTTON_Y = 520;
int INSTRUCTION_BUTTON_Y = 420;
int SETTING_BUTTON_Y = 320;
int EXIT_BUTTON_Y = 220;

// Club selection positions
int clubWidth = 200;
int clubHeight = 280;
int clubSpacing = 80;
int clubsStartX = 140;

// Hover image positions
int hoverWidth = 200;
int hoverHeight = 280;
int hoverOffsetX = 60;
int hoverOffsetY = -80;

int choosingInstruction, club, aeris, ignis, aquae, terra, aerishover, ignishover, aquaehover, terrahover;

int storyImages[10];
int storyIndex = 0;


// Projectile system variables
typedef struct {
	double x, y;
	bool active;
	double velocityX;
	double velocityY;
} Projectile;

void checkSettingsHovers() {
	soundButtonHovered = (mouseX >= 300 && mouseX <= 500 && mouseY >= 350 && mouseY <= 450);
	musicButtonHovered = (mouseX >= 580 && mouseX <= 780 && mouseY >= 350 && mouseY <= 450);
	settingsBackHovered = (mouseX >= 100 && mouseX <= 220 && mouseY >= 60 && mouseY <= 105);
}

#define MAX_PROJECTILES 10
Projectile projectiles[MAX_PROJECTILES];
int projectileImage;
bool projectileImageLoaded = false;

// Projectile game mechanics
int projectileHits = 0;
// Player jump animation
int playerJumpFrames[9];
bool playerJumpImageLoaded = false;

// Enemy projectile throwing animation 
int enemyProjectileFrames[8];
// Enemy projectile system
typedef struct {
	double x, y;
	bool active;
	double velocityX;
	double velocityY;
	int animationFrame;
	int animationTimer;
} EnemyProjectile;

#define MAX_ENEMY_PROJECTILES 5
EnemyProjectile enemyProjectiles[MAX_ENEMY_PROJECTILES];
int enemyProjectileImage;
bool enemyProjectileImageLoaded = false;

#define SAFE_JUMP_HEIGHT 60.0  // Minimum jump height to avoid projectiles

// Player jumping mechanics
bool playerJumping = false;
bool playerOnGround = true;
double playerJumpY = 0.0;
double playerJumpVelocity = 0.0;
double gravity = -0.8;
double jumpStrength = 15.0;
double groundLevel = 80.0;
int jumpAnimationFrame = 0;
int jumpAnimationTimer = 0;

// Level 2 background
int level2BattleBg, aqlevel2BattleBg;

int successfulDodges = 0;
int consecutiveDodges = 0;

// Combat enhancement variables
int comboCounter = 0;
int maxCombo = 0;
bool criticalHit = false;
int criticalTimer = 0;
double playerSpeedBoost = 1.0;
int speedBoostTimer = 0;
int perfectDodgeBonus = 0;
bool enemyStunned = false;
int stunTimer = 0;

// Level 3 enemy charge animation frames
int enemyChargeFrames[13];
int enemyLevel3ProjectileImage;
bool enemyLevel3ProjectileImageLoaded = false;

// Level 3 battle state
bool level3BattleStarted = false;
int level3bg; 

void initProjectiles();
void updateProjectiles();
void initEnemyProjectiles();
void updateEnemyProjectiles();
void updatePlayerJump();
void fireEnemyProjectile();
void drawProjectiles();
void drawEnemyProjectiles();

// Initialize projectiles
void initProjectiles() {
	//player projectiles
	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i].active = false;
		projectiles[i].x = 0;
		projectiles[i].y = 0;
		projectiles[i].velocityX = 0;
		projectiles[i].velocityY = 0;
	}
	projectileHits = 0;

	//enemy projectiles
	for (int i = 0; i < MAX_ENEMY_PROJECTILES; i++) {
		enemyProjectiles[i].active = false;
		enemyProjectiles[i].x = 0;
		enemyProjectiles[i].y = 0;
		enemyProjectiles[i].velocityX = 0;
		enemyProjectiles[i].velocityY = 0;
		enemyProjectiles[i].animationFrame = 0;
		enemyProjectiles[i].animationTimer = 0;
	}
}




void updateEnemyProjectiles() {
	for (int i = 0; i < MAX_ENEMY_PROJECTILES; i++) {
		if (enemyProjectiles[i].active) {
			// Move projectile
			enemyProjectiles[i].x += enemyProjectiles[i].velocityX;
			enemyProjectiles[i].y += enemyProjectiles[i].velocityY;

			// Projectile animation
			enemyProjectiles[i].animationTimer++;
			if (enemyProjectiles[i].animationTimer > 5) {
				enemyProjectiles[i].animationFrame = (enemyProjectiles[i].animationFrame + 1) % 8;
				enemyProjectiles[i].animationTimer = 0;
			}

			// Collision detection considering jumping
			double playerTop = playerBattleY + playerJumpY + 200;
			double playerBottom = playerBattleY + playerJumpY + 80;
			double playerLeft = playerBattleX + 50;
			double playerRight = playerBattleX + 200;

			// Only check collision if player is on or near ground
			bool canBeHit = (playerJumpY < 50.0);

			if (canBeHit &&
				enemyProjectiles[i].x >= playerLeft &&
				enemyProjectiles[i].x <= playerRight &&
				enemyProjectiles[i].y >= playerBottom &&
				enemyProjectiles[i].y <= playerTop) {
				// Hit the player
				enemyProjectiles[i].active = false;
				int damage = 10;
				if (isDefending) {
					damage = 5;
					printf("Enemy projectile hit! Blocked! Player health: %d (-%d)\n", playerHealth - damage, damage);
				}
				else {
					printf("Enemy projectile hit! Player health: %d (-%d)\n", playerHealth - damage, damage);
				}
				playerHealth -= damage;
				comboCounter = 0;  // Reset combo on hit
				criticalHit = false;
				consecutiveDodges = 0; // Reset streak when hit
			}
			else if (!canBeHit && enemyProjectiles[i].y >= playerBottom - 30 &&
				enemyProjectiles[i].y <= playerTop + 30) {
				// Close dodge: player was in danger but jumped in time
				successfulDodges++;
				consecutiveDodges++;
				printf("Close dodge! Total dodges: %d\n", successfulDodges);

				// Reward consecutive dodges with small mana gain
				if (consecutiveDodges >= 3) {
					playerMana = (playerMana + 5 > 50) ? 50 : playerMana + 5;
					printf("Dodge streak bonus! +5 mana\n");
					consecutiveDodges = 0;
				}
			}

			// Remove projectile if it goes off screen
			if (enemyProjectiles[i].x < -50 || enemyProjectiles[i].x > 1330 ||
				enemyProjectiles[i].y > 770 || enemyProjectiles[i].y < -50) {
				enemyProjectiles[i].active = false;
			}
		}
	}
}
void updatePlayerJump() {
	if (playerJumping) {
		jumpAnimationTimer++;
		if (jumpAnimationTimer > 4) { // Change frame every 4 ticks for smoother animation
			jumpAnimationFrame++;
			jumpAnimationTimer = 0;
			if (jumpAnimationFrame >= 9) {
				jumpAnimationFrame = 8; // Hold on last jump frame
			}
			printf("Jump frame: %d, Image ID: %d\n", jumpAnimationFrame,
				jumpAnimationFrame < 9 ? playerJumpFrames[jumpAnimationFrame] : -1);
		}

		// Apply gravity and update position
		playerJumpVelocity += gravity;
		playerJumpY += playerJumpVelocity;

		// Check if landed
		if (playerJumpY <= 0) {
			playerJumpY = 0;
			playerJumpVelocity = 0;
			playerJumping = false;
			playerOnGround = true;
			jumpAnimationFrame = 0;
			jumpAnimationTimer = 0;
			printf("Player landed!\n");
		}
	}
}

void loadBattleImages() {
	if (!battleImagesLoaded) {
		char filename[50];

		// Load player attack frames
		for (int i = 0; i < 7; i++) {
			sprintf_s(filename, "player_attack_%d.png", i + 1);
			playerAttackFrames[i] = iLoadImage(filename);
		}

		// Load player special frames
		for (int i = 0; i < 14; i++) {
			sprintf_s(filename, "player_special_%d.png", i + 1);
			playerSpecialFrames[i] = iLoadImage(filename);
		}

		// Load player defend frames
		for (int i = 0; i < 3; i++) {
			sprintf_s(filename, "player_defend_%d.png", i + 1);
			playerDefendFrames[i] = iLoadImage(filename);
			// If defend images don't exist
			if (playerDefendFrames[i] <= 0 && i < 7) {
				playerDefendFrames[i] = playerAttackFrames[i % 7];
			}
		}

		// Load enemy attack frames
		for (int i = 0; i < 9; i++) {
			sprintf_s(filename, "enemy_attack_%d.png", i + 1);
			enemyAttackFrames[i] = iLoadImage(filename);
		}

		// Load enemy special frames
		for (int i = 0; i < 7; i++) {
			sprintf_s(filename, "enemy_special_%d.png", i + 1);
			enemySpecialFrames[i] = iLoadImage(filename);
		}

		battleImagesLoaded = true;
		printf("Battle images loaded!\n");
	}
	// Load projectile image
	projectileImage = iLoadImage("projectile.png");
	if (projectileImage > 0) {
		projectileImageLoaded = true;
		printf("Projectile image loaded!\n");
	}
	else {
		printf("Projectile image not found, using fallback graphics.\n");
	}

	char filename[50];
	for (int i = 0; i < 8; i++) {
		sprintf_s(filename, "enemy_throw_%d.png", i + 1);
		enemyProjectileFrames[i] = iLoadImage(filename);
	}
	printf("Enemy projectile frames loaded (8 frames)!\n");

	// Load level 2 background
	level2BattleBg = iLoadImage("level2bg.png");

}
// Load jump frames, enemy throwing frames and enemy projectile image
void loadLevel2BattleImages() {
	// Load player jump frames (9 frames)
	char filename[50];
	for (int i = 0; i < 9; i++) {
		sprintf_s(filename, "player_jump_%d.png", i + 1);
		playerJumpFrames[i] = iLoadImage(filename);
		printf("Loading %s: ID = %d\n", filename, playerJumpFrames[i]);
	}
	playerJumpImageLoaded = true;
	printf("Player jump frames loaded (9 frames)!\n");

	// Load enemy projectile frames (8 frames) - these are the actual projectiles
	for (int i = 0; i < 8; i++) {
		sprintf_s(filename, "enemy_throw_%d.png", i + 1);
		enemyProjectileFrames[i] = iLoadImage(filename);
	}
	printf("Enemy projectile frames loaded (8 frames)!\n");

	// Load Level 2 background
	level2BattleBg = iLoadImage("level2bg.png");
}
// Load Level 3 battle images
void loadLevel3BattleImages() {
	// Load enemy charge frames (13 frames)
	char filename[50];
	for (int i = 0; i < 13; i++) {
		sprintf_s(filename, "enemy_charge_%d.png", i + 1);
		enemyChargeFrames[i] = iLoadImage(filename);
		printf("Loading %s: ID = %d\n", filename, enemyChargeFrames[i]);
	}
	printf("Enemy charge frames loaded (13 frames)!\n");

	// Load enemy level 3 projectile image
	enemyLevel3ProjectileImage = iLoadImage("enemy_3rd_projectile.png");
	if (enemyLevel3ProjectileImage > 0) {
		enemyLevel3ProjectileImageLoaded = true;
		printf("Enemy Level 3 projectile image loaded!\n");
	}
	else {
		printf("Enemy Level 3 projectile image not found, using fallback.\n");
	}

	// Load Level 3 background (you'll need to add this image to your project)
	level3bg = iLoadImage("level3bg.png");
}

// Initialize Level 3 battle
void initLevel3Battle() {
	battleState = BATTLE_IDLE;
	playerHealth = 100;
	enemyHealth = 100;
	playerMana = 50;
	enemyMana = 50;
	currentBattleFrame = 0;
	animationTimer = 0;
	battleTimer = 0;
	enemyAttackCooldown = 0;
	continuousAttackTimer = 0;
	playerTurn = false;
	battleStarted = true;
	isDefending = false;

	// Initialize positions (same as Level 1)
	playerBattleX = initialPlayerX;
	enemyBattleX = initialEnemyX;
	playerBattleY = 30; 
	enemyBattleY = 30;
	enemyMoving = false;
	enemyStarted = false;
	gameStartTimer = 0;
	enemyAnimationTimer = 0;
	playerAnimationTimer = 0;

	// Initialize jumping (from Level 2)
	playerJumping = false;
	playerOnGround = true;
	playerJumpY = 0.0;
	playerJumpVelocity = 0.0;
	jumpAnimationFrame = 0;
	jumpAnimationTimer = 0;

	// Initialize projectiles (from Level 1)
	initProjectiles();
	initEnemyProjectiles();

	printf("Level 3 Battle initialized!\n");
}

// Update Level 3 battle
void updateLevel3Battle() {
	updateProjectiles();
	updateEnemyProjectiles();
	updatePlayerJump();

	if (!battleStarted) return;

	animationTimer++;
	battleTimer++;
	continuousAttackTimer++;

	// Handle battle states
	switch (battleState) {
	case BATTLE_IDLE:
		enemyAttackCooldown++;
		if (enemyAttackCooldown > 240) { // Same timing as Level 1 (4 seconds)
			if (enemyMana >= 20 && rand() % 3 == 0) { // 33% chance for special
				battleState = BATTLE_ENEMY_SPECIAL;
			}
			else {
				battleState = BATTLE_ENEMY_ATTACK;
			}
			currentBattleFrame = 0;
			animationTimer = 0;
			enemyAttackCooldown = 0;
		}
		break;

	case BATTLE_PLAYER_ATTACK:
		if (animationTimer > 8) {
			currentBattleFrame++;
			animationTimer = 0;
			if (currentBattleFrame >= 7) {
				enemyHealth -= 15;
				currentBattleFrame = 0;
				battleState = BATTLE_IDLE;
				printf("Player attacks! Enemy health: %d\n", enemyHealth);
			}
		}
		break;

	case BATTLE_PLAYER_SPECIAL:
		if (animationTimer > 6) {
			currentBattleFrame++;
			animationTimer = 0;
			if (currentBattleFrame >= 14) {
				enemyHealth -= 30;
				playerMana -= 20;
				currentBattleFrame = 0;
				battleState = BATTLE_IDLE;
				printf("Player special attack! Enemy health: %d, Player mana: %d\n", enemyHealth, playerMana);
			}
		}
		break;

	case BATTLE_PLAYER_DEFEND:
		if (animationTimer > 10) {
			currentBattleFrame++;
			animationTimer = 0;
			if (currentBattleFrame >= 3) {
				currentBattleFrame = 2;
			}
		}
		break;

	case BATTLE_ENEMY_ATTACK:
		if (animationTimer > 7) {
			currentBattleFrame++;
			animationTimer = 0;
			// Fire projectile when charge reaches frame 7
			if (currentBattleFrame == 7) {
				fireEnemyProjectile();
			}
			if (currentBattleFrame >= 13) {
				currentBattleFrame = 0;
				battleState = BATTLE_IDLE;
			}
		}
		break;

	case BATTLE_ENEMY_SPECIAL:
		if (animationTimer > 6) {
			currentBattleFrame++;
			animationTimer = 0;
			// Fire multiple projectiles for special attack
			if (currentBattleFrame == 5 || currentBattleFrame == 8 || currentBattleFrame == 11) {
				fireEnemyProjectile();
			}
			if (currentBattleFrame >= 13) {
				enemyMana -= 20;
				currentBattleFrame = 0;
				battleState = BATTLE_IDLE;
			}
		}
		break;
	}

	// Regenerate mana
	if (battleTimer % 60 == 0) {
		if (playerMana < 50) playerMana += 2;
		if (enemyMana < 50) enemyMana += 2;
	}

	// Check win/lose conditions
	if (enemyHealth <= 0 && battleState != BATTLE_WIN) {
		battleState = BATTLE_WIN;
		printf("Player wins Level 3!\n");
		if (currentLevel >= 3 && currentLevel < 4) {
			levelUnlocked[currentLevel + 1] = 1;
			printf("Level %d unlocked!\n", currentLevel + 1);
		}
	}
	else if (playerHealth <= 0 && battleState != BATTLE_LOSE) {
		battleState = BATTLE_LOSE;
		playerLives--;
		printf("Player loses Level 3! Lives remaining: %d\n", playerLives);

		if (playerLives <= 0) {
			gameOver = true;
			printf("Game Over!\n");
		}
	}
}

// Draw Level 3 battle characters
void drawLevel3BattleCharacters() {
	int playerImageToShow = -1;
	int enemyImageToShow = -1;

	// Calculate player's actual Y position (ground + jump)
	double actualPlayerY = playerBattleY + playerJumpY;

	// Draw player (same logic as Level 2 with jumping)
	if (playerJumping) {
		if (jumpAnimationFrame < 9 && playerJumpFrames[jumpAnimationFrame] > 0) {
			playerImageToShow = playerJumpFrames[jumpAnimationFrame];
		}
		else {
			if (walkRight[0] > 0) {
				playerImageToShow = walkRight[0];
			}
		}
	}
	else if (battleState == BATTLE_IDLE || battleState < BATTLE_PLAYER_ATTACK) {
		if (dPressed && characterLoaded) {
			if (walkRight[currentFrame] > 0) {
				playerImageToShow = walkRight[currentFrame];
			}
		}
		else if (aPressed && characterLoaded) {
			if (walkLeft[currentFrame] > 0) {
				playerImageToShow = walkLeft[currentFrame];
			}
		}
		else {
			if (walkRight[0] > 0) {
				playerImageToShow = walkRight[0];
			}
		}
	}
	else {
		if (!playerJumping) {
			switch (battleState) {
			case BATTLE_PLAYER_ATTACK:
				if (currentBattleFrame < 7 && playerAttackFrames[currentBattleFrame] > 0) {
					playerImageToShow = playerAttackFrames[currentBattleFrame];
				}
				break;
			case BATTLE_PLAYER_SPECIAL:
				if (currentBattleFrame < 14 && playerSpecialFrames[currentBattleFrame] > 0) {
					playerImageToShow = playerSpecialFrames[currentBattleFrame];
				}
				break;
			case BATTLE_PLAYER_DEFEND:
				if (currentBattleFrame < 3 && playerDefendFrames[currentBattleFrame] > 0) {
					playerImageToShow = playerDefendFrames[currentBattleFrame];
				}
				else if (currentBattleFrame >= 2 && playerDefendFrames[2] > 0) {
					playerImageToShow = playerDefendFrames[2];
				}
				break;
			default:
				if (isDefending && playerDefendFrames[0] > 0) {
					playerImageToShow = playerDefendFrames[0];
				}
				else if (walkRight[0] > 0) {
					playerImageToShow = walkRight[0];
				}
				break;
			}
		}
		else {
			if (jumpAnimationFrame < 9 && playerJumpFrames[jumpAnimationFrame] > 0) {
				playerImageToShow = playerJumpFrames[jumpAnimationFrame];
			}
		}
	}

	// Display player
	if (playerImageToShow > 0) {
		iShowImage(playerBattleX, (int)actualPlayerY, 300, 300, playerImageToShow);
	}
	else {
		if (isDefending) {
			iSetColor(0, 255, 0);
		}
		else {
			iSetColor(0, 0, 255);
		}
		iFilledRectangle(playerBattleX, (int)actualPlayerY, 100, 100);
		iSetColor(255, 255, 255);
		iText(playerBattleX + 10, (int)actualPlayerY + 50, "PLAYER");
	}

	// Draw enemy using charge frames
	if (battleState >= BATTLE_ENEMY_ATTACK) {
		switch (battleState) {
		case BATTLE_ENEMY_ATTACK:
			if (currentBattleFrame < 13 && enemyChargeFrames[currentBattleFrame] > 0) {
				enemyImageToShow = enemyChargeFrames[currentBattleFrame];
			}
			break;
		case BATTLE_ENEMY_SPECIAL:
			if (currentBattleFrame < 13 && enemyChargeFrames[currentBattleFrame] > 0) {
				enemyImageToShow = enemyChargeFrames[currentBattleFrame];
			}
			break;
		default:
			if (enemyChargeFrames[0] > 0) {
				enemyImageToShow = enemyChargeFrames[0];
			}
			break;
		}
	}
	else {
		if (enemyChargeFrames[0] > 0) {
			enemyImageToShow = enemyChargeFrames[0];
		}
	}

	// Display enemy
	if (enemyImageToShow > 0) {
		iShowImage(enemyBattleX, enemyBattleY, 300, 300, enemyImageToShow);
	}
	else {
		iSetColor(255, 0, 0);
		iFilledRectangle(enemyBattleX, enemyBattleY, 100, 100);
		iSetColor(255, 255, 255);
		iText(enemyBattleX + 10, enemyBattleY + 50, "ENEMY");
	}

	drawProjectiles();
	drawEnemyProjectiles();
}

// Draw Level 3 battle UI
void drawLevel3BattleUI() {
	// Same health/mana bars as other levels
	iSetColor(100, 0, 0);
	iFilledRectangle(50, 650, 300, 30);
	iSetColor(0, 200, 0);
	iFilledRectangle(50, 650, (playerHealth * 300) / 100, 30);
	iSetColor(255, 255, 255);
	char playerHealthText[50];
	sprintf_s(playerHealthText, "Player HP: %d/100", playerHealth);
	iText(50, 630, playerHealthText);

	iSetColor(0, 0, 100);
	iFilledRectangle(50, 600, 200, 20);
	iSetColor(100, 150, 255);
	iFilledRectangle(50, 600, (playerMana * 200) / 50, 20);
	char playerManaText[50];
	sprintf_s(playerManaText, "Mana: %d/50", playerMana);
	iText(50, 580, playerManaText);

	iSetColor(100, 0, 0);
	iFilledRectangle(930, 650, 300, 30);
	iSetColor(200, 0, 0);
	iFilledRectangle(930, 650, (enemyHealth * 300) / 100, 30);
	char enemyHealthText[50];
	sprintf_s(enemyHealthText, "Enemy HP: %d/100", enemyHealth);
	iText(930, 630, enemyHealthText);

	iSetColor(0, 0, 100);
	iFilledRectangle(1030, 600, 200, 20);
	iSetColor(150, 100, 255);
	iFilledRectangle(1030, 600, (enemyMana * 200) / 50, 20);
	char enemyManaText[50];
	sprintf_s(enemyManaText, "Enemy Mana: %d/50", enemyMana);
	iText(1030, 580, enemyManaText);

	// Level 3 specific controls
	if (battleState != BATTLE_WIN && battleState != BATTLE_LOSE) {
		iSetColor(255, 255, 0);
		iText(350, 120, "DEFEAT THE CHARGING ENEMY WITH ALL YOUR SKILLS!", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(280, 90, "'Z' = Attack + Projectile  'X' = Special  'C' = Defend  'SPACE' = Jump", GLUT_BITMAP_TIMES_ROMAN_24);

		if (isDefending) {
			iSetColor(255, 255, 0);
			iText(550, 60, "DEFENDING!");
		}

		if (playerJumping) {
			iSetColor(0, 255, 255);
			iText(580, 60, "JUMPING!");
		}

		if (playerMana < 20) {
			iSetColor(255, 100, 100);
			iText(450, 10, "Not enough mana for special attack!");
		}
	}

	// Win/lose messages
	if (battleState == BATTLE_WIN) {
		iSetColor(255, 255, 0);
		iText(580, 350, "LEVEL 3 COMPLETE!", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(570, 320, "Press ENTER to continue", GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else if (battleState == BATTLE_LOSE) {
		if (gameOver) {
			iSetColor(255, 0, 0);
			iText(580, 350, "GAME OVER!", GLUT_BITMAP_TIMES_ROMAN_24);
			iText(550, 320, "Press ENTER to return to menu", GLUT_BITMAP_TIMES_ROMAN_24);
		}
		else {
			iSetColor(255, 0, 0);
			iText(600, 350, "YOU WERE DEFEATED!", GLUT_BITMAP_TIMES_ROMAN_24);
			char livesMsg[100];
			sprintf_s(livesMsg, "Lives remaining: %d - Press ENTER to retry", playerLives);
			iText(500, 320, livesMsg, GLUT_BITMAP_TIMES_ROMAN_24);
		}
	}

	// Draw projectile status
	iSetColor(255, 165, 0);
	char projectileStatus[100];
	sprintf_s(projectileStatus, "Hits: %d", projectileHits);
	iText(400, 550, projectileStatus);

	iSetColor(255, 255, 255);
	char livesText[50];
	sprintf_s(livesText, "Lives: %d", playerLives);
	iText(50, 550, livesText);
}

// Reset Level 3 battle
void resetLevel3Battle() {
	playerJumping = false;
	playerOnGround = true;
	playerJumpY = 0.0;
	playerJumpVelocity = 0.0;
	jumpAnimationFrame = 0;
	jumpAnimationTimer = 0;
	playerBattleX = initialPlayerX;
	enemyBattleX = initialEnemyX;
	initProjectiles();
	initEnemyProjectiles();
	printf("Level 3 battle reset!\n");
}

// Initialize enemy projectiles
void initEnemyProjectiles() {
	for (int i = 0; i < MAX_ENEMY_PROJECTILES; i++) {
		enemyProjectiles[i].active = false;
		enemyProjectiles[i].x = 0;
		enemyProjectiles[i].y = 0;
		enemyProjectiles[i].velocityX = 0;
		enemyProjectiles[i].velocityY = 0;
		enemyProjectiles[i].animationFrame = 0;
		enemyProjectiles[i].animationTimer = 0;
	}
}

// Fire enemy projectile
void fireEnemyProjectile() {
	// Find an inactive projectile slot
	for (int i = 0; i < MAX_ENEMY_PROJECTILES; i++) {
		if (!enemyProjectiles[i].active) {
			enemyProjectiles[i].active = true;
			enemyProjectiles[i].x = enemyBattleX; // Start from enemy's position
			enemyProjectiles[i].y = enemyBattleY + 80;
			enemyProjectiles[i].animationFrame = 0;
			enemyProjectiles[i].animationTimer = 0;

			// Calculate velocity towards player
			// Calculate velocity towards player (aim lower when player is jumping)
			double targetY = (playerBattleY + playerJumpY + 80);
			if (playerJumping) {
				// Aim at where player will land, not where they are in air
				targetY = playerBattleY + 80; // Ground level
			}

			double deltaX = (playerBattleX + 150) - enemyProjectiles[i].x;
			double deltaY = targetY - enemyProjectiles[i].y;
			double distance = sqrt(deltaX * deltaX + deltaY * deltaY);

			// Normalize and set speed
			double speed = 6.0;
			enemyProjectiles[i].velocityX = (deltaX / distance) * speed;
			enemyProjectiles[i].velocityY = (deltaY / distance) * speed;

			printf("Enemy projectile fired!\n");
			break;
		}
	}
}

void drawEnemyProjectiles() {
	for (int i = 0; i < MAX_ENEMY_PROJECTILES; i++) {
		if (enemyProjectiles[i].active) {
			if (enemyProjectileFrames[enemyProjectiles[i].animationFrame] > 0) {
				iShowImage((int)enemyProjectiles[i].x - 10, (int)enemyProjectiles[i].y - 10, 60, 60,
					enemyProjectileFrames[enemyProjectiles[i].animationFrame]);
			}
			else {
				iSetColor(0, 100, 255);
				iFilledCircle((int)enemyProjectiles[i].x, (int)enemyProjectiles[i].y, 8);
			}
		}
	}
}


// Initialize Level 2 battle
void initLevel2Battle() {
	battleState = BATTLE_IDLE;
	playerHealth = 100;
	enemyHealth = 100;
	playerMana = 50;
	enemyMana = 50;
	currentBattleFrame = 0;
	animationTimer = 0;
	battleTimer = 0;
	enemyAttackCooldown = 0;
	continuousAttackTimer = 0;
	playerTurn = false;
	battleStarted = true;
	isDefending = false;


	// Initialize positions
	playerBattleX = initialPlayerX;
	enemyBattleX = initialEnemyX;
	enemyMoving = false;
	enemyStarted = false;
	gameStartTimer = 0;
	enemyAnimationTimer = 0;
	playerAnimationTimer = 0;

	// Initialize jumping
	playerJumping = false;
	playerOnGround = true;
	playerJumpY = 0.0;
	playerJumpVelocity = 0.0;
	jumpAnimationFrame = 0;
	jumpAnimationTimer = 0;

	successfulDodges = 0;
	consecutiveDodges = 0;

	printf("Level 2 Battle initialized!\n");
	initEnemyProjectiles();
}

// Update Level 2 battle
void updateLevel2Battle() {
	updateEnemyProjectiles();
	updatePlayerJump();

	if (!battleStarted) return;

	animationTimer++;
	battleTimer++;
	continuousAttackTimer++;

	// Speed boost system
	if (speedBoostTimer > 0) {
		speedBoostTimer--;
		if (speedBoostTimer <= 0) {
			playerSpeedBoost = 1.0;
		}
	}

	// Critical hit timer
	if (criticalTimer > 0) {
		criticalTimer--;
	}

	// Enemy stun system
	if (enemyStunned) {
		stunTimer--;
		if (stunTimer <= 0) {
			enemyStunned = false;
		}
	}

	// Handle battle states
	switch (battleState) {
	case BATTLE_IDLE:
		enemyAttackCooldown++;
		if (enemyAttackCooldown > 180) { // Faster attacks in Level 2 (3 seconds)
			if (enemyMana >= 20 && rand() % 4 == 0) { // 25% chance for special
				battleState = BATTLE_ENEMY_SPECIAL;
			}
			else {
				battleState = BATTLE_ENEMY_ATTACK;
			}
			currentBattleFrame = 0;
			animationTimer = 0;
			enemyAttackCooldown = 0;
		}
		break;

	case BATTLE_PLAYER_ATTACK:
		if (animationTimer > 8) {
			currentBattleFrame++;
			animationTimer = 0;
			if (currentBattleFrame >= 7) {
				int damage = 15;
				comboCounter++;

				// Critical hit chance (20% base + 5% per combo)
				if (rand() % 100 < (20 + comboCounter * 5)) {
					damage *= 2;
					criticalHit = true;
					criticalTimer = 60;
					printf("CRITICAL HIT! ");
				}

				// Combo damage bonus
				damage += (comboCounter / 3) * 5;

				enemyHealth -= damage;
				if (comboCounter > maxCombo) maxCombo = comboCounter;

				// Stun chance on high combos
				if (comboCounter >= 5 && rand() % 100 < 30) {
					enemyStunned = true;
					stunTimer = 120;
					printf("Enemy stunned! ");
				}
				currentBattleFrame = 0;
				battleState = BATTLE_IDLE;
				printf("Enemy health: %d (-%d, Combo: %d)\n", enemyHealth, damage, comboCounter);
			}
		}

		break;

	case BATTLE_PLAYER_SPECIAL:
		if (animationTimer > 6) {
			currentBattleFrame++;
			animationTimer = 0;
			if (currentBattleFrame >= 14) {
				enemyHealth -= 30;
				playerMana -= 20;
				currentBattleFrame = 0;
				battleState = BATTLE_IDLE;
				printf("Player special attack! Enemy health: %d, Player mana: %d\n", enemyHealth, playerMana);
			}
		}
		break;

	case BATTLE_PLAYER_DEFEND:
		if (animationTimer > 10) {
			currentBattleFrame++;
			animationTimer = 0;
			if (currentBattleFrame >= 3) {
				currentBattleFrame = 2;
			}
		}
		break;

	case BATTLE_ENEMY_ATTACK:
		if (animationTimer > 7) {
			currentBattleFrame++;
			animationTimer = 0;
			// Fire projectile when attack reaches frame 4
			if (currentBattleFrame == 4) {
				fireEnemyProjectile();
			}
			if (currentBattleFrame >= 9) {
				// Attack animation complete, projectile already fired by throwing animation
				currentBattleFrame = 0;
				battleState = BATTLE_IDLE;
			}
		}
		break;

	case BATTLE_ENEMY_SPECIAL:
		if (animationTimer > 9) {
			currentBattleFrame++;
			animationTimer = 0;
			// Fire multiple projectiles for special attack
			if (currentBattleFrame == 3 || currentBattleFrame == 5) {
				fireEnemyProjectile();
			}
			if (currentBattleFrame >= 7) {
				// Special attack complete, projectiles already fired by throwing animation
				enemyMana -= 20;
				currentBattleFrame = 0;
				battleState = BATTLE_IDLE;
			}
		}
		break;
	}

	// Regenerate mana
	if (battleTimer % 60 == 0) {
		if (playerMana < 50) playerMana += 2;
		if (enemyMana < 50) enemyMana += 2;
	}

	// Check win/lose conditions
	if (enemyHealth <= 0 && battleState != BATTLE_WIN) {
		battleState = BATTLE_WIN;
		printf("Player wins Level 2!\n");
		if (currentLevel >= 2 && currentLevel < 4) {
			levelUnlocked[currentLevel + 1] = 1;
			printf("Level %d unlocked!\n", currentLevel + 1);
		}
	}
	else if (playerHealth <= 0 && battleState != BATTLE_LOSE) {
		battleState = BATTLE_LOSE;
		playerLives--;
		printf("Player loses Level 2! Lives remaining: %d\n", playerLives);

		if (playerLives <= 0) {
			gameOver = true;
			printf("Game Over!\n");
		}

	}




	// In BATTLE_PLAYER_ATTACK case, replace the damage dealing:






}

// Draw Level 2 battle characters
void drawLevel2BattleCharacters() {
	int playerImageToShow = -1;
	int enemyImageToShow = -1;

	// Calculate player's actual Y position (ground + jump)
	double actualPlayerY = playerBattleY + playerJumpY;

	// Draw player
	if (playerJumping) {
		// Show jumping animation (9 frames)
		if (jumpAnimationFrame < 9 && playerJumpFrames[jumpAnimationFrame] > 0) {
			playerImageToShow = playerJumpFrames[jumpAnimationFrame];
		}

		else {
			// Fallback - use first walk frame if jump frames aren't loaded
			if (walkRight[0] > 0) {
				playerImageToShow = walkRight[0];
			}
		}
	}
	else if (battleState == BATTLE_IDLE || battleState < BATTLE_PLAYER_ATTACK) {
		// Movement phase or idle
		if (dPressed && characterLoaded) {
			if (walkRight[currentFrame] > 0) {
				playerImageToShow = walkRight[currentFrame];
			}
		}
		else if (aPressed && characterLoaded) {
			if (walkLeft[currentFrame] > 0) {
				playerImageToShow = walkLeft[currentFrame];
			}
		}

		else {
			if (walkRight[0] > 0) {
				playerImageToShow = walkRight[0];
			}
		}
	}
	else {
		// Battle phase animations
		if (!playerJumping) {

			switch (battleState) {
			case BATTLE_PLAYER_ATTACK:
				if (currentBattleFrame < 7 && playerAttackFrames[currentBattleFrame] > 0) {
					playerImageToShow = playerAttackFrames[currentBattleFrame];
				}
				break;
			case BATTLE_PLAYER_SPECIAL:
				if (currentBattleFrame < 14 && playerSpecialFrames[currentBattleFrame] > 0) {
					playerImageToShow = playerSpecialFrames[currentBattleFrame];
				}
				break;
			case BATTLE_PLAYER_DEFEND:
				if (currentBattleFrame < 3 && playerDefendFrames[currentBattleFrame] > 0) {
					playerImageToShow = playerDefendFrames[currentBattleFrame];
				}
				else if (currentBattleFrame >= 2 && playerDefendFrames[2] > 0) {
					playerImageToShow = playerDefendFrames[2];
				}
				break;
			default:
				if (isDefending && playerDefendFrames[0] > 0) {
					playerImageToShow = playerDefendFrames[0];
				}
				else if (walkRight[0] > 0) {
					playerImageToShow = walkRight[0];
				}
				break;
			}
		}
		else {
			// Player is jumping during battle - show jump animation
			if (jumpAnimationFrame < 9 && playerJumpFrames[jumpAnimationFrame] > 0) {
				playerImageToShow = playerJumpFrames[jumpAnimationFrame];
			}
		}
	}

	// Display player
	if (playerImageToShow > 0) {
		iShowImage(playerBattleX, (int)actualPlayerY, 300, 300, playerImageToShow);
	}
	else {
		if (isDefending) {
			iSetColor(0, 255, 0);
		}
		else {
			iSetColor(0, 0, 255);
		}
		iFilledRectangle(playerBattleX, (int)actualPlayerY, 100, 100);
		iSetColor(255, 255, 255);
		iText(playerBattleX + 10, (int)actualPlayerY + 50, "PLAYER");
	}

	if (battleState >= BATTLE_ENEMY_ATTACK) {
		// Regular battle animations
		switch (battleState) {
		case BATTLE_ENEMY_ATTACK:
			if (currentBattleFrame < 9 && enemyAttackFrames[currentBattleFrame] > 0) {
				enemyImageToShow = enemyAttackFrames[currentBattleFrame];
			}
			break;
		case BATTLE_ENEMY_SPECIAL:
			if (currentBattleFrame < 7 && enemySpecialFrames[currentBattleFrame] > 0) {
				enemyImageToShow = enemySpecialFrames[currentBattleFrame];
			}
			break;
		default:
			if (enemyAttackFrames[0] > 0) {
				enemyImageToShow = enemyAttackFrames[0];
			}
			break;
		}
	}
	else {
		if (enemyAttackFrames[0] > 0) {
			enemyImageToShow = enemyAttackFrames[0];
		}
	}

	// Display enemy
	if (enemyImageToShow > 0) {
		iShowImage(enemyBattleX, enemyBattleY, 300, 300, enemyImageToShow);
	}
	else {
		iSetColor(255, 0, 0);
		iFilledRectangle(enemyBattleX, enemyBattleY, 100, 100);
		iSetColor(255, 255, 255);
		iText(enemyBattleX + 10, enemyBattleY + 50, "ENEMY");
	}

	drawEnemyProjectiles();
}

// Draw Level 2 battle UI
void drawLevel2BattleUI() {
	// Same health/mana bars as Level 1
	iSetColor(100, 0, 0);
	iFilledRectangle(50, 650, 300, 30);
	iSetColor(0, 200, 0);
	iFilledRectangle(50, 650, (playerHealth * 300) / 100, 30);
	iSetColor(255, 255, 255);
	char playerHealthText[50];
	sprintf_s(playerHealthText, "Player HP: %d/100", playerHealth);
	iText(50, 630, playerHealthText);

	iSetColor(0, 0, 100);
	iFilledRectangle(50, 600, 200, 20);
	iSetColor(100, 150, 255);
	iFilledRectangle(50, 600, (playerMana * 200) / 50, 20);
	char playerManaText[50];
	sprintf_s(playerManaText, "Mana: %d/50", playerMana);
	iText(50, 580, playerManaText);

	iSetColor(100, 0, 0);
	iFilledRectangle(930, 650, 300, 30);
	iSetColor(200, 0, 0);
	iFilledRectangle(930, 650, (enemyHealth * 300) / 100, 30);
	char enemyHealthText[50];
	sprintf_s(enemyHealthText, "Enemy HP: %d/100", enemyHealth);
	iText(930, 630, enemyHealthText);

	iSetColor(0, 0, 100);
	iFilledRectangle(1030, 600, 200, 20);
	iSetColor(150, 100, 255);
	iFilledRectangle(1030, 600, (enemyMana * 200) / 50, 20);
	char enemyManaText[50];
	sprintf_s(enemyManaText, "Enemy Mana: %d/50", enemyMana);
	iText(1030, 580, enemyManaText);

	// Show dodge counter

	// Show combat stats (moved to top-left corner)
	iSetColor(0, 255, 0);
	char dodgeText[50];
	sprintf_s(dodgeText, "Dodges: %d", successfulDodges);
	iText(50, 520, dodgeText);

	char comboText[50];
	sprintf_s(comboText, "Combo: %d (Max: %d)", comboCounter, maxCombo);
	iText(50, 500, comboText);

	// Critical hit indicator
	if (criticalTimer > 0) {
		iSetColor(255, 255, 0);
		iText(50, 480, "CRITICAL!");
	}

	// Speed boost indicator  
	if (speedBoostTimer > 0) {
		iSetColor(0, 255, 255);
		iText(50, 460, "SPEED BOOST!");
	}

	// Enemy stun indicator
	if (enemyStunned) {
		iSetColor(255, 100, 100);
		iText(930, 500, "STUNNED!");
	}
	// Level 2 specific controls
	if (battleState != BATTLE_WIN && battleState != BATTLE_LOSE) {
		iSetColor(255, 255, 0);
		iText(400, 120, "DODGE THE WATER PROJECTILES!", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(350, 90, "'Z' = Attack  'X' = Special  'C' = Defend  'SPACE' = Jump", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(300, 60, "Build combos for critical hits! Perfect dodges give speed boost!");

		if (isDefending) {
			iSetColor(255, 255, 0);
			iText(550, 60, "DEFENDING!");
		}

		if (playerJumping) {
			iSetColor(0, 255, 255);
			iText(580, 60, "JUMPING!");
		}

		if (playerMana < 20) {
			iSetColor(255, 100, 100);
			iText(450, 10, "Not enough mana for special attack!");
		}
	}


	// Win/lose messages
	if (battleState == BATTLE_WIN) {
		iSetColor(255, 255, 0);
		iText(580, 350, "LEVEL 2 COMPLETE!", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(570, 320, "Press ENTER to continue", GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else if (battleState == BATTLE_LOSE) {
		if (gameOver) {
			iSetColor(255, 0, 0);
			iText(580, 350, "GAME OVER!", GLUT_BITMAP_TIMES_ROMAN_24);
			iText(550, 320, "Press ENTER to return to menu", GLUT_BITMAP_TIMES_ROMAN_24);
		}
		else {
			iSetColor(255, 0, 0);
			iText(600, 350, "YOU WERE DEFEATED!", GLUT_BITMAP_TIMES_ROMAN_24);
			char livesMsg[100];
			sprintf_s(livesMsg, "Lives remaining: %d - Press ENTER to retry", playerLives);
			iText(500, 320, livesMsg, GLUT_BITMAP_TIMES_ROMAN_24);
		}
	}

	iSetColor(255, 255, 255);
	char livesText[50];
	sprintf_s(livesText, "Lives: %d", playerLives);
	iText(50, 550, livesText);
}

// Fire a projectile
void fireProjectile() {


	// Find an inactive projectile slot
	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (!projectiles[i].active) {
			projectiles[i].active = true;
			projectiles[i].x = playerBattleX + 150; // Start from player's position
			projectiles[i].y = playerBattleY + 80;

			// Calculate velocity towards enemy
			double deltaX = (enemyBattleX + 150) - projectiles[i].x;
			double deltaY = (enemyBattleY + 80) - projectiles[i].y;
			double distance = sqrt(deltaX * deltaX + deltaY * deltaY);

			// Normalize and set speed (adjust speed as needed)
			double speed = 8.0;
			projectiles[i].velocityX = (deltaX / distance) * speed;
			projectiles[i].velocityY = (deltaY / distance) * speed;

			printf("Projectile fired!\n");
			break;
		}
	}
}
// Update projectiles
void updateProjectiles() {
	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (projectiles[i].active) {
			// Move projectile
			projectiles[i].x += projectiles[i].velocityX;
			projectiles[i].y += projectiles[i].velocityY;

			// Check collision with enemy (simple box collision)
			if (projectiles[i].x >= enemyBattleX &&
				projectiles[i].x <= enemyBattleX + 300 &&
				projectiles[i].y >= enemyBattleY &&
				projectiles[i].y <= enemyBattleY + 300) {
				// Hit the enemy
				projectiles[i].active = false;
				projectileHits++;
				printf("Projectile hit! Hits: %d/3\n", projectileHits);
			}

			// Remove projectile if it goes off screen
			if (projectiles[i].x > 1280 || projectiles[i].x < 0 ||
				projectiles[i].y > 720 || projectiles[i].y < 0) {
				projectiles[i].active = false;
			}
		}
	}
}

// Draw projectiles
void drawProjectiles() {
	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (projectiles[i].active) {
			if (projectileImageLoaded && projectileImage > 0) {
				iShowImage((int)projectiles[i].x - 10, (int)projectiles[i].y - 10, 60, 60, projectileImage);
			}
			else {
				// Fallback - draw as yellow circle
				iSetColor(255, 255, 0);
				iFilledCircle((int)projectiles[i].x, (int)projectiles[i].y, 8);
			}
		}
	}
}


void loadAllImages() {
	if (!imagesLoaded) {
		level1bg = iLoadImage("level1bg.png");
		bg1Image = iLoadImage("bg1.png");
		menuImage = iLoadImage("menu1.png");
		settingsImage = iLoadImage("setting.png");
		levelBgImage = iLoadImage("levelBgImage.jpeg");
		club = iLoadImage("club.png");
		on = iLoadImage("on.png");
		off = iLoadImage("off.png");
		aqlevel1bg = iLoadImage("aqlevel1bg.jpg");
		arlevel1bg = iLoadImage("arlevel1bg.jpg");
		terralevel1bg = iLoadImage("terralevel1bg.jpg");
		instructionBgImage = iLoadImage("instructionBgImage.png");
		aqlevel2BattleBg = iLoadImage("aqlevel2BattleBg.png");

		level1Image = iLoadImage("level1Image.png");
		level2Image = iLoadImage("level2Image.png");
		level3Image = iLoadImage("level3Image.png");
		level4Image = iLoadImage("level4Image.png");

		level2Locked = iLoadImage("level2Locked.png");
		level3Locked = iLoadImage("level3Locked.png");
		level4Locked = iLoadImage("level4Locked.png");

		startButton = iLoadImage("start.png");
		instructionButton = iLoadImage("instructions.png");
		settingButton = iLoadImage("settings.png");
		exitButton = iLoadImage("exit.png");

		ignisClub = iLoadImage("ignis.png");
		aquaeClub = iLoadImage("aquae.png");
		terraClub = iLoadImage("terra.png");
		aerisClub = iLoadImage("aeris.png");

		ignisHover = iLoadImage("ignisHover.png");
		aquaeHover = iLoadImage("aquaeHover.png");
		terraHover = iLoadImage("terraHover.png");
		aerisHover = iLoadImage("aerisHover.png");

		if (ignisHover <= 0) ignisHover = ignisClub;
		if (aquaeHover <= 0) aquaeHover = aquaeClub;
		if (terraHover <= 0) terraHover = terraClub;
		if (aerisHover <= 0) aerisHover = aerisClub;

		storyImages[0] = iLoadImage("1.png");
		storyImages[1] = iLoadImage("2.png");
		storyImages[2] = iLoadImage("3.png");
		storyImages[3] = iLoadImage("4.png");
		storyImages[4] = iLoadImage("5.png");
		storyImages[5] = iLoadImage("6.png");
		storyImages[6] = iLoadImage("7.png");


		imagesLoaded = true;
		loadLevel2BattleImages();
		loadLevel3BattleImages();
	}

	if (!characterLoaded) {
		char filename[50];
		for (int i = 0; i < 6; i++) {
			sprintf_s(filename, "walk_right_%d.png", i + 1);
			walkRight[i] = iLoadImage(filename);
		}
		for (int i = 0; i < 6; i++) {
			sprintf_s(filename, "walk_left_%d.png", i + 1);
			walkLeft[i] = iLoadImage(filename);
		}
		characterLoaded = true;
	}

	// Load battle images
	loadBattleImages();
	loadLevel2BattleImages();
}

void initBattle() {
	battleState = BATTLE_IDLE; // Start with idle instead of enemy attack
	playerHealth = 100;
	enemyHealth = 100;
	playerMana = 50;
	enemyMana = 50;
	currentBattleFrame = 0;
	animationTimer = 0;
	battleTimer = 0;
	enemyAttackCooldown = 0;
	continuousAttackTimer = 0;
	playerTurn = false; // Enemy starts at first here
	battleStarted = true;
	isDefending = false;
	// Initialize movement variables
	playerBattleX = initialPlayerX;
	enemyBattleX = initialEnemyX;
	enemyMoving = false;
	enemyStarted = false;
	gameStartTimer = 0;
	enemyAnimationTimer = 0;
	playerAnimationTimer = 0;
	printf("Battle initialized - Enemy attacking immediately!\n");
	initProjectiles();
}

void updateBattle() {
	updateProjectiles();
	if (!battleStarted) return;

	animationTimer++;
	battleTimer++;
	continuousAttackTimer++;

	// Handle battle states
	switch (battleState) {
	case BATTLE_IDLE:
		// Enemy continuously attacks with short breaks
		enemyAttackCooldown++;
		if (enemyAttackCooldown > 350) { // 2.5 second break between attacks
			// Enemy AI: Mix of normal and special attacks
			if (enemyMana >= 20 && rand() % 3 == 0) { // 33% chance for special if mana available
				battleState = BATTLE_ENEMY_SPECIAL;
			}
			else {
				battleState = BATTLE_ENEMY_ATTACK;
			}
			currentBattleFrame = 0;
			animationTimer = 0;
			enemyAttackCooldown = 0;
		}
		break;

	case BATTLE_PLAYER_ATTACK:
		if (animationTimer > 8) {
			currentBattleFrame++;
			animationTimer = 0;
			if (currentBattleFrame >= 7) {
				enemyHealth -= 15;
				currentBattleFrame = 0;
				battleState = BATTLE_IDLE;
				printf("Player attacks! Enemy health: %d\n", enemyHealth);
			}
		}
		break;

	case BATTLE_PLAYER_SPECIAL:
		if (animationTimer > 6) {
			currentBattleFrame++;
			animationTimer = 0;
			if (currentBattleFrame >= 14) {
				enemyHealth -= 30;
				playerMana -= 20;
				currentBattleFrame = 0;
				battleState = BATTLE_IDLE;
				printf("Player special attack! Enemy health: %d, Player mana: %d\n", enemyHealth, playerMana);
			}
		}
		break;

	case BATTLE_PLAYER_DEFEND:
		if (animationTimer > 10) {
			currentBattleFrame++;
			animationTimer = 0;
			if (currentBattleFrame >= 3) {
				currentBattleFrame = 2; // Hold on last defend frame
			}
		}
		// Defending continues until player releases defend key
		break;

	case BATTLE_ENEMY_ATTACK:
		if (animationTimer > 7) {
			currentBattleFrame++;
			animationTimer = 0;
			if (currentBattleFrame >= 9) {
				// Check if player is defending
				int damage = 12;
				if (isDefending) {
					damage = 3; // Greatly reduced damage when defending
					printf("Enemy attacks! Blocked! Player health: %d (-%d)\n", playerHealth - damage, damage);
				}
				else {
					printf("Enemy attacks! Player health: %d (-%d)\n", playerHealth - damage, damage);
				}
				playerHealth -= damage;
				currentBattleFrame = 0;
				battleState = BATTLE_IDLE;
			}
		}
		break;

	case BATTLE_ENEMY_SPECIAL:
		if (animationTimer > 9) {
			currentBattleFrame++;
			animationTimer = 0;
			if (currentBattleFrame >= 7) {
				int damage = 25;
				if (isDefending) {
					damage = 8; // Reduced special damage when defending
					printf("Enemy special attack! Partially blocked! Player health: %d (-%d)\n", playerHealth - damage, damage);
				}
				else {
					printf("Enemy special attack! Player health: %d (-%d)\n", playerHealth - damage, damage);
				}
				playerHealth -= damage;
				enemyMana -= 20;
				currentBattleFrame = 0;
				battleState = BATTLE_IDLE;
			}
		}
		break;
	}

	// Regenerate mana slowly
	if (battleTimer % 60 == 0) {
		if (playerMana < 50) playerMana += 2;
		if (enemyMana < 50) enemyMana += 2;
	}

	// Check win/lose conditions and unlock next level if win
	if (enemyHealth <= 0 && battleState != BATTLE_WIN) {
		battleState = BATTLE_WIN;
		printf("Player wins!\n");
		// Unlock next level after win (if any)
		if (currentLevel >= 1 && currentLevel < 4) {
			levelUnlocked[currentLevel + 1] = 1;
			printf("Level %d unlocked!\n", currentLevel + 1);
		}
	}
	else if (playerHealth <= 0 && battleState != BATTLE_LOSE) {
		battleState = BATTLE_LOSE;
		playerLives--;
		printf("Player loses! Lives remaining: %d\n", playerLives);

		if (playerLives <= 0) {
			gameOver = true;
			printf("Game Over! No lives remaining.\n");
		}
	}
}
void updateMovement() {
	gameStartTimer++;

	// Start enemy movement after 4 seconds (240 frames at 60fps)
	if (gameStartTimer > 0 && !enemyStarted) {
		enemyStarted = true;
		enemyMoving = true;
		printf("Enemy started moving!\n");
	}

	// Update enemy movement
	if (enemyMoving && enemyBattleX > 900) { // Stop at battle position
		enemyBattleX -= walkSpeed;
	}
	else if (enemyMoving) {
		enemyMoving = false; // Stop moving when reached position
		// Start actual battle after enemy is in position
		if (playerBattleX >= 100) {
			battleState = BATTLE_ENEMY_ATTACK;
			printf("Battle phase started!\n");
		}
	}

	// Update player animation when moving
	if (dPressed || aPressed) {
		playerAnimationTimer++;
		if (playerAnimationTimer >= animationSpeed) {
			currentFrame++;
			if (currentFrame >= 6) {
				currentFrame = 0;
			}
			playerAnimationTimer = 0;
		}
	}
}

void drawBattleUI() {
	// Draw health bars
	iSetColor(100, 0, 0);
	iFilledRectangle(50, 650, 300, 30);
	iSetColor(0, 200, 0);
	iFilledRectangle(50, 650, (playerHealth * 300) / 100, 30);
	iSetColor(255, 255, 255);
	char playerHealthText[50];
	sprintf_s(playerHealthText, "Player HP: %d/100", playerHealth);
	iText(50, 630, playerHealthText);

	// Player mana bar
	iSetColor(0, 0, 100);
	iFilledRectangle(50, 600, 200, 20);
	iSetColor(100, 150, 255);
	iFilledRectangle(50, 600, (playerMana * 200) / 50, 20);
	char playerManaText[50];
	sprintf_s(playerManaText, "Mana: %d/50", playerMana);
	iText(50, 580, playerManaText);

	// Enemy health bar
	iSetColor(100, 0, 0);
	iFilledRectangle(930, 650, 300, 30);
	iSetColor(200, 0, 0);
	iFilledRectangle(930, 650, (enemyHealth * 300) / 100, 30);
	char enemyHealthText[50];
	sprintf_s(enemyHealthText, "Enemy HP: %d/100", enemyHealth);
	iText(930, 630, enemyHealthText);

	// Enemy mana bar
	iSetColor(0, 0, 100);
	iFilledRectangle(1030, 600, 200, 20);
	iSetColor(150, 100, 255);
	iFilledRectangle(1030, 600, (enemyMana * 200) / 50, 20);
	char enemyManaText[50];
	sprintf_s(enemyManaText, "Enemy Mana: %d/50", enemyMana);
	iText(1030, 580, enemyManaText);

	// Draw controls - always show available actions
	if (battleState != BATTLE_WIN && battleState != BATTLE_LOSE) {
		iSetColor(255, 255, 0);
		iText(400, 120, "SURVIVE THE CONTINUOUS ATTACKS!", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(390, 90, "'Z' = Attack  'X' = Special  'C' = Defend (Hold)", GLUT_BITMAP_TIMES_ROMAN_24);

		if (isDefending) {
			iSetColor(255, 255, 0);
			iText(550, 60, "DEFENDING!", GLUT_BITMAP_TIMES_ROMAN_24);
		}

		if (playerMana < 20) {
			iSetColor(255, 100, 100);
			iText(450, 30, "Not enough mana for special attack!");
		}
	}

	// Draw win/lose messages
	if (battleState == BATTLE_WIN) {
		iSetColor(255, 255, 0);
		iText(600, 350, "YOU SURVIVED!", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(570, 320, "Press ENTER to continue", GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else if (battleState == BATTLE_LOSE) {
		if (gameOver) {
			iSetColor(255, 0, 0);
			iText(580, 350, "GAME OVER!", GLUT_BITMAP_TIMES_ROMAN_24);
			iText(550, 320, "Press ENTER to return to menu", GLUT_BITMAP_TIMES_ROMAN_24);
		}
		else {
			iSetColor(255, 0, 0);
			iText(600, 350, "YOU WERE DEFEATED!", GLUT_BITMAP_TIMES_ROMAN_24);
			char livesMsg[100];
			sprintf_s(livesMsg, "Lives remaining: %d - Press ENTER to retry", playerLives);
			iText(500, 320, livesMsg, GLUT_BITMAP_TIMES_ROMAN_24);
		}
	}
	// Draw projectile status
	iSetColor(255, 165, 0);
	char projectileStatus[100];
	sprintf_s(projectileStatus, "Hits: %d", projectileHits);
	iText(50, 520, projectileStatus);

	iSetColor(255, 255, 255);
	char livesText[50];
	sprintf_s(livesText, "Lives: %d", playerLives);
	iText(50, 550, livesText);
}

void drawBattleCharacters() {
	int playerImageToShow = -1;
	int enemyImageToShow = -1;

	// Draw player (left side)
	if (battleState == BATTLE_IDLE || battleState < BATTLE_PLAYER_ATTACK) {
		// Movement phase - show walking animation
		if (dPressed && characterLoaded) {
			// Moving right
			if (walkRight[currentFrame] > 0) {
				playerImageToShow = walkRight[currentFrame];
			}
		}
		else if (aPressed && characterLoaded) {
			// Moving left
			if (walkLeft[currentFrame] > 0) {
				playerImageToShow = walkLeft[currentFrame];
			}
		}
		else {
			// Idle - show first frame
			if (walkRight[0] > 0) {
				playerImageToShow = walkRight[0];
			}
		}
	}
	else {
		// Battle phase - existing battle animation code
		switch (battleState) {
		case BATTLE_PLAYER_ATTACK:
			if (currentBattleFrame < 7 && playerAttackFrames[currentBattleFrame] > 0) {
				playerImageToShow = playerAttackFrames[currentBattleFrame];
			}
			break;
		case BATTLE_PLAYER_SPECIAL:
			if (currentBattleFrame < 14 && playerSpecialFrames[currentBattleFrame] > 0) {
				playerImageToShow = playerSpecialFrames[currentBattleFrame];
			}
			break;
		case BATTLE_PLAYER_DEFEND:
			if (currentBattleFrame < 3 && playerDefendFrames[currentBattleFrame] > 0) {
				playerImageToShow = playerDefendFrames[currentBattleFrame];
			}
			else if (currentBattleFrame >= 2 && playerDefendFrames[2] > 0) {
				playerImageToShow = playerDefendFrames[2];
			}
			break;
		default:
			if (isDefending && playerDefendFrames[0] > 0) {
				playerImageToShow = playerDefendFrames[0];
			}
			else if (walkRight[0] > 0) {
				playerImageToShow = walkRight[0];
			}
			break;
		}
	}

	// Display player
	if (playerImageToShow > 0) {
		iShowImage(playerBattleX, playerBattleY, 300, 300, playerImageToShow);
	}
	else {
		if (isDefending) {
			iSetColor(0, 255, 0);
		}
		else {
			iSetColor(0, 0, 255);
		}
		iFilledRectangle(playerBattleX, playerBattleY, 100, 100);
		iSetColor(255, 255, 255);
		iText(playerBattleX + 10, playerBattleY + 50, "PLAYER");
	}

	// Draw enemy (right side) - Use existing enemy frames
	if (battleState >= BATTLE_ENEMY_ATTACK) {
		// Battle phase - existing enemy battle animations
		switch (battleState) {
		case BATTLE_ENEMY_ATTACK:
			if (currentBattleFrame < 9 && enemyAttackFrames[currentBattleFrame] > 0) {
				enemyImageToShow = enemyAttackFrames[currentBattleFrame];
			}
			break;
		case BATTLE_ENEMY_SPECIAL:
			if (currentBattleFrame < 7 && enemySpecialFrames[currentBattleFrame] > 0) {
				enemyImageToShow = enemySpecialFrames[currentBattleFrame];
			}
			break;
		default:
			if (enemyAttackFrames[0] > 0) {
				enemyImageToShow = enemyAttackFrames[0];
			}
			break;
		}
	}
	else {
		// Movement/Idle phase - use first enemy attack frame
		if (enemyAttackFrames[0] > 0) {
			enemyImageToShow = enemyAttackFrames[0];
		}
	}

	// Display enemy
	if (enemyImageToShow > 0) {
		iShowImage(enemyBattleX, enemyBattleY, 300, 300, enemyImageToShow);
	}
	else {
		iSetColor(255, 0, 0);
		iFilledRectangle(enemyBattleX, enemyBattleY, 100, 100);
		iSetColor(255, 255, 255);
		iText(enemyBattleX + 10, enemyBattleY + 50, "ENEMY");
	}

	drawProjectiles();
}
void updateStepAnimation() {
	bool wNewPress = wPressed && !lastWPressed;
	bool aNewPress = aPressed && !lastAPressed;
	bool sNewPress = sPressed && !lastSPressed;
	bool dNewPress = dPressed && !lastDPressed;

	if (wNewPress || aNewPress || sNewPress || dNewPress) {
		currentFrame++;
		if (currentFrame >= 6) {
			currentFrame = 0;
		}
		printf("New step! Current frame: %d\n", currentFrame);
	}

	lastWPressed = wPressed;
	lastAPressed = aPressed;
	lastSPressed = sPressed;
	lastDPressed = dPressed;
}

void checkLevelHover() {
	if (mouseX >= 100 && mouseX <= 350 && mouseY >= 200 && mouseY <= 550) {
		hoveringLevel1 = true;
	}
	else {
		hoveringLevel1 = false;
	}
}

void checkClubHovers() {
	ignisHovered = aquaeHovered = terraHovered = aerisHovered = false;

	int ignisX = clubsStartX;
	int aquaeX = clubsStartX + clubWidth + clubSpacing;
	int terraX = clubsStartX + 2 * (clubWidth + clubSpacing);
	int aerisX = clubsStartX + 3 * (clubWidth + clubSpacing);
	int clubY = 220;

	if (mouseX >= ignisX && mouseX <= (ignisX + clubWidth) &&
		mouseY >= clubY && mouseY <= (clubY + clubHeight)) {
		ignisHovered = true;
	}

	if (mouseX >= aquaeX && mouseX <= (aquaeX + clubWidth) &&
		mouseY >= clubY && mouseY <= (clubY + clubHeight)) {
		aquaeHovered = true;
	}

	if (mouseX >= terraX && mouseX <= (terraX + clubWidth) &&
		mouseY >= clubY && mouseY <= (clubY + clubHeight)) {
		terraHovered = true;
	}

	if (mouseX >= aerisX && mouseX <= (aerisX + clubWidth) &&
		mouseY >= clubY && mouseY <= (clubY + clubHeight)) {
		aerisHovered = true;
	}
}

void checkMenuButtonHovers() {
	if (mouseX >= BUTTON_CENTER_X && mouseX <= (BUTTON_CENTER_X + BUTTON_WIDTH) &&
		mouseY >= START_BUTTON_Y && mouseY <= (START_BUTTON_Y + BUTTON_HEIGHT)) {
		startHovered = true;
	}
	else {
		startHovered = false;
	}

	if (mouseX >= BUTTON_CENTER_X && mouseX <= (BUTTON_CENTER_X + BUTTON_WIDTH) &&
		mouseY >= INSTRUCTION_BUTTON_Y && mouseY <= (INSTRUCTION_BUTTON_Y + BUTTON_HEIGHT)) {
		instructionHovered = true;
	}
	else {
		instructionHovered = false;
	}

	if (mouseX >= BUTTON_CENTER_X && mouseX <= (BUTTON_CENTER_X + BUTTON_WIDTH) &&
		mouseY >= SETTING_BUTTON_Y && mouseY <= (SETTING_BUTTON_Y + BUTTON_HEIGHT)) {
		settingHovered = true;
	}
	else {
		settingHovered = false;
	}

	if (mouseX >= BUTTON_CENTER_X && mouseX <= (BUTTON_CENTER_X + BUTTON_WIDTH) &&
		mouseY >= EXIT_BUTTON_Y && mouseY <= (EXIT_BUTTON_Y + EXIT_BUTTON_HEIGHT)) {
		exitHovered = true;
	}
	else {
		exitHovered = false;
	}
}

void iDraw()
{
	iClear();
	if (currentPage == 1)
	{
		if (bg1Image > 0) {
			iShowImage(0, 0, 1280, 720, bg1Image);
			iSetColor(218, 165, 32);
			iText(400, 150, "Begin your journey into the mystical realm...", GLUT_BITMAP_TIMES_ROMAN_24);
		}
		else {
			iSetColor(50, 50, 100);
			iFilledRectangle(0, 0, 1280, 720);
		}
	}

	//Menu Page
	else if (currentPage == 2)
	{
		if (menuImage > 0) {
			iShowImage(0, 0, 1280, 720, menuImage);
		}
		else {
			iSetColor(100, 50, 50);
			iFilledRectangle(0, 0, 1280, 720);
		}

		checkMenuButtonHovers();

		if (startButton > 0) {
			if (startHovered) {
				iShowImage(BUTTON_CENTER_X - 5, START_BUTTON_Y - 3, BUTTON_WIDTH + 10, BUTTON_HEIGHT + 6, startButton);
			}
			else {
				iShowImage(BUTTON_CENTER_X, START_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, startButton);
			}
		}

		if (instructionButton > 0) {
			if (instructionHovered) {
				iShowImage(BUTTON_CENTER_X - 5, INSTRUCTION_BUTTON_Y - 3, BUTTON_WIDTH + 10, BUTTON_HEIGHT + 6, instructionButton);
			}
			else {
				iShowImage(BUTTON_CENTER_X, INSTRUCTION_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, instructionButton);
			}
		}

		if (settingButton > 0) {
			if (settingHovered) {
				iShowImage(BUTTON_CENTER_X - 5, SETTING_BUTTON_Y - 3, BUTTON_WIDTH + 10, BUTTON_HEIGHT + 6, settingButton);
			}
			else {
				iShowImage(BUTTON_CENTER_X, SETTING_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, settingButton);
			}
		}

		if (exitButton > 0) {
			if (exitHovered) {
				iShowImage(BUTTON_CENTER_X - 5, EXIT_BUTTON_Y - 3, BUTTON_WIDTH + 10, EXIT_BUTTON_HEIGHT + 6, exitButton);
			}
			else {
				iShowImage(BUTTON_CENTER_X, EXIT_BUTTON_Y, BUTTON_WIDTH, EXIT_BUTTON_HEIGHT, exitButton);
			}
		}
	}

	// PAGE 3: Club Selection
	else if (currentPage == 3)
	{
		if (club > 0) {
			iShowImage(0, 0, 1280, 720, club);
		}
		else {
			iSetColor(25, 25, 50);
			iFilledRectangle(0, 0, 1280, 720);
		}

		iSetColor(255, 255, 255);
		iText(480, 630, "CHOOSE YOUR ELEMENTAL CLUB", GLUT_BITMAP_TIMES_ROMAN_24);

		checkClubHovers();

		int ignisX = clubsStartX;
		int aquaeX = clubsStartX + clubWidth + clubSpacing;
		int terraX = clubsStartX + 2 * (clubWidth + clubSpacing);
		int aerisX = clubsStartX + 3 * (clubWidth + clubSpacing);
		int clubY = 220;

		if (ignisClub > 0) {
			iShowImage(ignisX, clubY, clubWidth, clubHeight, ignisClub);
			if (ignisHovered && ignisHover > 0) {
				int hoverX = ignisX + hoverOffsetX;
				int hoverY = clubY + hoverOffsetY;
				iShowImage(hoverX, hoverY, hoverWidth, hoverHeight, ignisHover);
			}
		}
		else {
			iSetColor(255, 69, 0);
			iFilledRectangle(ignisX, clubY, clubWidth, clubHeight);
		}

		iSetColor(255, 140, 0);
		iText(ignisX + 70, clubY + 300, "IGNIS", GLUT_BITMAP_TIMES_ROMAN_24);

		if (aquaeClub > 0) {
			iShowImage(aquaeX, clubY, clubWidth, clubHeight, aquaeClub);
			if (aquaeHovered && aquaeHover > 0) {
				int hoverX = aquaeX + hoverOffsetX;
				int hoverY = clubY + hoverOffsetY;
				iShowImage(hoverX, hoverY, hoverWidth, hoverHeight, aquaeHover);
			}
		}
		else {
			iSetColor(0, 191, 255);
			iFilledRectangle(aquaeX, clubY, clubWidth, clubHeight);
		}

		iSetColor(0, 206, 209);
		iText(aquaeX + 65, clubY + 300, "AQUAE", GLUT_BITMAP_TIMES_ROMAN_24);

		if (terraClub > 0) {
			iShowImage(terraX, clubY, clubWidth, clubHeight, terraClub);
			if (terraHovered && terraHover > 0) {
				int hoverX = terraX + hoverOffsetX;
				int hoverY = clubY + hoverOffsetY;
				iShowImage(hoverX, hoverY, hoverWidth, hoverHeight, terraHover);
			}
		}
		else {
			iSetColor(139, 69, 19);
			iFilledRectangle(terraX, clubY, clubWidth, clubHeight);
		}

		iSetColor(160, 82, 45);
		iText(terraX + 65, clubY + 300, "TERRA", GLUT_BITMAP_TIMES_ROMAN_24);

		if (aerisClub > 0) {
			iShowImage(aerisX, clubY, clubWidth, clubHeight, aerisClub);
			if (aerisHovered && aerisHover > 0) {
				int hoverX = aerisX + hoverOffsetX;
				int hoverY = clubY + hoverOffsetY;
				iShowImage(hoverX, hoverY, hoverWidth, hoverHeight, aerisHover);
			}
		}
		else {
			iSetColor(224, 255, 255);
			iFilledRectangle(aerisX, clubY, clubWidth, clubHeight);
		}

		iSetColor(135, 206, 235);
		iText(aerisX + 65, clubY + 300, "AERIS", GLUT_BITMAP_TIMES_ROMAN_24);

		iSetColor(200, 50, 50);
		iFilledRectangle(100, 60, 120, 45);
		iSetColor(255, 255, 255);
		iText(140, 75, "BACK");
	}

	// PAGE 4: Level Selection
	else if (currentPage == 4)
	{
		if (levelBgImage > 0) {
			iShowImage(0, 0, 1280, 720, levelBgImage);
		}

		iSetColor(255, 255, 255);
		iText(520, 630, "SELECT LEVEL", GLUT_BITMAP_TIMES_ROMAN_24);

		// Level 1
		if (level1Image > 0) {
			if (hoveringLevel1) {
				iShowImage(80, 180, 270, 370, level1Image);
			}
			else {
				iShowImage(100, 200, 250, 350, level1Image);
			}
		}
		// Level 2
		if (levelUnlocked[2]) {
			if (level2Image > 0) iShowImage(370, 200, 250, 350, level2Image);
		}
		else {
			if (level2Locked > 0) iShowImage(370, 200, 250, 350, level2Locked);
		}

		// Level 3
		if (levelUnlocked[3]) {
			if (level3Image > 0) iShowImage(640, 200, 250, 350, level3Image);
		}
		else {
			if (level3Locked > 0) iShowImage(640, 200, 250, 350, level3Locked);
		}

		// Level 4
		if (levelUnlocked[4]) {
			if (level4Image > 0) iShowImage(910, 200, 250, 350, level4Image);
		}
		else {
			if (level4Locked > 0) iShowImage(910, 200, 250, 350, level4Locked);
		}


		iSetColor(200, 50, 50);
		iFilledRectangle(100, 60, 120, 45);
		iSetColor(255, 255, 255);
		iText(140, 75, "BACK");

		checkLevelHover();
	}

	// PAGE 5: Instructions
	else if (currentPage == 5)
	{
		if (instructionBgImage > 0) {
			iShowImage(0, 0, 1280, 720, instructionBgImage);
		}
		else {
			iSetColor(1, 50, 32);
			iFilledRectangle(0, 0, 1280, 720);
		}

		iSetColor(255, 255, 255);
		iText(550, 650, "INSTRUCTIONS", GLUT_BITMAP_TIMES_ROMAN_24);


		iSetColor(255, 255, 0);
		iText(400, 600, "SURVIVAL MODE - DEFEND YOURSELF!", GLUT_BITMAP_TIMES_ROMAN_24);

		iSetColor(255, 255, 255);
		iText(200, 550, "* Enemy attacks continuously - you must survive!", GLUT_BITMAP_HELVETICA_18);
		iText(200, 520, "* Press 'Z' to attack the enemy", GLUT_BITMAP_HELVETICA_18);
		iText(200, 490, "* Press 'X' for special attack (costs 20 mana)", GLUT_BITMAP_HELVETICA_18);
		iText(200, 460, "* Hold 'C' to defend (greatly reduces damage)", GLUT_BITMAP_HELVETICA_18);
		iText(200, 430, "* Mana regenerates slowly over time", GLUT_BITMAP_HELVETICA_18);
		iText(200, 400, "* Defeat the enemy before your health reaches zero!", GLUT_BITMAP_HELVETICA_18);

		iSetColor(255, 100, 100);
		iText(300, 350, "Movement Controls (before battle):", GLUT_BITMAP_HELVETICA_18);
		iText(200, 320, "* Use WASD or Arrow keys to move", GLUT_BITMAP_HELVETICA_18);
		iText(200, 290, "* Move to the center to trigger the battle", GLUT_BITMAP_HELVETICA_18);
		iText(200, 260, "* Press Spacebar to jump.", GLUT_BITMAP_HELVETICA_18);
		iSetColor(200, 50, 50);
		iFilledRectangle(100, 60, 120, 45);
		iSetColor(255, 255, 255);
		iText(140, 75, "BACK");

	}

	// PAGE 6: Battle Game
	else if (currentPage == 6)
	{
		if (level1bg > 0) {
			iShowImage(0, 0, 1280, 720, level1bg);
		}
		else{
			iSetColor(100, 50, 50);
			iFilledRectangle(0, 0, 1280, 720);
		}
	}

	else if (currentPage == 7){
		iShowImage(240, 60, 800, 600, storyImages[storyIndex]);

		iSetColor(200, 50, 50);
		iFilledRectangle(1050, 60, 120, 45);
		iSetColor(255, 255, 255);
		iText(1090, 75, "NEXT");

		iSetColor(200, 50, 50);
		iFilledRectangle(100, 60, 120, 45);
		iSetColor(255, 255, 255);
		iText(140, 75, "BACK");

		iSetColor(255, 255, 0);
		iText(500, 20, "Press SPACE BAR to skip", GLUT_BITMAP_HELVETICA_18);

	}

	else if (currentPage == 8)
	{
		if (aqlevel1bg > 0) {
			iShowImage(0, 0, 1280, 720, aqlevel1bg);
		}
		else{
			iSetColor(100, 50, 50);
			iFilledRectangle(0, 0, 1280, 720);
		}
	}

	else if (currentPage == 9)
	{
		if (terralevel1bg > 0) {
			iShowImage(0, 0, 1280, 720, terralevel1bg);
		}
		else{
			iSetColor(100, 50, 50);
			iFilledRectangle(0, 0, 1280, 720);
		}
	}
	else if (currentPage == 10)
	{
		if (arlevel1bg > 0) {
			iShowImage(0, 0, 1280, 720, arlevel1bg);
		}
		else{
			iSetColor(100, 50, 50);
			iFilledRectangle(0, 0, 1280, 720);
		}
	}
	// PAGE 11: Level 2 Battle
	else if (currentPage == 11)
	{
		if (level2BattleBg > 0) {
			iShowImage(0, 0, 1280, 720, level2BattleBg);
		}
		else {
			iSetColor(50, 100, 150);
			iFilledRectangle(0, 0, 1280, 720);
		}


	}
	else if (currentPage == 12)
	{
		if (aqlevel2BattleBg > 0) {
			iShowImage(0, 0, 1280, 720, aqlevel2BattleBg);
		}
		else {
			iSetColor(50, 100, 150);
			iFilledRectangle(0, 0, 1280, 720);
		}
	}
	else if (currentPage == 13)
	{
		if (level2BattleBg > 0) {
			iShowImage(0, 0, 1280, 720, level2BattleBg);
		}
		else {
			iSetColor(50, 100, 150);
			iFilledRectangle(0, 0, 1280, 720);
		}
	}
	else if (currentPage == 14)
	{
		if (level2BattleBg > 0) {
			iShowImage(0, 0, 1280, 720, level2BattleBg);
		}
		else {
			iSetColor(50, 100, 150);
			iFilledRectangle(0, 0, 1280, 720);
		}
	}
	// PAGE 15: Level 3 Battle
	else if (currentPage == 15)
	{
		if (level3bg > 0) {
			iShowImage(0, 0, 1280, 720, level3bg);
		}
		else {
			iSetColor(50, 50, 100);
			iFilledRectangle(0, 0, 1280, 720);
		}
	}

	//PAGE 16
	// PAGE 16: Settings Page
	else if (currentPage == 16)
	{
		// Show settings background
		if (settingsImage > 0) {
			iShowImage(0, 0, 1280, 720, settingsImage);
		}

		checkSettingsHovers();

		// Sound Button (left)
		if (soundButtonHovered) {
			iSetColor(100, 255, 100);
		}
		else {
			iSetColor(70, 150, 70);
		}
		iFilledRectangle(540,450, 200, 100);
		iSetColor(255, 255, 255);
		iRectangle(540, 450, 200, 100);
		iText(620, 490, "SOUND", GLUT_BITMAP_TIMES_ROMAN_24);

		// Show sound status
		iSetColor(255, 255, 0);
		if (soundEnabled) {
			iText(620, 460, "ON", GLUT_BITMAP_HELVETICA_18);
		}
		else {
			iText(615, 460, "OFF", GLUT_BITMAP_HELVETICA_18);
		}

		// Music Button (right)
		if (musicButtonHovered) {
			iSetColor(100, 100, 255);
		}
		else {
			iSetColor(70, 70, 150);
		}
		iFilledRectangle(540, 320, 200, 100);
		iSetColor(255, 255, 255);
		iRectangle(540, 320, 200, 100);
		iText(620, 360, "MUSIC", GLUT_BITMAP_TIMES_ROMAN_24);

		// Show music status
		iSetColor(255, 255, 0);
		if (musicEnabled) {
			iText(620, 330, "ON", GLUT_BITMAP_HELVETICA_18);
		}
		else {
			iText(615, 330, "OFF", GLUT_BITMAP_HELVETICA_18);
		}

		// Back button
		if (settingsBackHovered) {
			iSetColor(255, 100, 100);
		}
		else {
			iSetColor(200, 50, 50);
		}
		iFilledRectangle(100, 60, 120, 45);
		iSetColor(255, 255, 255);
		iText(140, 75, "BACK");
	}

	else {
		iSetColor(0, 0, 0);
		iText(550, 350, "Page not found!");
	}

	if (currentPage == 6 || currentPage == 8 || currentPage == 9 || currentPage == 10)
	{

		// Check if we're in battle mode or movement mode
		if (battleStarted) {
			// Battle mode - fighting system active
			updateBattle();
			drawBattleCharacters();
			drawBattleUI();
		}
		else {
			// Movement mode - show walking character
			if (characterLoaded) {
				int imageToShow;
				if (facingRight) {
					imageToShow = walkRight[currentFrame];
				}
				else {
					imageToShow = walkLeft[currentFrame];
				}

				if (imageToShow > 0) {
					iShowImage(rec_x, rec_y, 540, 540, imageToShow);
				}
				else {
					// Fallback
					iSetColor(255, 0, 0);
					iFilledRectangle(rec_x, rec_y, 100, 100);
				}
			}

			//step-by-step animation for movement
			updateStepAnimation();

			// Check if player reached battle trigger point (center of screen)
			if (rec_x >= 400 && !battleStarted) {
				initBattle();
				printf("Battle triggered!\n");
			}

			// Show instruction to move to center
			iSetColor(255, 255, 0);
			iText(350, 100, "Move to the center to face the enemy! (Use WASD or Arrow keys)", GLUT_BITMAP_TIMES_ROMAN_24);
		}
	}
	if (currentPage == 11 || currentPage == 12 || currentPage == 13 || currentPage == 14){
		// Check if we're in battle mode or movement mode
		if (battleStarted) {
			// Battle mode - Level 2 fighting system
			updateLevel2Battle();
			drawLevel2BattleCharacters();
			drawLevel2BattleUI();
		}
		else {
			// Movement mode - same as Level 1
			if (characterLoaded) {
				int imageToShow;
				if (facingRight) {
					imageToShow = walkRight[currentFrame];
				}
				else {
					imageToShow = walkLeft[currentFrame];
				}

				if (imageToShow > 0) {
					iShowImage(rec_x, rec_y, 540, 540, imageToShow);
				}
				else {
					iSetColor(255, 0, 0);
					iFilledRectangle(rec_x, rec_y, 100, 100);
				}
			}

			updateStepAnimation();

			// Check if player reached battle trigger point
			if (rec_x >= 400 && !battleStarted) {
				initLevel2Battle();
				printf("Level 2 Battle triggered!\n");
			}

			// Show instruction
			iSetColor(255, 255, 0);
			iText(350, 100, "Move to the center to face the water enemy! (Use WASD or Arrow keys)", GLUT_BITMAP_TIMES_ROMAN_24);
		}
	}
	if (currentPage == 15) {
		// Check if we're in battle mode or movement mode
		if (battleStarted) {
			// Battle mode - Level 3 fighting system
			updateLevel3Battle();
			drawLevel3BattleCharacters();
			drawLevel3BattleUI();
		}
		else {
			// Movement mode - same as other levels
			if (characterLoaded) {
				int imageToShow;
				if (facingRight) {
					imageToShow = walkRight[currentFrame];
				}
				else {
					imageToShow = walkLeft[currentFrame];
				}

				if (imageToShow > 0) {
					iShowImage(rec_x, rec_y, 540, 540, imageToShow);
				}
				else {
					iSetColor(255, 0, 0);
					iFilledRectangle(rec_x, rec_y, 100, 100);
				}
			}

			updateStepAnimation();

			// Check if player reached battle trigger point
			if (rec_x >= 400 && !battleStarted) {
				initLevel3Battle();
				printf("Level 3 Battle triggered!\n");
			}

			// Show instruction
			iSetColor(255, 255, 0);
			iText(300, 100, "Move to the center to face the charging enemy! (Use WASD or Arrow keys)", GLUT_BITMAP_TIMES_ROMAN_24);
		}
	}


	// Sound on/off button (appears on all pages)
	iSetColor(178, 190, 181);
	iFilledRectangle(1190, 665, 70, 35);
	if (soundOn) {
		iShowImage(1190, 665, 70, 35, on);
	}
	else {
		iShowImage(1190, 665, 70, 35, off);
	}
}

void iMouseMove(int mx, int my)
{
	mouseX = mx;
	mouseY = my;
}

void iPassiveMouseMove(int mx, int my)
{
	mouseX = mx;
	mouseY = my;
}

void manageSound(){
	if (soundOn){
		mciSendString("pause bgsong ", NULL, 0, NULL);
		soundOn = false;
	}
	else{
		mciSendString("play bgsong repeat", NULL, 0, NULL);
		soundOn = true;
	}
}

void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// Sound button
		if (mx > 1190 && mx < 1260 && my > 665 && my < 700){
			manageSound();
		}

		if (currentPage == 1){
			if (mx >= 200 && mx <= 1080 && my >= 140 && my <= 160){
				currentPage = 2;
			}
		}

		else if (currentPage == 2) {
			if (mx >= BUTTON_CENTER_X && mx <= (BUTTON_CENTER_X + BUTTON_WIDTH) &&
				my >= START_BUTTON_Y && my <= (START_BUTTON_Y + BUTTON_HEIGHT)) {
				playBeepIfEnabled();
				currentPage = 7;
				printf("Start button clicked!\n");
			}

			else if (mx >= BUTTON_CENTER_X && mx <= (BUTTON_CENTER_X + BUTTON_WIDTH) &&
				my >= INSTRUCTION_BUTTON_Y && my <= (INSTRUCTION_BUTTON_Y + BUTTON_HEIGHT)) {
				playBeepIfEnabled();
				currentPage = 5;
				printf("Instruction button clicked!\n");
			}

			else if (mx >= BUTTON_CENTER_X && mx <= (BUTTON_CENTER_X + BUTTON_WIDTH) &&
				my >= SETTING_BUTTON_Y && my <= (SETTING_BUTTON_Y + BUTTON_HEIGHT)) {
				playBeepIfEnabled();  
				currentPage = 16;     
				printf("Setting button clicked!\n");
			}

			else if (mx >= BUTTON_CENTER_X && mx <= (BUTTON_CENTER_X + BUTTON_WIDTH) &&
				my >= EXIT_BUTTON_Y && my <= (EXIT_BUTTON_Y + EXIT_BUTTON_HEIGHT)) {
				playBeepIfEnabled();
				printf("Exit button clicked!\n");
				exit(0);
			}

		}

		else if (currentPage == 3) {
			int ignisX = clubsStartX;
			int aquaeX = clubsStartX + clubWidth + clubSpacing;
			int terraX = clubsStartX + 2 * (clubWidth + clubSpacing);
			int aerisX = clubsStartX + 3 * (clubWidth + clubSpacing);
			int clubY = 220;

			if (mx >= ignisX && mx <= (ignisX + clubWidth) &&
				my >= clubY && my <= (clubY + clubHeight)) {
				playBeepIfEnabled();
				selectedClub = 1;
				currentPage = 4;
				printf("Ignis Club selected!\n");
			}

			else if (mx >= aquaeX && mx <= (aquaeX + clubWidth) &&
				my >= clubY && my <= (clubY + clubHeight)) {
				playBeepIfEnabled();
				selectedClub = 2;
				currentPage = 4;
				printf("Aquae Club selected!\n");
			}

			else if (mx >= terraX && mx <= (terraX + clubWidth) &&
				my >= clubY && my <= (clubY + clubHeight)) {
				playBeepIfEnabled();
				selectedClub = 3;
				currentPage = 4;
				printf("Terra Club selected!\n");
			}

			else if (mx >= aerisX && mx <= (aerisX + clubWidth) &&
				my >= clubY && my <= (clubY + clubHeight)) {
				playBeepIfEnabled();
				selectedClub = 4;
				currentPage = 4;
				printf("Aeris Club selected!\n");
			}

			else if (mx >= 100 && mx <= 220 && my >= 60 && my <= 105) {
				storyIndex = 6;
				currentPage = 7;
				printf("Back button clicked!\n");
			}
		}

		else if (currentPage == 4) {

			if (mx >= 100 && mx <= 220 && my >= 60 && my <= 105) {
				currentPage = 3;
				//printf("Back to menu from instructions!\n");
			}
			if (selectedClub == 1){
				if (mx >= 100 && mx <= 350 && my >= 200 && my <= 550) {
					if (levelUnlocked[1]) {
						playBeepIfEnabled();
						currentLevel = 1;
						currentPage = 6;
						battleStarted = false; // Reset battle when entering
						printf("Level 1 selected!\n");
					}
					else{
						mciSendString("play locked from 0", NULL, 0, NULL);
						printf("Level 1 locked!\n");
					}
				}

				else if (mx >= 370 && mx <= 620 && my >= 200 && my <= 550){
					if (levelUnlocked[2]) {
						playBeepIfEnabled();
						currentLevel = 2;
						currentPage = 11;
						battleStarted = false; // Reset battle when entering
						resetLevel2Battle();
						printf("Level 2 selected!\n");
					}
					else{
						mciSendString("play locked from 0", NULL, 0, NULL);
						printf("Level 2 locked!\n");
					}
				}
				else if (mx >= 640 && mx <= 890 && my >= 200 && my <= 550){
					if (levelUnlocked[3]) {
						playBeepIfEnabled();
						currentLevel = 3;
						currentPage = 15; 
						battleStarted = false;
						resetLevel3Battle();
						printf("Level 3 selected!\n");
					}
					else{
						mciSendString("play locked from 0", NULL, 0, NULL);
						printf("Level 3 locked!\n");
					}
				}
				else if (mx >= 910 && mx <= 1160 && my >= 200 && my <= 550){
					if (levelUnlocked[4]) {
						playBeepIfEnabled();
						currentLevel = 4;
						currentPage = 6;
						battleStarted = false; // Reset battle when entering
						printf("Level 1 selected!\n");
					}
					else{
						mciSendString("play locked from 0", NULL, 0, NULL);
						printf("Level 1 locked!\n");
					}
				}

			}
			else if (selectedClub == 2){
				if (mx >= 100 && mx <= 350 && my >= 200 && my <= 550) {
					if (levelUnlocked[1]) {
						playBeepIfEnabled();
						currentLevel = 1;
						currentPage = 8;
						battleStarted = false; // Reset battle when entering
						printf("Level 1 selected!\n");
					}
					else{
						mciSendString("play locked from 0", NULL, 0, NULL);
						printf("Level 1 locked!\n");
					}
				}

				else if (mx >= 370 && mx <= 620 && my >= 200 && my <= 550){
					if (levelUnlocked[2]) {
						playBeepIfEnabled();
						currentLevel = 2;
						currentPage = 11;
						battleStarted = false; // Reset battle when entering
						resetLevel2Battle();
						printf("Level 2 selected!\n");
					}
					else{
						mciSendString("play locked from 0", NULL, 0, NULL);
						printf("Level 2 locked!\n");
					}
				}
				else if (mx >= 640 && mx <= 890 && my >= 200 && my <= 550){
					if (levelUnlocked[3]) {
						playBeepIfEnabled();
						currentLevel = 3;
						currentPage = 15;
						battleStarted = false;
						resetLevel3Battle();
						printf("Level 3 selected!\n");
					}
					else{
						mciSendString("play locked from 0", NULL, 0, NULL);
						printf("Level 3 locked!\n");
					}
				}
				else if (mx >= 910 && mx <= 1160 && my >= 200 && my <= 550){
					if (levelUnlocked[4]) {
						playBeepIfEnabled();
						currentLevel = 4;
						currentPage = 8;
						battleStarted = false; // Reset battle when entering
						printf("Level 1 selected!\n");
					}
					else{
						mciSendString("play locked from 0", NULL, 0, NULL);
						printf("Level 1 locked!\n");
					}
				}

			}
			else if (selectedClub == 3){
				if (mx >= 100 && mx <= 350 && my >= 200 && my <= 550) {
					if (levelUnlocked[1]) {
						playBeepIfEnabled();
						currentLevel = 1;
						currentPage = 9;
						battleStarted = false; // Reset battle when entering
						printf("Level 1 selected!\n");
					}
					else{
						mciSendString("play locked from 0", NULL, 0, NULL);
						printf("Level 1 locked!\n");
					}
				}

				else if (mx >= 370 && mx <= 620 && my >= 200 && my <= 550){
					if (levelUnlocked[2]) {
						playBeepIfEnabled();
						currentLevel = 2;
						currentPage = 11;
						battleStarted = false; // Reset battle when entering
						resetLevel2Battle();
						printf("Level 2 selected!\n");
					}
					else{
						mciSendString("play locked from 0", NULL, 0, NULL);
						printf("Level 2 locked!\n");
					}
				}
				else if (mx >= 640 && mx <= 890 && my >= 200 && my <= 550){
					if (levelUnlocked[3]) {
						playBeepIfEnabled();
						currentLevel = 3;
						currentPage = 15;
						battleStarted = false;
						resetLevel3Battle();
						printf("Level 3 selected!\n");
					}
					else{
						mciSendString("play locked from 0", NULL, 0, NULL);
						printf("Level 3 locked!\n");
					}
				}
				else if (mx >= 910 && mx <= 1160 && my >= 200 && my <= 550){
					if (levelUnlocked[4]) {
						playBeepIfEnabled();
						currentLevel = 4;
						currentPage = 9;
						battleStarted = false; // Reset battle when entering
						printf("Level 1 selected!\n");
					}
					else{
						mciSendString("play locked from 0", NULL, 0, NULL);
						printf("Level 1 locked!\n");
					}
				}

			}
			else if (selectedClub == 4){
				if (mx >= 100 && mx <= 350 && my >= 200 && my <= 550) {
					if (levelUnlocked[1]) {
						playBeepIfEnabled();
						currentLevel = 1;
						currentPage = 10;
						battleStarted = false; // Reset battle when entering
						printf("Level 1 selected!\n");
					}
					else{
						mciSendString("play locked from 0", NULL, 0, NULL);
						printf("Level 1 locked!\n");
					}
				}

				else if (mx >= 370 && mx <= 620 && my >= 200 && my <= 550){
					if (levelUnlocked[2]) {
						playBeepIfEnabled();
						currentLevel = 2;
						currentPage = 11;
						battleStarted = false; // Reset battle when entering
						resetLevel2Battle();
						printf("Level 2 selected!\n");
					}
					else{
						mciSendString("play locked from 0", NULL, 0, NULL);
						printf("Level 2 locked!\n");
					}
				}
				else if (mx >= 640 && mx <= 890 && my >= 200 && my <= 550){
					if (levelUnlocked[3]) {
						playBeepIfEnabled();
						currentLevel = 3;
						currentPage = 15;
						battleStarted = false;
						resetLevel3Battle();
						printf("Level 3 selected!\n");
					}
					else{
						mciSendString("play locked from 0", NULL, 0, NULL);
						printf("Level 3 locked!\n");
					}
				}
				else if (mx >= 910 && mx <= 1160 && my >= 200 && my <= 550){
					if (levelUnlocked[4]) {
						playBeepIfEnabled();
						currentLevel = 4;
						currentPage = 10;
						battleStarted = false; // Reset battle when entering
						printf("Level 1 selected!\n");
					}
					else{
						mciSendString("play locked from 0", NULL, 0, NULL);
						printf("Level 1 locked!\n");
					}
				}
			}
		}



		else if (currentPage == 5) {
			if (mx >= 100 && mx <= 220 && my >= 60 && my <= 105) {
				currentPage = 2;
				printf("Back to menu from instructions!\n");
			}
		}

		else if (currentPage == 6) {
			if (mx >= 100 && mx <= 220 && my >= 660 && my <= 705) {
				currentPage = 4;
				battleStarted = false; // Reset battle when leaving
				printf("Back to level selection from game!\n");
			}
		}
		else if (currentPage == 7){
			if (mx >= 1050 && mx <= 1170 && my >= 60 && my <= 105){
				storyIndex++;
				if (storyIndex >= 7){
					storyIndex = 0;
					currentPage = 3;
				}
			}
			else if (mx >= 100 && mx <= 220 && my >= 60 && my <= 105){
				if (storyIndex <= 0){
					currentPage = 2;
				}
				else {
					storyIndex--;
				}
			}
		}

		else if (currentPage == 16) {
			// Sound button clicked
			if (mx >= 540 && mx <= 740 && my >= 450 && my <= 550) {
				soundEnabled = !soundEnabled;
				if (soundEnabled) {
					playBeepIfEnabled();
				}
				printf("Sound toggled!\n");
			}

			// Music button clicked  
			else if (mx >= 540 && mx <= 740 && my >= 320 && my <= 420) {
				musicEnabled = !musicEnabled;
				if (musicEnabled) {
					mciSendString("play bgsong repeat", NULL, 0, NULL);
				}
				else {
					mciSendString("pause bgsong", NULL, 0, NULL);
				}
				playBeepIfEnabled();
			}

			// Back button
			else if (mx >= 100 && mx <= 220 && my >= 60 && my <= 105) {
				playBeepIfEnabled();
				currentPage = 2;
			}
		}
	}
}

void iKeyboard(unsigned char key)
{
	// General controls
	if (key == 27) { // ESC key
		exit(0);
	}
	if (currentPage == 7) { // Story page
		if (key == 'n' || key == 'N') {
			storyIndex++;
			if (storyIndex >= 7) {
				storyIndex = 0;
				currentPage = 3; // After story ends
			}
		}
		else if (key == 'b' || key == 'B') {
			if (storyIndex <= 0) {
				currentPage = 2; // Go to previous page
			}
			else {
				storyIndex--;
			}
		}
		else if (key == ' ' || key == 32) { // Space bar to skip
			storyIndex = 0; // reset if needed
			currentPage = 3; // Jump to next page immediately
		}
	}
}

void iSpecialKeyboard(unsigned char key)
{
	// Handle special keys if needed
}

void fixedUpdate()
{
	if (currentPage == 7) { // Story page
		// Check for new key presses (to avoid continuous triggering)
		static bool lastNPressed = false, lastBPressed = false, lastSpacePressed = false;

		bool nPressed = isKeyPressed('n') || isKeyPressed('N');
		bool bPressed = isKeyPressed('b') || isKeyPressed('B');
		bool spacePressed = isKeyPressed(' ');

		// Only trigger on new press (not while held)
		if (nPressed && !lastNPressed) {
			storyIndex++;
			if (storyIndex >= 7) {
				storyIndex = 0;
				currentPage = 3;
			}
			printf("N pressed - story index: %d\n", storyIndex);
		}

		if (bPressed && !lastBPressed) {
			if (storyIndex <= 0) {
				currentPage = 2;
			}
			else {
				storyIndex--;
			}
			printf("B pressed - story index: %d\n", storyIndex);
		}

		if (spacePressed && !lastSpacePressed) {
			storyIndex = 0;
			currentPage = 3;
			printf("Space pressed - skipping story\n");
		}

		// Update last key states
		lastNPressed = nPressed;
		lastBPressed = bPressed;
		lastSpacePressed = spacePressed;

		return; // Exit early, don't process other controls on story page
	}

	// Level 1 Battle controls (Pages 6, 8, 9, 10)
	else if (currentPage == 6 || currentPage == 8 || currentPage == 9 || currentPage == 10) {
		// Handle battle controls first
		if (battleStarted) {
			if (battleState == BATTLE_IDLE) {
				// Update current key states for movement
				aPressed = isKeyPressed('a') || isKeyPressed('A') || isSpecialKeyPressed(GLUT_KEY_LEFT);
				dPressed = isKeyPressed('d') || isKeyPressed('D') || isSpecialKeyPressed(GLUT_KEY_RIGHT);

				// Move player
				if (dPressed && playerBattleX < 100) { // Can move right until battle position
					playerBattleX += walkSpeed;
					facingRight = true;
				}
				if (aPressed && playerBattleX > -100) { // Can move left but not too far
					playerBattleX -= walkSpeed;
					facingRight = false;
				}
			}

			// Update movement animations and enemy movement
			updateMovement();
			// Update battle key states
			zPressed = isKeyPressed('z') || isKeyPressed('Z');
			xPressed = isKeyPressed('x') || isKeyPressed('X');
			cPressed = isKeyPressed('c') || isKeyPressed('C');
			enterPressed = isKeyPressed('\r');

			// Check for new key presses
			bool zNewPress = zPressed && !lastZPressed;
			bool xNewPress = xPressed && !lastXPressed;
			bool cNewPress = cPressed && !lastCPressed;
			bool enterNewPress = enterPressed && !lastEnterPressed;

			// Handle defending (hold to defend)
			if (cPressed) {
				if (!isDefending) {
					// Start defending
					isDefending = true;
					battleState = BATTLE_PLAYER_DEFEND;
					currentBattleFrame = 0;
					animationTimer = 0;
					printf("Player started defending!\n");
				}
			}
			else {
				if (isDefending) {
					// Stop defending
					isDefending = false;
					if (battleState == BATTLE_PLAYER_DEFEND) {
						battleState = BATTLE_IDLE;
					}
					printf("Player stopped defending!\n");
				}
			}

			// Handle attack controls (can attack while defending)
			if (zNewPress) {
				// Basic attack
				if (battleState == BATTLE_IDLE || battleState == BATTLE_PLAYER_DEFEND) {
					battleState = BATTLE_PLAYER_ATTACK;
					currentBattleFrame = 0;
					animationTimer = 0;
					isDefending = false; // Stop defending when attacking
					fireProjectile();
					mciSendString("play beep from 0", NULL, 0, NULL);
					printf("Player basic attack!\n");
				}
			}
			else if (xNewPress) {
				// Special attack
				if ((battleState == BATTLE_IDLE || battleState == BATTLE_PLAYER_DEFEND) && playerMana >= 20) {
					battleState = BATTLE_PLAYER_SPECIAL;
					currentBattleFrame = 0;
					animationTimer = 0;
					isDefending = false; // Stop defending when attacking
					mciSendString("play ggsong from 0", NULL, 0, NULL);
					printf("Player special attack!\n");
				}
				else if (playerMana < 20) {
					printf("Not enough mana for special attack!\n");
				}
			}
			else if (enterNewPress) {
				if (battleState == BATTLE_WIN) {
					currentPage = 4;
					battleStarted = false;
					printf("Returning to level selection after victory!\n");
				}
				else if (battleState == BATTLE_LOSE) {
					if (gameOver) {
						// Reset everything and go back to main menu
						playerLives = 3;
						gameOver = false;
						currentPage = 2;
						battleStarted = false;
						printf("Game Over! Returning to main menu.\n");
					}
					else {
						// Restart battle with remaining lives
						initBattle();
						printf("Battle restarted! Lives remaining: %d\n", playerLives);
					}
				}
			}

			// Update last key states for battle
			lastZPressed = zPressed;
			lastXPressed = xPressed;
			lastCPressed = cPressed;
			lastEnterPressed = enterPressed;
		}
		else {
			// Movement controls work only when battle is not active
			// Update current key states
			wPressed = isKeyPressed('w') || isKeyPressed('W') || isSpecialKeyPressed(GLUT_KEY_UP);
			aPressed = isKeyPressed('a') || isKeyPressed('A') || isSpecialKeyPressed(GLUT_KEY_LEFT);
			sPressed = isKeyPressed('s') || isKeyPressed('S') || isSpecialKeyPressed(GLUT_KEY_DOWN);
			dPressed = isKeyPressed('d') || isKeyPressed('D') || isSpecialKeyPressed(GLUT_KEY_RIGHT);

			// Move character only on new key press (not while held)
			bool wNewPress = wPressed && !lastWPressed;
			bool aNewPress = aPressed && !lastAPressed;
			bool sNewPress = sPressed && !lastSPressed;
			bool dNewPress = dPressed && !lastDPressed;

			if (wNewPress)
			{
				rec_y += 65;
			}
			if (aNewPress)
			{
				rec_x -= 65;
				facingRight = false;
			}
			if (sNewPress)
			{
				rec_y -= 65;
			}
			if (dNewPress)
			{
				rec_x += 65;
				facingRight = true;
			}

			if (isKeyPressed(' ')) {
				mciSendString("play ggsong from 0", NULL, 0, NULL);
			}

			// Boundary checking
			if (rec_x < 0) rec_x = 0;
			if (rec_x > 740) rec_x = 740;
			if (rec_y < 0) rec_y = 0;
			if (rec_y > 180) rec_y = 180;

			// Update last movement key states
			lastWPressed = wPressed;
			lastAPressed = aPressed;
			lastSPressed = sPressed;
			lastDPressed = dPressed;
		}
	}

	// Level 2 Battle controls (Pages 11, 12, 13, 14)
	else if (currentPage == 11 || currentPage == 12 || currentPage == 13 || currentPage == 14) {
		if (battleStarted) {
			// Handle battle controls
			if (battleState == BATTLE_IDLE) {
				// Update current key states for movement
				aPressed = isKeyPressed('a') || isKeyPressed('A') || isSpecialKeyPressed(GLUT_KEY_LEFT);
				dPressed = isKeyPressed('d') || isKeyPressed('D') || isSpecialKeyPressed(GLUT_KEY_RIGHT);

				// Move player
				if (dPressed && playerBattleX < 100) {
					playerBattleX += walkSpeed;
					facingRight = true;
				}
				if (aPressed && playerBattleX > -100) {
					playerBattleX -= walkSpeed;
					facingRight = false;
				}
			}

			updateMovement();

			// Update battle key states
			zPressed = isKeyPressed('z') || isKeyPressed('Z');
			xPressed = isKeyPressed('x') || isKeyPressed('X');
			cPressed = isKeyPressed('c') || isKeyPressed('C');
			enterPressed = isKeyPressed('\r');
			bool spacePressed = isKeyPressed(' ');

			// Check for new key presses
			bool zNewPress = zPressed && !lastZPressed;
			bool xNewPress = xPressed && !lastXPressed;
			bool cNewPress = cPressed && !lastCPressed;
			bool enterNewPress = enterPressed && !lastEnterPressed;

			// Handle jumping
			if (spacePressed && playerOnGround && !playerJumping) {
				playerJumping = true;
				playerOnGround = false;
				playerJumpVelocity = jumpStrength;
				jumpAnimationFrame = 0;
				jumpAnimationTimer = 0;
				printf("Player jumped!\n");
			}

			// Handle defending
			if (cPressed) {
				if (!isDefending) {
					isDefending = true;
					battleState = BATTLE_PLAYER_DEFEND;
					currentBattleFrame = 0;
					animationTimer = 0;
				}
			}
			else {
				if (isDefending) {
					isDefending = false;
					if (battleState == BATTLE_PLAYER_DEFEND) {
						battleState = BATTLE_IDLE;
					}
				}
			}

			// Handle attacks
			if (zNewPress) {
				if (battleState == BATTLE_IDLE || battleState == BATTLE_PLAYER_DEFEND) {
					battleState = BATTLE_PLAYER_ATTACK;
					currentBattleFrame = 0;
					animationTimer = 0;
					isDefending = false;
					mciSendString("play beep from 0", NULL, 0, NULL);
				}
			}
			else if (xNewPress) {
				if ((battleState == BATTLE_IDLE || battleState == BATTLE_PLAYER_DEFEND) && playerMana >= 20) {
					battleState = BATTLE_PLAYER_SPECIAL;
					currentBattleFrame = 0;
					animationTimer = 0;
					isDefending = false;
					mciSendString("play ggsong from 0", NULL, 0, NULL);
				}
			}
			else if (enterNewPress) {
				if (battleState == BATTLE_WIN) {
					currentPage = 4;
					battleStarted = false;
					printf("Returning to level selection after Level 2 victory!\n");
				}
				else if (battleState == BATTLE_LOSE) {
					if (gameOver) {
						playerLives = 3;
						gameOver = false;
						currentPage = 2;
						battleStarted = false;
						printf("Game Over! Returning to main menu.\n");
					}
					else {
						initLevel2Battle();
						printf("Level 2 battle restarted! Lives remaining: %d\n", playerLives);
					}
				}
			}

			// Update last key states for Level 2 battle
			lastZPressed = zPressed;
			lastXPressed = xPressed;
			lastCPressed = cPressed;
			lastEnterPressed = enterPressed;
		}
		else {
			// Level 2 Movement controls (same as other levels)
			wPressed = isKeyPressed('w') || isKeyPressed('W') || isSpecialKeyPressed(GLUT_KEY_UP);
			aPressed = isKeyPressed('a') || isKeyPressed('A') || isSpecialKeyPressed(GLUT_KEY_LEFT);
			sPressed = isKeyPressed('s') || isKeyPressed('S') || isSpecialKeyPressed(GLUT_KEY_DOWN);
			dPressed = isKeyPressed('d') || isKeyPressed('D') || isSpecialKeyPressed(GLUT_KEY_RIGHT);

			bool wNewPress = wPressed && !lastWPressed;
			bool aNewPress = aPressed && !lastAPressed;
			bool sNewPress = sPressed && !lastSPressed;
			bool dNewPress = dPressed && !lastDPressed;

			if (wNewPress) rec_y += 65;
			if (aNewPress) { rec_x -= 65; facingRight = false; }
			if (sNewPress) rec_y -= 65;
			if (dNewPress) { rec_x += 65; facingRight = true; }

			if (isKeyPressed(' ')) {
				mciSendString("play ggsong from 0", NULL, 0, NULL);
			}

			// Boundary checking
			if (rec_x < 0) rec_x = 0;
			if (rec_x > 740) rec_x = 740;
			if (rec_y < 0) rec_y = 0;
			if (rec_y > 180) rec_y = 180;

			// Update last movement key states
			lastWPressed = wPressed;
			lastAPressed = aPressed;
			lastSPressed = sPressed;
			lastDPressed = dPressed;
		}
	}

	// Level 3 Battle controls (Page 15)
	else if (currentPage == 15) {
		if (battleStarted) {
			// Handle battle controls
			if (battleState == BATTLE_IDLE) {
				// Update current key states for movement
				aPressed = isKeyPressed('a') || isKeyPressed('A') || isSpecialKeyPressed(GLUT_KEY_LEFT);
				dPressed = isKeyPressed('d') || isKeyPressed('D') || isSpecialKeyPressed(GLUT_KEY_RIGHT);

				// Move player
				if (dPressed && playerBattleX < 100) {
					playerBattleX += walkSpeed;
					facingRight = true;
				}
				if (aPressed && playerBattleX > -100) {
					playerBattleX -= walkSpeed;
					facingRight = false;
				}
			}

			updateMovement();

			// Update battle key states
			zPressed = isKeyPressed('z') || isKeyPressed('Z');
			xPressed = isKeyPressed('x') || isKeyPressed('X');
			cPressed = isKeyPressed('c') || isKeyPressed('C');
			enterPressed = isKeyPressed('\r');
			bool spacePressed = isKeyPressed(' ');

			// Check for new key presses
			bool zNewPress = zPressed && !lastZPressed;
			bool xNewPress = xPressed && !lastXPressed;
			bool cNewPress = cPressed && !lastCPressed;
			bool enterNewPress = enterPressed && !lastEnterPressed;

			// Handle jumping (from Level 2)
			if (spacePressed && playerOnGround && !playerJumping) {
				playerJumping = true;
				playerOnGround = false;
				playerJumpVelocity = jumpStrength;
				jumpAnimationFrame = 0;
				jumpAnimationTimer = 0;
				printf("Player jumped!\n");
			}

			// Handle defending
			if (cPressed) {
				if (!isDefending) {
					isDefending = true;
					battleState = BATTLE_PLAYER_DEFEND;
					currentBattleFrame = 0;
					animationTimer = 0;
					printf("Player started defending!\n");
				}
			}
			else {
				if (isDefending) {
					isDefending = false;
					if (battleState == BATTLE_PLAYER_DEFEND) {
						battleState = BATTLE_IDLE;
					}
					printf("Player stopped defending!\n");
				}
			}

			// Handle attacks
			if (zNewPress) {
				// Basic attack with projectile (from Level 1)
				if (battleState == BATTLE_IDLE || battleState == BATTLE_PLAYER_DEFEND) {
					battleState = BATTLE_PLAYER_ATTACK;
					currentBattleFrame = 0;
					animationTimer = 0;
					isDefending = false;
					fireProjectile(); // Fire projectile like Level 1
					mciSendString("play beep from 0", NULL, 0, NULL);
					printf("Player attack with projectile!\n");
				}
			}
			else if (xNewPress) {
				// Special attack
				if ((battleState == BATTLE_IDLE || battleState == BATTLE_PLAYER_DEFEND) && playerMana >= 20) {
					battleState = BATTLE_PLAYER_SPECIAL;
					currentBattleFrame = 0;
					animationTimer = 0;
					isDefending = false;
					mciSendString("play ggsong from 0", NULL, 0, NULL);
					printf("Player special attack!\n");
				}
				else if (playerMana < 20) {
					printf("Not enough mana for special attack!\n");
				}
			}
			else if (enterNewPress) {
				if (battleState == BATTLE_WIN) {
					currentPage = 4;
					battleStarted = false;
					printf("Returning to level selection after Level 3 victory!\n");
				}
				else if (battleState == BATTLE_LOSE) {
					if (gameOver) {
						// Reset everything and go back to main menu
						playerLives = 3;
						gameOver = false;
						currentPage = 2;
						battleStarted = false;
						printf("Game Over! Returning to main menu.\n");
					}
					else {
						// Restart Level 3 battle with remaining lives
						initLevel3Battle();
						printf("Level 3 battle restarted! Lives remaining: %d\n", playerLives);
					}
				}
			}

			// Update last key states
			lastZPressed = zPressed;
			lastXPressed = xPressed;
			lastCPressed = cPressed;
			lastEnterPressed = enterPressed;
		}
		else {
			// Level 3 Movement controls (same as other levels)
			wPressed = isKeyPressed('w') || isKeyPressed('W') || isSpecialKeyPressed(GLUT_KEY_UP);
			aPressed = isKeyPressed('a') || isKeyPressed('A') || isSpecialKeyPressed(GLUT_KEY_LEFT);
			sPressed = isKeyPressed('s') || isKeyPressed('S') || isSpecialKeyPressed(GLUT_KEY_DOWN);
			dPressed = isKeyPressed('d') || isKeyPressed('D') || isSpecialKeyPressed(GLUT_KEY_RIGHT);

			bool wNewPress = wPressed && !lastWPressed;
			bool aNewPress = aPressed && !lastAPressed;
			bool sNewPress = sPressed && !lastSPressed;
			bool dNewPress = dPressed && !lastDPressed;

			if (wNewPress) rec_y += 65;
			if (aNewPress) { rec_x -= 65; facingRight = false; }
			if (sNewPress) rec_y -= 65;
			if (dNewPress) { rec_x += 65; facingRight = true; }

			if (isKeyPressed(' ')) {
				mciSendString("play ggsong from 0", NULL, 0, NULL);
			}

			// Boundary checking
			if (rec_x < 0) rec_x = 0;
			if (rec_x > 740) rec_x = 740;
			if (rec_y < 0) rec_y = 0;
			if (rec_y > 180) rec_y = 180;

			// Update last movement key states
			lastWPressed = wPressed;
			lastAPressed = aPressed;
			lastSPressed = sPressed;
			lastDPressed = dPressed;
		}
	}
}

void resetLevel2Battle() {
	playerJumping = false;
	playerOnGround = true;
	playerJumpY = 0.0;
	playerJumpVelocity = 0.0;
	jumpAnimationFrame = 0;
	jumpAnimationTimer = 0;
	playerBattleX = initialPlayerX;
	enemyBattleX = initialEnemyX;
	initEnemyProjectiles();
	printf("Level 2 battle reset!\n");
}

int main()
{
	// Initialize random seed for AI
	srand(time(NULL));

	mciSendString("open \"Audios//Wii_Music.mp3\" alias bgsong", NULL, 0, NULL);
	mciSendString("open \"Audios//gameover.mp3\" alias ggsong", NULL, 0, NULL);
	mciSendString("open \"Audios//beep.wav\" alias beep", NULL, 0, NULL);
	mciSendString("open \"Audios//locked.wav\" alias locked", NULL, 0, NULL);
	mciSendString("play bgsong repeat", NULL, 0, NULL);

	iInitialize(1280, 720, "The Trials of Elementa");
	loadAllImages();

	iStart();
	return 0;
}
