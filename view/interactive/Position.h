#pragma once
class Position
{
private:
	int _absolutePosition{0};
	float _relativePosition{0.0f};

	void check();
public:
	Position();
	Position(int absolutePosition, float relativePosition);
	explicit Position(int absolutePosition);
	explicit Position(float relativePosition);

	int GetValue(bool isX) const;
};

