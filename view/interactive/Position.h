#pragma once
class Position
{
protected:
	int _absolutePosition{0};
	float _relativePosition{0.0f};
public:
	Position();
	Position(int absolutePosition, float relativePosition);
	explicit Position(int absolutePosition);
	explicit Position(float relativePosition);

	virtual int GetValue() const = 0;
};

class PositionX: public Position {
public:
	PositionX() : Position() {}
	PositionX(int absolutePosition, float relativePosition) : Position(absolutePosition, relativePosition) {}
	explicit PositionX(int absolutePosition) : Position(absolutePosition) {}
	explicit PositionX(float relativePosition) : Position(relativePosition) {}

	int GetValue() const override;
};

class PositionY : public Position {
public:
	PositionY() : Position() {}
	PositionY(int absolutePosition, float relativePosition) : Position(absolutePosition, relativePosition) {}
	explicit PositionY(int absolutePosition) : Position(absolutePosition) {}
	explicit PositionY(float relativePosition) : Position(relativePosition) {}

	int GetValue() const override;
};