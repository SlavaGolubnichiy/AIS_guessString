#include <chrono>

class Timer
{
	private:
	bool isStarted = 0;
	std::chrono::steady_clock::time_point startPoint = std::chrono::steady_clock::time_point(std::chrono::nanoseconds(0));
	std::chrono::steady_clock::time_point endPoint = std::chrono::steady_clock::time_point(std::chrono::nanoseconds(0));
	double elapsed = 0;

	public:
	Timer();
	void start();
	void stop();
	double getElapsedSeconds();
};


Timer::Timer()
{
}

void Timer::start()
{
	if (isStarted)
	{
		throw std::logic_error("timer is already started");
	}
	startPoint = std::chrono::high_resolution_clock::now();
}

void Timer::stop()
{
	endPoint = std::chrono::high_resolution_clock::now();
	isStarted = 0;
}

double Timer::getElapsedSeconds()
{
	std::chrono::duration<double> duration = endPoint - startPoint;
	elapsed = duration.count();
	return elapsed;
}
