#include "IntField.h"

#include <stdexcept>

std::string Console::IntField::GetStr() const
{	
	return "<" + _str + ">";
}

Console::IntField::IntField(PositionX x, PositionY y, const std::function<int()>& getValue,
                            const std::function<void(int)>& setValue, const bool xCentered, const bool enableLeftRightArrow) :
	BasicField(x, y, 
		[getValue]() { return std::to_string(getValue()); }, 
		[setValue, this](const std::string& str)
		{
			try
			{
				const int value = std::stoi(str);
				setValue(value);
			}
			catch (std::exception)
			{
				onBackspace();
			}
		}, 
	           IsDigit, xCentered, false)
{
	_enableLeftRightArrow = enableLeftRightArrow;
}

void Console::IntField::OnKeyPress(const char key)
{
	BasicField::OnKeyPress(key);

	if (_str.length() > 1 && _str.starts_with("0"))
	{
		_str.erase(0, 1);
	}

	if (key == '-' && !_str.starts_with("0") && !_str.starts_with("-"))
	{
		_str.insert(0, "-");
	}

	if (_enableLeftRightArrow)
	{
		const int value = std::stoi(_str);

		if (key == Key::Left)
		{
			_str = std::to_string(value - 1);
		}
		else if (key == Key::Right)
		{
			_str = std::to_string(value + 1);
		}
	}

	_setValue(_str);
}

void Console::IntField::onBackspace()
{
	if (_str.length() > 0)
	{
		_str.pop_back();
		if (_str.empty())
		{
			_str = "0";
		}
		else if (_str == "-")
		{
			_str = "0";
		}
		_setValue(_str);
	}
}

