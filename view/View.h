#pragma once
#include "Screen.h"
#include "interactive/InteractiveObject.h"

namespace Console
{
	class Controller;
	class InteractiveObject;

	class View
	{
	private:
		int _currentButton{ 0 };
		std::vector<InteractiveObject*> _components;

		// Search the nearest component in the given direction
		void move(bool isHorizontal, bool isPositive);

	protected:
		void setComponents(const std::vector<InteractiveObject*>& components) { _components = components; }
		void addComponent(InteractiveObject* component) { _components.push_back(component); }

		int getMaxButton() const { return static_cast<int>(_components.size()); }
		void setCurrentButton(const int currentButton) { if (currentButton >= 0) _currentButton = currentButton; }
		void incrementCurrentButton() { _currentButton++; }
		void decrementCurrentButton() { _currentButton--; }
		int getCurrentButton() const { return _currentButton; }

		void removeComponent(const int index) { _components.erase(_components.begin() + index); }
		void removeComponent(InteractiveObject* component) { _components.erase(std::remove(_components.begin(), _components.end(), component), _components.end()); }
		template<class T>
		T* getComponent(const int index) const { return dynamic_cast<T*>(_components[index]); }
		void clearComponents() { _components.clear(); }

		/**
		 * \brief Used to move to the next button according to the position of the current button
		 * and the direction taken by the user. Need to be called in the OnKeyPressed method.
		 * \param key The key pressed by the user
		 * \param horizontal If enabled, the user can move horizontally (left and right)
		 * \param vertical If enabled, the user can move vertically (up and down)
		 */
		void moveBetweenComponents(char key, bool horizontal = true, bool vertical = true);

	public:
		View(int defaultButton = 0);

		/**
		 * \brief Call before the render function to prepare the screen
		 * \param screen The screen to draw on
		 */
		virtual void Update(Screen& screen);
		/**
		 * \brief Call when the user press a key, need to be call at the end of the Update function to correct the button selection
		 * \param key The key pressed
		 */
		virtual void OnKeyPressed(char key);
		/**
		 * \brief Called every 50ms
		 * \param deltaTime The time since the last tick in nanoseconds
		 */
		virtual void OnTick(int deltaTime) {}
	};
}


