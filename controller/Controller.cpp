#include "Controller.h"
#include "../utilities/Utility.h"

#include <conio.h>
#include <thread>

namespace Console
{
	void Controller::GoBack()
	{
		this->_view = this->_views.top();
		this->_views.pop();
	}

	void Controller::ClearStack()
	{
		_views = std::stack<View*>();
	}

	void Controller::update()
	{
		// Update the view
		if (this->_view != nullptr)
		{
			this->_view->Update(this->_screen);
		}

		for (auto& func : this->_queue)
		{
			func();
		}

		this->_queue.clear();
	}

	void Controller::onKeyPressed(const char key)
	{
		// If the view is not null, call the view's onKeyPressed method
		if (this->_view != nullptr)
		{
			this->_view->OnKeyPressed(key);
		}

		// If the key is Escape, exit the program or go back to the last view
		if (key == Key::Escape)
		{
			if (_views.empty())
			{
				// Close the program
				this->_view = nullptr;
				Utility::sleep(300);
				exit(0);
			}

			GoBack();
		}
	}

	void Controller::onTick(const int deltaTime)
	{
		// If the view is not null, call the view's OnTick method
		if (this->_view != nullptr)
		{
			this->_view->OnTick(deltaTime);
		}
	}

	void Controller::refresh()
	{
		_screen.Reset();
		this->update();
		_screen.Render();
	}

	void Controller::Start()
	{
		// Start a thread to receive key presses
		std::thread keyThread([this]()
			{
				// Get every key pressed
				while (const char key = static_cast<char>(_getch()))
				{
					this->onKeyPressed(key);
				}
			}
		);
		keyThread.detach();

		auto now = std::chrono::steady_clock::now();
		int time = 0;

		while (true)
		{
			// Get the time difference between now and the last tick
			const auto deltaTime = static_cast<int>((std::chrono::steady_clock::now() - now).count());
			now = std::chrono::steady_clock::now();

			this->onTick(deltaTime);
			this->refresh();

			time += deltaTime;
			TICK++;
			_fpsCounter++;

			if (time >= 1000000)
			{
				time = 0;
				FPS = _fpsCounter;
				_fpsCounter = 0;
				TICK = 0;
			}
		}
	}

	void Controller::ChangeView(View* view)
	{
		if (this->_view != nullptr)
		{
			// Push the current view to the stack
			this->_views.push(this->_view);
		}

		// Set the new view as the current view
		this->_view = view;
	}

	void Controller::SetView(View* view)
	{
		// Set the new view as the current view
		this->_view = view;
	}
}

