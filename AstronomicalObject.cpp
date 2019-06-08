#include "AstronomicalObject.h"
#include <cmath>

AstronomicalObject::AstronomicalObject(){}

AstronomicalObject::AstronomicalObject(bool _stationaryObject, std::string _name, unsigned int _mass, Vector2 _position, Velocity _velocity, float _radius, olc::Pixel _color) {
	stationaryObject = _stationaryObject;
	//std::cout << _name<< ": created\n";
	name = _name;
	mass = _mass;
	position = _position;
	velocity = _velocity;
	radius = _radius;
	color = _color;
}

float AstronomicalObject::GetMass() {
	return mass;
}

Vector2 AstronomicalObject::GetPosition() {
	return position;
}

Velocity AstronomicalObject::GetVelocity() {
	return velocity;
}

float AstronomicalObject::GetRadius() {
	return radius;
}

void AstronomicalObject::AddNearbyObject(AstronomicalObject* object) {
	if (object != this) {
		nearbyObjects.emplace_back(object);
	}
}

void AstronomicalObject::UpdatePosition(float deltaTime) {
	if (stationaryObject) return;
	for (auto &body : nearbyObjects) {
		gravitationalVector = CalculateGravitationalVector(*body, *this);
		this->velocity.direction = AddVector(gravitationalVector, velocity.direction);
		//if (body->name == "Sun" || body->name == "Sun2") {
		//	//std::cout << name << ": new velocity = " << velocity.direction.x << " " << velocity.direction.y << std::endl;
		//}
	}
	//velocity.speed = 0.0005f;
	position.x += velocity.direction.x * velocity.speed;
	position.y += velocity.direction.y * velocity.speed;
	orbitalPositionsUpdateCount++;
	if (orbitalPositionsUpdateCount >= 100) {
		if (orbitalPositions.size() > 300) {
			orbitalPositions.erase(orbitalPositions.begin());
		}
		//std::cout << "speed: " << velocity.speed << std::endl;
		orbitalPositions.emplace_back(position);
		orbitalPositionsUpdateCount = 0;
	}

	//std::cout << "new position: " << position.x << " " << position.y << std::endl;
}

float AstronomicalObject::Distance(Vector2 point1, Vector2 point2) {
	return (sqrt(pow((point1.x - point2.x), 2) + pow((point1.y - point2.y), 2)));
}

Vector2 AstronomicalObject::CalculateGravitationalVector(AstronomicalObject sourceObject, AstronomicalObject affectedObject) {
	float gForce = GravitationalForce(sourceObject, affectedObject) * 1000;
	Vector2 unitVector = GetUnitVector(sourceObject.position, affectedObject.position);
	return(Vector2(gForce * unitVector.x, gForce * unitVector.y));
}


float AstronomicalObject::GravitationalForce(AstronomicalObject sourceObject, AstronomicalObject affectedObject) {
	return ((GRAVITATIONAL_CONSTANT * sourceObject.GetMass() * affectedObject.GetMass()) / (pow(Distance(sourceObject.position, affectedObject.position) * PIXEL_TO_DISTANCE_SCALE, 2)));
}

Vector2 AstronomicalObject::GetVectorFromAToB(Vector2 thisObject, Vector2 thatObject) {
	return(Vector2(thisObject.x - thatObject.x, thisObject.y - thatObject.y));
}

float AstronomicalObject::GetVectorMagnitude(Vector2 vector) {
	return(sqrt((pow(vector.x, 2) + pow(vector.y, 2))));
}

Vector2 AstronomicalObject::TransformIntoUnityVector(Vector2 vector, float magnitude) {
	return(Vector2(vector.x / magnitude, vector.y / magnitude));
}

Vector2 AstronomicalObject::GetUnitVector(Vector2 thisObjectPos, Vector2 thatObjectPos) {
	Vector2 vector = GetVectorFromAToB(thisObjectPos, thatObjectPos);
	return (TransformIntoUnityVector(vector, GetVectorMagnitude(vector)));
}

Vector2 AstronomicalObject::AddVector(Vector2 vector1, Vector2 vector2) {
	return Vector2(vector1.x + vector2.x, vector1.y + vector2.y);
}
/*
	Vector2 vector = Vector2(affectedObject.position.x - sourceObject.position.x, affectedObject.position.y - sourceObject.position.y);
	float gForce = GravitationalForce(sourceObject, affectedObject);
	//float acceleration = gForce * 10000 / affectedObject.solarMass;
	//affectedObject.velocity.speed += acceleration;
	//affectedObject.velocity.speed += gForce * 10000 / affectedObject.solarMass;
	//std::cout << "gforce " << gForce << std::endl;
	return Vector2(vector.x * -gForce * 10000, vector.y * -gForce * 10000);
*/