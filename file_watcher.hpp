#include <string>
#include <chrono>
#include <functional>
#include <filesystem>
#include <thread>

class file_watcher {
private:
	std::string m_file_path;
	std::chrono::milliseconds m_delay;
public:
	file_watcher(std::string_view file_path, const std::chrono::milliseconds delay)
		: m_file_path(file_path)
		, m_delay(delay)
	{
	}

	void run(std::function<void(std::string_view)> foo)
	{
		auto last_write_time = std::filesystem::last_write_time(m_file_path);
		auto current_write_time = last_write_time;
		while (true)
		{
			std::this_thread::sleep_for(m_delay);

			current_write_time = std::filesystem::last_write_time(m_file_path);
			if (current_write_time > last_write_time)
			{
				last_write_time = current_write_time;
				foo(m_file_path);
			}
		}
	}
};