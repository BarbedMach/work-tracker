export module Utility;
import WorkLogItem;

export [[nodiscard]] auto stringToDate(const std::string& dateStr) -> date_t {
	std::istringstream ss(dateStr);
	std::tm tm = {};
	if (!(ss >> std::get_time(&tm, "%Y-%m-%d"))) {
		throw std::invalid_argument("Invalid date format");
	}
	return date_t{ std::chrono::year{tm.tm_year + 1900},
				   std::chrono::month{static_cast<unsigned int>(tm.tm_mon + 1)},
				   std::chrono::day{static_cast<unsigned int>(tm.tm_mday)} };
}

export [[nodiscard]] auto stringToTime(const std::string& timeStr) -> time_t {
	std::istringstream ss(timeStr);
	std::tm tm = {};
	if (!(ss >> std::get_time(&tm, "%H:%M"))) {
		throw std::invalid_argument("Invalid time format");
	}

	return time_t{ std::chrono::hours{tm.tm_hour} + std::chrono::minutes{tm.tm_min} };
}