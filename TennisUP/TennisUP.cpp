#include <raylib.h>

Color Green = Color{ 38, 185, 154, 255 };

int player_score = 0;
int cpu_score = 0;

class Ball 
{
	public:
	float x, y;
	int speed_x, speed_y;
	int radius;

	void Draw() {
		DrawCircle(x, y, radius, WHITE);
	}

	void Update() {
		x += speed_x;
		y += speed_y;

		if (y + radius >= GetScreenHeight() || y - radius <= 0)
		{
			speed_y *= -1;
		}
		if (x + radius >= GetScreenWidth() || x - radius <= 0)
		{
			speed_x *= -1;
		}
		if (x + radius >= GetScreenWidth()) {
			cpu_score++;
			ResetBall();
		}

		if (x - radius <= 0) {
			player_score++;
			ResetBall();
		}
	}

	void ResetBall() {
		x = GetScreenWidth() / 2;
		y = GetScreenHeight() / 2;

		int speed_choices[2] = { -1, 1 };
		speed_x *= speed_choices[GetRandomValue(0, 1)];
		speed_y *= speed_choices[GetRandomValue(0, 1)];
	}
};

class Paddle
{
	protected:
	void LimitMovement() {
		if (y <= 0) {
			y = 0;
		}
		if (y + height >= GetScreenHeight()) {
			y = GetScreenHeight() - height;
		}
	}

	public:
	float x, y;
	float width, height;
	int speed;

	void Draw() {
		DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, BLUE);
	}

	void Update() 
	{
		if (IsKeyDown(KEY_UP))
		{
			y = y - speed;
		}
		if (IsKeyDown(KEY_DOWN))
		{
			y = y + speed;
		}
		LimitMovement();
	}
};

class CpuPaddle : public Paddle
{
public:
	void Draw() {
		DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, RED);
	}
	void Update(int ball_y)
	{
		if (y + height / 2 > ball_y)
		{
			y = y - speed;
		}
		if (y + height / 2 <= ball_y)
		{
			y = y + speed;
		}
		LimitMovement();
	}
};

class Player2Paddle : public Paddle
{
public:
	void Draw() {
		DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, RED);
	}
	void Update()
	{
		if (IsKeyDown(KEY_W))
		{
			y = y - speed;
		}
		if (IsKeyDown(KEY_S))
		{
			y = y + speed;
		}
		LimitMovement();
	}
};

Ball ball;
Paddle player;
Player2Paddle player2;
CpuPaddle cpu;

int main()
{
	const int screenWidth = 1280;
	const int screenHeight = 800;

	InitWindow(screenWidth, screenHeight, "Tennis");
	SetTargetFPS(60);

	ball.radius = 20;
	ball.x = screenWidth / 2;
	ball.y = screenHeight / 2;
	ball.speed_x = 7;
	ball.speed_y = 7;

	player.width = 25;
	player.height = 120;
	player.x = screenWidth - player.width - 10;
	player.y = screenHeight / 2 - player.height / 2;
	player.speed = 6;

	player2.height = 120;
	player2.width = 25;
	player2.x = 10;
	player2.y = screenHeight / 2 - cpu.height / 2;
	player2.speed = 6;

	cpu.height = 120;
	cpu.width = 25;
	cpu.x = 10;
	cpu.y = screenHeight / 2 - cpu.height / 2;
	cpu.speed = 6;

	enum GameState { MENU, GAME, END };
	GameState current_state = MENU;
	int countPlayers = 1;

	while (WindowShouldClose() == false)
	{
		BeginDrawing();

		switch (current_state) 
		{
			case MENU:
			{
				ClearBackground(Green);
				if (countPlayers == 1)
				{
					DrawText("Press SPACE to start the game", screenWidth / 2 - 220, screenHeight / 2 + 100, 30, WHITE);
					DrawText("1 player", screenWidth / 2 - 50, screenHeight / 2 - 100, 40, WHITE);
				}
				else
				{
					DrawText("Press SPACE to start the game", screenWidth / 2 - 220, screenHeight / 2 + 100, 30, WHITE);
					DrawText("2 players", screenWidth / 2 - 50, screenHeight / 2 - 100, 40, WHITE);
				}
				if (IsKeyPressed(KEY_ONE)) {
					countPlayers = 1;
				}
				if (IsKeyPressed(KEY_TWO)) {
					countPlayers = 2;
				}
				if (IsKeyPressed(KEY_SPACE)) {
					current_state = GAME;
				}
				break;
			}
			case GAME:
			{
				ball.Update();
				player.Update();
				if (countPlayers == 1) cpu.Update(ball.y);
				else player2.Update();

				if (CheckCollisionCircleRec({ ball.x, ball.y }, ball.radius, { player.x, player.y, player.width, player.height })) {
					ball.speed_x *= -1;
				}

				if (CheckCollisionCircleRec({ ball.x, ball.y }, ball.radius, { cpu.x, cpu.y, cpu.width, cpu.height })) {
					ball.speed_x *= -1;
				}

				if (CheckCollisionCircleRec({ ball.x, ball.y }, ball.radius, { player2.x, player2.y, player2.width, player2.height })) {
					ball.speed_x *= -1;
				}

				if (IsKeyPressed(KEY_SPACE)) {
					current_state = MENU;
					player_score = 0;
					cpu_score = 0;
				}

				ClearBackground(Green);
				DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, Green);
				DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);
				ball.Draw();
				if (countPlayers == 2) player2.Draw();
				if (countPlayers == 1) cpu.Draw();
				player.Draw();
				DrawText(TextFormat("%i", cpu_score), screenWidth / 4 - 20, 20, 80, WHITE);
				DrawText(TextFormat("%i", player_score), 3 * screenWidth / 4 - 20, 20, 80, WHITE);
				if (cpu_score == 5 || player_score == 5) current_state = END;
				break;
			}
			case END:
			{
				ClearBackground(Green);
				if (player_score == 5)
				{
					DrawText("Blue Victory!!!", screenWidth / 2 - 200, screenHeight / 2 - 100, 60, BLUE);
				}
				else
				{
					DrawText("Red Victory!!!", screenWidth / 2 - 200, screenHeight / 2 - 100, 60, RED);
				}
				DrawText("Press SPACE to continue", screenWidth / 2 - 140, screenHeight / 2 + 100, 20, WHITE);
				if (IsKeyPressed(KEY_SPACE)) {
					current_state = MENU;
					player_score = 0;
					cpu_score = 0;
				}
				break;
			}
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
 