#pragma once
#include "../view/View.h"
#include "../view/Screen.h"

#include <functional>
#include <stack>

namespace Console
{
	class View;

	class Controller
	{
	protected:
		Screen _screen;
		// The current view
		View* _view{ nullptr };
		// The previous views
		std::stack<View*> _views;
		// Used to execute operations at the end of the update
		std::vector<std::function<void()>> _queue;

	private:
		/**
		 * \brief Refresh screen rendering
		 */
		void refresh();
		/**
		 * \brief Used to draw things on the screen before rendering
		 */
		void update();
		/**
		 * \brief Called when a key is pressed
		 * \param key The key pressed
		 */
		void onKeyPressed(char key);
		/**
		 * \brief Called every 50ms
		 * \param deltaTime The time since the last tick
		 */
		void onTick(int deltaTime);

		// Used to calculate the FPS
		int _fpsCounter{ 0 };
	public:
		// The current FPS (updated every second)
		inline static int FPS = 0;
		// Incremented every tick
		inline static int TICK = 0;

		/**
		 * \brief Start the controller
		 */
		void Start();
		/**
		 * \brief Switch to a new view
		 * \param view The new view
		 */
		void ChangeView(View* view);
		/**
		 * \brief Set a new view without pushing the current view to the stack
		 * \param view The new view
		 */
		void SetView(View* view);
		/**
		 * \brief Go back to previous view
		 */
		void GoBack();
		/**
		 * \brief Clear the stack views
		 */
		void ClearStack();
		/**
		 * \brief Add an operation to the queue
		 * \param func The operation
		 */
		void AddToQueue(const std::function<void()>& func) { _queue.push_back(func); }
	};
}


