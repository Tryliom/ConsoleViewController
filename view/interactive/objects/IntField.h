#pragma once
#include "BasicField.h"

namespace Console
{
	class IntField: public BasicField
	{
	protected:
		bool _enableLeftRightArrow{ true };

		std::string GetStr() const override;
		void onBackspace() override;
	public:
		IntField(PositionX x, PositionY y, const std::function<int()>& getValue, const std::function<void(int)>& setValue, bool xCentered = false, bool enableLeftRightArrow = true);

		void OnKeyPress(char key) override;
	};
}
