#include "Controller.h"
#include "../utilities/Utility.h"

#include <conio.h>
#include <thread>

namespace Console
{
	void Controller::GoBack()
	{
		this->_view = this->_views.top();
		this->_view->OnOpenView(this);
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
			this->_view->Update(this, this->_screen);
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
			this->_view->OnKeyPressed(this, key);
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
			this->_view->OnTick(this, deltaTime);
		}
	}

	void Controller::refresh()
	{
		_screen.Reset();
		this->update();
		_screen.Render();
	}

	void Controller::startRenderingThread()
	{
		std::thread renderThread([this]()
			{
				while (true)
				{
					this->refresh();
					_fpsCounter++;
				}
			}
		);
		renderThread.detach();

		std::thread fpsThread([this]()
			{
				while (true)
				{
					Utility::sleep(1000);
					FPS = _fpsCounter;
					_fpsCounter = 0;
					TICK = 0;
				}
			}
		);
		fpsThread.detach();

		std::thread tickThread([this]()
			{
				auto now = std::chrono::steady_clock::now();

				while (true)
				{
					auto deltaTime = std::chrono::steady_clock::now() - now;
					now = std::chrono::steady_clock::now();

					this->onTick(deltaTime.count());
					TICK++;
				}
			}
		);
		tickThread.detach();
	}

	void Controller::Start()
	{
		startRenderingThread();

		// Get every key pressed
		while (const char key = static_cast<char>(_getch()))
		{
			if (_canPressKey) {
				_canPressKey = false;
				this->onKeyPressed(key);
				_canPressKey = true;
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

