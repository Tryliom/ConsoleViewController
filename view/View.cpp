#include "View.h"

#include <algorithm>

namespace Console
{
	View::View(const int defaultButton)
	{
		_currentButton = defaultButton;
	}

	void View::Update(Screen& screen)
	{
		for (const auto& component : _components)
		{
			component->Draw(screen, component == _components[_currentButton]);
		}
	}

	void View::OnKeyPressed(const char key)
	{
		const int maxButtons = getMaxButton();

		if (maxButtons == 0)
			return;

		if (_currentButton >= maxButtons)
		{
			_currentButton = maxButtons - 1;
		}
		if (_currentButton < 0)
		{
			_currentButton = 0;
		}

		if (_currentButton >= 0)
		{
			_components[_currentButton]->OnKeyPress(key);
		}
	}

	void View::move(bool isHorizontal, bool isPositive)
	{
		std::vector<InteractiveObject*> nearestButtons;

		for (const auto& component : _components)
		{
			if (isHorizontal)
			{
				if (isPositive ? 
						component->GetX().GetValue() > _components[_currentButton]->GetX().GetValue() :
						component->GetX().GetValue() < _components[_currentButton]->GetX().GetValue()
					)
				{
					nearestButtons.push_back(component);
				}
			}
			else
			{
				if (isPositive ?
						component->GetY().GetValue() > _components[_currentButton]->GetY().GetValue() :
						component->GetY().GetValue() < _components[_currentButton]->GetY().GetValue()
					)
				{
					nearestButtons.push_back(component);
				}
			}
		}

		std::ranges::sort(
			nearestButtons.begin(), 
			nearestButtons.end(), 
			[this, isHorizontal](const InteractiveObject* a, const InteractiveObject* b)
				{
					const int aDistanceX = abs(a->GetX().GetValue() - _components[_currentButton]->GetX().GetValue());
					const int aDistanceY = abs(a->GetY().GetValue() - _components[_currentButton]->GetY().GetValue());
					const int bDistanceX = abs(b->GetX().GetValue() - _components[_currentButton]->GetX().GetValue());
					const int bDistanceY = abs(b->GetY().GetValue() - _components[_currentButton]->GetY().GetValue());

					if (isHorizontal)
					{
						if (aDistanceY == bDistanceY)
						{
							return aDistanceX < bDistanceX;
						}
						
						return aDistanceY < bDistanceY;
					}

					if (aDistanceX == bDistanceX)
					{
						return aDistanceY < bDistanceY;
					}

					return aDistanceX < bDistanceX;
				}
		);

		if (!nearestButtons.empty())
		{
			// Get index of the first button of the nearestButton list
			_currentButton = static_cast<int>(
				std::distance(
					_components.begin(), 
					std::find(
						_components.begin(), 
						_components.end(), 
							nearestButtons[0]
						)
				)
			);
		}
	}

	void View::moveBetweenComponents(const char key, const bool horizontal, const bool vertical)
	{
		if (_components.empty())
			return;

		if (Key::Up == key && vertical)
		{
			move(false, false);
		}
		else if (Key::Down == key && vertical)
		{
			move(false, true);
		}
		else if (Key::Left == key && horizontal)
		{
			move(true, false);
		}
		else if (Key::Right == key && horizontal)
		{
			move(true, true);
		}
	}
}
