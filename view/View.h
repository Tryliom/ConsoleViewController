#pragma once
#include "Screen.h"
#include "../controller/Controller.h"
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
	protected:
		void setComponents(const std::vector<InteractiveObject*>& components) { _components = components; }
		void AddComponent(InteractiveObject* component) { _components.push_back(component); }
	public:
		View(int defaultButton = 0);
		/**
		 * \brief Call before the render function to prepare the screen
		 * \param controller The controller to get the data from
		 * \param screen The screen to draw on
		 */
		virtual void Update(Controller* controller, Screen& screen);
		/**
		 * \brief Call when the user press a key, need to be call at the end of the Update function to correct the button selection
		 * \param controller The controller to get the data from
		 * \param key The key pressed
		 */
		virtual void OnKeyPressed(Controller* controller, char key);

		int GetMaxButton() const { return static_cast<int>(_components.size()); }
		void SetCurrentButton(const int currentButton) { if (currentButton >= 0) _currentButton = currentButton; }
		void IncrementCurrentButton() { _currentButton++; }
		void DecrementCurrentButton() { _currentButton--; }
		int GetCurrentButton() const { return _currentButton; }
		void RemoveComponent(const int index) { _components.erase(_components.begin() + index); }
		void RemoveComponent(InteractiveObject* component) { _components.erase(std::remove(_components.begin(), _components.end(), component), _components.end()); }
		template<class T>
		T* GetComponent(const int index) const { return dynamic_cast<T*>(_components[index]); }
		void ClearComponents() { _components.clear(); }
	};
}


