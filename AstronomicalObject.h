#ifndef ASTRONOMICAL_OBJECT
#define ASTRONOMICAL_OBJECT
#include "olcPixelGameEngine.h"
#include <cstring>
#include <vector>

const float GRAVITATIONAL_CONSTANT = 6.67408E-11;
const float PIXEL_TO_DISTANCE_SCALE = 10.0f;

struct Vector2
{
	float x = 0.0f;
	float y = 0.0f;
public:
	Vector2() {	}
	Vector2(float _x, float _y) {
		x = _x;
		y = _y;
	}
};

struct Velocity {
	Vector2 direction;
	float speed = 0;
public:
	Velocity() {}
	Velocity(Vector2 _direction, Vector2 point1, Vector2 point2, float scale) {
		direction = _direction;
		speed = (sqrt(pow((point1.x - point2.x), 2) + pow((point1.y - point2.y), 2))) * scale;
	}
	Velocity(Vector2 _direction, float _velocity) {
		direction = _direction;
		speed = _velocity;
	}
};

class AstronomicalObject {
private:
	bool stationaryObject = false;
	unsigned int mass = 0;
	Vector2 position;
	Velocity velocity;
	float radius = 0.0f;
	int orbitalPositionsUpdateCount = 0;

public:
	Vector2 gravitationalVector;
	std::vector<Vector2> orbitalPositions;
	std::string name = "noname";
	std::vector<AstronomicalObject*> nearbyObjects;
	olc::Pixel color = olc::WHITE;
	AstronomicalObject();
	AstronomicalObject(bool _stationaryObject, std::string _name,unsigned int _solarMass, Vector2 _position, Velocity _velocity, float _radius, olc::Pixel _color);
	float GetMass();
	Vector2 GetPosition();
	Velocity GetVelocity();
	float GetRadius();
	void AddNearbyObject(AstronomicalObject *object);

	void UpdatePosition(float deltaTime);

private:
	float Distance(Vector2 point1, Vector2 point2);
	Vector2 CalculateGravitationalVector(AstronomicalObject sourceObject, AstronomicalObject affectedObject);
	float GravitationalForce(AstronomicalObject sourceObject, AstronomicalObject affectedObject);
	Vector2 AddVector(Vector2 vector1, Vector2 vector2);
	Vector2 GetVectorFromAToB(Vector2 vector1, Vector2 vector2);
	Vector2 TransformIntoUnityVector(Vector2 vector, float magnitude);
	Vector2 GetUnitVector(Vector2 thisObjectPos, Vector2 thatObjectPos);
	float GetVectorMagnitude(Vector2 vector);
};


#endif // !ASTRONOMICAL_OBJECT
