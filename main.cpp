#define OLC_PGE_APPLICATION
#include <iostream>
#include "AstronomicalObject.h"
#include "olcPixelGameEngine.h"
#include <string>
#include <vector>

class MyApplication : public olc::PixelGameEngine {
private:
	float deltaTime;
	float scale = 1.0f;
	AstronomicalObject *sun;
	AstronomicalObject *sun2;
	AstronomicalObject *sun3;
	AstronomicalObject *tempPlanet;
	bool isPlacingNewPlanet = false;
	float distanceToSpeedScale = 0.00001f;
	std::vector<AstronomicalObject*> astronomicalObjects;

public:
	MyApplication() {
		sAppName = "Gravity Simulation";
		astronomicalObjects.reserve(20);
	}

	bool OnUserCreate() override {
		sun =  new AstronomicalObject(true,"Sun", 500000000, Vector2(ScreenWidth()/2, ScreenHeight()/2), Velocity(Vector2(0.0, 0.0), 0.0), 5.0, olc::YELLOW);
		astronomicalObjects.emplace_back(sun);
		//sun2 = new AstronomicalObject(true,"Sun2", 500000000, Vector2(ScreenWidth()/2 + 100, ScreenHeight()/2), Velocity(Vector2(0.0, 0.0), 0.0), 5.0, olc::YELLOW);
		//astronomicalObjects.emplace_back(sun2);
		//sun3 = new AstronomicalObject(true,"Sun3", 500000000, Vector2(ScreenWidth()/2 + 0, ScreenHeight()/2 + 100), Velocity(Vector2(0.0, 0.0), 0.0), /5.0,olc::YELLOW);
		//astronomicalObjects.emplace_back(sun3);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		//std::cout << fElapsedTime << std::endl;
		deltaTime = fElapsedTime;
		if (GetKey(olc::R).bPressed) {
			if (isPlacingNewPlanet) {
				AddNewTempPlanetAsNewBody();
			}
			else {
				isPlacingNewPlanet = true;
				tempPlanet = new AstronomicalObject(false,"tempplanet", 50, Vector2(GetMouseX(), GetMouseY()), Velocity(Vector2(0.0, 0.0), 0.0), 2.0, olc::WHITE);
			}
		}
		UpdateBodies();
		UpdateScreen();
		return true;
	}

	void RefreshScreen() {
		Clear(olc::BLACK);
	}

	void UpdateScreen() {
		RefreshScreen();
		if (isPlacingNewPlanet) {
			FillCircle(tempPlanet->GetPosition().x, tempPlanet->GetPosition().y, tempPlanet->GetRadius() * scale, tempPlanet->color);
			DrawLine(GetMouseX(), GetMouseY(), tempPlanet->GetPosition().x, tempPlanet->GetPosition().y, olc::RED);
		}
		for (auto body : astronomicalObjects) {
			FillCircle(body->GetPosition().x, body->GetPosition().y, body->GetRadius() * scale, body->color);
			DrawCircle(body->GetPosition().x, body->GetPosition().y, body->GetRadius()* scale + 1);
			//DrawLine(body->GetPosition().x, body->GetPosition().y, body->GetVelocity().direction.x, body->GetVelocity().direction.y, olc::RED);
			//DrawLine(body->GetPosition().x, body->GetPosition().y, body->gravitationalVector.x, body->gravitationalVector.y, olc::GREEN);
			for (auto pos : body->orbitalPositions) {
				Draw(pos.x, pos.y, olc::GREEN);
			}
		}
	}

	void AddNewTempPlanetAsNewBody() {
		AstronomicalObject* planetToAdd = new AstronomicalObject(
			false,
			"planet" + std::to_string(astronomicalObjects.size() + 1),
			500,
			Vector2(tempPlanet->GetPosition().x, tempPlanet->GetPosition().y),
			Velocity(Vector2(GetMouseX() - tempPlanet->GetPosition().x, GetMouseY() - tempPlanet->GetPosition().y), tempPlanet ->GetPosition(), Vector2(GetMouseX(), GetMouseY()), distanceToSpeedScale),
			2.0f,
			olc::WHITE);
		for (auto body : astronomicalObjects) {
			body->AddNearbyObject(planetToAdd);
			planetToAdd->AddNearbyObject(body);
		}
		astronomicalObjects.emplace_back(planetToAdd);
		delete tempPlanet;
		isPlacingNewPlanet = false;
	}

	void UpdateBodies() {
		for (auto body : astronomicalObjects) {
			body->UpdatePosition(deltaTime);
		}
	}
};



int main(int argc, char* argv[]) {
	
	MyApplication myApplication;

	if (myApplication.Construct(500, 500, 2, 2)) {
		myApplication.Start();
	}

	return 0;
}